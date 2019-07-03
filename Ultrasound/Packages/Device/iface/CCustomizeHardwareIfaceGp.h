// CCustomizeHardwareIfaceGp.h
#pragma once

//
// CCustomizeHardwareIfaceGp -
//
//   Application dependent Hardware UI options.
//     Must be defined after Session init. Not saved!
//     Can be changed at any time.
//
//   By default all options are applied to all devices.
//
ASSUME_IMPL_FOR_IFACE(CCustomizeHardwareIface, CCustomizeHardwareIfaceGp)
ASSUME_IMPL_FOR_IFACE(CCustomizeHardwareImpl, CCustomizeHardwareIface)

class CCustomizeHardwareIfaceGp : public object
{
public:
    CCustomizeHardwareIfaceGp();
    //NEW_LOCAL(CCustomizeHardwareImpl, CCustomizeHardwareIfaceGp)
    //NEW_GP(MyProject, CCustomizeHardwareImpl, CCustomizeHardwareIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // An option to hide [On]/[Off] button for all visible devices.
    //   (you can still call SetDeviceOnlineRecordingMode() etc.)
    bool x_bShowOnOffButton = true
            xauto(Get);
    void x_bShowOnOffButton_Set(
            bool value)
            vhook;

    // Playback
    bool x_bShowMultiPlayControls = true
            xauto(Get);
    void x_bShowMultiPlayControls_Set(
            bool value)
            vhook;

    // Filter channel/cluster sliders, probe and AGC controls.
    //    By default all are HIDDEN!
    //    Always call Hide() first, and then Show() 
    void HideAllChannelClusterAdjusters()
            vhook;
    void ShowChannelClusterAdjusters(
            int iChannel,
            int iCluster)
            vhook;

// Operations

// UI

protected:
    virtual void Onx_bShowOnOffButton_Set(
            bool value)
            v1pure;
    virtual void Onx_bShowMultiPlayControls_Set(
            bool value)
            v1pure;
    virtual void OnShowChannelClusterAdjusters(
            int iChannel,
            int iCluster)
            v1pure;
    virtual void OnHideAllChannelClusterAdjusters()
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
