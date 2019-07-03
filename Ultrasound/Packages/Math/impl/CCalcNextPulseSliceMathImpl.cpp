// CCalcNextFlowSliceMathImpl.cpp

#include "CDucalcProject.h"
#include "CFlowContextImpl.h"
#include "CCalcNextFlowSliceMathImpl.h"
#include "CFlowCurveIfaceGp.h"
#include "CTunerProfile.h"
#include "CTunerValueSetting.h"
#include "CTunerValueIndicator.h"
#include "CTunerValueMutator.h"
#include "CRangeDetector.h"
#include "CTunerProfileList.h"
#include "CTunerHistory.h"
#include "CLayerSequence.h"
#include "CLayerStats.h"
//#include "CGlobalConfigIface.h"
#include "CGlobalFlowConfigIfaceGp.h"
#include "CFlowCurveImpl.h"
#include "SuperSharedUsoundStreams.h"

// Put other includes BEFORE this
#include "CCalcNextFlowSliceMathImpl_SelfTest.h" //(dont_uninclude)

CCalcNextFlowSliceMathImpl::CCalcNextFlowSliceMathImpl()
{
}

void CCalcNextFlowSliceMathImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);
}

void CCalcNextFlowSliceMathImpl::PrepareSourceCurve(
        ref<CFlowContextImpl> rFlow,
        ref<CTunerProfile> rTunerProfile)
{
    // Debug branch
    if (rTunerProfile->x_rTestLogic->GetTuneSettingAsBool())
    {
        selftest_PrepareSourceCurve(
            rFlow,
            rTunerProfile);
        return;
    }

    // Init curves in CFlowContext
    InitSourceCurves(
        rFlow, 
        rTunerProfile);

    // Save media range locally
    int nCurrentAverage = 0;
    int nMaxAverage = 0;
    int iMaxPos = rFlow->x_rRawSourceCurve->GetWorkingCount();
    int nMaxAmplitude = 0;

    // Don't let the signal go down too much after the max point
    LevelMediaAfterMaxPoint(
        rFlow, 
        rTunerProfile,
        out nCurrentAverage,
        out nMaxAverage,
        iMaxPos,
        out nMaxAmplitude);

    // Find max level 
    FindMediaMaxAndSmooth(
        rFlow, 
        rTunerProfile,
        out nCurrentAverage,
        out nMaxAverage,
        out iMaxPos);

    // Increase non-noise signal over detected background noise if possible
    AccentuateOverPureBlackBackground(
        rFlow, 
        rTunerProfile,
        nMaxAmplitude);
}

void CCalcNextFlowSliceMathImpl::InitSourceCurves(
        ref<CFlowContextImpl> rFlow,
        ref<CTunerProfile> rTunerProfile)
{
    rTunerProfile;

    // Source curve shortcut
    ref<CFlowCurveImpl> rSourceCurve = rFlow->x_rSourceCurve;

    // Init curves
    rSourceCurve->SetWorkingCount(
        rFlow->x_rRawSourceCurve->GetWorkingCount());

    // Fill with invalid values to catch errors
    rSourceCurve->Fill(-1);
    rFlow->x_rBlackAccentuatedSourceCurve->Fill(-1);
}

void CCalcNextFlowSliceMathImpl::FindMediaMaxAndSmooth(
        ref<CFlowContextImpl> rFlow,
        ref<CTunerProfile> rTunerProfile,
        out int& out_nCurrentAverage,
        out int& out_nMaxAverage,
        out int& out_iMaxPos)
{
    ASSERT(out_nCurrentAverage == 0);
    ASSERT(out_nMaxAverage == 0);
    ASSERT(out_iMaxPos == rFlow->x_rRawSourceCurve->GetWorkingCount());

    // Source curve shortcut
    ref<CFlowCurveImpl> rSourceCurve = rFlow->x_rSourceCurve;

    // Copy raw signal
    int nPreFilterRawMedia = 
        rTunerProfile->x_rPreFilterRawMedia->
            GetTuneSettingAsInt();
    repeat(iPos, rFlow->x_rRawSourceCurve->GetWorkingCount())
    {
        int nAmplitude = rFlow->x_rRawSourceCurve->GetWorkingAmplitudeAt(iPos);

        // Calc average according to settings
        int nNewCurrentAverage =
            math::GDivNot0(
                nAmplitude + out_nCurrentAverage * nPreFilterRawMedia,
                nPreFilterRawMedia + 1, 
                or_divby 1);
        out_nCurrentAverage = nNewCurrentAverage;

        // Max found?
        if (out_nCurrentAverage > out_nMaxAverage)
        {
            out_iMaxPos = iPos;
            out_nMaxAverage = out_nCurrentAverage;
        }

        rSourceCurve->SetWorkingAmplitudeAt(iPos, nAmplitude);
    }

    // Level up signal down from the max point
    out_nCurrentAverage = out_nMaxAverage;
    rTunerProfile->x_rDisplaySourceMediaMax->IndicateTune(out_nMaxAverage);
    rTunerProfile->x_rDisplaySourceMediaMaxPos->IndicateTune(out_iMaxPos);
}

void CCalcNextFlowSliceMathImpl::LevelMediaAfterMaxPoint(
        ref<CFlowContextImpl> rFlow,
        ref<CTunerProfile> rTunerProfile,
        out int& out_nCurrentAverage,
        out int& out_nMaxAverage,
        const int iMaxPos,
        out int& out_nMaxAmplitude)
{
    out_nMaxAmplitude = 0;

    // Source curve shortcut
    ref<CFlowCurveImpl> rSourceCurve = rFlow->x_rSourceCurve;

    // Stabilization setting
    num numMaximizeLowMedia = 
        rTunerProfile->x_rMaximizeLowMedia->
            GetTuneSettingAsNum();

    // Alternate logic?
    int nSimplisticDifferentiationFilter = 
        rTunerProfile->x_rSimplisticDifferentiationFilter->
            GetTuneSettingAsInt();

    // Don't maximize when iMaxPos is very high
    if (iMaxPos < 
            rFlow->x_rRawSourceCurve->GetWorkingCount() - 
                nSimplisticDifferentiationFilter)
    {
        numMaximizeLowMedia = 0;
    }

    // 2015: **** this is very old code, check tests! ****

    // Don't let the media go down too much since max pos is reached
    for (int iPos = iMaxPos; 
        iPos < rSourceCurve->GetWorkingCount();
        iPos++)
    {
        int nAmplitude = rSourceCurve->GetWorkingAmplitudeAt(iPos);

        if (numMaximizeLowMedia)
        {
            // If this is set we want to keep as close to the maximum level 
            //    as possible (as set in the profile)
            int nNormalLevel = (nAmplitude + out_nCurrentAverage) / 2;
            out_nCurrentAverage = 
                (int) (
                    numMaximizeLowMedia * out_nMaxAverage + 
                    (1.0 - numMaximizeLowMedia) * nNormalLevel);
        }
        else
        {
            // Just smooth the transition a little
            out_nCurrentAverage = 
                (nAmplitude + out_nCurrentAverage + out_nMaxAverage) / 3;
        }

        // I wanted to remove it, but has just changed my mind: this signal leveling
        //   is important for bottom-level noise.
        //   It drives the curve down more smoothly.
        rSourceCurve->SetWorkingAmplitudeAt(iPos, out_nCurrentAverage);

        // Detecting maximum
        if (nAmplitude > out_nMaxAmplitude)
            out_nMaxAmplitude = nAmplitude;
    }
}

void CCalcNextFlowSliceMathImpl::AccentuateOverPureBlackBackground(
        ref<CFlowContextImpl> rFlow,
        ref<CTunerProfile> rTunerProfile,
        const int nMaxAmplitude)
{
    // This hack allows us to better flow top signal sections
    //   in cases where we can reliably detect and separate background noise.
    // (see also an end-user checkbox: x_bFlowExtraAccentAtTop forcing 
    //  even stronger effect)

    // Accentuation controlling variables
    int nIncreaseOverBlack = 0;
    int nBlackViolationSum = 0;
    int nBlackZoneHeight = 0;

    // Load and init all valiables
    PrepareToAccentuate(
        rFlow,
        rTunerProfile,
        nMaxAmplitude,
        out nIncreaseOverBlack);

    // Actual curve accentuation, 
    //   we will decide a bit later if we want to use the resulting curve
    RenderAccentuatedCurve(
        rFlow,
        rTunerProfile,
        nMaxAmplitude,
        nIncreaseOverBlack,
        out nBlackViolationSum,
        out nBlackZoneHeight);

    // Final accentuation decision making is here
    DecideAndApplyAccentuation(
        rFlow,
        rTunerProfile,
        nMaxAmplitude,
        nBlackViolationSum,
        nBlackZoneHeight);
}

void CCalcNextFlowSliceMathImpl::PrepareToAccentuate(
        ref<CFlowContextImpl> rFlow,
        ref<CTunerProfile> rTunerProfile,
        const int nMaxAmplitude,
        out int& out_nIncreaseOverBlack)
{
    // Maintain average value history
    int nAverageMaxAmplitude = 
        rTunerProfile->x_rMutateHorizontallyAveragedMediaMax->
            GetCurrentTuneMutationAsInt();

    // Smooth the stats
    nAverageMaxAmplitude = 
        (nAverageMaxAmplitude * 2 + nMaxAmplitude) / 3;

    // Save
    rTunerProfile->x_rMutateHorizontallyAveragedMediaMax->
        FeedNextTuneMutation(nAverageMaxAmplitude);

    // Source curve shortcut
    ref<CFlowCurveImpl> rSourceCurve = rFlow->x_rSourceCurve;

    //
    // Increase signal level until we get into Black Zone
    // (leave nCurrentAverage value from prev iteration)
    //

    // Load profile
    bool bDetectBlackBack = 
        rTunerProfile->x_rDetectBlackBack->
            GetTuneSettingAsBool();
    bool bForceAccentuation = 
        rTunerProfile->x_rForceAccentuation->
            GetTuneSettingAsBool();
    if (!bDetectBlackBack && !bForceAccentuation)
        return;

    // User wanted stronger accent?
    int nIncreaseOverBlackTop = 0;
    if (rFlow->_x_pFlowConfig.Get().Valid()->
            x_bFlowExtraAccentAtTop)
    {
        nIncreaseOverBlackTop = 
            debug("Flow.Customizable.ExtraAccentAtTopCoef?/4e").
                InputIntOrSet(
                    CGlobalFlowConfigIfaceGp::C_nFlowExtraAccentAtTopCoef);
    }
    else
    {
        nIncreaseOverBlackTop = 
            rTunerProfile->
                x_rBlackAccentuationCoefAtTop->
                    GetTuneSettingAsInt();
    }

    int nIncreaseOverBlackBottom = 
        rTunerProfile->x_rBlackAccentuationCoefAtBottom->
            GetTuneSettingAsInt();

    // Load media range history
    ASSERT(out_nIncreaseOverBlack == 0);
    out_nIncreaseOverBlack = 0;
    int iRangeMin = -1;
    int iRangeMax = -1;
[...]

    // Debug dump
    rTunerProfile->x_rDisplayFlowRangeMin->
        IndicateTune(iRangeMin);
    rTunerProfile->x_rDisplayFlowRangeMax->
        IndicateTune(iRangeMax);

    // Normalize increase according to the current signal maximum
    out_nIncreaseOverBlack = 
        math::GMulDiv<int>(
            out_nIncreaseOverBlack, 
            nAverageMaxAmplitude,
            C_nDucalcMediaAmplitudeRange);

    // Debug dump
    rTunerProfile->x_rDisplayBlackAccentuationAddition->
        IndicateTune(out_nIncreaseOverBlack);
}

void CCalcNextFlowSliceMathImpl::DecideAndApplyAccentuation(
        ref<CFlowContextImpl> rFlow,
        ref<CTunerProfile> rTunerProfile,
        const int nMaxAmplitude,
        const int nBlackViolationSum,
        const int nBlackZoneHeight)
{
    nMaxAmplitude;

    // Source curve shortcut
    ref<CFlowCurveImpl> rSourceCurve = rFlow->x_rSourceCurve;

    //
    // Black Zone?
    //

    num numBlackZoneThreshold = 
        rTunerProfile->x_rBlackZoneConditionAverageMax->
            GetTuneSettingAsNum();

    int nMinBlackZoneHeight = 
        rTunerProfile->x_rMinBlackZoneHeight->
            GetTuneSettingAsInt();

    int nLimitBlackZoneHistory = //C_nRoundedMediaSamplesPerSecond;
        rTunerProfile->x_rLimitBlackZoneHistorySamples->
            GetTuneSettingAsInt();

    int nBlackZoneConfirmation = 
        rTunerProfile->x_rMutatePositiveBlackZoneConfirmation->
            GetCurrentTuneMutationAsInt();

    // Report the actual black level
    num numActualBlackViolation = 
        math::GDiv0(num(nBlackViolationSum), num(nBlackZoneHeight), or_divby 1.);
    if (numActualBlackViolation > 0)
    {
        rTunerProfile->x_rDisplayBlackness->
            IndicateTune(numActualBlackViolation);
    }

    if (nBlackZoneHeight > nMinBlackZoneHeight &&
            numActualBlackViolation < numBlackZoneThreshold)
    {
        // Confirm black zone
        nBlackZoneConfirmation++;
        if (nBlackZoneConfirmation > nLimitBlackZoneHistory)
            nBlackZoneConfirmation = nLimitBlackZoneHistory;
    }
    else
    {
        // Confirm non-black zone
        nBlackZoneConfirmation--;
        if (nBlackZoneConfirmation < -nLimitBlackZoneHistory)
            nBlackZoneConfirmation = -nLimitBlackZoneHistory;
    }

    //debug("Flow.Math.Vertical./show.BlackZone:").FloatBlink(
        //"Black = " + Str(nBlackZoneConfirmation));

    rTunerProfile->x_rMutatePositiveBlackZoneConfirmation->
        FeedNextTuneMutation(nBlackZoneConfirmation);

    bool bForceAccentuation = 
        rTunerProfile->x_rForceAccentuation->
            GetTuneSettingAsBool();

    // Is it really a black zone?
    if (nBlackZoneConfirmation > 0 ||
        bForceAccentuation)
    {
        // Copy accentuated curve into the source
        rSourceCurve->
            CopyFrom(
                rFlow->
                    x_rBlackAccentuatedSourceCurve);
    }
}

int CCalcNextFlowSliceMathImpl::OnCalcFlowPosImpl(
        ref<CFlowCurveIfaceGp> rCurve,
        ref<CFlowContextImpl> rFlow,
        ref<CTunerProfile> rTunerProfile)
{
    // Debug branch
    if (rTunerProfile->x_rTestLogic->GetTuneSettingAsBool())
    {
        return
            selftest_OnCalcFlowPosImpl(
                rCurve,
                rFlow,
                rTunerProfile);
    }

    // Count iterations
    int nHorizontalRecalcCounter = 
        rTunerProfile->x_rHorizontalRecalcCounter->
            GetCurrentTuneMutationAsInt();
    nHorizontalRecalcCounter++;
    rTunerProfile->x_rHorizontalRecalcCounter->
        FeedNextTuneMutation(nHorizontalRecalcCounter);

    // Prefilter the curve
    rFlow->x_rRawSourceCurve = rCurve;
    PrepareSourceCurve(rFlow, rTunerProfile);

    // Media curve shortcut
    ref<CFlowCurveImpl> rSourceCurve = rFlow->x_rSourceCurve;
    int nPointCount = rSourceCurve->GetWorkingCount();

    // Load previous pos
    int iRecentFlowPos = 
        rTunerProfile->x_rRecentFlowPos->
            GetCurrentTuneMutationAsInt();
    int iPreviousRecentFlowPos = iRecentFlowPos;

    // Contrast detector filter window height
    int nSimplisticDifferentiationFilter = 
        rTunerProfile->x_rSimplisticDifferentiationFilter->
            GetTuneSettingAsInt();

    int iPrecisePos = off;
    if failed(nSimplisticDifferentiationFilter > 0)
        return nPointCount - 1;

    int nDelta = nSimplisticDifferentiationFilter;

    // Refactoring check
    bool bOptimizationTest = 
        rTunerProfile->x_rOptimizationTest->
            GetTuneSettingAsBool();

    // Load rank settings
    int nRankMaximizingDifference = 
        rTunerProfile->x_rRankMaximizingDifference->
            GetTuneSettingAsInt();
    int nRankLockingPos = 
        rTunerProfile->x_rRankLockingPos->
            GetTuneSettingAsInt();
    int nRankLockingSignalNoiseDifference = 
        rTunerProfile->x_rRankLockingSignalNoiseDifference->
            GetTuneSettingAsInt();
    int nRankLockingNoiseLevel = 
        rTunerProfile->x_rRankLockingNoiseLevel->
            GetTuneSettingAsInt();
    int nRankLockingSignalLevel = 
        rTunerProfile->x_rRankLockingSignalLevel->
            GetTuneSettingAsInt();

    // Stats Layer count
    int nLayerHeight = 
        rTunerProfile->x_rLayerHeight->
            GetTuneSettingAsInt();
    int nLayerCount = 
        math::GDivNot0(C_nDucalcMediaHeightRange, nLayerHeight, or_divby 1) + 1;

    // Init/update layer array allocation
    rFlow->x_rLayerSequence->SetCount(nLayerCount);

    // Calc stats
[...]

    // Round up 
    numUpdatedAverageDirection = 
        RoundTuneToFixedPrecision(numUpdatedAverageDirection); 

    rTunerProfile->x_rRecentAverageDirection->
        FeedNextTuneMutation(numUpdatedAverageDirection);

    // Update ranges
    rFlow->x_rRangeDetector->FeedNextPoint(
        iRecentFlowPos, // next nAmplitude
        C_nRoundedMediaSamplesPerSecond); // nHalfTimePeriod

    // Save tuner history
    if (debug("Flow.Debug.StoreHistoryLog?").InputBool())
    {
        ref<CTunerProfileList> rTunerProfileListCopy;
        rTunerProfileListCopy->FastCopyTunerValuesFrom(
            rFlow->GetGlobalTunerProfileList());
        rFlow->x_rTunerHistory->Add(rTunerProfileListCopy);
    }

    return iRecentFlowPos;
}

void CCalcNextFlowSliceMathImpl::OnApplyPostFilter(
        ref<CFlowCurveIfaceGp> rCurve,
        ref<CFlowContextImpl> rFlow,
        ref<CTunerProfile> rTunerProfile)
{
    ref<CFlowCurveImpl> rCurveImpl = rCurve;

    // Debug branch
    if (rTunerProfile->x_rTestLogic->GetTuneSettingAsBool())
    {
        selftest_OnApplyPostFilter(
            rCurve,
            rFlow,
            rTunerProfile);
        return;
    }

    int nPointCount = rCurveImpl->GetWorkingCount();

    // Filter is off?
    if (nPointCount <= 1)
        return;

    /*
    rCurveImpl->ApplySquareFilter(
        debug("Flow.Math.HorizontalFilterWindow?").
            InputIntOrSet(CFlowContextIface::C_nHorizontalFilterWindow));
    */

    // Adapt filter to the current derivative speed
    int iMiddlePoint = nPointCount / 2;

    // *****.....
    int nLeftAverage = 
        rCurveImpl->CalcAverageFromTo(0, iMiddlePoint);

    // .....*****
    int nRightAverage = 
        rCurveImpl->CalcAverageFromTo(iMiddlePoint, nPointCount - 1);

    // ..*****...
    int nPresentAverage =
        rCurveImpl->CalcAverageFromTo(
            iMiddlePoint - iMiddlePoint / 2 - 1, iMiddlePoint + iMiddlePoint / 3 - 1);

    // ......****
    int nFutureAverage = 
        rCurveImpl->CalcAverageFromTo(
            iMiddlePoint + iMiddlePoint / 3, nPointCount - 1);

    // Adaptive mode
[...]

    ref<CFlowCurveImpl> rOriginalCurve = rFlow->x_rOriginalCurveCache;
    rOriginalCurve->CopyFrom(rCurve);
    int iFrom = nPointCount - nPostFilterDistance;
    if (iFrom >= iToRange)
        iFrom = math::GMax(0, iToRange);
    rOriginalCurve->RunSquareFilterOnRange(
        rCurve,
        nPostFilterWindow,
        iFrom,
        iToRange);
}

num CCalcNextFlowSliceMathImpl::ScaleToCurrentPosInVerticalRange(
        int iPreliminaryPos,
        num numTop,
        num numBottom,
        int iRangeMin,
        int iRangeMax,
        int nPointCount,
        ref<CTunerProfile> rTunerProfile)
{
    // Bottom invalid signal correction
    int nBuggyEdge = 
        rTunerProfile->x_rDeadBottomMediaPoints->
            GetTuneSettingAsInt();
    int nBottomLimit = nPointCount - nBuggyEdge;
    if (iRangeMax > nBottomLimit)
    {
        iRangeMax = nBottomLimit;
        if (iRangeMin > iRangeMax)
            iRangeMin = iRangeMax;
    }

    //     Min  r a n g e   Max
    // ... Top ........... Bottom ...
    //     0.3       0.6     0.8
    //                i
    //            noise start
    //       threshold depending on
    //            iPos level

    //ASSERT(numBottom > numTop);
    ASSERT(iRangeMax >= iRangeMin);
    ASSERT(iRangeMin >= 0);

    num numBlend = 
        numTop +
            math::GMulDiv<num>(
                iPreliminaryPos - iRangeMin,
                numBottom - numTop,
                iRangeMax - iRangeMin + 1);

    num numMax = math::GMax(numTop, numBottom);
    num numMin = math::GMin(numTop, numBottom);
    if (numBlend < numMin)
        numBlend = numMin;
    if (numBlend > numMax)
        numBlend = numMax;

    return numBlend;
}
