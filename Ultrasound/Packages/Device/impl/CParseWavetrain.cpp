// CParseWavetrain.cpp

#include "CProject.h"
#include "CParseWavetrain.h"
#include "CAppEventFloaterIfaceGp.h"

CParseWavetrain::CParseWavetrain()
{
}

void CParseWavetrain::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CParseWavetrain!");
}

bool CParseWavetrain::ParseIncomingStreamIntoJoinedClusterBuffers(
        int nActiveClusters,
        int nUsefulPerCluster,
        sbuf sbufParse,
        out sbuf& out_sbufClustersJoined,
        out int& out_iBufferId)
{
    ASSERT(nActiveClusters >= 1 && nActiveClusters <= C_nMaxClusterBuffers);

    if (CProject::GGetIt().x_bHackMode)
    {
        m_bForceDisplayErrors = true;
    }

    // calc useful stuff
    int nInitialLeftCorrupted = m_sbufLeftCorrupted.Len();
    int nParseBytes = sbufParse.Len();
    int nExpectStreamBytes = 
        GCalcFullUnparsedLengthForAllClusters(
            nActiveClusters,
            nUsefulPerCluster);

    // check if we are given a valid data length
    int nNextReadExpected = GetNextReadBytes(nActiveClusters, nUsefulPerCluster);
    ASSERT(nNextReadExpected == nParseBytes);

    //
    // Compose the parse source
    //

    sbuf sbufParseFixed = 
        CompilePortionToParse(
            nActiveClusters,
            nUsefulPerCluster,
            sbufParse);
    ASSERT(sbufParseFixed == sbufParse || m_bRecoveringFromCorruption);

    // new source
    sbufParse = sbufParseFixed;
    nParseBytes = sbufParse.Len();

    //
    // Locate and extract data between valid headers
    //

    int iStartPos = 
        FindValidFormatPos(
            nActiveClusters,
            nUsefulPerCluster,
            sbufParse);

    if (iStartPos == -1)
    {
        // valid data not found!
        if (sbufParse.Len() > nExpectStreamBytes)
        {
            // we already checked double-length portion
            ASSERT(nInitialLeftCorrupted >= nExpectStreamBytes);
            ASSERT(sbufParse.Len() == nExpectStreamBytes * 2);

            // get rid of the first half of the block 
            //   where could not be found any valid data
            HandleDiscardCorruptBytes(
                sbufParse.Len() - nExpectStreamBytes,
                sbufParse,
                "(Full drop)");

            ASSERT(m_sbufLeftCorrupted == "");
            ASSERT(m_sbufUnreadNextTime == "");
            m_sbufLeftCorrupted = sbufParse.GetRight(nExpectStreamBytes);
            m_sbufUnreadNextTime = "";

            ASSERT(m_bRecoveringFromCorruption);

            return false; // failed to get any data
        }
        else
        {
            ASSERT(sbufParse.Len() == nExpectStreamBytes);

            // we did not have m_sbufLeftCorrupted this time 
            ASSERT(nInitialLeftCorrupted == 0);

[...]

        // corrupt data skipped
        //   but we have found the valid block
        HandleDiscardCorruptBytes(
            iStartPos,
            sbufParse,
            "(Partial skip)");

        // return valid data
        ReadRawDataFrom(
            nActiveClusters,
            nUsefulPerCluster,
            sbufParse,
            out out_sbufClustersJoined,
            out out_iBufferId,
            iStartPos);

        // save the remainder for the future unread
        nParseBytes = sbufParse.Len();
        ASSERT(nParseBytes == nExpectStreamBytes * 2);
        int nSaveEndBytes = nParseBytes - iStartPos - nExpectStreamBytes;
        ASSERT(nSaveEndBytes >= 0 && nSaveEndBytes < nExpectStreamBytes);

        ASSERT(m_sbufLeftCorrupted == "");
        ASSERT(m_sbufUnreadNextTime == "");
        m_sbufLeftCorrupted = "";
        m_sbufUnreadNextTime = sbufParse.GetRight(nSaveEndBytes);

        m_bRecoveringFromCorruption = nSaveEndBytes >= 1;

        return true; // valid data received
    }
}

int CParseWavetrain::FindValidFormatPos(
        int nActiveClusters,
        int nUsefulPerCluster,
        sbuf sbufParse)
{
    int nParseBytes = sbufParse.Len();
    int nExpectStreamBytes = 
        GCalcFullUnparsedLengthForAllClusters(
            nActiveClusters,
            nUsefulPerCluster);

    int iScanLastPos = nParseBytes - nExpectStreamBytes;
    ASSERT(iScanLastPos == 0 || m_bRecoveringFromCorruption);
    repeat(iScan, iScanLastPos + 1)
    {
        if (CheckAllClusterHeadersAt(
                nActiveClusters,
                nUsefulPerCluster,
                sbufParse,
                iScan))
        {
            // found valid format!
            return iScan;
        }
    }

    return -1; // valid pos not found
}

sbuf CParseWavetrain::CompilePortionToParse(
        int nActiveClusters,
        int nUsefulPerCluster,
        sbuf sbufParse)
{
    //int nParseBytes = sbufParse.Len();
    int nExpectStreamBytes = 
        GCalcFullUnparsedLengthForAllClusters(
            nActiveClusters,
            nUsefulPerCluster);

    ASSERTM(
        m_sbufLeftCorrupted == "" ||
        m_sbufUnreadNextTime == "",
        "we cannot have both non-empry "
            "m_sbufLeftCorrupted and m_sbufUnreadNextTime");

    // start parsing from the failed block
    if (m_sbufLeftCorrupted != "")
    {
        ASSERT(m_bRecoveringFromCorruption);

        // it goes before the current read
        sbufParse = m_sbufLeftCorrupted + sbufParse;
        int nParseBytes = sbufParse.Len();
        ASSERT(nParseBytes == nExpectStreamBytes * 2);

        m_sbufLeftCorrupted = "";
    }
    else
    {
        int nParseBytes = sbufParse.Len();
        ASSERT(nParseBytes == nExpectStreamBytes || m_sbufUnreadNextTime != "");
    }

    // need to use the defered portion?
    if (m_sbufUnreadNextTime != "")
    {
        ASSERT(m_bRecoveringFromCorruption);

        // it goes before the current read
        sbufParse = m_sbufUnreadNextTime + sbufParse;
        int nParseBytes = sbufParse.Len();
        ASSERT(nParseBytes == nExpectStreamBytes);

        m_sbufUnreadNextTime = "";
    }
    else
    {
        int nParseBytes = sbufParse.Len();
        ASSERT(nParseBytes == nExpectStreamBytes || nParseBytes == nExpectStreamBytes * 2);
    }

    return sbufParse;
}

int CParseWavetrain::GCalcFullUnparsedLengthForAllClusters(
        int nActiveClusters,
        int nUsefulPerCluster)
{
    nActiveClusters;

    int nExpectStreamBytes = 
        //nActiveClusters 
        C_nAskBlocksAtOnce
            * 
            (E_StreamHeader_count + nUsefulPerCluster);

    return nExpectStreamBytes;
}

int CParseWavetrain::GetNextReadBytes(
        int nActiveClusters,
        int nUsefulPerCluster)
{
    int nExpectStreamBytes = 
        GCalcFullUnparsedLengthForAllClusters(
            nActiveClusters,
            nUsefulPerCluster);

    if failed(nExpectStreamBytes >= 1)
    {
        nExpectStreamBytes = 1;
    }

    return nExpectStreamBytes;
}

void CParseWavetrain::HandleDiscardCorruptBytes(
        int nBytes,
        sbuf sbufCutFrom,
        str sDebugInfo)
{
    nBytes;

    if (!debug::GIsTest() &&
        m_bForceDisplayErrors)
    {
        //if (debug::GIsAllowed...())
        {
            static int s_nCorruptionEvents = 0;
            s_nCorruptionEvents++;

            static tick s_tickLastLog = 0;
            if (sys::GGetUtc() - s_tickLastLog > 500)
            {
                s_tickLastLog = sys::GGetUtc();

                sys::GDisplayDebugInRelease(
                    60, 
                    5, 
                    " B err #" + 
                        Str(s_nCorruptionEvents) + 
                        " (" + Bytes(nBytes) + ")   ");
            }
        }

        if (debug("Device.Parser.LogCorruption?").InputBool())
        {
            CProject::GGetProjectWarningFloater()->
                LogNewEvent(
                    "Wavetrain data corruption.",
                    "Discarding " + 
                        Bytes(nBytes) + " " + sDebugInfo + 
                        "\n\n" + 
                        RichFontCourier(
                            sbufCutFrom.
                                GetLeft(nBytes).
                                    GetHexDumpWithLineSplit(" ", 16, "\n")));
        }
    }
}

[...]