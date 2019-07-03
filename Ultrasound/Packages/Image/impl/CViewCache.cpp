// CViewCache.cpp

#include "CUsmainProject.h"
#include "CViewCache.h"
#include "CImageImpl.h"
#include "CViewState.h"
#include "CDocIface.h"
#include "CRender.h"
#include "CStoreIface.h"
#include "CBaselineIface.h"
#include "CGeneratorListIface.h"
#include "CImmediatePaintIfaceGp.h"
#include "CMatrixFormatGp.h"

CViewCache::CViewCache()
{
}

void CViewCache::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    //rFAIL("make sure it is OK to expose CViewCache!");
}

void CViewCache::PaintFullMediaUiImage(
        draw dc,
        size sizeUi)
{
    sizeUi;

    EnsureViewCacheUpdated();

    //rASSERT(sizeUi == _m_rPicture->GetRasterSize());

    _m_rScaledPicture->
        BlitToDc(
            dc, 
            MAX_ALPHA, 
            point(0, 0));

    // draw controller
    ref<CRender> rRender;

    // init drawing controller
    rRender->
        InitRenderForDrawingAction(
            this,
            out _m_rRawPicture,
            out _m_rRawPicture1pixel,
            out _m_rScaledPicture,
            _m_rImmediatePaintObjectCache,
            _m_sizeUi,
            _m_pUiViewCacheMediaLayer);

    // Draw curves over the image
    rRender->
        RenderAllCurves(
            dc);

    // Baseline line
    rRender->
        LineBaseline(
            dc);
}

void CViewCache::RerenderViewCache()
{
    // draw controller
    ref<CRender> rRender;

    // partial update?
    if (_m_bRenderOnlyNextOnlineSliceNextTime)
    {
        rRender->x_bIncrementalDrawOneSlice = true;
        _m_bRenderOnlyNextOnlineSliceNextTime = false;
    }

    // init drawing controller
    rRender->
        InitRenderForDrawingAction(
            this,
            out _m_rRawPicture,
            out _m_rRawPicture1pixel,
            out _m_rScaledPicture,
            _m_rImmediatePaintObjectCache,
            _m_sizeUi,
            _m_pUiViewCacheMediaLayer);

    // Go render!
    rRender->RenderImageToCache();

    // we are ready.
    _m_bUpToDate = true;
}

void CViewCache::HandleViewCacheMouse(
        EMouse eMouse,
        point pointClientMousePos,
        ref<CUiPanelGp> rUi)
{
    NOTEST; //todo UI?

    // objects
    ref<CImageImpl> rImage = 
        _x_pParentImage.Get().Valid();
    ref<CStoreIface> rStore = 
        rImage->GetRelatedStore();
    ref<CBaselineIface> rBaseline = 
        rStore->x_rBaseline;

    if (rBaseline->
            TryMoveBaselineOnViewCacheMouse(
                eMouse,
                pointClientMousePos,
                rUi))
    {
        // baseline grabbed the message
    }
    else
    if (x_bInteractiveGenerator)
    {
        // notify debug generator
        rImage->
            DigGeneratorList()->
                HandleMouseOverMediaImage(
                    eMouse,
                    pointClientMousePos,
[...]