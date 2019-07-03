// CCalcNextFlowSliceMathImpl.h

#include "CCalcNextFlowSliceAdapterImpl.h" // base class

class CFlowContextImpl;
class CFlowCurveIfaceGp;
class CTunerProfile;

//
// CCalcNextFlowSliceMathImpl - contains PRD-compliant flow calculation code
//

class CCalcNextFlowSliceMathImpl : public CCalcNextFlowSliceAdapterImpl
{
public:
    CCalcNextFlowSliceMathImpl();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    static const int C_nFinestFlowTuningPrecision = 10000; // fixed point

// Attributes

// Operations

// UI

protected:

    virtual int OnCalcFlowPosImpl(
            ref<CFlowCurveIfaceGp> rCurve,
            ref<CFlowContextImpl> rFlow,
            ref<CTunerProfile> rTunerProfile);

    virtual void OnApplyPostFilter(
            ref<CFlowCurveIfaceGp> rCurve,
            ref<CFlowContextImpl> rFlow,
            ref<CTunerProfile> rTunerProfile);

private:

    // Preprocess source curve
    void PrepareSourceCurve(
            ref<CFlowContextImpl> rFlow,
            ref<CTunerProfile> rTunerProfile);

    // Init source curves
    void InitSourceCurves(
            ref<CFlowContextImpl> rFlow,
            ref<CTunerProfile> rTunerProfile);

    // Detect raw media range,
    //   also digest x_rRawSourceCurve into x_rSourceCurve
    void FindMediaMaxAndSmooth(
            ref<CFlowContextImpl> rFlow,
            ref<CTunerProfile> rTunerProfile,
            out int& out_nCurrentAverage,
            out int& out_nMaxAverage,
            out int& out_iMaxPos);

    // Do not allow sharp media level falling after the max pos is reached
    void LevelMediaAfterMaxPoint(
            ref<CFlowContextImpl> rFlow,
            ref<CTunerProfile> rTunerProfile,
            out int& out_nCurrentAverage,
            out int& out_nMaxAverage,
            const int iMaxPos,
            out int& out_nMaxAmplitude);

    // This hack allows us to better flow top signal sections
    //   in cases where we can reliably detect and separate background noise
    void AccentuateOverPureBlackBackground(
            ref<CFlowContextImpl> rFlow,
            ref<CTunerProfile> rTunerProfile,
            const int nMaxAmplitude);

    // Load and init all valiables for AccentuateOverPureBlackBackground()
    void PrepareToAccentuate(
            ref<CFlowContextImpl> rFlow,
            ref<CTunerProfile> rTunerProfile,
            const int nMaxAmplitude,
            out int& out_nIncreaseOverBlack);

    // Actual curve accentuation (called from AccentuateOverPureBlackBackground())
    void RenderAccentuatedCurve(
            ref<CFlowContextImpl> rFlow,
            ref<CTunerProfile> rTunerProfile,
            const int nMaxAmplitude,
            int nIncreaseOverBlack,
            out int& out_nBlackViolationSum,
            out int& out_nBlackZoneHeight);

    // Final accentuation decision making is here
    void DecideAndApplyAccentuation(
            ref<CFlowContextImpl> rFlow,
            ref<CTunerProfile> rTunerProfile,
            const int nMaxAmplitude,
            const int nBlackViolationSum,
            const int nBlackZoneHeight);

    // Curve roaming for Noise swing adjust
    int FindStableLevelPosAboveNoise(
            ref<CFlowCurveImpl> rCurve,
            int iFirstElement,
            int iLastElement,
            num numNoiseLevel,
            ref<CTunerProfile> rTunerProfile);

    // Local Derivative Detection
    void FindMaxRaisingPos(
            ref<CFlowCurveIfaceGp> rCurve,
            int iFirstElement,
            int iLastElement,
            int iLastPosHint,
            num numLockToFoundPosCoef,
            out int& out_iRaisingStart,
            out int& out_iRaisingEnd,
            ref<CTunerProfile> rTunerProfile);

    // Linear intensity adjustment coefficient scalling
    num ScaleToCurrentPosInVerticalRange(
            int iPreliminaryPos,
            num numTop,
            num numBottom,
            int iRangeMin,
            int iRangeMax,
            int nPointCount,
            ref<CTunerProfile> rTunerProfile);

    // Cleans up veray small numbers to avoid marginal FP states
    num RoundTuneToFixedPrecision(
            num numTune);

    // I leave previous implemetation to match to it for refactoring
    // see CCalcNextFlowSliceMathImpl_SelfTest.h
    void selftest_PrepareSourceCurve(
            ref<CFlowContextImpl> rFlow,
            ref<CTunerProfile> rTunerProfile);
    int selftest_OnCalcFlowPosImpl(
            ref<CFlowCurveImpl> rCurve,
            ref<CFlowContextImpl> rFlow,
            ref<CTunerProfile> rTunerProfile);
    void selftest_OnApplyPostFilter(
            ref<CFlowCurveImpl> rCurve,
            ref<CFlowContextImpl> rFlow,
            ref<CTunerProfile> rTunerProfile);

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
};
