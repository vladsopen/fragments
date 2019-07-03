// CParseEcgFile.cpp

#include "CProject.h"
#include "CParseEcgFile.h"
#include "CWaveDb.h"
#include "CWaveDocImpl.h"
#include "VirtOs_file.h"
#include "CParseEcgBlock.h"
#include "CAudioSliceGp.h"
#include "CWaveSegment.h"
#include "CWaveChannelType.h"

CParseEcgFile::CParseEcgFile()
{
    _init_AttachToParseEcgFile();
}

void CParseEcgFile::_init_AttachToParseEcgFile()
{
    //x_r->_x_p = this;
}

void CParseEcgFile::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToParseEcgFile();

    rFAIL("make sure it is OK to expose CParseEcgFile!");
}

bool CParseEcgFile::UpdateFromRawWave()
{
    bool bUpdated = false;

    path pathWave = 
        GetWaveDoc()->x_pathWaveDoc;
    ref<CWaveDb> rWaveDb = 
        GetWaveDoc()->x_rWaveDb;

    // we scan up to the current file size
    str sSizeError;
    big nnTotalFileBytes = 
        file(pathWave).GetSize(out sSizeError);
    rASSTR(sSizeError);

    utc utcStart = sys::GGetUtc();

    // align
    nnTotalFileBytes /= C_nEcgBlockBytes;
    nnTotalFileBytes *= C_nEcgBlockBytes;

    // scan fixed blocks from the last known position
    big nnThisBlockBytes = 0;
    big iiReadAt = 0;
    for (
        iiReadAt = m_nnParsedSoFar;
        iiReadAt < nnTotalFileBytes + C_nEcgBlockBytes;
        iiReadAt += nnThisBlockBytes)
    {
        // accept incomplete blocks
        nnThisBlockBytes = 
            math::GMin((big) C_nEcgBlockBytes, nnTotalFileBytes - iiReadAt);

        // end of loop
        if (nnThisBlockBytes <= 0)
        {
            break;
        }

        m_nnBlocksAllocated++;

        // debug
        if (x_nMaxParserBlocks != -1 &&
            m_nnBlocksAllocated > x_nMaxParserBlocks)
        {
            break;
        }

        // time for progress? (debug only)
        if (sys::GGetUtc() - utcStart > 1000)
        {
            ACT(
                TRANSLATE(
                    "Preparing [... UI-string-removed]");
        }

        bUpdated = true;

        // a segment for each channel
        repeat(iSegmentChannel, C_nEcgTotalChannels)
        {
            ref<CWaveSegment> rWaveSegment;

            rWaveSegment->x_iiWaveSegment = 
                m_nnBlocksAllocated - 1;

            rWaveSegment->x_iChannelAffinity = 
                iSegmentChannel;

            //rWaveSegment->_x_rSegmentAudioSlice = rIterAudioSlice;

            rWaveSegment->x_iiSegmentStartInFile = 
                iiReadAt;

            rWaveSegment->x_nnSegmentBytesInFile = 
                nnThisBlockBytes;

            rASSERT(rWaveSegment->x_tickSegmentDuration == -1);
                // later...

            rWaveDb->
                AddSegmentWave(
                    rWaveSegment);
        }
    }

    m_nnParsedSoFar = iiReadAt;

    return bUpdated;
}

void CParseEcgFile::LoadEcgBlock(
        ref<CWaveSegment> rWaveSegment)
{
    path pathWave = 
        GetWaveDoc()->x_pathWaveDoc;
    ref<CWaveDb> rWaveDb = 
        GetWaveDoc()->x_rWaveDb;

    big iiReadAt = 
        rWaveSegment->x_iiSegmentStartInFile;
    big nnThisBlockBytes = 
        rWaveSegment->x_nnSegmentBytesInFile;

    sbuf sbufBlock;
    str sLoadError;
    if (!file(pathWave).
            LoadFragment(
                iiReadAt,
                nnThisBlockBytes,
                out sbufBlock,
                out sLoadError))
    {
        rFAIL(sLoadError); //TODO: error
        //break;
        sbufBlock = RichErrorBold("File read failed. /ERR:WAI/ " + sLoadError);
    }
    else
    if (sbufBlock.IsAbbrev(CParseEcgFile::C_sEcgBlockSignature))
    {
        // ECG?
        char cBlockType = 
            sbufBlock.GetAt(C_nEcgHeaderBlockTypeByte);
        if (cBlockType
                == 
                C_cEcgHeaderBlockTypeIsWave)
        {
            //
            // Wave Block
            //

            ParseEcgBlock(
                rWaveSegment,
                sbufBlock);

            type<CWaveChannelType> typeChannelType = 
                rWaveSegment->GetSegmentChannelType();

            num numFreq = 
                typeChannelType->GetChannelFreq();
            rASSERT(numFreq > 0);

            rWaveSegment->x_tickSegmentDuration = 
                (big)
                math::GMulDiv(
                    (num) rWaveSegment->_x_rSegmentAudioSlice->GetCount(),
                    1000.0, // ms
                    numFreq);

            return;
        }
        else
        {
            //
            // SMDEV Header | Alarm block
            //

            sbufBlock = 
                str((sbufBlock.GetTruncateLeft(C_nEcgBinaryHeaderBytes)).c()).
                    GetLeftAbbrev(1000, "...");

            // normal header?
            if (iiReadAt == 0)
            {
                // just skip, and send the rest to the proprietary parser
                rWaveSegment->x_sSegmentDebug = sbufBlock;
                sbufBlock = 

[...]