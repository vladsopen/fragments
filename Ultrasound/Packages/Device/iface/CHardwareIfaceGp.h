// CHardwareIfaceGp.h
#pragma once

#include "SuperSharedUsoundConst.h"

class CSpeedometerIfaceGp;
class CConpadBarIfaceGp;
class CMultiPlayIfaceGp;
class CAudioSliceGp;
class CRawSlotGp;
class CSendClusterIfaceGp;
class CCustomizeHardwareIfaceGp;
class CDeviceIfaceGp;

//
// CHardwareIfaceGp -
//
//   Controls Scanner devices and emulators for CSessionIfaceGp
//
//ASSUME_IMPL_FOR_IFACE(CHardwareIface, CHardwareIfaceGp)
//ASSUME_IMPL_FOR_IFACE(CHardwareImpl, CHardwareIface)

class CHardwareIfaceGp : public object
{
public:
    CHardwareIfaceGp();
    //NEW_LOCAL(CHardwareImpl, CHardwareIfaceGp)
    //NEW_GP(MyProject, CHardwareImpl, CHardwareIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // App options
    ref<CCustomizeHardwareIfaceGp> _x_rCustomizeHardware
            xauto(Get);

    // Maximum nubmer of channels supported by the software
    int GetMaxChannelCount()
            vhook;
    int GetMaxClusterPerChannelCount()
            vhook;

    // For Usound LegacyD / Monitor only
    bool _x_bHackSendAudioViaWireToEmulator = false
            xauto(Get, Set);

    // Multiple document Parallelizer powered media/audio playback controller
    ref<CMultiPlayIfaceGp> x_rMultiPlay
            xauto(Get);

    // "Nx"
    ref<CSpeedometerIfaceGp> GetSpeedometer()
            vhook;

    // End-of-data detection
    utc x_utcLastAudioOrRasterReceived = null()
            xauto(Get, Set)
            xassert(true);

// Operations

    // Multiple device controllers are available at the same time.
    //   To access the one you want select a EScannerDevice value.
    ref<CDeviceIfaceGp> GetDevice(
            EScannerDevice eScannerDevice)
            vhook;

    // This is for Parallelizer to send new raw audio online
    bool DispatchNextOnlineSend(
            EScannerDevice eScannerDevice,
            ref<CRawSlotGp> rTargetRawSlot,
            ref<CAudioSliceGp> rAudioSlice,
            ptr<CSendClusterIfaceGp> pSendCluster)
            vhook;

    // Total offline
    void StopOnlineRecordingInAllDevices()
            vhook;

    // Create device control bar
    //   (we can create multiple bars if we want a separate bar for
    //    every scanner channel)
    ref<CConpadBarIfaceGp> NewConpadBar()
            vhook;

// UI

protected:

    virtual int OnGetMaxChannelCount()
            v1pure;
    virtual int OnGetMaxClusterPerChannelCount()
            v1pure;
    virtual ref<CSpeedometerIfaceGp> OnGetSpeedometer()
            v1pure;
    virtual ref<CDeviceIfaceGp> OnGetDevice(
            EScannerDevice eScannerDevice)
            v1pure;
    virtual bool OnDispatchNextOnlineSend(
            EScannerDevice eScannerDevice,
            ref<CRawSlotGp> rTargetRawSlot,
            ref<CAudioSliceGp> rAudioSlice,
            ptr<CSendClusterIfaceGp> pSendCluster)
            v1pure;
    virtual void OnStopOnlineRecordingInAllDevices()
            v1pure;
    virtual ref<CConpadBarIfaceGp> OnNewConpadBar()
            v1pure;

private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
