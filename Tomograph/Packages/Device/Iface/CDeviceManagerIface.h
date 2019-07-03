// CDeviceManagerIface.h
#pragma once

class CDownloaderIface;
class CTomographIface;

//
// CDeviceManagerIface - controls the device redirecting calls to
//   the specific device link (which could be an emulator)
//
ASSUME_IMPL_FOR_IFACE(CDeviceManagerImpl, CDeviceManagerIface);

class CDeviceManagerIface : public object
{
public:
    CDeviceManagerIface();
    NEW_LOCAL(CDeviceManagerImpl, CDeviceManagerIface)
    //NEW_GP(MyProject, CDeviceManagerImpl, CDeviceManagerIface)

// Constants

// Attributes

    // A check required for safe exit cleanup
    bool IsTomographOpen()
            vhook;

    // Activate device emulator or real device
    bool IsTomographEmulatorEnabled()
            vhook;
    void EnableTomographEmulatorWuu(
            bool bEnable)
            vhook;

    // Unit test needs the internal device object
    //   APPLICATION HAS NO ACCESS TO the underlying device object,
    //   the Manager manages everything!
    ref<CTomographIface> GetInternalTomographIface()
            vhook;

    // Error status
    rich GetFullDeviceErrorInfoOr(
            rich richReturnIfNoErrors)
            vhook;

    // Non-critical errors
    rich GetDeviceWarnings()
            vhook;

// Operations

    // Preliminary init handler
    void PrepareToNewSession()
            vhook;

    // Initialize to start download.
    //   Must be explicit.
    bool InitTomograph()
            vhook;

    // One-time close.
    //   Must be explicit! It is critical for callback pointer cleanup!
    void CloseTomograph()
            vhook;

    // Initiate sample data download
    int GetTomogramCountAndStartDownload()
            vhook;

    // Called upon successful download
    bool EraseAllTomogramsFromDevice()
            vhook;

    // Emergency menu erase
    bool EraseInEmergency()
            vhook;

    // Call this from the timer callback to enable the open device to receive its data.
    //   During this call the client may (or may not) receive one or more
    //   callback calls.
    void HandleIdleTomographPolling(
            ref<CDownloaderIface> rDownloader)
            vhook;

    // Termination cleaup
    void HandleTerminateSession()
            vhook;

    // App activation handler
    void HandleAppActivationStateChange(
            CProject::EActivationStateChange eActivationStateChange)
            vhook;

// UI

    // Emulation UI
    ref<UiContainer> NewUiTomographControls()
            vhook;

protected:
    virtual bool OnIsTomographOpen()
            v1pure;
    virtual ref<CTomographIface> OnGetInternalTomographIface()
            v1pure;
    virtual rich OnGetFullDeviceErrorInfoOr(
            rich richReturnIfNoErrors)
            v1pure;
    virtual rich OnGetDeviceWarnings()
            v1pure;
    virtual void OnPrepareToNewSession()
            v1pure;
    virtual bool OnInitTomograph()
            v1pure;
    virtual void OnCloseTomograph()
            v1pure;
    virtual void OnHandleIdleTomographPolling(
            ref<CDownloaderIface> rDownloader)
            v1pure;
    virtual int OnGetTomogramCountAndStartDownload()
            v1pure;
    virtual bool OnEraseAllTomogramsFromDevice()
            v1pure;
    virtual bool OnEraseInEmergency()
            v1pure;
    virtual void OnHandleTerminateSession()
            v1pure;
    virtual bool OnIsTomographEmulatorEnabled()
            v1pure;
    virtual void OnEnableTomographEmulatorWuu(
            bool bEnable)
            v1pure;
    virtual void OnHandleAppActivationStateChange(
            CProject::EActivationStateChange eActivationStateChange)
            v1pure;
    virtual ref<UiContainer> OnNewUiTomographControls()
            v1pure;
private:
};

// Inherited:
class CDeviceManagerImpl;
