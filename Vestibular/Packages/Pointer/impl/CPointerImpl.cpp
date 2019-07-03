// CPointerImpl.cpp

#include "CProject.h"
#include "CPointerImpl.h"
#include "CReadingIface.h"
#include "CSkinizerCommonIfaceGp.h"
#include "CRangesIface.h"
#include "CPatientLinkIface.h"
#include "CSessionIface.h"
#include "CPointOffset.h"
#include "CGlobalConfigIface.h"
#include "CColorizerCommonIfaceGp.h"
#include "CAcousticsIface.h"
#include "CDbReadingIface.h"

CPointerImpl::CPointerImpl()
{
    _init_AttachProperties();
}

void CPointerImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachProperties();

    rFAIL("make sure it is OK to expose CPointerImpl!");
}

void CPointerImpl::_init_AttachProperties()
{
    x_rPointOffset->_x_pPointer = this;
}

static void GCalcUpdatedAverage(
        out num& out_numAverageValue,
        num numNextValue,
        num numDeltaJumpFilter)
{
    int nAverageWindow = debug("Indication.Filter?/4e").InputIntOrSet(30);

    num numDelta = math::GAbs(numNextValue - out_numAverageValue);

    /*
    nAverageWindow = 
        math::GDiv0(
            nAverageWindow,
            int(0.3 * numDelta),
            or_return nAverageWindow)
            + 
            1;
            */

    out_numAverageValue =
        math::GDivNot0(
            out_numAverageValue * (nAverageWindow - 1) + numNextValue,
            (num) nAverageWindow,
            or_return numNextValue);

    // fast go to zero
    if (math::GAbs(numNextValue) < 1.0)
    {
        out_numAverageValue = 0;
    }
}

bool CPointerImpl::GScalePositionToRange(
        num numNewRangeX,
        num numNewRangeY,
        ref<CReadingIface> rReading,
        ref<CRangesIface> rRanges,
        out num& out_numPosX,
        out num& out_numPosY)
{
    num numRangeX = 
        rRanges->x_numMaxXmm - rRanges->x_numMinXmm;

    if (numRangeX < 1)
    {
        return false;
    }

    out_numPosX = 
        numNewRangeX / 2 + 
            math::GMulDiv(
                rReading->GetVirtualXmm(rRanges),
                (num) numNewRangeX,
                numRangeX);

    num numRangeY = 
        rRanges->x_numMaxYmm - rRanges->x_numMinYmm;

    if (numRangeY < 1)
    {
        return false;
    }

    out_numPosY = 
        numNewRangeY / 2 + 
            math::GMulDiv(
                rReading->GetVirtualYmm(rRanges),
                (num) numNewRangeY,
                numRangeY);

    return true;
}

void CPointerImpl::OnReceiveReadingForPointer(
        ref<CReadingIface> rReading,
        ref<CRangesIface> rRanges)
{
    m_rLastReading = rReading;
    m_rRanges = rRanges;

    // smooth indication

    GCalcUpdatedAverage(
        out m_numAveragedKiloInternal,
        rReading->x_numKilo,
        C_numDeltaJumpFilterKilo);

    if (m_numAveragedKiloInternal < 1)
    {
        m_numAveragedKiloInternal = 0;
    }

    // dont flick the kilo too often
    if (sys::GGetUtc() - _m_tickLastKiloIndicationUpdate < 300)
    {
        // collect interval average
        m_numAveragedKiloInternalSum += m_numAveragedKiloInternal;
        m_nAveragedKiloInternalSumCount++;
    }
    else
    {
        // commit average for indication
        _m_tickLastKiloIndicationUpdate = sys::GGetUtc();
        m_numAveragedKiloIndication = 
            math::GDiv0(
                m_numAveragedKiloInternalSum,
                (num) m_nAveragedKiloInternalSumCount,
                or_return m_numAveragedKiloInternal);

        // start new average iuterval
        m_nAveragedKiloInternalSumCount = 0;
        m_numAveragedKiloInternalSum = 0;
    }

    if (m_numAveragedKiloInternal < 1)
    {
        m_numAveragedKiloIndication = 0;
    }

    GCalcUpdatedAverage(
        out m_numAveragedXmm,
        rReading->GetVirtualXmm(rRanges),
        C_numDeltaJumpFilterPos);

    GCalcUpdatedAverage(
        out m_numAveragedYmm,
        rReading->GetVirtualYmm(rRanges),
        C_numDeltaJumpFilterPos);

    if (!rReading->_x_bBatchReading)
    {
        SendToOffsetAcoustics(
            rReading,
            rRanges);

        _m_ptrmapUiPointerGraph.UpdateTreeState();
        _m_ptrmapUiPointerGraph.UpdatePaint();
    }
}

void CPointerImpl::SendToOffsetAcoustics(
        ref<CReadingIface> rReading,
        ref<CRangesIface> rRanges)
{
    // Send offset to acoustics
    ref<CSessionIface> rSession =
        _x_pSession.Get().Valid();
    ref<CAcousticsIface> rAcoustics = 
        rSession->x_rAcoustics;

    //
    // calculate positions in comparative scale
    //
    //const num C_numScale = 10000;

    /*
    // pointer position scaled
    num numPosX = 0;
    num numPosY = 0;
    if (!GScalePositionToRange(
            C_numScale,
            C_numScale,
            rReading,
            rRanges,
            out numPosX,
            out numPosY))
    {
        return;
    }
    */

    // offset position scaled
    num numOffsetXmm = 0;
    num numOffsetYmm = 0;
    if (!x_rPointOffset->
            ConvertUmovePositionToReadingRange(
                rRanges,
                out numOffsetXmm,
                out numOffsetYmm))
    {
        return;
    }

    //
    // calculate delta
    //
    num numDeltaX = 
        numOffsetXmm - rReading->GetVirtualXmm(rRanges);
    num numDeltaY = 
        numOffsetYmm - rReading->GetVirtualYmm(rRanges);
    num numDelta = 
        math::GSqrt(numDeltaX * numDeltaX + numDeltaY * numDeltaY);

    //
    // send out to play
    //

    rAcoustics->
        PlayPointerPlayOffset(
            numDelta);

    nothing();
}

str CPointerImpl::GetLastReadingReport()
{
    return m_rLastReading->GetReadingDebugReport(m_rRanges);
}

str CPointerImpl::GetXmmReport()
{
    return 
        "x: " + 
            RichBold(Str((int) ( 
                m_numAveragedXmm - 
                    m_rRanges->x_numAddedToXmm)));
}

str CPointerImpl::GetYmmReport()
{
    return 
        "y: " + 
            RichBold(Str((int) (
                m_numAveragedYmm - 
                    m_rRanges->x_numAddedToYmm)));
}

str CPointerImpl::GetShiftReport()
{
    return 
        "" +
            TRANSLATE(
                "offset[... UI-string-removed]");
}

void CPointerImpl::DrawPointerGraph(
        draw dc,
        ref<CUiPanelGp> rUi)
{
    DrawPointerBack(dc, rUi);
    DrawIdealPressurePoint(dc, rUi);
    DrawPointerPosition(dc, rUi);

    // draw Demo marker
    ref<CGlobalConfigIface> rGlobalConfig = 
        CProject::GGetIt().x_rGlobalConfig;
    //if (rGlobalConfig->_x_bDeviceSignalEmulator)
    {
        dc.SetBkMode(TRANSPARENT);
        dc.SetTextColor(rgb(255, 0, 0));
        dc.SetTextAlign(TA_RIGHT | TA_BOTTOM);
        dc.OutText(
            rUi->GetInstantSize().w - 5, 
            rUi->GetInstantSize().h - 2, 
            //"DEMO");
            rGlobalConfig->
                _x_sDeviceLicensingStatus.Get().
                    GetStub(
                        rGlobalConfig->_x_bDeviceSignalEmulator?
                            "demo"
                            :
                            ""));
    }
}

[...]