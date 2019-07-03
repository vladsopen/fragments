// CUcomDriverEnumeratorForEmulator.h
#pragma once

#include "CUcomDriverEnumeratorForEmulatorIface.h"

//
// CUcomDriverEnumeratorForEmulator - 
//
//   Emulator device detector for CUcomEnumeratorImpl
//
//ASSUME_IMPL_FOR_IFACE(CUcomDriverEnumeratorForEmulatorImpl, CUcomDriverEnumeratorForEmulator)

class CUcomDriverEnumeratorForEmulator : public CUcomDriverEnumeratorForEmulatorIface
{
public:
    CUcomDriverEnumeratorForEmulator();
    //NEW_LOCAL(CUcomDriverEnumeratorForEmulatorImpl, CUcomDriverEnumeratorForEmulator)
    //NEW_GP(MyProject, CUcomDriverEnumeratorForEmulatorImpl, CUcomDriverEnumeratorForEmulator)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // Loop breaker
    static const int C_nMaxEmulatorDevicesInEnum = 1000;

    // Max descriptor string length possible
    static const int C_nMaxEmulatorStringBytes = 1000;

    // Max descriptor string length possible
    static const int C_nMaxEmulatorStringCount = 4;

// Attributes

// Operations

// UI

protected:

    virtual str OnGetUcomDriverProtocolName()
            ;
    virtual void OnEnumerateUcomDriverDeviceSlots(
            ref<CUcomEnumeratorIfaceGp> rUcomEnumerator,
            out str& out_sError)
            ;

private:

    // use once 
    bool _m_bEnumerateEmulatorCalled = false;

    // device list context
    //GUID _m_dataHidGuid = GUID();
    array<str> m_asEmulatedDeviceNames;

    // Get device list
    void InitEmulatorDeviceList(
            out str& out_sInitError,
            //out HANDLE& out_hDeviceClassEnum,
            ref<CUcomEnumeratorIfaceGp> rUcomEnumerator);

    // Get device info
    bool IterateEmulatorDevices(
            //HANDLE hDeviceClassEnum,
            int iDevice,
            out text& out_textBrand,
            out int& out_nDeviceVendorId,
            out int& out_nDeviceProductId,
            out str& out_sDevicePath,
            out str& out_sError,
            ref<CUcomEnumeratorIfaceGp> rUcomEnumerator);

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
