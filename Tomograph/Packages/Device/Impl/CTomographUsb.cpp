// CTomographUsb.cpp

#include "CProject.h"
#include "CTomographUsb.h"
#include "CTomogramIface.h"
#include "CMarkerIface.h"
#include "CMarkerListIface.h"
#include "CCurveIface.h"
#include "VirtOs_ask.h"
#include "CDeviceManagerImpl.h"
#include "CUsbFrameFromTomograph.h"
#include "CDownloaderIface.h"

CTomographUsb::CTomographUsb()
{
}

CTomographUsb::~CTomographUsb()
{
    HAIRY; // searching for slow destructors

    // reliable destruction and cleanup is important here 
    // but I dont want to force-close lose handles at undefined moment where it can
    // cause fatal crashes in the USB DLL

    // It must be closed explicitly by the client!
    rASSERT(_m_hSiDevice == INVALID_HANDLE_VALUE);
    //CloseTomograph();

    _m_hSiDevice = INVALID_HANDLE_VALUE;
}

void CTomographUsb::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    FAIL("cant expose CTomographUsb");
}

bool CTomographUsb::OnIsTomographOpen()
{
    if (_m_hSiDevice != INVALID_HANDLE_VALUE)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CTomographUsb::OnHandleDeviceSelected()
{
    // no actions required here
}

bool CTomographUsb::OnOpenTomograph()
{
    if failed(_m_hSiDevice == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    // Open device
    if (ReportSiUsbError(
            "Open-Device",
            SI_Open(0, &_m_hSiDevice), // opening first (0) device supporting only one.
            TRANSLATE(
                "Tomograph [... UI-string-removed]");
        _m_hSiDevice = INVALID_HANDLE_VALUE;
        return false;
    }

    // Set timeouts
    tick tickBothTimeouts = C_tickSiUsbReadWriteTimeouts;
    if (ReportSiUsbError(
            "Set-Timeouts",
            SI_SetTimeouts(
[...]
    // Reset buffers/queues
    if (ReportSiUsbError(
            "Flush-Buffers",
            SI_FlushBuffers(_m_hSiDevice, true, true), // both read and write
            TRANSLATE(
                "USB initia[... UI-string-removed]");
        _m_hSiDevice = INVALID_HANDLE_VALUE;
        return false;
    }

    // Version compatibility check
    if (!CheckTomographVersions())
    {
        return false;
    }

    return true;
}

sloc CTomographUsb::OnStringizeSiUsbErrorCode(
        int nSiError)
{
    // (I don't use TRY_STRINGIZE_CODE because for a few error codes I 
    //  managed to provide some friendly description)

    sloc slocError;

    if (nSiError == C_nSiResponseParserError)
    {
        slocError = 
            TRANSLATE(
                "Device res[... UI-string-removed]");
    }
    else
    if (nSiError == SI_SUCCESS)
    {
        slocError = 
            TRANSLATE(
                "USB operat[... UI-string-removed]");
    }
    else
    if (nSiError == SI_DEVICE_NOT_FOUND)
    {
        slocError = 
            TRANSLATE(
                "USB device[... UI-string-removed]");
    }
    else
    if (nSiError == SI_INVALID_HANDLE)
    {
        slocError = 
            TRANSLATE(
                "USB device[... UI-string-removed]");
    }
    else
    if (nSiError == SI_READ_ERROR)
    {
        slocError = 
            TRANSLATE(
                "USB read f[... UI-string-removed]");
    }
    else
    if (nSiError == SI_RX_QUEUE_NOT_READY)
    {
        slocError = 
            TRANSLATE(
                "USB data i[... UI-string-removed]");
    }
    else
    if (nSiError == SI_WRITE_ERROR)
    {
        slocError = 
            TRANSLATE(
                "USB write [... UI-string-removed]");
    }
    else
    if (nSiError == SI_RESET_ERROR)
    {
        slocError = 
            TRANSLATE(
                "USB reset [... UI-string-removed]");
    }
    else
    if (nSiError == SI_INVALID_PARAMETER)
    {
        slocError = 
            TRANSLATE(
                "USB operat[... UI-string-removed]");
    }
    else
    if (nSiError == SI_INVALID_REQUEST_LENGTH)
    {
        slocError = 
            TRANSLATE(
                "USB reques[... UI-string-removed]");
    }
    else
    if (nSiError == SI_DEVICE_IO_FAILED)
    {
        slocError = 
            TRANSLATE(
                "USB input/[... UI-string-removed]");
    }
    else
    if (nSiError == SI_INVALID_BAUDRATE)
    {
        slocError = 
            TRANSLATE(
                "USB baudra[... UI-string-removed]");
    }
    else
    if (nSiError == SI_FUNCTION_NOT_SUPPORTED)
    {
        slocError = 
            TRANSLATE(
                "USB functi[... UI-string-removed]");
    }
    else
    if (nSiError == SI_GLOBAL_DATA_ERROR)
    {
        slocError = 
            TRANSLATE(
                "USB global[... UI-string-removed]");
    }
    else
    if (nSiError == SI_SYSTEM_ERROR_CODE)
    {
        slocError = 
            TRANSLATE(
                "USB system[... UI-string-removed]");
    }
    else
    if (nSiError == SI_READ_TIMED_OUT)
    {
        slocError = 
            TRANSLATE(
                "USB read t[... UI-string-removed]");
    }
    else
    if (nSiError == SI_WRITE_TIMED_OUT)
    {
        slocError = 
            TRANSLATE(
                "USB write [... UI-string-removed]");
    }
    else
    {
        rFAIL("unknown SiError code: " + Str(nSiError));

        slocError = 
            TRANSLATE(
                "Unknown US[... UI-string-removed]");
    }

    if (nSiError != C_nSiResponseParserError)
    {
        slocError += " [SI_" + Hex00(nSiError, 2) + "]"; 
    }

    return slocError;
}

bool CTomographUsb::OnExecuteTomographCommand(
        ETomographCommand eTomographCommand)
{
    str sCommand = "0x" + Hex00((int) eTomographCommand, 2);

    // Only available for opened device
    if failed(_m_hSiDevice != INVALID_HANDLE_VALUE)
    {
        ReportTomographCallErrorOnce(
            " [Command " + sCommand + " sent to closed device] ");

        return false;
    }

    DWORD dwBytesWritten = (DWORD) off;
    const int C_nSendBytes = 1;
    if (ReportSiUsbError(
            "Write-Command",
            SI_Write(_m_hSiDevice, &eTomographCommand, C_nSendBytes, &dwBytesWritten),
            TRANSLATE(
                "Tomograph [... UI-string-removed]");
    }
    else
    {
        // Only available for opened device
        if failed(dwBytesWritten == C_nSendBytes)
        {
            ReportTomographCallErrorOnce(
                " [Command " + sCommand + 
                    " sent " + Str((int) dwBytesWritten) + 
                    " instead of " + Str(C_nSendBytes) + "] ");
            return false;
        }

        // success
        return true;
    }
}

sbuf CTomographUsb::TryReadFromTomograph(
        int nBytes)
{
    // Only available for opened device
    if failed(_m_hSiDevice != INVALID_HANDLE_VALUE)
    {
        ReportTomographCallErrorOnce(
            " [An attempt to read from closed device] ");

        return sbuf("");
    }

    // Allocate input buffer
    sbuf sbufRawRead;
[...]
            ReportTomographCallErrorOnce(
                " [Read " + Str((int) dwBytesRead) + 
                    " bytes instead of " + Str(nBytes) + "] ");

            sbufRawRead = "";
        }
    }

    return sbufRawRead;
}

bool CTomographUsb::CheckTomographVersions()
{
    // all should be fine until now
    rASSERT(x_richUsbError == "");

    // Send the command
    if (!ExecuteTomographCommand(E_TomographCommand_GetVersions))
    {
        // failed and must have been reported
        rASSERT(x_richUsbError != "");
        return false;
    }

    int nExpectingBytes = C_nTomographGetVersionsResponseBytes;

    // read response
    sbuf sbufResponse = TryReadFromTomograph(nExpectingBytes);

    // Length OK?
    if (ReportSiUsbError(
            "Versions-Response",
            sbufResponse.Len() != nExpectingBytes?
                C_nSiResponseParserError 
                :
                C_nSiSuccess,
            TRANSLATE(
                "Tomograph [... UI-string-removed]");
    }

    int nSignatureAb = (unsigned char) sbufResponse.GetAt(1);
    int nSignatureCd = (unsigned char) sbufResponse.GetAt(3);

    int nForceSignatureAb = debug("Device.Version.ForceSignAb?").InputInt();
    int nForceSignatureCd = debug("Device.Version.ForceSignCd?").InputInt();

    if (nForceSignatureAb)
        nSignatureAb = nForceSignatureAb;
    if (nForceSignatureCd)
        nSignatureCd = nForceSignatureCd;

    // Signatures OK?
    if (ReportSiUsbError(
            "Versions-Response",
            nSignatureAb != 0xab || nSignatureCd != 0xcd?
                C_nSiResponseParserError 
                :
                C_nSiSuccess,
            TRANSLATE(
                "Tomograph [... UI-string-removed]");
    }

    //
    // Digest version info
    //

    int nNewFeaturesVersion = (unsigned char) sbufResponse.GetAt(0);
    int nOldCutOffVersion = (unsigned char) sbufResponse.GetAt(2);

    int nForceNewFeatures = debug("Device.Version.ForceNewFeatures?").InputInt();
    int nForceOldCutOff = debug("Device.Version.ForceOldCutOff?").InputInt();

    if (nForceNewFeatures)
        nNewFeaturesVersion = nForceNewFeatures;
    if (nForceOldCutOff)
        nOldCutOffVersion = nForceOldCutOff;

    str sReport;
    bool bFatalIncompatibility = false;
    if (nNewFeaturesVersion < C_nTomographExpectedNewFeaturesVersion)
    {
        sReport = 
            TRANSLATE(
                "WARNING: T[... UI-string-removed]");

        bFatalIncompatibility = true;
    }
    else
    if (nNewFeaturesVersion > C_nTomographExpectedNewFeaturesVersion)
    {
        sReport = 
            TRANSLATE(
                "WARNING: T[... UI-string-removed]");

        bFatalIncompatibility = false;
    }

    if (nOldCutOffVersion < C_nTomographExpectedOldCutOffVersion)
    {
        sReport = 
            TRANSLATE(
                "ERROR: The[... UI-string-removed]");

        bFatalIncompatibility = true;
    }
    else
    if (nOldCutOffVersion > C_nTomographExpectedOldCutOffVersion)
    {
        sReport = 
            TRANSLATE(
                "ERROR: The[... UI-string-removed]");

        bFatalIncompatibility = true;
    }

    if (sReport != "")
    {
        str sVersionComparison = 
            "" +
                "Tomograph v. " + 
                Str(nNewFeaturesVersion) + "/" +
                Str(nOldCutOffVersion) + ", " + 
                "Tomo software expects v. " + 
                Str(C_nTomographExpectedNewFeaturesVersion) + "/" +
                Str(C_nTomographExpectedOldCutOffVersion);

        if (ReportSiUsbError(
                "Version-Compatibility",
                bFatalIncompatibility?
                    C_nSiResponseParserError 
                    :
                    C_nSiSuccess,
                sReport,
                "/ERR:EVFI/ " + sVersionComparison))
        {
            return false;
        }

        // Just a warning
        x_richUsbWarnings = 
            x_richUsbWarnings + 
                sReport + "\n\n" + sVersionComparison + "\n\n";
    }

    return true;
}

bool CTomographUsb::OnEraseAllTomograms()
{
    // all should be fine until now0
    rASSERT(x_richUsbError == "");

    // Clear the buffer
    TryReadTomographFrame();

    // Send the command
    if (!ExecuteTomographCommand(E_TomographCommand_EraseAll))
    {
        // failed and must have been reported
        rASSERT(x_richUsbError != "");
        return false;
    }
    else
    {
        // a pause to let the device complete all ops
        sys::GSleepInTicks(
            debug("Device.EraseTimeout?/4e").
                InputTickOrSet(C_tickEraseAllTimeout));

        // Rerequest sample count to wait for completion
        int nErasedCount = GetTomogramCountAndStartDownload();

        if (ReportSiUsbError(
                "Parse-Count",
                nErasedCount != 0?
                    C_nSiResponseParserError 
                    :
                    C_nSiSuccess,
                TRANSLATE(
                    "Could not [... UI-string-removed]");
    }
}

[...]