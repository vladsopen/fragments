// CSyncInImpl.cpp

#include "CProject.h"
#include "CSyncInImpl.h"
#include "CNetImpl.h"
#include "CSessionIface.h"
#include "CNetSyncInIfaceGp.h"
#include "CNetLocatorIfaceGp.h"
#include "CStoreIface.h"
#include "VirtOs_dir.h"
#include "VirtOs_CFileList.h"
#include "CActionIface.h"
#include "CNetDriverIfaceGp.h"
#include "VirtOs_CAppGp.h"
#include "VirtOs_CGlobalUi.h"
#include "VirtOs_text.h"
#include "CSyncCook.h"
#include "CNetEnablePumpIfaceGp.h"
#include "CUvEventCounterIfaceGp.h"
#include "CUvEventCountableIfaceGp.h"
#include "CGlobalConfigIface.h"

void CSyncInImpl::InitSyncIn()
{
    rASSERT(!_m_bOneTimeInitSyncInImplOk);
    _m_bOneTimeInitSyncInImplOk = true;
    _m_bSyncInImplOpened = true;

    ref<CNetImpl> rNet = 
        GetNet();

    // Init locator
    rNet->
        InitNetLocator(
            x_rNetSyncIn->x_rNetLocator);

    // Init Net
    x_rNetSyncIn->
        InitNetSyncIn(
            rNet->NewNetDriver());

    // synchronize ON/OFF switch
    x_rNetSyncIn->x_rNetEnablePumpSync =
        rNet->x_rNetEnablePumpSync;
}

void CSyncInImpl::HandleDownloadTimer()
{
    rASSERT(_m_bSyncInImplOpened);

    bool bHaveJobs = false;
    ForcePollSyncIn(
        out bHaveJobs);

    HandleImportTimer();
}

void CSyncInImpl::GoImportActions(
        out bool& out_bHaveJobs)
{
    bool bSomethingUpdated = 
        DoGoImportActionsNoTiming(
            out out_bHaveJobs);

    this->x_utcLastImportComplete = sys::GGetUtc();

    ref<CGlobalConfigIface> rGlobalConfig =
        CProject::GGetIt().
            x_rGlobalConfig;

    if (bSomethingUpdated
        &&
        rGlobalConfig->_x_bRefreshAllOnImport
        &&
        !debug::GIsTest())
    {
        CAppGp::GGet()->x_rUi->RefreshAllFrames(&CUiPanelGp::InvalidateContent);
    }
}

bool CSyncInImpl::DoGoImportActionsNoTiming(
        out bool& out_bHaveJobs)
{
    rASSERT(_m_bSyncInImplOpened);

    ref<CGlobalConfigIface> rGlobalConfig =
        CProject::GGetIt().
            x_rGlobalConfig;

    TESTLOG(
        "+Sync",
        "\n"
            "Downloader: GoImportActions...\n");
    TESTLOG(
        "+GoImport",
        "\n"
            "Downloader: GoImportActions...\n");

    path pathInbox = 
        GetDownloadFolder();

    // read inbox
    ref<CFileList> rFileList = 
        dir(pathInbox).
            GetFilesByMask(
                "*");

    // parse old files first
    rFileList->SortFilesByNameAscending();

    // get next waiting file
    file fileIter;
    moment momentPrevFile;
    path pathPrev;
    path pathDeleteOldExtraWhenNewFound;
    bool bSomethingUpdated = false;
    utc utcStarted = sys::GGetUtcPhysical();
    for (
        iter iFile;
        rFileList->
            Iterate(
                out iFile,
                out fileIter);)
    {
        if (rGlobalConfig->_x_bShowImportListUpdateAct
            ||
            debug::GIsTest()
            ||
            sys::GGetUtcPhysical() - utcStarted > 3000)
        {
            ACT(
                str() +
                    TRANSLATE(
                        "Receiving [... UI-string-removed]");
        }

        path pathImport = fileIter.GetFullPath();
[...]
        if (!ImportMultiActionFile(
                pathImport,
                out out_bHaveJobs))
        {
            break;
        }
    }

    // update file count and hide import button in UI
    bool bHaveJobs = false;
    ForcePollSyncIn(
        out bHaveJobs);
    _m_ptrmapUiSyncInStatus.UpdateContentAndLayoutNow();

    TESTLOG(
        "+Sync",
        "\n"
            "done Downloader: GoImportActions.\n");

    return bSomethingUpdated;
}

[...]