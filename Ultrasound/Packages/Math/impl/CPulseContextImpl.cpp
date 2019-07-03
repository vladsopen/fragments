// CFlowContextImpl.cpp
// ("CFlowContextImpl.h")

#include "CDucalcProject.h"
#include "CFlowContextImpl.h"
#include "CFlowCurveImpl.h"
#include "CRangeDetector.h"
#include "CMediaDocIfaceGp.h" //(dont_uninclude)
#include "CTunerProfileList.h"
#include "CTunerProfile.h"
#include "CTunerHistory.h"
//#include "CCalcNextFlowSliceImpl.h"
#include "CTunerValueMutator.h"
#include "CTunerValueSetting.h"
#include "CLayerSequence.h"
#include "CLayerStats.h"
#include "CCalcNextFlowSliceIface.h"
#include "SuperSharedUsoundStreams.h"

//**************************************************
// all graphics are experimental debug-only quality!
//**************************************************

CFlowContextImpl::CFlowContextImpl()
{
    // no data
}

void CFlowContextImpl::HandleCurveChange()
{
    _m_pUiFlowImpl.DefaultIfNull()->InvalidateState();
    _m_pUiFlowImpl.DefaultIfNull()->InvalidatePaint();
}

ref<CFlowCurveIfaceGp> CFlowContextImpl::OnGetInputMediaCurve()
{
    return x_rMediaCurveCache;
}

int CFlowContextImpl::OnGetFlowPostFilterCalcDelaySlices()
{
    ref<CTunerProfile> rTunerProfile = GetActiveProfile();

    // Filter setting dictate the length
    int nPostFilterHistoryPoints =
        rTunerProfile->x_rPostFilterHistoryPoints->
            GetTuneSettingAsInt();

    return nPostFilterHistoryPoints;
}

ref<CFlowCurveIfaceGp> CFlowContextImpl::OnGetRecentHistoryCurve()
{
    ref<CFlowCurveImpl> rFlowCurveHistoryCache = 
        x_rFlowCurveHistoryCache;

    ref<CTunerProfile> rTunerProfile = GetActiveProfile();

    // Filter setting dictate the length
    int nPostFilterHistoryPoints = GetFlowPostFilterCalcDelaySlices();

    rFlowCurveHistoryCache->
        SetWorkingCount(
            nPostFilterHistoryPoints);

    return rFlowCurveHistoryCache;
}

void CFlowContextImpl::DrawCalculationResult(
        draw dc,
        int nWidth,
        int nHeight)
{
    // Original image scale pixel-to-point
    int nOriginalWidth = x_rSourceCurve->GetWorkingCount();
    int nOriginalHeight = C_nDucalcMediaHeightRange;

    // Skip empty data
    if (nOriginalWidth <= 0)
    {
        dc.OutText(0, 0, "Empty curve");
        return;
    }

    // Create temp picture best fitting original pixel resolution
    ref<CPicture> rFixedScalePicture;
    rFixedScalePicture->SetResolution(
        size(nOriginalWidth, nOriginalHeight));

    // Clear the background
    rFixedScalePicture->
        DrawBar(
            rect(0, 0, size(nOriginalWidth, nOriginalHeight)), 
            rgb(255, 255, 253));

    // Draw into the temp picture first
    draw dcFixedScalePicture = rFixedScalePicture->GetDc(dc);
    DrawCalculationResultAsFixedScalePicture(
        dcFixedScalePicture,
        nOriginalWidth,
        nOriginalHeight);

    // Calc best-fitting scale for uniform pixel sizes
    int nUniformWidth = CalcUniformExtent(nOriginalWidth, nWidth);
    int nUniformHeight = CalcUniformExtent(nOriginalHeight, nHeight);

    // Scale the picture into view
    rFixedScalePicture->BlitToDc(
        dc, 
        MAX_ALPHA, 
        point(0, 0), 
        size(nUniformWidth, nUniformHeight));
}

void CFlowContextImpl::DrawCalculationResultAsFixedScalePicture(
        draw dc,
        int nWidth,
        int nHeight)
{
    // (Debug-only drawing)

    int nClusterSize = 0; //GetClusterSize();
    int nExtendedCount = x_rSourceCurve->GetWorkingCount(); //GetExtendedCount();
    int nLeftMarginX = 0; //math::GMulDiv<int>(GetLeftMargin(), nWidth, nExtendedCount);
    int nRightMarginX = 0; //math::GMulDiv<int>(GetRightMargin(), nWidth, nExtendedCount);

    // Precalculate everything
    int iNoisePosX = 
        GetActiveProfile()->x_rRecentFlowPos->GetCurrentTuneMutationAsInt();

    // Cluster discontinuity margins
    dc.SetBrush(rgb(255, 245, 253));
    dc.Rectangle(
        0, 
        0, 
        math::GMulDiv<int>(nClusterSize, nWidth, nExtendedCount), 
        nHeight);
    dc.Rectangle(
        math::GMulDiv<int>(nExtendedCount - nClusterSize, nWidth, nExtendedCount), 
        0, 
        nWidth, 
        nHeight);

    // Curve draw area
    rect rectFitCurvesInto(0, 0, nWidth, nHeight);

    /*
    // Raising ranges
    {
        int nScaleLineCut = m_apointDebugRaising.GetCount() * 2;
        repeat(i, m_apointDebugRaising.GetCount())
        {
            int nLight = 150 + math::GMulDiv<int>(i, 100, m_apointDebugRaising.GetCount());
            dc.SetPen(PS_SOLID, 1, rgb(nLight, nLight, 0));
            dc.Line(
                math::GMulDiv<int>(m_apointDebugRaising[i].x, nWidth, nExtendedCount), 
                math::GMulDiv<int>(i, nHeight, nScaleLineCut), 
                draw::equal, 
                nHeight);
            dc.SetPen(PS_SOLID, 1, rgb(0, nLight, nLight));
            dc.Line(
                math::GMulDiv<int>(m_apointDebugRaising[i].y, nWidth, nExtendedCount), 
                0, 
                draw::equal, 
                math::GMulDiv<int>(nHeight - i, nHeight, nScaleLineCut));
        }
    }
    */

    /*
    // Noise
    {
        dc.SetPen(PS_SOLID, 1, rgb(190, 190, 100));
        dc.Line(0, nHeight - x_nNoiseLevel - 1, nWidth, draw::equal);
    }
    */

    // Max point
    dc.SetPen(PS_SOLID, 1, rgb(255, 0, 0));
    dc.Line(
        math::GMulDiv<int>(iNoisePosX, nWidth, nExtendedCount),
        nHeight / 10, 
        draw::equal, 
        nHeight - nHeight / 10);

    // Working Curve 2
    {
        dc.SetPen(PS_SOLID, 1, rgb(255, 0, 255));
        x_rPreparedCurveForEdgeSwingAdjust->DrawCurve(
            dc, rectFitCurvesInto, 0, x_nAmplitudeRange);
    }

    // Source Curve
    {
        dc.SetPen(PS_SOLID, 1, rgb(0, 0, 255));

        rect rectFitSourceCurveInto = rectFitCurvesInto;
        rectFitSourceCurveInto.left = nLeftMarginX;
        rectFitSourceCurveInto.right -= nRightMarginX;

        x_rSourceCurve->DrawCurve(
            dc, rectFitSourceCurveInto, 0, x_nAmplitudeRange);
    }

    // Working Curve 1
    {
        dc.SetPen(PS_SOLID, 1, rgb(0, 255, 255));

        int nSimplisticDifferentiationFilter = 
            GetActiveProfile()->x_rSimplisticDifferentiationFilter->
                GetTuneSettingAsInt();

        x_rSourceCurve->
            RunSquareFilter(
                x_rPreparedCurveWithStrongFilter, 
                nSimplisticDifferentiationFilter);

        x_rPreparedCurveWithStrongFilter->DrawCurve(
            dc, rectFitCurvesInto, 0, x_nAmplitudeRange);
    }

    /*
    // Diff
    {
        dc.SetPen(PS_SOLID, 1, rgb(0, 255, 0));
        x_rSourceCurveDiff->DrawCurve(
            dc, 
            rectFitCurvesInto, 
            CFlowCurveIfaceGp::C_numAutoMinMax, 
            CFlowCurveIfaceGp::C_numAutoMinMax);
    }
    */

    /*
    // Ratio
    {
        dc.SetPen(PS_SOLID, 1, rgb(180, 130, 130));
        x_rRatioCurve->DrawCurve(
            dc, 
            rectFitCurvesInto, 
            CFlowCurveIfaceGp::C_numAutoMinMax, 
            CFlowCurveIfaceGp::C_numAutoMinMax);
    }

    // LocalRatioSum
    {
        dc.SetPen(PS_SOLID, 1, rgb(240, 220, 220));
        x_rLocalRatioSumCurve->DrawCurve(
            dc, 
            rectFitCurvesInto, 
            CFlowCurveIfaceGp::C_numAutoMinMax, 
            CFlowCurveIfaceGp::C_numAutoMinMax);
    }
    */
}

void CFlowContextImpl::ScaleToSize(
        int nWidth,
        int nHeight)
{
    x_nAmplitudeRange = math::GMax(1, nHeight);
    x_rSourceCurve->SetWorkingCount(nWidth);

    HandleCurveChange();
}

void CFlowContextImpl::HandleMouse(
        ref<CUiPanelGp> rUi,
        EMouse eMouse,
        int iMousePosX,
        int iMousePosY,
        int nWidth,
        int nHeight)
{
    if (eMouse == E_Mouse_LeftButtonDown)
    {
        m_iPrevMouseX = iMousePosX;
        rUi->SetMouseInputCapture(true);
    }
    else
    if (eMouse == E_Mouse_LeftButtonUp)
    {
        rUi->SetMouseInputCapture(false);
        m_iPrevMouseX = INT_MAX;
    }
    else
    if (eMouse == E_Mouse_Move)
    {
        if (sys::GIsAccess(...))
        {
            //int nDeltaX = posMouseOnScreen.x - s_iScreenStartX;
            //int nClusterSize = GetClusterSize();
            //int nLeftMarginX = GetLeftMargin();
            //int nRightMarginX = GetRightMargin();
            int nExtendedCount = x_rSourceCurve->GetWorkingCount(); 
                //GetExtendedCount();

            //x_rSourceCurve->SetWorkingCount(nWidth - (nLeftMargin + nRightMargin));
            //int nExtendedCount = GetExtendedCount();

            // Calc prev point
            int iOtherX = m_iPrevMouseX;
            if (iOtherX == INT_MAX)
                iOtherX = iMousePosX;
            m_iPrevMouseX = iMousePosX;
            int iDrawX = iMousePosX;

            // Prepare to intepolate amplitude
            int nMouseAmplitude = 
                math::GMulDiv<int>(nHeight - iMousePosY - 1, x_nAmplitudeRange, nHeight);
            int nPrevAmplitude = (int)
                x_rSourceCurve->GetVirtualAmplitudeAt(
                    math::GMulDiv<int>(iOtherX, nExtendedCount, nWidth));

            // Draw from prev to current pos
            int nPoints = math::GAbs(iOtherX - iMousePosX) + 1;
            repeat(i, nPoints)
            {
                int nAmplitude =
                    math::GMulDiv<int>(nMouseAmplitude, nPoints - i, nPoints) +
                        math::GMulDiv<int>(nPrevAmplitude, i, nPoints);

                if (iDrawX >= 0 && iDrawX < nWidth)
                {
                    x_rSourceCurve->
                        SetVirtualAmplitudeAt(
                            math::GMulDiv<int>(iDrawX, nExtendedCount, nWidth), 
                            nAmplitude);
                }
                iDrawX += (iOtherX < iMousePosX? -1 : 1)
            }

            HandleCurveChange();
        }
    }
}

ref<CTunerProfile> CFlowContextImpl::GetActiveProfile()
{
    ref<CTunerProfileList> rTunerProfileList = GetGlobalTunerProfileList();

    // Bottom x_rFlowDown always uses x_rMirrorTuner
    //  (we don't need to debug it)
    if (x_bUseMirrorTuner)
        return rTunerProfileList->x_rMirrorTuner;

    // Choose a link
    link<CTunerProfile, memberid> linkTunerProfile = 
        x_bUseCompareProfile?
            rTunerProfileList->x_linkCompareTunerProfile
            :
            rTunerProfileList->x_linkActiveTunerProfile;

    // Query existing selection
    ptr<CTunerProfile> pOurTunerProfile = 
        linkTunerProfile.
            FindPtr(rTunerProfileList);

    if not_null(ref<CTunerProfile> rOurTunerProfile = pOurTunerProfile)
    {
        return rOurTunerProfile;
    }
    else
    {
        FAIL("no Active tuner profile selected, we expect "
                "x_link(Active/Compare)TunerProfile "
                "to always be a non-null links");
        return null();
    }
}

void CFlowContextImpl::OnHandleDrawMediaHistogramPosChange(
        int iHistogramSourcePos)
{
    if (iHistogramSourcePos != m_nPrevHistogramSourcePos)
    {
        m_nPrevHistogramSourcePos = iHistogramSourcePos;
        x_rTunerHistory->x_iViewOffset = iHistogramSourcePos;

        _m_pUiFlowImpl.DefaultIfNull()->InvalidatePaint();
    }
}

void CFlowContextImpl::OnResetAndClearContent()
{
    // Reset the history
    this->x_rTunerHistory->RemoveAll();
    GetActiveProfile()->HandleRestartReset();
    GetGlobalTunerProfileList()->HandleRestartReset();
}

int CFlowContextImpl::OnCalcFlowPos(
        ref<CFlowCurveIfaceGp> rCurve)
{
    ref<CCalcNextFlowSliceIface> rCalcNextFlowSlice;

    return 
        rCalcNextFlowSlice->CalcFlowPos(
            rCurve,
            this);
}

ref<UiContainer> CFlowContextImpl::OnNewMainFlowTestPaintUi(
        ref<CMediaDocIfaceGp> rMedia)
{
    ref<UiFlowImpl> sub;

    sub->m_rFlow = this;
    sub->m_rMedia = rMedia;

    return sub;
}
