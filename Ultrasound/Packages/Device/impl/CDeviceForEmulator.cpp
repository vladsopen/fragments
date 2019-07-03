// CDeviceForEmulator.cpp

#include "CUsmainProject.h"
#include "CDeviceForEmulator.h"
#include "CRawCapture.h"
#include "CRawCaptureList.h"
#include "CAdjusterTypeImpl.h"
#include "CArgUsoundAdjusterStateGp.h"
#include "VirtOs_ask.h"

CDeviceForEmulator::CDeviceForEmulator()
{
}

void CDeviceForEmulator::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CDeviceForEmulator!");
}

sloc CDeviceForEmulator::OnGetDeviceNameForUi()
{
    NOTEST;

    return "Emulator";
}

sloc CDeviceForEmulator::OnGetUnusualDeviceNote()
{
    return "Uscan Emulator";
}

res CDeviceForEmulator::OnGetLargeDeviceIcon()
{
    NOTEST; 

    return RES__DEMO_PACIFIER_32x32();
}

rich CDeviceForEmulator::OnGetDeviceOnlineDisableInfo()
{
    return __super::OnGetDeviceOnlineDisableInfo();
}

void CDeviceForEmulator::OnStartRecordingInAllCaptures()
{
    // notify all captures
    ref<CRawCapture> rIterRawCapture;
    for (
        iter i;
        x_rRawCaptureList->
            IterateRawCaptures(
                out i,
                out rIterRawCapture);)
    {
        rASSERT(rIterRawCapture->_x_tickAutoplayEmulation == -1);
        rASSERT(rIterRawCapture->x_iPlayingAtComposite == -1);

        if (rIterRawCapture->_x_tickAutoplayEmulation == -1)
        {
            rIterRawCapture->TriggerOnlineModeEmulation();
        }
    }
}

void CDeviceForEmulator::OnStopRecordingInAllCaptures()
{
    // notify all captures
    ref<CRawCapture> rIterRawCapture;
    for (
        iter i;
        x_rRawCaptureList->
            IterateRawCaptures(
                out i,
                out rIterRawCapture);)
    {
        //rASSERT(rIterRawCapture->_x_tickAutoplayEmulation != -1);
        //rASSERT(rIterRawCapture->x_iPlayingAtComposite == -1);

        if (rIterRawCapture->_x_tickAutoplayEmulation != -1)
        {
            rIterRawCapture->TriggerOnlineModeEmulation();
        }
    }
}

void CDeviceForEmulator::OnSetDeviceOnline(
        bool bOnline)
{
    bOnline;
}

ref<CArgUsoundAdjusterStateGp> CDeviceForEmulator::OnGetSlotAdjusterForSliders(
        int iChannel,
        type<CAdjusterTypeImpl> typeAdjuster)
{
    iChannel;

    ref<CArgUsoundAdjusterStateGp> rArgUsoundAdjusterState;

    if (!IsDeviceInitOk())
    {
        rArgUsoundAdjusterState->_x_richAdjusterValueForUi = "<no-init>";

        return rArgUsoundAdjusterState;
    }

    rArgUsoundAdjusterState->_x_richAdjusterValueForUi = "<invaid-call>";

    if (!typeAdjuster->IsAdjustedWithSlider())
    {
        if (typeAdjuster.IsSelecting(type<CAdjusterForProbe>()))
        {
            rFAIL("GetSlotAdjuster does not support probe, call specific methods");
        }
        else
        if (typeAdjuster.IsSelecting(type<CAdjusterForAgc>()))
        {
            rFAIL("GetSlotAdjuster does not support AGC, call specific methods");
        }
        else
        {
            rFAIL("unknown non-slider Adjuster, update Wire link!");
        }

        return rArgUsoundAdjusterState;
    }

    num numMetricValue = 
        GetEmulatedAdjuster(typeAdjuster) * typeAdjuster->GetEmulatorMultiplier();
    /*
    if (numMetricValue <= 1.0)
    {
        numMetricValue = 1.0;
    }
    */

    rArgUsoundAdjusterState->_x_richAdjusterValueForUi = Str(numMetricValue);
    rArgUsoundAdjusterState->_x_iAdjusterInternalPos = GetEmulatedAdjuster(typeAdjuster);
    rArgUsoundAdjusterState->_x_numAdjusterMm = rArgUsoundAdjusterState->_x_iAdjusterInternalPos;
    rArgUsoundAdjusterState->_x_numAdjusterHz = 
        rArgUsoundAdjusterState->_x_iAdjusterInternalPos * 1000;
    rArgUsoundAdjusterState->_x_nAdjusterInternalRange = 10;
    rArgUsoundAdjusterState->_x_slocAdjusterUiUnits = "em";
    rArgUsoundAdjusterState->_x_nAdjusterInternalSmallStep = 1;
    rArgUsoundAdjusterState->_x_nAdjusterInternalLargeStep = 1;

    rASSERT(rArgUsoundAdjusterState->_x_nAdjusterInternalRange > 0);
    rASSERT(rArgUsoundAdjusterState->_x_iAdjusterInternalPos >= 0);
    rASSERT(
        rArgUsoundAdjusterState->_x_iAdjusterInternalPos 
            < 
            rArgUsoundAdjusterState->_x_nAdjusterInternalRange);

    return rArgUsoundAdjusterState;
}

void CDeviceForEmulator::OnSetSlotAdjusterForSliders(
        int iChannel,
        type<CAdjusterTypeImpl> typeAdjuster,
        int iSetInternalPos)
{
    iChannel;

    if (!IsDeviceInitOk())
    {
        rFAIL("SetSlotAdjuster called for uninitialized Emulator");
        return;
    }

    if (!typeAdjuster->IsAdjustedWithSlider())
    {
        if (typeAdjuster.IsSelecting(type<CAdjusterForProbe>()))
        {
            rFAIL("SetSlotAdjuster does not support probe, call specific methods");
        }
        else
        if (typeAdjuster.IsSelecting(type<CAdjusterForAgc>()))
        {
            rFAIL("GetSlotAdjuster does not support AGC, call specific methods");
        }
        else
        {
            rFAIL("unknown non-slider Adjuster, update Wire link!");
        }

        return;
    }

    SetEmulatedAdjuster(typeAdjuster, iSetInternalPos);
        //math::GDivNot0(
            //iSetInternalPos,
            //typeAdjuster->GetEmulatorMultiplier(),
            //or_return 0);
}

bool CDeviceForEmulator::OnIsSlotAdjusterVisibleForCurrentProbe(
        int iChannel,
        type<CAdjusterTypeImpl> typeAdjuster)
{
    iChannel;
    typeAdjuster;

    return true;
}

bool CDeviceForEmulator::OnIterateDeviceProbes(
        int iChannel,
        out iter& out_i,
        out int& out_iProbe,
        out str& out_sProbeForMenu,
        out str& out_sDisableInfo)
{
    iChannel;

    int iNext = out_i._internal_GetIndex();

    // On the first iteration (iNext == -1)
    iNext++;

    // Any elements left?
    if (iNext < C_nEmulateProbes)
    {
        out_iProbe = iNext;
        out_sProbeForMenu = Str(iNext) + "Em MHz";
        out_sDisableInfo = (iNext % 3)? "Test disable" : "";

        out_i._internal_SetIndex(iNext);
        return true;
    }
    else
    {
        return false;
    }
}

void CDeviceForEmulator::OnSelectDeviceProbeByIndex(
        int iChannel,
        int iProbe)
{
    iChannel;

    if (!IsDeviceInitOk())
    {
        rFAIL("SelectWireProbeByIndex called for uninitialized Emulator");
        return;
    }
[...]

void CDeviceForEmulator::OnTestClass()
{
    CTA_ONLY(OnTestClass);

    // adjuster test

    assertoff aoAgcBoolRange("iSetInternalPos == 0 || iSetInternalPos == 1");
    assertoff aoOffRange("_x_iAdjusterInternalPos < rArgUsoundAdjusterState->_x_nAd");
    assertoff aoNoSession("!IsNull()");

    InitDeviceForNewSession();

    repeat(iChannel, C_nPreallocateChannels)
    {
        TESTLOG(
            "",
            "\n"
                "Test Adjusters for iChannel = " + Str(iChannel) + "\n");

        SetDeviceAgc(!IsDeviceAgcActivated());

        type<CAdjusterTypeIfaceGp> typeIterAdjusterTypeIface;
        for (
            iter i;
            type<CAdjusterTypeIfaceGp>()->GetDerivedList().
                Iterate(
                    out i,
                    out typeIterAdjusterTypeIface);)
        {
            type<CAdjusterTypeImpl> typeIterAdjusterType = 
                typeIterAdjusterTypeIface;

            ref<CArgUsoundAdjusterStateGp> rArgUsoundAdjusterState = 
                GetDeviceChannelAdjuster(
                    iChannel,
                    typeIterAdjusterType);

            TESTLOG(
                "",
                "Adjuster: " + 
                    typeIterAdjusterType->GetAdjusterNameForUi() + 
                    " at " + 
                    Str(rArgUsoundAdjusterState->_x_iAdjusterInternalPos) + 
                    " == " +
                    rArgUsoundAdjusterState->_x_richAdjusterValueForUi +
                    " < " +
                    Str(rArgUsoundAdjusterState->_x_nAdjusterInternalRange) + 
                    "\n");

            SetDeviceChannelAdjuster(
                iChannel,
                typeIterAdjusterType,
                rArgUsoundAdjusterState->_x_iAdjusterInternalPos + 1);
        }
    }

    CloseDeviceAtSessionEnd();
}
