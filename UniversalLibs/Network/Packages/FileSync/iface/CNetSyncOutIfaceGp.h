// CNetSyncOutIfaceGp.h
#pragma once

#include "CNetSyncAbstractIfaceGp.h"

class CFileMultipartSplitIfaceGp;
class CNetSyncProgressIfaceGp;
class CNetDriverIfaceGp;
class CNetLocatorIfaceGp;

//
// CNetSyncOutIfaceGp -
//
//   Incremental synchronization data sender.
//     All input data must be placed into files and it is being
//     lazily sent to the server within Poll() calls.
//     Create a NetSyncIn for data reception.
//
//   NetSyncOut splits files into small parts internally.
//
//ASSUME_IMPL_FOR_IFACE(CNetSyncOutIface, CNetSyncOutIfaceGp)
//ASSUME_IMPL_FOR_IFACE(CNetSyncOutImpl, CNetSyncOutIface)

class CNetSyncOutIfaceGp : public CNetSyncAbstractIfaceGp
{
public:
    CNetSyncOutIfaceGp();
    //NEW_LOCAL(CNetSyncOutImpl, CNetSyncOutIfaceGp)
    NEW_GP(Net, CNetSyncOutImpl, CNetSyncOutIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // For progress indicator
    int GetLastLocalOutboxFileCount()
            vhook;
    big GetLastLocalOutboxFileBytes()
            vhook;
    int GetLastLocalUploadedFileCount()
            vhook;

    // 0...1
    num GetUploadProgress()
            vhook;

    // Private path for temp files being uploaded
    path GetInternalLocalTempPumpOutFolder()
            vhook;

    // HTTP upload limits file size
    int x_nMultipartFileSplitBytes = 1 * 1024 * 1024
            xauto(Get, Set)
            xassert(value > 0 && value < 10 * 1024 * 1024);

// Operations

    // Init with a specific network driver
    void InitNetSyncOut(
            ref<CNetDriverIfaceGp> rNewDriver)
            vhook;

    // Call before destructing
    void CloseNetSyncOut()
            vhook;

    // Put a file into output queue.
    //   This function will not return until the file is split into parts.
    bool StartFileSync(
            path pathOriginal,
            str sRestoreIntoSubdirs,
            unidef unidefExtraHeader,
            str sSecurityId,
            ptr<CFileMultipartSplitIfaceGp> pFileMultipartSplit,
            out ref<CNetSyncProgressIfaceGp>& out_rProgress)
            vhook;

    // Call often to process the data
    bool PollNetSyncOut()
            vhook;

// UI

    // Public status panel
    ref<UiContainer> NewUiNetSyncOutStatus()
            vhook;
    int x_nNetSyncOutFontSize = 20
            xauto(Get, Set)
            xassert(value >= 5);

    // Pump Easy Display
    ref<UiContainer> NewUiNetSyncOutDisplay()
            vhook;

protected:
    virtual int OnGetLastLocalOutboxFileCount()
            v1pure;
    virtual int OnGetLastLocalUploadedFileCount()
            v1pure;
    virtual num OnGetUploadProgress()
            v1pure;
    virtual big OnGetLastLocalOutboxFileBytes()
            v1pure;
    virtual path OnGetInternalLocalTempPumpOutFolder()
            v1pure;
    virtual void OnInitNetSyncOut(
            ref<CNetDriverIfaceGp> rNewDriver)
            v1pure;
    virtual void OnCloseNetSyncOut()
            v1pure;
    virtual bool OnStartFileSync(
            path pathOriginal,
            str sRestoreIntoSubdirs,
            unidef unidefExtraHeader,
            str sSecurityId,
            ptr<CFileMultipartSplitIfaceGp> pFileMultipartSplit,
            out ref<CNetSyncProgressIfaceGp>& out_rProgress)
            v1pure;
    virtual bool OnPollNetSyncOut()
            v1pure;
    virtual ref<UiContainer> OnNewUiNetSyncOutStatus()
            v1pure;
    virtual ref<UiContainer> OnNewUiNetSyncOutDisplay()
            v1pure;
private:
    //bool _m_bOneTimeInitNetSyncOutIfaceGpOk = false;

    void _init_AttachToNetSyncOutIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
