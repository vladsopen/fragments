// CNetSyncInIfaceGp.h
#pragma once

#include "CNetSyncAbstractIfaceGp.h"

class CNetConnectionInIfaceGp;
class CNetDriverIfaceGp;
class CNetLocatorIfaceGp;

//
// CNetSyncInIfaceGp -
//
//   Incoming data update syncronizer.
//     All input data is received to files in the application format
//     in which it was sent using NetSyncOut.
//
//ASSUME_IMPL_FOR_IFACE(CNetSyncInIface, CNetSyncInIfaceGp)
//ASSUME_IMPL_FOR_IFACE(CNetSyncInImpl, CNetSyncInIface)

class CNetSyncInIfaceGp : public CNetSyncAbstractIfaceGp
{
public:
    CNetSyncInIfaceGp();
    //NEW_LOCAL(CNetSyncInImpl, CNetSyncInIfaceGp)
    NEW_GP(Net, CNetSyncInImpl, CNetSyncInIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // optional MultipartMerge ExtraHeader unidef is saved into
    //   a meta file with this ext added
    static const path C_pathExtraHeaderExt = ".somext";
        SEE_ALSO(C_pathLateMetaExt)
    static const path C_pathLateMetaInExtraHeaderUnidef = "_Ending";

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Private path for temp files being downloaded
    path GetInternalLocalTempPumpInFolder()
            vhook;

    // Resetable connection for transient stats only
    ref<CNetConnectionInIfaceGp> GetCurrentNetConnectionIn()
            vhook;

    // For stats
    int x_nLastRestoredFileCount = 0
            xauto(Get)
            xassert(true);
    int x_nLastRawFileCount = 0
            xauto(Get)
            xassert(true);
    utc x_utcLastSyncInReception
            xauto(Get)
            xassert(true);
    int GetDownloadedFileCount()
            vhook;
    big GetDownloadedBytes()
            vhook;
    int GetServerPendingFileCount()
            vhook;
    // 0...1
    num GetDownloadProgress()
            vhook;

// Operations

    // Init with a specific network driver
    void InitNetSyncIn(
            ref<CNetDriverIfaceGp> rNewDriver)
            vhook;

    // Call before destructing
    void CloseNetSyncIn()
            vhook;

    // Recover reveived files into pathReceivedFolder
    bool PollNetSyncIn(
            path pathRestoreInto,
            path pathCountPendingRestoredFileMask)
            vhook;

    // Ignore all data before this moment
    void ForceSkipToNewestNow()
            vhook;

// UI

    // Public status panel
    ref<UiContainer> NewUiNetSyncInStatus()
            vhook;

    // Pump Easy Display
    ref<UiContainer> NewUiNetSyncInDisplay()
            vhook;

protected:
    virtual path OnGetInternalLocalTempPumpInFolder()
            v1pure;
    virtual ref<CNetConnectionInIfaceGp> OnGetCurrentNetConnectionIn()
            v1pure;
    virtual int OnGetDownloadedFileCount()
            v1pure;
    virtual big OnGetDownloadedBytes()
            v1pure;
    virtual int OnGetServerPendingFileCount()
            v1pure;
    virtual num OnGetDownloadProgress()
            v1pure;
    virtual void OnInitNetSyncIn(
            ref<CNetDriverIfaceGp> rNewDriver)
            v1pure;
    virtual void OnCloseNetSyncIn()
            v1pure;
    virtual bool OnPollNetSyncIn(
            path pathRestoreInto,
            path pathCountPendingRestoredFileMask)
            v1pure;
    virtual void OnForceSkipToNewestNow()
            v1pure;
    virtual ref<UiContainer> OnNewUiNetSyncInStatus()
            v1pure;
    virtual ref<UiContainer> OnNewUiNetSyncInDisplay()
            v1pure;
private:
    //bool _m_bOneTimeInitNetSyncInIfaceGpOk = false;

    void _init_AttachToNetSyncInIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
