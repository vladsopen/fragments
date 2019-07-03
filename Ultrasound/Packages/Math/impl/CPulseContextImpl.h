// CFlowContextImpl.h
// ("CFlowContextImpl.cpp")

#include "CFlowContextIface.h" // (base class)

class CRangeDetector;
class CFlowCurveImpl;
class CTunerProfileList;
class CTunerProfile;
class CTunerHistory;
class CLayerSequence;

//
// Flow curve calculation package implementation
//

class CFlowContextImpl : public CFlowContextIface
{
public:
    CFlowContextImpl();

// Attributes

    // Calc config
    ref<CTunerProfileList> GetGlobalTunerProfileList();
    //ref<CTunerProfileList> GetTunerProfileList();
    ref<CTunerProfile> GetActiveProfile();

    // Tuner History storage
    ref<CTunerHistory> x_rTunerHistory
            xauto(Get);

    // Media stats 
    ref<CLayerSequence> x_rLayerSequence
            xauto(Get);

    // Curve History cache
    ref<CFlowCurveImpl> x_rMediaCurveCache
            xauto(Get);
    ref<CFlowCurveImpl> x_rFlowCurveHistoryCache
            xauto(Get);
    ref<CFlowCurveImpl> x_rOriginalCurveCache
            xauto(Get);

    // Historical Curve Variation Detector
    ref<CRangeDetector> x_rRangeDetector
            xauto(Get);

    int x_nAmplitudeRange = C_nDucalcMediaAmplitudeRange
            xauto(Get, Set)
            xassert(value > 0);

    ref<CFlowCurveImpl> x_rRawSourceCurve
            xauto(Get, Set);

    ref<CFlowCurveImpl> x_rSourceCurve
            xauto(Get, Set);

    ref<CFlowCurveImpl> x_rBlackAccentuatedSourceCurve
            xauto(Get, Set);

    ref<CFlowCurveImpl> x_rPreparedCurveWithStrongFilter
            xauto(Get);

    ref<CFlowCurveImpl> x_rPreparedCurveForEdgeSwingAdjust
            xauto(Get);

    // Simple mean filter
    int x_nPrevMeanCurveFromBaseline = 0
            xauto(Get, Set)
            xassert(value >= 0 && value < oo);

    // Calls doc for recalc update
    void RecalcFlowViaDoc(
            ref<CMediaDocIfaceGp> rMedia);

// Calculations

    void HandleCurveChange();

//public for UiFlowImpl:

    ptr<CUiPanelGp> _m_pUiFlowImpl;

    void DrawCalculationResult(
            draw dc,
            int nWidth,
            int nHeight);

    void HandleMouse(
            ref<CUiPanelGp> rUi,
            EMouse eMouse,
            int iMousePosX,
            int iMousePosY,
            int nWidth,
            int nHeight);

    void ScaleToSize(
            int nWidth,
            int nHeight);

protected:
    virtual ref<CFlowCurveIfaceGp> OnGetInputMediaCurve();
    virtual ref<CFlowCurveIfaceGp> OnGetRecentHistoryCurve();
    virtual int OnGetFlowPostFilterCalcDelaySlices();
    virtual void OnResetAndClearContent();
    virtual ref<UiContainer> OnNewMainFlowTestPaintUi(
            ref<CMediaDocIfaceGp> rMedia);
    virtual int OnCalcFlowPos(
            ref<CFlowCurveIfaceGp> rCurve);
    virtual void OnHandleDrawMediaHistogramPosChange(
            int iHistogramSourcePos);

private:
    int m_iPrevMouseX = INT_MAX;
    //ref<CTunerProfileList> m_rTunerProfileList;
    //bool _m_bTunerProfileListLoaded = false;
    int m_nPrevHistogramSourcePos = -1;

    // Positive or negative value limited by C_nRoundedMediaSamplesPerSecond
    //  indicating current Black Zone status
    //int m_nBlackZoneConfirmation = 0;

    //int m_nAverageMaxAmplitude = 0;

    enum { C_nMinSourceCurveWidth = 16 };

    //
    // Debug Drawing
    //

    void DrawCalculationResultAsFixedScalePicture(
            draw dc,
            int nWidth,
            int nHeight);

    int CalcUniformExtent(
            int nFixedPixelExtent,
            int nUiViewExtent);
};

// ("UiFlowImpl.cpp")
class UiFlowImpl : public UiContainer
{
public:
    virtual void Ui();

    ref<CFlowContextImpl> m_rFlow;
    ref<CMediaDocIfaceGp> m_rMedia;
};

