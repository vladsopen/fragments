// CRender.h
#pragma once

class CIoControlIfaceGp;
class CDocIface;
class CImmediatePaintIfaceGp;
class CCurveMapConfigIface;
class CBaselineIface;
class CCurveTypeIface;
class CMatrixSliceIface;
class CMatrixFormatGp;
class CLocalPaletteConfigIface;
class CGlobalPaletteConfigIface;
class CColoring;
class CViewState;
class CStoreIface;
class CImageImpl;
class CViewCache;

//
// CRender - 
//
//   A controller object painting media image (through all cache modes)
//     for the current view state and with online incremental optimizations.
//
//   An instance of the object can be used only for a SINGLE drawing action,
//     either full windows update or an online incremental paint.
//
//ASSUME_IMPL_FOR_IFACE(CRenderImpl, CRender)

class CRender : public object
{
public:
    CRender();
    //NEW_LOCAL(CRenderImpl, CRender)
    //NEW_GP(MyProject, CRenderImpl, CRender)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // tiny line flying left to right online
    static const int C_nOnlineFontLineHightlightHeight = 2;

    // we never render images for better quality than this
    static const int C_nMaxRawImageResolutionX = 1000;

// Attributes

    // Force online optimization: only append the most recent slice 
    //   assuming everything else in the background is up to date.
    bool x_bIncrementalDrawOneSlice = false
            xauto(Get, Set);

// Operations

    // Must be called before anything else and only once.
    //   Only some properties can be set before this.
    void InitRenderForDrawingAction(
            ref<CViewCache> rViewCache,
            out ref<CPicture>& out_rRawPicture,
            out ref<CPicture>& out_rRawPicture1pixel,
            out ref<CPicture>& out_rScaledPicture,
            ref<CImmediatePaintIfaceGp> rImmediatePaintObjectCache,
            size sizeUi,
            ptr<CUiPanelGp> pUiViewCache);

    // Makes main image for both x_bIncrementalDrawOneSlice modes
    void RenderImageToCache();

    // Final curve rendering
    void RenderAllCurves(
            draw dc);

    // Baseline line
    void LineBaseline(
            draw dc);

// UI

protected:
private:

    // init/run-once state
    bool _m_bRenderInitOk = false;
    bool _m_bRenderCannotReuse = false;
    bool _m_bRenderNothingToDraw = false;

    // ref cache for all drawing operations - 
    //   we dont want slow queries inside the loops
    //
    // All inited once in InitRenderForDrawingAction() and never changes.
    // *** THIS IS NOT A ref<> DEPENDENCY ERROR ***
    // We need this kind of optimization to avoid complex property, and ptr<>
    //   routing during whole drawing procedure (mostly debug perf).
    // This is safe until CRender is used only as a local object and never 
    //   saved anywhere.
    //
    ref<CViewCache> _m_rViewCache;
    ref<CImageImpl> _m_rImage;
    ref<CDocIface> _m_rDoc;
    ref<CStoreIface> _m_rStore;
    ref<CViewState> _m_rViewState;
    ref<CColoring> _m_rColoring;
    ref<CGlobalPaletteConfigIface> _m_rGlobalPaletteConfig;
    ref<CLocalPaletteConfigIface> _m_rLocalPaletteConfig;
    ref<CMatrixFormatGp> _m_rMatrixFormat;
    ref<CBaselineIface> _m_rBaseline;
    ref<CCurveMapConfigIface> _m_rCurveMapConfig;
    ref<CIoControlIfaceGp> _m_rIoControl;

    // fast access to cache state members
    ref<CPicture> _m_rRawPicture;
    ref<CPicture> _m_rRawPicture1pixel;
    ref<CPicture> _m_rScaledPicture;
    ref<CImmediatePaintIfaceGp> _m_rImmediatePaintObjectCache;
    size _m_sizeUi = size();
    ptr<CUiPanelGp> _m_pUiViewCacheMediaLayer;
    bool _m_bNoFontSource = false;
    bool _m_bTurbo = false;
    bool _m_bHackMode = false;
    bool _m_bUseThinRawPicture = true;
    bool _m_bDrawCurveLinesFast = true;
    bool _m_bEnableDrawCurves = true;
    bool _m_bRaster = false;
    bool _m_bRasterRainbow = false;
    bool _m_bRasterAverageRgb = false;
    bool _m_bPrinting = false;
    int _m_nCurveLinePenThickness = 1;
    int _m_nMaxRasterLuminocityAdd = 0;

    // metrics we precalculate at init
    int _m_nStoredSlices = -off;
    int _m_nPassedSlices = -off;
    int _m_nCorrectedPassedSlices = -off;
    int _m_nDrawSlices = -off;
    int _m_nViewWidthSlices = -off;
    int _m_iStartDrawSlice = -off;
    int _m_iScrollSlice = -off;
    int _m_nBaselineInMatrixLength = -off;
    int _m_nBaselineInUiHeight = -off;
    int _m_nDivScaleDownRawPicture = -off;

    // incremental update zone
    point _m_pointScaledZone = point();
    size _m_sizeScaledZone = size();
    point _m_pointRawZone = point();
    size _m_sizeRawZone = size();

    // Raw slice render loop
    void RenderAllSlicesToRawCache(
            int iStartDrawSlice,
            int nDrawSlices,
            int iScrollSlice,
            int nCorrectedPassedSlices);

    // Render one slice
    void RenderNextSliceToRawCache(
            int iPosX,
            ref<CMatrixSliceIface> rMatrixSlice,
            ref<CPicture> rRawPicture);

    enum ELineRender { E_LineRender_MoveTo, E_LineRender_LineTo };

    // Curve renderer
    void RenderNextCurve(
            draw dc,
            type<CCurveTypeIface> typeCurveType);

    // Curve renderer
    void RenderNextCurveSegmentToScaledOutput(
            draw dc,
            ELineRender eLineRender,
            int iDrawX,
            ref<CMatrixSliceIface> rMatrixSlice,
            type<CCurveTypeIface> typeCurveType,
            out str& out_sTestLog);

    // Copy _m_rRawPicture into _m_rScaledPicture
    void ScaleRawPictureToFullUiSize(
            draw dcScaledPicture);

    // Copy a part of _m_rRawPicture into _m_rScaledPicture
    void ScaleRawPictureIncrement(
            draw dcScaledPicture);

    // Incremental paint directly into the window
    void OutputScaledPictureIncrement(
            ref<CUiPanelGp> rUi,
            draw dcScaledPicture);

    // Debug lines
    void WriteOnPictureMarkings(
            draw dc);

    // OutText() on scaled picture
    bool WriteOnPictureProfilingAndLogoInfo(
            draw dc);

    // When there is no data
    void DrawEmptyImagePlaceholder(
            draw dcScaledPicture);

    // MoveTo/LineTo
    void DoDrawCurveLine(
            draw dc,
            ELineRender eLineRender,
            int iDrawX,
            int nAmplitude,
            out str& out_sTestLog);

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
