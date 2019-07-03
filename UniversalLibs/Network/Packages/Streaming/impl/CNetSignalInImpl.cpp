// CNetSignalInImpl.cpp

#include "CNetProject.h"
#include "CNetSignalInImpl.h"
#include "VirtOs_CFileList.h"
#include "VirtOs_dir.h"
#include "CNetSharedConstIface.h"
#include "VirtOs_file.h"
#include "CNetStreamFormatReader.h"
#include "CNetStreamSubchannelReader.h"
#include "CNetDriverIfaceGp.h"
#include "CNetLocatorIfaceGp.h"
#include "CNetSignalSetup.h"
#include "CNetPumpInIfaceGp.h"
#include "CAppEventFloaterIfaceGp.h"
#include "CNetStatsIfaceGp.h"
#include "CNetBinalIface.h"
#include "CNetErrorsIface.h"
#include "CArchiveLzmaSessionIfaceGp.h"
#include "CArchiveLzmaOptionsIfaceGp.h"
#include "CNetConnectionInIfaceGp.h"
#include "CNetDisplayIfaceGp.h"

CNetSignalInImpl::CNetSignalInImpl()
{
    _init_AttachToNetSignalInImpl();
}

void CNetSignalInImpl::_init_AttachToNetSignalInImpl()
{
    //x_r->_x_p = this;
}

void CNetSignalInImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToNetSignalInImpl();

    rFAIL("make sure it is OK to expose CNetSignalInImpl!");
}

bool CNetSignalInImpl::OnInitUiIsOpen()
{
    if (!_m_bNetSignalInOpened)
    {
        return false;
    }

    ref<CNetLocatorIfaceGp> rLocator =
        GetNetSignalLocator().
            Valid();

    return
        x_rNetSignalSetupIn->
            InitUiIsOpenForSignal(
                rLocator,
                this);
}

ref<CNetPumpInIfaceGp> CNetSignalInImpl::x_rNetPumpInForSignal_Get()
{
    return m_rNetBinalForIn->GetSessionManagedNetPumpIn();
}

void CNetSignalInImpl::OnInitNetSignalIn(
        ref<CNetLocatorIfaceGp> rLocator)
{
    if (_m_bOneTimeInitNetSignalInImplOk)
    {
        return;
    }

    _m_bOneTimeInitNetSignalInImplOk = true;
    _m_bNetSignalInOpened = true;

    //
    // stats
    //

    x_rNetStatsHook->
        InitNetStats(
            "App-Input");
    x_rNetStatsNetwork->
        InitNetStats(
            "Reception");

    //
    // Pump
    //

    ref<CNetDriverIfaceGp> rNewDriver =
        x_rNetSignalSetupIn->
            NewNetDriverForSignals();

    x_rNetSignalSetupIn->
        InitNetLocatorForSignals(
            rLocator);

    rLocator->_x_bStartNewStint = true;

    m_rNetBinalForIn->
        InitNetBinal(
            (ptr<CNetLocatorIfaceGp>) null(), // out
            rLocator);
    m_rNetBinalForIn->
        RestartWithDriver(
            rNewDriver);

    /*
    x_rNetPumpInForSignal->
        InitNetPump(
            pNetDriver,
            rLocator);
            */
}

void CNetSignalInImpl::OnCloseNetSignalIn()
{
    _m_bNetSignalInOpened = false;

    // Close outer object (session) not inner (pump)
    m_rNetBinalForIn->
        CloseNetBinal();
    //x_rNetPumpOutForSignal->
    //    CloseNetPump();
}

sbuf CNetSignalInImpl::OnReadNetSignalPortion(
        int iSubchannel,
        int nMinBytes,
        int nMaxBytes,
        out int& out_nErrorCount)
{
    ref<CNetLocatorIfaceGp> rLocator =
        GetNetSignalLocator().
            Valid();

    // handle chase option change to prevent needless file-delete reset
    if (m_bPrevPollOnChase != rLocator->x_bChaseRealTime)
    {
        m_bPrevPollOnChase = rLocator->x_bChaseRealTime;

        ResetTimingAndDelay();
    }

    // try first time...
    sbuf sbufNext;

    if (x_bFreezeAppFeed)
    {
        // just update stats
        if (sys::GGetUtc() - _m_utcLastPausedPolling < 1000)
        {
            return sbuf();
        }

        _m_utcLastPausedPolling = sys::GGetUtc();

        FindNextIncomingFile();
    }
    else
    {
        sbufNext =
            TryNextPortionOrNextFile(
                iSubchannel,
                nMinBytes,
                nMaxBytes,
                out out_nErrorCount);

        // may be we need to load next file
        if (sbufNext == "")
        {
            sbufNext =
                TryNextPortionOrNextFile(
                    iSubchannel,
                    nMinBytes,
                    nMaxBytes,
                    out out_nErrorCount);
        }
    }

    // stats
    x_rNetStatsHook->
        HandlePollingEvent();
    x_rNetStatsHook->
        HandleTransferEvent(
            sbufNext.Len()); // bytes

    // reset watchdog
    if (sbufNext != ""
        ||
        _m_utcLastDataForAppOrReset == null())
    {
        _m_utcLastDataForAppOrReset = sys::GGetUtc();
    }
    else
    if (math::GAbs(GTickToBig(x_tickDelaySignalInput)) > 
[...]
    {
        if (rLocator->x_bChaseRealTime)
        {
            ResetAllSignalReading();
        }
    }

    if (m_bDeferResetTimingAndDelay)
    {
        ResetTimingAndDelay();
        m_bDeferResetTimingAndDelay = false;
    }

    if (!x_bFeedSignalToApp)
    {
        sbufNext = "";
    }

    if (sbufNext != ""
        &&
        sys::GGetUtc() - _m_utcLastDisplaySignalToApp > 1000)
    {
        _m_utcLastDisplaySignalToApp = sys::GGetUtc();

        rLocator->
            _x_rNetDisplay->
                LogDisplay(
                    CNetDisplayIfaceGp::E_Gravity_HappyNews,
                    CNetDisplayIfaceGp::E_SkipState_Never,
                    TRANSLATE(
                        "Signal is [... UI-string-removed]");
    }
    else
    if (x_bFreezeAppFeed
        &&
        sys::GGetUtc() - _m_utcLastDisplaySignalToApp > 1000)
    {
        _m_utcLastDisplaySignalToApp = sys::GGetUtc();

        rLocator->
            _x_rNetDisplay->
                LogDisplay(
                    CNetDisplayIfaceGp::E_Gravity_IdleStatus,
                    CNetDisplayIfaceGp::E_SkipState_Never,
                    TRANSLATE(
                        "Signal ren[... UI-string-removed]");
    }

    return sbufNext;
}

sbuf CNetSignalInImpl::TryNextPortionOrNextFile(
        int iSubchannel,
        int nMinBytes,
        int nMaxBytes,
        out int& out_nErrorCount)
{
    sbuf sbufNext;

    rASSERT(iSubchannel >= 0 && iSubchannel < oo);
    ref<CNetStreamSubchannelReader> rSubchannelReader = 
        _m_mapSubchannels[iSubchannel];

    if (MayNeedPurgeServer())
    {
        return sbuf("");
    }

    if (!m_bNextParserReady)
    {
        // Find next oldest file
        FindNextIncomingFileAndLoad();
    }

    if (m_bNextParserReady)
    {
        // Read up until time sync
        ReceiveAllSubchannels();

        // Return what we have for the requested channel
        //sbufNext = 
        //    rSubchannelReader->x_sbufReceivedCache;

        // Limit read according to the app wishes
        int nBytesRead = 
            rSubchannelReader->
                GetSubchannelContentLength(); // sbufNext.Len();

        if (nBytesRead < nMinBytes)
        {
            // keep waiting
            sbufNext = "";
        }
        else
        if (nBytesRead <= nMaxBytes)
        {
            // return everything
            sbufNext = 
                rSubchannelReader->
                    GetSubchannelContent();

            rSubchannelReader->
                ClearSubchannelContent();
        }
        else
        {
            ASSERT(nBytesRead > nMaxBytes);

            // cut a part
            sbufNext = 
                rSubchannelReader->
                    ReadSubchannelContent(
                        nMaxBytes);
        }

        // what's left?
        nBytesRead = 
            rSubchannelReader->
                GetSubchannelContentLength(); // sbufNext.Len();

        if (nBytesRead <= 0
            &&
            m_nMaxBufferForSubchannel == iSubchannel)
        {
            m_nMaxBufferBytes = 0;
            m_nMaxBufferForSubchannel = -1;
        }
    }

    // always return actual error count
    out_nErrorCount = rSubchannelReader->x_nTotalErrors;

    return sbufNext;
}

sbuf CNetSignalInImpl::DecompressSignal(
        sbuf sbufArchive)
{
    ref<CNetLocatorIfaceGp> rLocator =
        GetNetSignalLocator().
            Valid();

    ref<CArchiveLzmaSessionIfaceGp> rLzma;

    // *** OBSOLETE! **************************************************
    // we can have the exact size from the header!
    sbuf sbufDecompressed = 
        StrChar(0, math::GMax(5 * 1024 * 1024, sbufArchive.Len() + 1000));
    // *****************************************************************

    str sError;
    rLzma->
        RunArchiveInMemory(
            CArchiveLzmaSessionIfaceGp::E_ArchiveCommand_Decompress,
            (ptr<CArchiveLzmaOptionsIfaceGp>) null(),
            sbufArchive,
            out sbufDecompressed,
            out sError);

    if (sError != "")
    {
        CNetProject::GGetIt().
            x_rGlobalNetErrors->
                LogNetError(
                    CNetErrorsIface::E_NetErrorFrom_Local,
                    CNetErrorsIface::E_NetErrorLevel_Warning,
                    TRANSLATE(
                        "Signal dec[... UI-string-removed]");

        return sbufArchive;
    }

    return sbufDecompressed;
}

str GReportTimeDelay(
        tick tickDelay)
{
    tick tickDelta = tickDelay;
    str sTickSign = "+";
    if (tickDelta < 0)
    {
        sTickSign = "-";
        tickDelta = -GTickToBig(tickDelta);
    }

    return 
        sTickSign + 
            " " +
            period::GFromTickToPeriod(tickDelta).
                ToStr("[yy-][mm-][dd, ]HH:MM:SS.TTT");
}

str CNetSignalInImpl::GetNetSignalInBufferingStats()
{
    str sReport = 
        str() +
            (m_nMaxBufferForSubchannel == -1?
                m_sLastBufferMax + " Now clean. "
                :
                RichBold(Bytes(m_nMaxBufferBytes)) +
                " in subch " +
                Str(m_nMaxBufferForSubchannel)
            ) +
            "";

    if (m_nMaxBufferForSubchannel != -1)
    {
        m_sLastBufferMax = sReport;
    }

    return sReport;
}

void CNetSignalInImpl::ChangeDelay(
        tick tickAdd)
{
    ref<CNetLocatorIfaceGp> rLocator =
        GetNetSignalLocator().
            Valid();

    if (tickAdd == 0)
    {
        return;
    }

    static const tick C_tickMaxChange = 10000;
    tickAdd = 
        math::GLimit(
            GTickToInt(tickAdd), 
            -GTickToInt(C_tickMaxChange), 
            GTickToInt(C_tickMaxChange));

    rLocator->
        _x_rNetDisplay->
            LogDisplay(
                CNetDisplayIfaceGp::E_Gravity_IdleNote,
                CNetDisplayIfaceGp::E_SkipState_Never,
                "" +
                    (tickAdd < 0?
                        TRANSLATE(
                            "Decreasing[... UI-string-removed]");

    x_tickDelaySignalInput += tickAdd;

    if (tickAdd > 0)
    {
        x_nAutoPlusDelay += 1;
    }
    else
    {
        x_nAutoMinusDelay += 1;
    }
}

[...]