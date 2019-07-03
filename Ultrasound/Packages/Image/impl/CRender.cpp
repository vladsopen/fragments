// CRender.cpp

#include "CUsmainProject.h"
#include "CRender.h"
#include "CImageImpl.h"
#include "CViewState.h"
#include "CViewCache.h"
#include "CDocIface.h"
#include "CStoreIface.h"
#include "CMatrixFormatGp.h"
#include "CMatrixSliceIface.h"
#include "CGlobalPaletteConfigIface.h"
#include "CLocalPaletteConfigIface.h"
#include "CAmplitudeToColorIndexGp.h"
#include "CColorListGp.h"
#include "CColoring.h"
#include "CImmediatePaintIfaceGp.h"
#include "CCurveSliceIface.h"
#include "CCurveTypeIface.h"
#include "CBaselineIface.h"
#include "CFromBaselineTypeIface.h"
#include "CCurveConfigIface.h"
#include "CCurveMapConfigIface.h"
#include "CVendorLogoSetupIfaceGp.h"
#include "CVendorLogoIfaceGp.h"
#include "VirtOs_CAppGp.h"
#include "COnlineModeIface.h"
#include "COnlineTypeIface.h"
#include "VirtOs_form.h"
#include "CIoControlIfaceGp.h"
#include "CPaletteVirtualizerIface.h"
#include "CCustomizeDocIfaceGp.h"
#include "VirtOs_CAppGp.h"
#include "VirtOs_CGlobalUi.h"
#include "CFontIfaceGp.h"
#include "CInitTypeIface.h"
#include "CAppBetaIfaceGp.h"
#include "CCurveNatureTypeIface.h"

CRender::CRender()
{
}

void CRender::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CRender!");
}

void CRender::InitRenderForDrawingAction(
        ref<CViewCache> rViewCache,
        out ref<CPicture>& out_rRawPicture,
        out ref<CPicture>& out_rRawPicture1pixel,
        out ref<CPicture>& out_rScaledPicture,
        ref<CImmediatePaintIfaceGp> rImmediatePaintObjectCache,
        size sizeUi,
        ptr<CUiPanelGp> pUiViewCache)
{
    // init and use once!
    rASSERT(!_m_bRenderInitOk);
    rASSERT(!_m_bRenderCannotReuse);
    _m_bRenderInitOk = true;

    // cache all refs
    _m_rViewCache = rViewCache;
    _m_rImage = rViewCache->_x_pParentImage.Get().Valid();
    _m_rDoc = _m_rImage->_x_pParentDoc.Get().Valid();
    _m_rStore = _m_rImage->GetRelatedStore();
    _m_rViewState = _m_rImage->x_rViewState;
    _m_rColoring = _m_rImage->x_rColoring;
    _m_rGlobalPaletteConfig = _m_rColoring->GetImageGlobalPaletteConfig();
    _m_rLocalPaletteConfig = _m_rColoring->x_rLocalPaletteConfig;
    _m_rMatrixFormat = _m_rDoc->x_rMatrixFormat;
    _m_rBaseline = _m_rStore->x_rBaseline;
    _m_rCurveMapConfig = _m_rDoc->GetCurveMapConfigRelevantToDoc();
    ref<CCustomizeDocIfaceGp> rCustomizeDoc = 
        _m_rDoc->_x_rCustomizeDoc;

    // also save cache data
    _m_rRawPicture = out_rRawPicture;
    _m_rRawPicture1pixel = out_rRawPicture1pixel;
    _m_rScaledPicture = out_rScaledPicture;
    _m_rImmediatePaintObjectCache = rImmediatePaintObjectCache;
    _m_sizeUi = sizeUi;
    _m_pUiViewCacheMediaLayer = pUiViewCache;
    _m_bTurbo = _m_rDoc->_x_bTurbo;
    _m_bRaster = _m_rDoc->_x_pDupomDoc;
    _m_bRasterRainbow = _m_rDoc->x_bRasterRainbow;
    _m_bRasterAverageRgb = _m_rDoc->x_bRasterAverageRgb;
    _m_nMaxRasterLuminocityAdd = debug("Raster.LuminocityAdd?").InputInt();
    _m_bHackMode = _m_rDoc->IsHackModeDoc();
    _m_bDrawCurveLinesFast = _m_rCurveMapConfig->x_bDrawCurveLinesFast;
    _m_nCurveLinePenThickness = _m_rCurveMapConfig->x_nCurveLinePenThickness;
    _m_bEnableDrawCurves = 
        !_m_bRaster 
        && 
        !rCustomizeDoc->x_bHideAllCurves;

    if not_null(
        ref<CUiPanelGp> rUi = _m_pUiViewCacheMediaLayer)
    {
        _m_bPrinting = rUi->IsPrintOrPreview();
    }

    int nHeightLengthPoints = _m_rMatrixFormat->x_nMatrixLength;

    // Check if RawPicture cache is matching unscaled sized
    {
        size sizeRawImage = 
            size(
                1 + 
                    math::GDivNot0(
                        _m_nViewWidthSlices, 
                        _m_nDivScaleDownRawPicture,
                        or_return 0), 
                nHeightLengthPoints);
        if (_m_rRawPicture->GetRasterSize() != sizeRawImage)
        {
            out_rRawPicture = null();
            _m_rRawPicture = out_rRawPicture;
            _m_rRawPicture->
                SetResolution(
                    sizeRawImage, 
                    CPicture::C_nDefaultBitsPerPixel);
        }
    }
}

void CRender::RenderImageToCache()
{
    // init and use once!
    rASSERT(_m_bRenderInitOk);
    rASSERT(!_m_bRenderCannotReuse);
    _m_bRenderCannotReuse = true;

    // Now we start working with the scaled target image
    draw dcScaledPicture = _m_rScaledPicture->GetDc(draw());

    // fast exit on failure
    if (!dcScaledPicture.IsReadyToDraw())
    {
        TESTLOG(
            "",
            "Scaled bitmap DC failed!\n");

        return;
    }

    // have source data?
    if (_m_bRenderNothingToDraw)
    {
        // Have a logo or Profiling Info?
        if (!WriteOnPictureProfilingAndLogoInfo(
                dcScaledPicture))
        {
            // No info at all
            DrawEmptyImagePlaceholder(dcScaledPicture);
        }

        return;
    }

    // Matrix -> raw unscaled cache
    RenderAllSlicesToRawCache(
        _m_iStartDrawSlice,
        _m_nDrawSlices,
        _m_iScrollSlice,
        _m_nCorrectedPassedSlices);

    TESTLOG(
        "",
        " slice paint done.\n");

    //
    // Scale
    //

    if (!x_bIncrementalDrawOneSlice)
    {
        // 6.5 s.
        ScaleRawPictureToFullUiSize(
            dcScaledPicture);
    }
    else
    {
        // 6.3 / 5.6 / debug idle: 5.4
        ScaleRawPictureIncrement(
            dcScaledPicture);
    }

    //
    // OnScreen Info
    //
    if (_m_rViewCache->x_bDrawPassedPos &&
        _m_bHackMode)
    {
        WriteOnPictureMarkings(
            dcScaledPicture);
    }
   
    //
    // Fast incremental paint
    //
    if (x_bIncrementalDrawOneSlice)
    {
        if not_null(
            ref<CUiPanelGp> rUi = _m_pUiViewCacheMediaLayer)
        {
            OutputScaledPictureIncrement(
                rUi,
                dcScaledPicture);
        }
    }
    else
    {
        // Profiling Info
        WriteOnPictureProfilingAndLogoInfo(
            dcScaledPicture);
    }
}

void CRender::OutputScaledPictureIncrement(
        ref<CUiPanelGp> rUi,
        draw dcScaledPicture)
{
    // Init Immediate Draw Controller
    ref<CImmediatePaintIfaceGp> rImmediatePaint = 
        _m_rImmediatePaintObjectCache;
    draw dcImmediate =
        rImmediatePaint->StartImmediatePaintDc(
            rUi);

    // update curve vectors
    _m_bRenderCannotReuse = false;
    RenderAllCurves(
        dcImmediate);

    // Update Baseline
    LineBaseline(dcImmediate);

    // Profiling Info
    WriteOnPictureProfilingAndLogoInfo(
        dcImmediate);

    // Done
    rImmediatePaint->StopImmediatePaint();
}

    //
    // === Profiled: there is no diff btw 
    //     ::BitBlt() and BlitToDcLowLevelPartial()
    //     and the later is even somehow faster a fraction of a second

    /*
    TESTLOG(
        "",
        "Incremental Blt: " 
            "(" + 
            Str(pointBlit.x) + 
            " <-- " + 
            Str(sizeBlit.h) + 
            " --> " +
            Str(pointBlit.x + sizeBlit.w) + 
            ")\n");

    // 6.5 s
    ::BitBlt(
        // Destination:
        dcImmediate.GetHdc(),
        pointBlit.x,
        pointBlit.y,
        sizeBlit.w, //_m_sizeUi.w,
        sizeBlit.h,
        // Source
        dcScaledPicture.GetHdc(),
        pointBlit.x,
        pointBlit.y,
        SRCCOPY);
        */

void CRender::RenderAllSlicesToRawCache(
        int iStartDrawSlice,
        int nDrawSlices,
        int iScrollSlice,
        int nCorrectedPassedSlices)
{
    bool bTestLog = 
        debug::GIsTest() && 
            _m_rImage->_x_bTestLogMediaDetails;

    // picture depends on the mode
    ref<CPicture> rRawPicture = 
        _m_bUseThinRawPicture? 
            _m_rRawPicture1pixel
            :
            _m_rRawPicture;

    // Iterate slices by direct indexing
    for (
        int iSlice = iStartDrawSlice; 
        iSlice < nDrawSlices;
        iSlice++)
    {
        // fake loop?
        //if (iOnlySlice > 0)
            //iSlice = iOnlySlice;

        int iRenderSourceSlice = iScrollSlice + iSlice;

        // Load slice data from storage
        ref<CMatrixSliceIface> rMatrixSlice = 
            _m_rStore->
                GetMatrixSliceAt(
                    iRenderSourceSlice,
                    ptr<CMatrixSliceIface>());

        // Cycled through Passed index
        int iRawDrawX = iSlice + nCorrectedPassedSlices;
        if (iRawDrawX >= _m_nViewWidthSlices)
        {
            iRawDrawX -= _m_nViewWidthSlices;

            if (bTestLog)
            {
                TESTLOG(
                    "",
                    "|> ");
            }
        }
        else
        {
            if (bTestLog)
            {
                TESTLOG(
                    "",
                    ">> ");
            }
        }

        if (bTestLog)
        {
            TESTLOG(
                "",
                "[#" + Str(iRenderSourceSlice) + "]-->(#" + Str(iRawDrawX) + ") ");
        }

        rASSERT(iRawDrawX >= 0 && iRawDrawX < _m_nViewWidthSlices);

        // Draw the slice colors
        RenderNextSliceToRawCache(
            iRawDrawX,
            rMatrixSlice,
            rRawPicture);

        // fake loop?
        //if (iOnlySlice > 0)
            //break;
    }
}

void CRender::RenderAllCurves(
        draw dc)
{
    if (!_m_bEnableDrawCurves)
    {
        return;
    }

    // init and use once!
    rASSERT(_m_bRenderInitOk);
    rASSERT(!_m_bRenderCannotReuse);
    _m_bRenderCannotReuse = true;

    // for all curves...
    type<CCurveTypeIface> typeIterCurveType;
    for (
        iter i;
        type<CCurveTypeIface>()->
            GetDerivedList().
                Iterate(
                    out i,
                    out typeIterCurveType);)
    {
        // Config OKs the drawing?
        ref<CCurveConfigIface> rIterCurveConfig = 
            _m_rCurveMapConfig->
                GetCurveConfigByType(
                    typeIterCurveType);

        if (rIterCurveConfig->x_bDrawCurve)
        {
            // Go draw!
            RenderNextCurve(
                dc,
                typeIterCurveType);
        }
    }
}

void CRender::DoDrawCurveLine(
        draw dc,
        ELineRender eLineRender,
        int iDrawX,
        int nAmplitude,
        out str& out_sTestLog)
{
    HAIRY; // profiling

    // start or append the line
    if (eLineRender == E_LineRender_MoveTo)
    {
        if (out_sTestLog.Len() > 0)
        {
            rASSERT(debug::GIsTest());

            out_sTestLog += 
                " ~|" + Str(nAmplitude) + "/~ ";
        }

        dc.MoveTo(
            iDrawX - (_m_nCurveLinePenThickness == 1? 1 : 0), 
            nAmplitude);
    }
    else
    {
        ASSERT(eLineRender == E_LineRender_LineTo);

        if (out_sTestLog.Len() > 0)
        {
            rASSERT(debug::GIsTest());

            out_sTestLog += 
                " ~/" + Str(nAmplitude) + "/~ ";
        }

        if (_m_nCurveLinePenThickness == 1)
        {
            NOTEST; //todo!!!

            dc.LineTo(iDrawX, nAmplitude);
        }
        else
        if (_m_bDrawCurveLinesFast)
        {
            NOTEST; //todo!!!

            point pointPrev = dc.GetCurrentPosition();

[...]

