// CParseEcgBlock.cpp

#include "CProject.h"
#include "CParseEcgBlock.h"
#include "CAudioSliceGp.h"
#include "CAudioSliceGp.cpp"
#include "CParseEcgFile.h"
#include "CWaveSegment.h"

CParseEcgBlock::CParseEcgBlock()
{
    _init_AttachToParseEcgBlock();
}

void CParseEcgBlock::_init_AttachToParseEcgBlock()
{
    //x_r->_x_p = this;
}

void CParseEcgBlock::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToParseEcgBlock();

    rFAIL("make sure it is OK to expose CParseEcgBlock!");
}

void CParseEcgBlock::ParseAgc12bit(
        ref<CWaveSegment> rWaveSegment,
        sbuf sbufBlock)
{
    rASSERT(!_m_bOneTimeInitParseEcgBlockOk);
    _m_bOneTimeInitParseEcgBlockOk = true;
    _m_bParseEcgBlockOpened = true;

    // read header
    int nDataSize = 
        SHORT_HILO(
            sbufBlock.GetAt(CParseEcgFile::C_nEcgHeaderDataSizeByteHi), 
            sbufBlock.GetAt(CParseEcgFile::C_nEcgHeaderDataSizeByteLo));

    //rASSERT(nDataSize > 4000 && nDataSize <= 4072);
    //ASSERT_SOFT(nDataSize == 4072);
    if failed(
        nDataSize > 0 && 
        nDataSize <= 
            CParseEcgFile::C_nEcgBlockBytes 
                - 
                CParseEcgFile::E_EcgHeader_length * sizeof(int))
    {
        return;
    }

    _m_arNewSlicePerChannel.
        SetCount(
            CParseEcgFile::C_nEcgTotalChannels);

    // round robin all channels
    int iCurrentChannel = 0;
    ref<CAudioSliceGp> rCurrentAudioSlice = 
        _m_arNewSlicePerChannel.
            GetAt(iCurrentChannel);

    int iSampleInMacroBlock = 0;

    // 12-bit unpack
    bool bEvenByte = true;

    int nEcgSamplesPerMacroBlock = 
        CParseEcgFile::C_nEcgEcgFreq 
            / 
            CParseEcgFile::C_nEcgSamplesPerMacroBlock;

    // ECG first, then motion sensors
    bool bEcg = true;

    // scan the block
    repeat(iByte, nDataSize - 1)
    {
        unsigned char cByte1 = 
            sbufBlock.
                GetAt(
                    CParseEcgFile::C_nEcgBinaryHeaderBytes 
                        + 
                        iByte);

        unsigned char cByte2 = 
            sbufBlock.
                GetAt(
                    CParseEcgFile::C_nEcgBinaryHeaderBytes 
                        + 
                        iByte + 1);

        // 12 bit unpack
        int nUnpacked = -1;
        if (bEvenByte)
        {
            // 64 C4 7F
            // ^^  ^   
            //  464
            nUnpacked = (cByte1) | ((cByte2 & 0xf) << 8); 
        }
        else
        {
            // 64 C4 7F
            //    ^  ^^
            //     7fc
            nUnpacked = (cByte1 >> 4) | (cByte2 << 4); 

            iByte++;
        }

        bEvenByte = !bEvenByte;

        rASSERT(nUnpacked >= 0 && nUnpacked <= CParseEcgFile::C_nEcgEcgMax);
        nUnpacked -= CParseEcgFile::C_nEcgEcgMiddle;

        // save next amplitude
        rCurrentAudioSlice = _m_arNewSlicePerChannel.GetAt(iCurrentChannel);
        rCurrentAudioSlice->Add((short) nUnpacked);

        // go round to the next channel
        iCurrentChannel++;
        if (bEcg)
        {
            if (iCurrentChannel == CParseEcgFile::C_nEcgEcgChannels)
            {
                iCurrentChannel = 0;
            }
            else
            {
                rASSERT(iCurrentChannel <= CParseEcgFile::C_nEcgEcgChannels);
            }
        }
        else
        {
            rASSERT(iCurrentChannel > CParseEcgFile::C_nEcgEcgChannels);
            rASSERT(iCurrentChannel <= CParseEcgFile::C_nEcgTotalChannels);
        }

        // next macro block?
        iSampleInMacroBlock++;

        // skip alignment after ECG data end
        if (iSampleInMacroBlock 
                == 
                nEcgSamplesPerMacroBlock * CParseEcgFile::C_nEcgEcgChannels)
        {
            rASSERT(bEcg);
            rASSERT(iCurrentChannel == 0);

            if ((iByte + 1) & 1)
            {
                CASSERT(CParseEcgFile::C_nEcgEcgChannels == 3);
                CASSERT(CParseEcgFile::C_nEcgEcgFreq == 200);
                //rFAIL("16 bit alignment is not tested with 200hz*3ch");
                iByte++;
                bEvenByte = true;
            }

            bEcg = false;
            iCurrentChannel = CParseEcgFile::C_nEcgEcgChannels;
        }

        // end of block?
        if (iCurrentChannel == CParseEcgFile::C_nEcgTotalChannels)
        {
            // start next macroblock with ECG
            iCurrentChannel = 0;
            iSampleInMacroBlock = 0;
            bEcg = true;

            // skip aligned data we dont need now
            iByte += 
                CParseEcgFile::C_nEcgMacroBlockEndingBytes 
                    ;//+ 1; // iByte is delayed: we read [iByte] & [iByte+1]

            bEvenByte = true;
        }
        else
        {
            rASSERT(iCurrentChannel < CParseEcgFile::C_nEcgTotalChannels);
        }
    }

    int iSegmentChannel = rWaveSegment->x_iChannelAffinity;
    rWaveSegment->_x_rSegmentAudioSlice = 
        _m_arNewSlicePerChannel.GetAt(iSegmentChannel);
}

bool CParseEcgBlock::IterateSlicesPerChannel(
        out iter& out_i,
        out ref<CAudioSliceGp>& out_value)
{
    rASSERT(_m_bOneTimeInitParseEcgBlockOk);
    return _m_arNewSlicePerChannel.Iterate(out out_i, out out_value);
}

