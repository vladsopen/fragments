// CDeviceImpl.h
#pragma once

#include "VirtOs_ontimer.h"
#include "CDeviceIface.h"

class CNetLink;
class CKeyboardSimulator;
class CSessionIface;
class CConnection;
class CUcomDeviceIfaceGp;
class CUcomEnumeratorIfaceGp;
class CUcomDetectionCallback;

//
// CDeviceImpl - 
//
//   Platform device interface.
//
//ASSUME_IMPL_FOR_IFACE(CDeviceImplImpl, CDeviceImpl)

class CDeviceImpl : public CDeviceIface
{
public:
    CDeviceImpl();
    //NEW_LOCAL(CDeviceImplImpl, CDeviceImpl)
    //NEW_GP(MyProject, CDeviceImplImpl, CDeviceImpl)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // Auto reconnect
    static const tick C_tickMinAutoRedetectAfterFailure = 1000;
    static const tick C_tickMinAutoRedetectAfterSuccess = 3000;
    static const tick C_tickMinAutoRedetectAfterNotFound = 3500;

// Attributes

    // We use first device found
    int x_iUseDevice = 0
            xauto(Get, Set)
            xassert(value >= 0 && value < oo);

    // Device status
    ref<CConnection> x_rConnection
            xauto(Get);

    // Device
    ref<CKeyboardSimulator> x_rKeyboardSimulator
            xauto(Get);

    // Network I/O mode
    ref<CNetLink> x_rNetLink
            xauto(Get);

// Operations

    // Auto redetect with a timeout
    bool HandleNeedAutoRedetect();

    // Diagnostics
    void ReportDeviceError(
            sloc slocFriendly,
            str sDetails);
    void ReportDeviceWarning(
            sloc slocFriendly,
            str sDetails);

// UI

    // DeviceDemoOrRetry
    ref<UiContainer> NewUiDeviceDemoOrRetry();
    ptrmap<CUiPanelGp> _m_ptrmapUiDeviceDemoOrRetry;

    // DeviceDebug
    //ref<UiContainer> NewUiDeviceDebug();
    ptrmap<CUiPanelGp> _m_ptrmapUiDeviceDebug;

protected:
    virtual rich OnGetPosturalDeviceStatus()
            ;
    virtual rich OnGetLamerDeviceStatusAndPrompt()
            ;
    virtual void OnRedetectDevice()
            ;
    virtual bool OnIsDeviceConnectedToEmulator()
            ;
    virtual void OnCloseConnections()
            ;
    virtual void OnHandleExternalPolling()
            ;
    virtual void OnHandleKeyboardEmulationConfigChange()
            ;
    virtual ref<UiContainer> OnNewUiDeviceDebug()
            ;
private:

    // autoredetect timeout
    tick _m_tickNextAllowedAutoRedetect = 0;

    // Ucom
    ref<CUcomEnumeratorIfaceGp> _m_rUcomEnumerator;
    ref<CUcomDetectionCallback> _m_rUcomDetectionCallback;

    bool m_bConnectedToEmulator = false;
    bool m_bNetInput = false;

    // Timer handler 
    void HandleRedetectTimer();
    ontimer<HandleRedetectTimer> _m_ontimerHandleRedetect;

    void _init_AttachProperties();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiDeviceDebug.cpp)
class UiDeviceDebug : public UiContainer
{
public:
    virtual void Ui();

    ref<CDeviceImpl> m_rDevice;
};

// (UiDeviceDemoOrRetry.cpp)
class UiDeviceDemoOrRetry : public UiContainer
{
public:
    virtual void Ui();

    ref<CDeviceImpl> m_rDevice;

private:
    void EqualizeText(
            ref<CUiActionGp> rUi);
};
