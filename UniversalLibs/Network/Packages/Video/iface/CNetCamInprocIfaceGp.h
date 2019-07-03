// CNetCamInprocIfaceGp.h
#pragma once

//
// CNetCamInprocIfaceGp -
//
//   Network I/O management for NetCam.
//
//ASSUME_IMPL_FOR_IFACE(CNetCamInprocIface, CNetCamInprocIfaceGp)
//ASSUME_IMPL_FOR_IFACE(CNetCamInprocImpl, CNetCamInprocIface)

class CNetCamInprocIfaceGp : public object
{
public:
    CNetCamInprocIfaceGp();
    //~CNetCamInprocIfaceGp();
    //NEW_LOCAL(CNetCamInprocImpl, CNetCamInprocIfaceGp)
    NEW_GP(Net, CNetCamInprocImpl, CNetCamInprocIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Recorder temp files
    path GetSenderQueuePath()
            vhook;

    // Player temp files
    path GetReceiverQueuePath()
            vhook;

    // Hide all toolbars starting as MONITORING camera window
    bool IsBareBoneCameraVideoUi()
            vhook;

    // Starts video recording or Net playback immediatelly
    bool IsAutoStartCamera()
            vhook;

// Operations

    // One-time opener
    void InitNetCamInprocAsSender()
            vhook;
    void InitNetCamInprocAsReceiver()
            vhook;

    // Cleanup
    void CloseNetCamInproc()
            vhook;

// UI

    // Default camera window location over MONITORING
    void ShapeMainNetCamWindow(
            ref<CUiPanelGp> rUiMainFrame)
            vhook;

    // End-user status display
    ref<UiContainer> NewUiNetCamEasyStatus()
            vhook;

protected:
    SEE_ALSO(CNetCamInprocImpl) // F12-lookup

    virtual path OnGetSenderQueuePath()
            v1pure;
    virtual path OnGetReceiverQueuePath()
            v1pure;
    virtual bool OnIsBareBoneCameraVideoUi()
            v1pure;
    virtual bool OnIsAutoStartCamera()
            v1pure;
    virtual void OnInitNetCamInprocAsSender()
            v1pure;
    virtual void OnInitNetCamInprocAsReceiver()
            v1pure;
    virtual void OnCloseNetCamInproc()
            v1pure;
    virtual void OnShapeMainNetCamWindow(
            ref<CUiPanelGp> rUiMainFrame)
            v1pure;
    virtual ref<UiContainer> OnNewUiNetCamEasyStatus()
            v1pure;

private:
    //bool _m_bOneTimeInitNetCamInprocIfaceGpOk = false;
    //bool _m_bNetCamInprocIfaceGpOpened = false;

    void _init_AttachToNetCamInprocIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
