// CTomographCommon.h
#pragma once

#include "CTomographIface.h"

class CDownloaderIface;
class CUsbFrameFromTomograph;
class CDeviceManagerImpl;

//
// CTomographCommon - 
//
//   Common device implementation for both real Tomograph and Emulator.
//   Inherited by
//     - CTomographUsb
//     - CTomographEmulator
//

class CTomographCommon : public CTomographIface
{
public:
    CTomographCommon();
    //NEW_LOCAL(CTomographCommonImpl, CTomographCommon)
    //NEW_GP(MyProject, CTomographCommonImpl, CTomographCommon)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // Low level command codes
    enum ETomographCommand
    {
        E_TomographCommand_Read = 0x00,
        E_TomographCommand_EraseAll = 0x01,
        E_TomographCommand_GetVersions = 0x02,
        E_TomographCommand_GetSampleCount = 0x03,
    };

    // How many bytes to read
    static const int C_nTomographGetVersionsResponseBytes = 4;
    static const int C_nTomographGetSampleCountResponseBytes = 2;

    // SI_SUCCESS for high level code
    static const int C_nSiSuccess = 0;

    // Special SI_xxx error code for Tomo internal errors
    static const int C_nSiResponseParserError = off;

// Attributes

    // Fixed by device implementation 
    int x_nTomographSamplePointCount = C_nRealTomographSamplePointCount
            xauto(Get, Set) // Set is only for tests
            xassert(value > 0);

    // Fixed (for real device) frame metrics (variable for tests)
    int x_nTomographFrameHeaderBytes = 4
            xauto(Get)
            xassert(value > 0);
    int x_nTomographFrameDataBytes = 2048 * 256
            xauto(Get, Set) // Set is only for tests
            xassert(value > 0);
    int GetTomographFrameTotalBytes();

    // Last (and final) error report
    rich x_richUsbError = ""
            xauto(Get, Set)
            xassert(true);

    // Last (and final) error report
    rich x_richUsbWarnings = ""
            xauto(Get, Set)
            xassert(true);

    // Parent controller for callback routing
    ptr<CDeviceManagerImpl> _x_pDeviceManager
            xauto(Get, Set);

    // A check required for safe exit cleanup
    bool IsTomographOpen()
            vhook;

// Operations

    // Prepare the device object to be used by app
    void HandleDeviceSelected()
            vhook;

    // Open the device for download
    //   Must be explicit.
    bool OpenTomograph()
            vhooked;

    // Called when evere device oprations are to be terminated 
    //   or before common cleaup called just-in-case
    //   Must be explicit! It is critical for callback pointer cleanup!
    void CloseTomograph()
            vhook;

    // Initiate asyncronous sample data download.
    int GetTomogramCountAndStartDownload()
            vhook;

    // Called upon successful download
    bool EraseAllTomograms()
            vhook;

    // Low level command execution
    bool ExecuteTomographCommand(
            ETomographCommand eTomographCommand)
            vhook;

    // Call this before expecting TryReadTomographFrame() to return something
    void RequestPrepareReadNextTomographFrame();

    // This may call the client back with incoming data points
    ref<CUsbFrameFromTomograph> TryReadTomographFrame()
            vhook;

    // This may call the client back with incoming data points
    void ParseTomographFrameAndCallbackWithResults(
            ref<CUsbFrameFromTomograph> rUsbFrameFromTomograph,
            ref<CDownloaderIface> rDownloader);

    // App activation handler
    void HandleAppActivationStateChange(
            CProject::EActivationStateChange eActivationStateChange)
            vhook;

    // Simplified error filter for calls to Silabs USBXpress
    bool ReportSiUsbError(
            str sFunction,
            int nSiError,
            sloc sFriendlyFinalStatus,
            str sMoreErrorInfo);

    // Simplified error filter for calls to Silabs USBXpress
    void ReportTomographCallErrorOnce(
            str sError);

    // Low level error stringizer
    sloc StringizeSiUsbErrorCode(
            int nSiError)
            vhook;

// UI

    // Emulation UI
    ref<UiContainer> NewUiTomographDeviceControls()
            vhook;

protected:
    virtual bool OnIsTomographOpen()
            v1pure;
    virtual void OnHandleDeviceSelected()
            v1pure;
    virtual bool OnOpenTomograph()
            v1pure;
    virtual void OnCloseTomograph()
            v1pure;
    virtual bool OnExecuteTomographCommand(
            ETomographCommand eTomographCommand)
            v1pure;
    virtual ref<CUsbFrameFromTomograph> OnTryReadTomographFrame()
            v1pure;
    virtual int OnGetTomogramCountAndStartDownload()
            v1pure;
    virtual bool OnEraseAllTomograms()
            v1pure;
    virtual void OnHandleAppActivationStateChange(
            CProject::EActivationStateChange eActivationStateChange)
            v1null;
    virtual sloc OnStringizeSiUsbErrorCode(
            int nSiError)
            v1pure;
    virtual ref<UiContainer> OnNewUiTomographDeviceControls()
            v1pure;

private:
    int m_nAmplitudesReceivedForCurrentSample = 0;

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
