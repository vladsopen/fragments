// CUcomDriverEnumeratorForEmulator.cpp

#include "CProject.h"
#include "CUcomDriverEnumeratorForEmulator.h"
#include "CUcomDetectionCallbackIfaceGp.h"
#include "CUcomEmulatorCommon.h"
#include "VirtOs_text.h"
#include "CUcomDriverSlotForEmulator.h"
#include "CUcomBrandIfaceGp.h"
#include "CUcomEnumeratorIfaceGp.h"

CUcomDriverEnumeratorForEmulator::CUcomDriverEnumeratorForEmulator()
{
}

void CUcomDriverEnumeratorForEmulator::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CUcomDriverEnumeratorForEmulator!");
}

str CUcomDriverEnumeratorForEmulator::OnGetUcomDriverProtocolName()
{
    // Short unique protocol name for debug identification
    return "Emulator";
}

void CUcomDriverEnumeratorForEmulator::OnEnumerateUcomDriverDeviceSlots(
        ref<CUcomEnumeratorIfaceGp> rUcomEnumerator,
        out str& out_sError)
{
    rASSERT(!_m_bEnumerateEmulatorCalled);
    _m_bEnumerateEmulatorCalled = true;

    //
    // Default Reports
    //

    str sErrorStub = "Unexpected Emulator scan abort /ERR:UFTU/";
    out_sError = sErrorStub;

    //
    // Get Your Device Info List 
    //   and
    //   1) Create a new slot for every device:
    //       ref<CUcomDriverSlotForEmulator> rNewUcomDriverSlot;
    //   2) Store ID info into the slot object
    //       rNewUcomDriverSlot->
    //           x_rUcomBrand->
    //               InitUcomBrandStringList(...)
    //   3) Report the slot to the enumerator
    //        rUcomEnumerator->
    //            ReportNextDeviceSlotInfoDuringEnumeration(...);
    //
    //
    // Note: if no devices found and no critical system errors detected 
    //   then just return an empty out_sError
    //if (dwEmulatorDevices == 0)
    //{
    //    out_sError = "";
    //    return;
    //}

    //
    // Initialize device list scan
    //

    str sInitError;
    //HANDLE hDeviceClassEnum = INVALID_HANDLE_VALUE;
    InitEmulatorDeviceList(
        out sInitError,
        //out hDeviceClassEnum,
        rUcomEnumerator);
    if (sInitError != "")
    {
        out_sError = sInitError;
        return;
    }

    //rASSERT(hDeviceClassEnum != INVALID_HANDLE_VALUE);

    //
    // Loop through the device list (repeat() may not work for some device types)
    //

    int nFoundDevices = 0;
    int iIter = 0;
    repeat(iDevice, CUcomDriverEnumeratorForEmulator::C_nMaxEmulatorDevicesInEnum)
    {
        // loop breaker
        nFoundDevices++;
        if (nFoundDevices >= oo)
        {
            out_sError = 
                "Too many devices " + 
                    Str(iDevice) + 
                    "/ERR:HIGE/";
            break;
        }

        //
        // Next Device Info
        //

        text textBrand = text();
        int nDeviceVendorId = 0;
        int nDeviceProductId = 0;
        str sDevicePath;
        if (!IterateEmulatorDevices(
                //hDeviceClassEnum,
                iDevice,
                out textBrand,
                out nDeviceVendorId,
                out nDeviceProductId,
                out sDevicePath,
                out out_sError,
                rUcomEnumerator))
        {
            break;
        }

        // Could not get any info for the device?
        if (textBrand.GetWordCount() == 0 &&
            nDeviceVendorId == 0 &&
            nDeviceProductId == 0 &&
            sDevicePath == "")
        {
            continue;
        }

        //
        // Detect our device
        //

        // Create new slot info to report to Ucom
        ref<CUcomDriverSlotForEmulator> rNewUcomDriverSlot;

        // Fill-in the brand info
        ref<CUcomBrandIfaceGp> rUcomBrandFound =
            rNewUcomDriverSlot->
                x_rUcomBrand;

        // Every device enumerator should add all strings it can dig up

        //text textBrand;
        //textBrand.AddWord(dataNode.Description);
        //textBrand.AddWord(dataNode.SerialNumber);

        rUcomBrandFound->
            InitUcomBrandStringList(
                textBrand,
                GetUcomDriverProtocolName());
        rUcomBrandFound->x_nDeviceVendorId = nDeviceVendorId;
        rUcomBrandFound->x_nDeviceProductId = nDeviceProductId;

        // Note: for Emulator there is no need to report individual errors
        //   for specific devices slots.
        rNewUcomDriverSlot->x_sSlotDetectionError = "";

        // Emulator-specific -- Emulator opens device by index
        rNewUcomDriverSlot->x_iDetectedDeviceIndex = iDevice;
        rNewUcomDriverSlot->x_sDetectedDevicePath = sDevicePath;

        // extra debug info
        str sLog = 
            "Emulator index " + Str(iDevice) + " " +
                //"flags=0x" + Hex((int) dataNode.Flags) + ", " +
                //"type=0x" + Hex((int) dataNode.Type) + ", " +
                //"h=0x" + Hex((int) dataNode.ftHandle) + "."
                " ";

        rUcomEnumerator->
            ReportNextDeviceSlotInfoDuringEnumeration(
                iIter++,
                rNewUcomDriverSlot,
                sLog);
    }

    rUcomEnumerator->
        LogScanProgress(
            "\n"
                "Checked " + 
                Str(nFoundDevices) + 
                " Emulator devices.\n" + 
                "\n");

    //
    // Cleanup
    //

    //if ok(hDeviceClassEnum != INVALID_HANDLE_VALUE)
    //{
    //    SetupDiDestroyDeviceInfoList(hDeviceClassEnum);
    //}

    // Done
    rUcomEnumerator->
        LogScanProgress(
            "Emulator detection complete.\n");

    if (out_sError == sErrorStub)
    {
        // no error reports
        out_sError = "";
    }

    // no critical errors
    //   (see also x_sSlotDetectionError)
    out_sError = "";
}

void CUcomDriverEnumeratorForEmulator::InitEmulatorDeviceList(
        out str& out_sInitError,
        //out HANDLE& out_hDeviceClassEnum,
        ref<CUcomEnumeratorIfaceGp> rUcomEnumerator)
{
    //
    // Init device list
    //

    //_m_dataHidGuid = GUID();
    //HidD_GetHidGuid(&_m_dataHidGuid);    

    if (m_asEmulatedDeviceNames.GetCount() == 0)
    {
        m_asEmulatedDeviceNames.Add("SomeDevice");
        m_asEmulatedDeviceNames.Add("Medm CNUS_01 HID 12345");
        m_asEmulatedDeviceNames.Add("another device");
    }

    //out_hDeviceClassEnum = 
        //SetupDiGetClassDevs()

    if (m_asEmulatedDeviceNames.GetCount() == 0)
    {
        out_sInitError =
            "Failed to allocate device enum /ERR:HDIE/ " + 
                //sys::GGetLastOsError()
                "";

        return;
    }

    rUcomEnumerator->
        LogScanProgress(
            "Emulator device enumeration started (" + 
                Str(m_asEmulatedDeviceNames.GetCount()) + 
                " devices)."
                "\n");
}

bool CUcomDriverEnumeratorForEmulator::IterateEmulatorDevices(
        //HANDLE hDeviceClassEnum,
        int iDevice,
        out text& out_textBrand,
        out int& out_nDeviceVendorId,
        out int& out_nDeviceProductId,
        out str& out_sDevicePath,
        out str& out_sError,
        ref<CUcomEnumeratorIfaceGp> rUcomEnumerator)
{
    out_sError = "";

    //
    // Query Next Device
    //

    str sDevice = 
        m_asEmulatedDeviceNames.GetAtOr(iDevice, "");

    if (sDevice == "")
    {
        return false; // end iterator
    }

    //
    // Device Details
    //

    out_textBrand.AddWord(sDevice);
    out_nDeviceVendorId = 1000 + iDevice; // emulator!
    out_nDeviceProductId = 2000 + iDevice; // emulator!
    out_sDevicePath = sDevice; // emulator!
    str sDeviceDetailedReport = "My fake device.";

    if (out_textBrand.GetWordCount() == 0)
    {
        rUcomEnumerator->
            LogScanProgress(
                "\n - Failed to get device [" + 
                    Str(iDevice) + 
                    "] details" + 
                    "\n");

        return true; // continue iterator
    }

    //
    // Report findings
    //

    rUcomEnumerator->
        LogScanProgress(
            "\n - Found [" + 
                Str(iDevice) + 
                "] " + 
                Hex00(out_nDeviceVendorId, 4) + 
                ":" + 
                Hex00(out_nDeviceProductId, 4) + 
                "\n   " +                        
                out_textBrand.CatenateTextSeparatingWith(" / ") +
                "\n   " +                        
                sDeviceDetailedReport +
                "\n");

    return true;
}

