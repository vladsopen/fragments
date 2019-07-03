// CDeviceIface.h
#pragma once

class CSessionIface;

//
// CDeviceIface -
//
//   MEDM Platform Control.
//
ASSUME_IMPL_FOR_IFACE(CDeviceImpl, CDeviceIface)

class CDeviceIface : public object
{
public:
    CDeviceIface();
    NEW_LOCAL(CDeviceImpl, CDeviceIface)
    //NEW_GP(Balance, CDeviceImpl, CDeviceIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CSessionIface> _x_pSession
            xauto(Get, Set);

    // Report status
    rich GetPosturalDeviceStatus()
            vhook;
    rich GetLamerDeviceStatusAndPrompt()
            vhook;

    // Disables internal timer and expects calls to HandleExternalPolling();
    bool x_bExternalPolling = false
            xauto(Get, Set);

    // We dont need emulator question in monitoring
    bool x_bSilentEmulatorFallback = false
            xauto(Get, Set);

    // Demo?
    bool IsDeviceConnectedToEmulator()
            vhook;

// Operations

    // Init
    void RedetectDevice()
            vhook;

    // Close
    void CloseConnections()
            vhook;

    // External timer/idle
    void HandleExternalPolling()
            vhook;

    // Update platform mode
    void HandleKeyboardEmulationConfigChange()
            vhook;

// UI

    ref<UiContainer> NewUiDeviceDebug()
            vhook;

protected:
    virtual rich OnGetPosturalDeviceStatus()
            v1pure;
    virtual rich OnGetLamerDeviceStatusAndPrompt()
            v1pure;
    virtual void OnRedetectDevice()
            v1pure;
    virtual bool OnIsDeviceConnectedToEmulator()
            v1pure;
    virtual void OnCloseConnections()
            v1pure;
    virtual void OnHandleExternalPolling()
            v1pure;
    virtual void OnHandleKeyboardEmulationConfigChange()
            v1pure;
    virtual ref<UiContainer> OnNewUiDeviceDebug()
            v1pure;
private:
    //bool _m_bOneTimeInitDeviceIfaceOk = false;

    void _init_AttachToDeviceIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
