// CDeviceIfaceGp.h
#pragma once

class CArgUsoundAdjusterStateGp;
class CHardwareIfaceGp;
class CAdjusterTypeIfaceGp;

//
// CDeviceIfaceGp -
//
//   A single device controller implemented or encapsulated by Usmain.
//     Note: multiple devices are available all at once in CHardwareIfaceGp
//
ASSUME_IMPL_FOR_IFACE(CDeviceCommon, CDeviceIfaceGp)

class CDeviceIfaceGp : public object
{
public:
    CDeviceIfaceGp();
    //NEW_LOCAL(CDeviceImpl, CDeviceIfaceGp)
    //NEW_GP(MyProject, CDeviceImpl, CDeviceIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    //
    // Cosmetics
    //

    // Internal name for debug UI
    str GetDeviceName()
            vhook;

    // Internal icon for debug UI
    res GetDeviceIconLarge()
            vhook;

    //
    // Object Relations
    //

    // Uscan device manager
    ref<CHardwareIfaceGp> GetParentHardware()
            vhook;

    //
    // Modes
    //

    // Activated device controls are visible in the end-user mode control panel.
    //   Without an active device control panel will not work.
    //   But a number of devices may actively send audio data and config to
    //   any Recorder Dcc subset with or without this Activation status.
    // You may activate more than 1 device at once (for testing?)
    bool x_bDeviceUiActivated = false
            xauto(Get);
    void x_bDeviceUiActivated_Set(
            bool value)
            vhook;

    // Start acquiring (or emulating) audio data on all channels 
    //   and sending it to Recorder Docs.
    //   All associated Doc Recorders will be switched to Online mode.
    void SetDeviceOnlineRecordingMode()
            vhook;

    // Switch all Recorder Docs to passive offline mode.
    //    Stop data acquisition on all channels.
    //    Stop emulator generators.
    //    Stop playback loops.
    void SetDeviceOfflineMode()
            vhook;

    // Start playback loops.
    //void SetDevicePlaybackMode() --- need more args
            //vhook;

    //
    // Device Parameter Adjusters
    //

    // Get a device parameter value for UI
    ref<CArgUsoundAdjusterStateGp> GetDeviceChannelAdjuster(
            int iChannel,
            type<CAdjusterTypeIfaceGp> typeAdjuster)
            vhook;

    // Set a device parameter value from UI
    void SetDeviceChannelAdjuster(
            int iChannel,
            type<CAdjusterTypeIfaceGp> typeAdjuster,
            int iSetInternalPos)
            vhook;

// Operations

// UI

protected:
    virtual str OnGetDeviceName()
            v1pure;
    virtual res OnGetDeviceIconLarge()
            v1pure;
    virtual ref<CHardwareIfaceGp> OnGetParentHardware()
            v1pure;
    virtual void Onx_bDeviceUiActivated_Set(
            bool value)
            v1pure;
    virtual void OnSetDeviceOnlineRecordingMode()
            v1pure;
    virtual void OnSetDeviceOfflineMode()
            v1pure;
    virtual ref<CArgUsoundAdjusterStateGp> OnGetDeviceChannelAdjuster(
            int iChannel,
            type<CAdjusterTypeIfaceGp> typeAdjuster)
            v1pure;
    virtual void OnSetDeviceChannelAdjuster(
            int iChannel,
            type<CAdjusterTypeIfaceGp> typeAdjuster,
            int iSetInternalPos)
            v1pure;

private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
