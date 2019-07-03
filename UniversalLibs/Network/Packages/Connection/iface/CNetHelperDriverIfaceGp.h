// CNetHelperDriverIfaceGp.h
#pragma once

class CNetCallbackInIfaceGp;
class CNetCallbackCommonIfaceGp;
class CNetLocatorIfaceGp;
class CNetConnectionCommonIfaceGp;
class CNetDriverIfaceGp;

//
// CNetHelperDriverIfaceGp -
//
//   Internal common driver funcriona. Not to be used from app!
//
ASSUME_IMPL_FOR_IFACE(CNetHelperDriverIface, CNetHelperDriverIfaceGp)
ASSUME_IMPL_FOR_IFACE(CNetHelperDriverImpl, CNetHelperDriverIface)

class CNetHelperDriverIfaceGp : public object
{
public:
    CNetHelperDriverIfaceGp();
    //NEW_LOCAL(CNetHelperDriverImpl, CNetHelperDriverIfaceGp)
    NEW_GP(Net, CNetHelperDriverImpl, CNetHelperDriverIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CNetConnectionCommonIfaceGp> _x_pNetConnection
            xauto(Get);

    // non-null after init
    ptr<CNetDriverIfaceGp> _x_pIoConnectionDriver
            xauto(Get);

    // non-null after init
    ptr<CNetLocatorIfaceGp> _x_pIoConnectionLocator
            xauto(Get);

    // non-null after init
    ptr<CNetCallbackCommonIfaceGp> _x_pIoConnectionCallback
            xauto(Get);

    /*
    // Server file list reading position
    path LoadPermanentLastPos(
            str sActualServer)
            vhook;
    void SavePermanentLastPos(
            str sActualServer,
            path pathDownloaded)
            vhook;
    */

// Operations

    // Internal init
    void HandleInitOpeningConnection(
            ptr<CNetConnectionCommonIfaceGp> pConnection,
            ptr<CNetDriverIfaceGp> pDriver,
            ptr<CNetCallbackCommonIfaceGp> pOn,
            ptr<CNetLocatorIfaceGp> pLocator)
            vhook;

    // Timer Polling init
    void StartNetDriverTimerPolling(
            tick tickTimeout)
            vhook;

    // Unifies final file reception
    //   moving the file to the folder where app reads it
    void CreateReceivedFileBox(
            ref<CNetDriverIfaceGp> rDriver,
            path pathDownloaded,
            ref<CNetLocatorIfaceGp> rLocator,
            ref<CNetCallbackInIfaceGp> rCallbackIn)
            vhook;

// UI

    // Basic common status and config plus optional provided by the driver
    ref<UiContainer> NewUiNetHelperDriverDash()
            vhook;

protected:
    virtual void OnHandleInitOpeningConnection(
            ptr<CNetConnectionCommonIfaceGp> pConnection,
            ptr<CNetDriverIfaceGp> pDriver,
            ptr<CNetCallbackCommonIfaceGp> pOn,
            ptr<CNetLocatorIfaceGp> pLocator)
            v1pure;
    virtual void OnStartNetDriverTimerPolling(
            tick tickTimeout)
            v1pure;
    virtual void OnCreateReceivedFileBox(
            ref<CNetDriverIfaceGp> rDriver,
            path pathDownloaded,
            ref<CNetLocatorIfaceGp> rLocator,
            ref<CNetCallbackInIfaceGp> rCallbackIn)
            v1pure;
    virtual ref<UiContainer> OnNewUiNetHelperDriverDash()
            v1pure;
private:
    //bool _m_bOneTimeInitNetHelperDriverIfaceGpOk = false;

    void _init_AttachToNetHelperDriverIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
