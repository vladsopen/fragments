// CHardwareImpl.h
#pragma once

#include "CHardwareIface.h"

class CSpeedometerImpl;
class COnPostDeviceState;
class CRawSlotGp;
class CDetailTypeIface;
class CSessionIface;
class CDeviceCommon;

//
// CHardwareImpl - see iface
//

class CHardwareImpl : public CHardwareIface
{
public:
    CHardwareImpl();
    ~CHardwareImpl();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Init called?
    bool IsHardwareInitOk();

    // UT support
    ref<CDeviceCommon> GetDeviceForClassTests();

    ref<CSpeedometerImpl> x_rSpeedometer
            xauto(Get);

    // Current selection
    //link<CDeviceCommon> x_linkSelection
    //        xauto(Get);
    //void x_linkSelection_Set(link<CDeviceCommon> value); // UI update

// array<> Publishing

    int GetDeviceCount()
        { return m_aDevice.GetCount(); }
    // void SetCount(int nNewSize)
    //     { return m_aDevice.SetCount(nNewSize); }
    // ref<CDeviceCommon> GetAt(int iAt)
    //     { return m_aDevice.GetAt(iAt); }
    // void SetAt(int iAt,
    //     { m_aDevice.SetAt(iAt,
    void AddDevice(
            ref<CDeviceCommon> rDeviceCommon);
    // void InsertAt(int iAt,
    //     { m_aDevice.InsertAt(iAt,
    // void RemoveAt(int iAt,
    //     { m_aDevice.RemoveAt(iAt,
    // void RemoveAll()
    //     { m_aDevice.RemoveAll(); }
    bool IterateDevices(out iter& out_i, out ref<CDeviceCommon>& out_value)
        { return m_aDevice.Iterate(out out_i, out out_value); }
    // see also IterateBackwards()

// Operations

    // Alarm impl
    //void HandleAlarm(
            //bool bPressed);

// UI

    // HardwareInternals
    //ref<UiContainer> NewUiHardwareBar();
    ptrmap<CUiPanelGp> _m_ptrmapUiHardwareBar;
    void UpdateUiHardwareBarOnDeviceSelectionChange();
    void UpdateUiHardwareBarOnCustomizationContentChange();

protected:
    virtual int OnGetMaxChannelCount()
            ;
    virtual int OnGetMaxClusterPerChannelCount()
            ;
    virtual ref<CDeviceIfaceGp> OnGetDevice(
            EScannerDevice eScannerDevice)
            ;
    virtual ref<CSpeedometerIfaceGp> OnGetSpeedometer()
            ;
    virtual void OnSetCustomizeHardware(
            ref<CCustomizeHardwareIfaceGp> rCustomizeHardware)
            ;
    virtual ref<CHardwareIface> OnNewHardwareAsPlayerOnly()
            ;
    virtual void OnInitHardwareForNewSession()
            ;
    virtual void OnInitHardwareForStandaloneUscanMode()
            ;
    virtual void OnCloseHardwareAtSessionEnd()
            ;
    virtual void OnHandleDeviceUpdatesAfterDocListOrRawSlotsChange()
            ;
    virtual bool OnDispatchNextOnlineSend(
            EScannerDevice eScannerDevice,
            ref<CRawSlotGp> rTargetRawSlot,
            ref<CAudioSliceGp> rAudioSlice,
            ptr<CSendClusterIfaceGp> pSendCluster)
            ;
    virtual void OnStopOnlineRecordingInAllDevices()
            ;
    virtual void OnHandleIdleIsOnlineMode(
            out CProject::EIdleNext& out_eIdleNextMayBeUpgraded)
            ;
    virtual ref<UiContainer> OnNewUiHardwareBar(
            type<CDetailTypeIface> typeDetailType)
            ;
    virtual ref<UiContainer> OnOpenUiHardwareBarFloater()
            ;
    virtual ref<CConpadBarIfaceGp> OnNewConpadBar()
            ;

private:

    // devices
    array< ref<CDeviceCommon> > m_aDevice;

    // session active? not hidden behind Patients?
    bool _m_bHardwareInitOk = false;
    bool _m_bHardwareEverInitialized = false;

    // Alarm handler
    //ref<COnPostDeviceState> _m_rOnPostDeviceState;

    // init parent back pointers in our members
    void _init_AttachProperties();

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    virtual void OnTestClass();
    void _test_RunOnHardware();
};

// (UiHardwareBar.cpp)
class UiHardwareBar : public UiContainer
{
public:
    virtual void Ui();

    ref<CHardwareImpl> m_rHardware;
    type<CDetailTypeIface> m_typeDetailType;
};
