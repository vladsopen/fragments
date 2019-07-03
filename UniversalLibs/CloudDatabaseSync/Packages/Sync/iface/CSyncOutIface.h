// CSyncOutIface.h
#pragma once

class CActionIface;
class CNetIface;

//
// CSyncOutIface -
//
//   Uploads local DB updates to the server.
//
ASSUME_IMPL_FOR_IFACE(CSyncOutImpl, CSyncOutIface)

class CSyncOutIface : public object
{
public:
    CSyncOutIface();
    //~CSyncOutIface();
    NEW_LOCAL(CSyncOutImpl, CSyncOutIface)
    //NEW_GP(CloudSync, CSyncOutImpl, CSyncOutIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CNetIface> _x_pNet
            xauto(Get, Set);

// Operations

    // One-time opener
    //void InitSyncOut();

    // Cleanup
    //void CloseSyncOut();

    // Put db update into outbox queue
    void UploadAction(
            ref<CActionIface> rAction)
            vhook;

    // Called on multiaction commit
    void FlushActionUpload()
            vhook;

// UI

    // Status bar
    ref<UiContainer> NewUiSyncOutStatus()
            vhook;

protected:
    SEE_ALSO(CSyncOutImpl) // F12-lookup

    virtual void OnUploadAction(
            ref<CActionIface> rAction)
            v1pure;
    virtual void OnFlushActionUpload()
            v1pure;
    virtual ref<UiContainer> OnNewUiSyncOutStatus()
            v1pure;

private:
    //bool _m_bOneTimeInitSyncOutIfaceOk = false;
    //bool _m_bSyncOutIfaceOpened = false;

    void _init_AttachToSyncOutIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
