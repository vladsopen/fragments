// CDeviceManagerImpl.cpp

#include "CProject.h"
#include "CDeviceManagerImpl.h"
#include "CTomographUsb.h"
#include "VirtOs_ask.h"
#include "CTomogramIface.h"
#include "CTomographEmulator.h"
#include "CDownloaderIface.h"
#include "CUsbFrameFromTomograph.h"

CDeviceManagerImpl::CDeviceManagerImpl()
{
}

/*
ref<CDeviceConfig> CDeviceManagerImpl::GetDeviceConfig()
{
    return Derive<CDeviceConfig>(GetPackageConfig()).Valid();
}
*/

ref<CTomographIface> CDeviceManagerImpl::OnGetInternalTomographIface()
{
    ASSERT(debug::GIsTest());

    return GetCurrentTomograph();
}

rich CDeviceManagerImpl::OnGetFullDeviceErrorInfoOr(
        rich richReturnIfNoErrors)
{
    if (!_m_bDeviceSessionInitOk)
        return richReturnIfNoErrors;

    rich richError = GetCurrentTomograph()->x_richUsbError;
    if (richError != "")
        return richError;
    else
        return richReturnIfNoErrors;
}

rich CDeviceManagerImpl::OnGetDeviceWarnings()
{
    if (!_m_bDeviceSessionInitOk)
        return "";

    rich richWarnings = GetCurrentTomograph()->x_richUsbWarnings;

    return richWarnings;
}

ref<CTomographCommon> CDeviceManagerImpl::GetCurrentTomograph()
{
    ASSERT(_m_bDeviceSessionInitOk);
    ASSERT(m_rCurrentTomograph._internal_IsObjectCreated());

    return m_rCurrentTomograph;
}

void CDeviceManagerImpl::OnPrepareToNewSession()
{
    // Make sure device selected, notified and emulator UI ready if needed
    if ok(!_m_bDeviceSessionInitOk)
    {
        ASSERT(!m_rCurrentTomograph._internal_IsObjectCreated());

        // Set partial-init flag to silence some asserts
        ASSERT(!_m_bDeviceSessionInitStarted);
        _m_bDeviceSessionInitStarted = true;

        // Initial mode
        if (debug("Device.StartWithEmulator?/4e").InputBool())
        {
            m_bEmulateTomograph = true;
        }
        else
        {
            m_bEmulateTomograph = false;
            //StartSecondMonitor();
        }

        // Apply to object selection
        ApplyEmulateTomographSwitch();

        // Never do this again
        _m_bDeviceSessionInitOk = true;
    }
}

void CDeviceManagerImpl::OnHandleTerminateSession()
{
    if (_m_bDeviceSessionInitOk)
    {
        GetCurrentTomograph()->CloseTomograph();
    }
}

bool CDeviceManagerImpl::OnIsTomographOpen()
{
    if (!_m_bDeviceSessionInitOk)
        return false;

    return 
        GetCurrentTomograph()->IsTomographOpen();
}

bool CDeviceManagerImpl::OnInitTomograph()
{
    rASSERT(_m_bDeviceSessionInitOk);

    // not already open?
    if failed(!IsTomographOpen())
    {
        ASSERT(GetCurrentTomograph()->IsTomographOpen());
        return false;
    }

    // Link the device back to us for callbacks
    GetCurrentTomograph()->_x_pDeviceManager = this;

    // Clear error status
    if (_m_bDeviceSessionInitOk)
    {
        GetCurrentTomograph()->x_richUsbError = "";
        GetCurrentTomograph()->x_richUsbWarnings = "";
    }

    // Open the device
    if (!GetCurrentTomograph()->OpenTomograph())
    {
        ask(
            TRANSLATE(
                "Unable to [... UI-string-removed]");

        GetCurrentTomograph()->CloseTomograph();

        return false;
    }

    return true;
}

void CDeviceManagerImpl::OnCloseTomograph()
{
    if (_m_bDeviceSessionInitOk)
    {
        // Close device
        GetCurrentTomograph()->CloseTomograph()
    }
}

bool CDeviceManagerImpl::OnEraseInEmergency()
{
    bool bWasOpen = IsTomographOpen();
    if (!bWasOpen)
    {
        InitTomograph();
    }

    // Emulate normal operations
    GetTomogramCountAndStartDownload();

    // Call erase
    bool bOk = EraseAllTomogramsFromDevice();

    if (!bWasOpen)
    {
        CloseTomograph();
    }

    return bOk;
}

void CDeviceManagerImpl::OnHandleIdleTomographPolling(
        ref<CDownloaderIface> rDownloader)
{
    rASSERT(_m_bDeviceSessionInitOk);

    TESTLOG(
        "DownloadDetails",
        "\n\n\n"
            "----- POLLING -----\n");

    if (!IsTomographOpen())
    {
        TESTLOG(
            "DownloadDetails",
            "Device is closed.\n");

        return;
    }

    // Save previous error status
    str sPrevDeviceReports = 
        GetFullDeviceErrorInfoOr("") + GetDeviceWarnings();

    TESTLOG(
        "DownloadDetails",
        Str(sPrevDeviceReports.Len()) + 
            " errors before.\n");

    // If we had already sent a request we may expect some data ready
    ref<CUsbFrameFromTomograph> rUsbFrameFromTomograph = 
        GetCurrentTomograph()->TryReadTomographFrame();

    TESTLOG(
        "DownloadDetails",
        "Got me a frame: " + 
            Str(rUsbFrameFromTomograph->x_sbufRawUsbFrame.Get().Len()) + 
            " bytes.\n");

    // Request next frame while we are leaving to do other idle work
    GetCurrentTomograph()->RequestPrepareReadNextTomographFrame();

    // Got some data?
    if (rUsbFrameFromTomograph->IsCompleteUsbFrameFromTomograph())
    {
        GetCurrentTomograph()->
            ParseTomographFrameAndCallbackWithResults(
                rUsbFrameFromTomograph,
                rDownloader);
    }
    else
    {
        TESTLOG(
            "DownloadDetails",
            "The frame is incomplete.\n");
    }

    // New error status?
    str sNewDeviceReports = 
        GetFullDeviceErrorInfoOr("") + GetDeviceWarnings();

    TESTLOG(
        "DownloadDetails",
        Str(sNewDeviceReports.Len()) + 
            " errors issued.\n");

    if (sNewDeviceReports != sPrevDeviceReports)
    {
        rDownloader->UpdateErrorReportingUi();
    }
}

void CDeviceManagerImpl::OnEnableTomographEmulatorWuu(
        bool bEnable)
{
    // Nothing to change?
    if (bEnable == m_bEmulateTomograph)
    {
        return;
    }

    // New mode
    this->m_bEmulateTomograph = bEnable;

    // Apply to device objects
    ApplyEmulateTomographSwitch();
}

[...]