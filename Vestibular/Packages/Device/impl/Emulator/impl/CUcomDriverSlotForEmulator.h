// CUcomDriverSlotForEmulator.h
#pragma once

#include "CUcomDriverSlotIfaceGp.h"

class CUcomBrandIfaceGp;

//
// CUcomDriverSlotForEmulator - Emulator device info slot, see iface
//
//ASSUME_IMPL_FOR_IFACE(CUcomDriverSlotForEmulatorImpl, CUcomDriverSlotForEmulator)

class CUcomDriverSlotForEmulator : public CUcomDriverSlotIfaceGp
{
public:
    CUcomDriverSlotForEmulator();
    //NEW_LOCAL(CUcomDriverSlotForEmulatorImpl, CUcomDriverSlotForEmulator)
    //NEW_GP(MyProject, CUcomDriverSlotForEmulatorImpl, CUcomDriverSlotForEmulator)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    //
    // Add/Remove ID parameters needed to open a device after detection
    //

    // For HID
    str x_sDetectedDevicePath = "---"
            xauto(Get, Set)
            xassert(true);

    // For USBXpress
    int x_iDetectedDeviceIndex = -1
            xauto(Get, Set)
            xassert(true);

// Operations

// UI

protected:
    virtual str OnReportFullSlotInfoForDebugAndAutoRefresh()
            ;
    virtual ref<CUcomDriverDeviceIfaceGp> OnNewUcomDeviceForSlot(
            ref<CUcomDeviceIfaceGp> rUcomDevice,
            out str& out_sError)
            ;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
