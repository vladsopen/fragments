// CUcomDriverSlotForEmulator.cpp

#include "CProject.h"
#include "CUcomDriverSlotForEmulator.h"
#include "CUcomDriverDeviceForEmulator.h"

CUcomDriverSlotForEmulator::CUcomDriverSlotForEmulator()
{
}

void CUcomDriverSlotForEmulator::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CUcomDriverSlotForEmulator!");
}

str CUcomDriverSlotForEmulator::OnReportFullSlotInfoForDebugAndAutoRefresh()
{
    str sReport = 
        __super::OnReportFullSlotInfoForDebugAndAutoRefresh();

    sReport += " Node " + Str(x_iDetectedDeviceIndex) + ". ";
    return sReport;
}

ref<CUcomDriverDeviceIfaceGp> CUcomDriverSlotForEmulator::OnNewUcomDeviceForSlot(
        ref<CUcomDeviceIfaceGp> rUcomDevice,
        out str& out_sError)
{
    ref<CUcomDriverDeviceForEmulator> rNewUcomDriverDeviceForEmulator;

    // Here we open the device connecting it to its slot
    rNewUcomDriverDeviceForEmulator->
        ConnectToEmulatorSlot(
            this,
            rUcomDevice,
            out out_sError);

    return rNewUcomDriverDeviceForEmulator;
}

