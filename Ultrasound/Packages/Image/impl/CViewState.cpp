// CViewState.cpp

#include "CUsmainProject.h"
#include "CViewState.h"
#include "CStoreIface.h"
#include "CViewCache.h"
#include "CImageImpl.h"
#include "CDocIface.h"
#include "CRulerSystemIface.h"
#include "CCalcupSystemIface.h"
#include "CDbDocIface.h"
#include "CUdbMirrorIfaceGp.h"
#include "CSessionIface.h"
#include "CDupomSessionIface.h"
#include "CDocMetricsIfaceGp.h"
#include "CRawSlotGp.h"
#include "CCustomizeDocIfaceGp.h"
#include "CNoteMarkingIface.h"
#include "CInitTypeIface.h"

CViewState::CViewState()
{
}

void CViewState::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    //rFAIL("make sure it is OK to expose CViewState!");
}

void CViewState::CheckViewState()
{
    ref<CImageImpl> rImage = _x_pParentImage.Get().Valid();
    ref<CDocIface> rDoc = rImage->GetParentDoc();
    ref<CStoreIface> rStore = rImage->GetRelatedStore();
    ref<CViewCache> rViewCache = rImage->x_rViewCache;

    //
    // Test all ranges and positions in context of current media storage
    //

    //int nStoreSlices = rStore->GetStoreRipeMatrixSliceCount();

    rASSERT(m_iScrollSlice >= 0);
    rASSERT(
        m_iScrollSlice <= nStoreSlices - math::GMin(m_nWidthSlices, nStoreSlices) 
        ||
        m_iScrollSlice == 0
        //||
        //(nStoreSlices - m_nWidthSlices == 0 
        //    &&
        //    m_iScrollSlice == 0) ||
        //m_nWidthSlices == 1);
        || 
        rDoc->_x_rCustomizeDoc->x_bHideAboveTimeBar
        );

    rASSERT(m_nWidthSlices > 0);
    //rASSERT(m_nWidthSlices <= nStoreSlices || m_nWidthSlices == 1);

    rASSERT(m_nPassedSlices >= 0);
    rASSERT(m_nPassedSlices <= m_nWidthSlices);
}

void CViewState::SetViewWidthSlices(
        int nWidthSlices)
{
    // check existing state against the storage
    CheckViewState();

    // changed?
    if (m_nWidthSlices != nWidthSlices)
    {
        ref<CImageImpl> rImage = _x_pParentImage.Get().Valid();
        ref<CViewCache> rViewCache = rImage->x_rViewCache;
        ref<CStoreIface> rStore = rImage->GetRelatedStore();
        int nStoreSlices = rStore->GetStoreRipeMatrixSliceCount();

        // modify
        int nOldWidthSlices = m_nWidthSlices;
        m_nWidthSlices = nWidthSlices;

        //
        // Adjust scrolling
        //

        if (m_nPassedSlices > nWidthSlices)
        {
            m_nPassedSlices = nWidthSlices;
        }

        if (nStoreSlices < nWidthSlices)
        {
            m_nPassedSlices = nStoreSlices;
        }

        m_iScrollSlice = nStoreSlices - nWidthSlices;

        //
        // Update
        //

        TESTLOG(
            "",
            "ViewState width = " + 
                Str(m_nWidthSlices) + " slices (passed counter is reset to " + 
                Str(m_nPassedSlices) + 
                ")\n");

        // recheck
        CheckViewState();

        // update view
        rViewCache->InvalidateViewCache();

        // Update Rulerbars
        ref<CDocIface> rDoc = 
            rImage->GetParentDoc();
        ref<CRulerSystemIface> rRulerSystem = 
            rDoc->_x_rRulerSystem;
        rRulerSystem->UpdateAllRulerbars();
        rImage->InvalidateImageBarSystemContent();

        // Update calculations
        if (nStoreSlices > 0)
        {
            ref<CCalcupSystemIface> rCalcupSystem = 
                rDoc->x_rCalcupSystem;
            rCalcupSystem->ResetManualRecalsPosOrUpdateAutomatic();
        }

        // Notify RASTER
        PrepareOrSendTimeWidthSync(true); // bSend

        // sync drawing pos
        if (nStoreSlices > m_nWidthSlices)
        {
            ScrollToEndScreenReset();
        }
    }
}

void CViewState::AdvanceViewPassed()
{
    // tiny function, mostly debug and logging - see uProto to understand the design

    ref<CImageImpl> rImage = _x_pParentImage.Get().Valid();
    ref<CStoreIface> rStore = rImage->GetRelatedStore();
    ref<CViewCache> rViewCache = rImage->x_rViewCache;

    // how many we've really stored?
    int nStoredSlices = rStore->GetStoreRipeMatrixSliceCount();

    TESTLOG(
        "",
        "AdvanceViewPassed: stored = " +
            Str(nStoredSlices) + ", ");

    // apply the delay by not advancing for the first slices received
    if (nStoredSlices == 0)
    {
        TESTLOG(
            "",
            "exiting!\n");

        return;
    }

    // We MUST have previous state painted before we draw incremental
    rViewCache->EnsureViewCacheUpdated();
    rASSERT(rViewCache->IsCacheCompletelyUpToDate());

    // online update optimization
    bool bPartialUpdateMode = 
        //rViewCache->IsCacheCompletelyUpToDate() &&  see Update above
            rViewCache->x_bEnableIncrementalUpdate;
    bool bPrevInvalidateLock = 
        rViewCache->LockCacheAndWindowInvalidates(
            true);

    // where are we?
    int nViewPassed = GetViewPassedSlices();
    int nViewWidth = GetViewWidthSlices();

    // step forward on the screen
    nViewPassed++;

    TESTLOG(
        "",
        " " +
            (bPartialUpdateMode? 
                "Partial Update Mode" 
                : 
                "No Partial Update Disabled") + 
            ", " +
            (bPrevInvalidateLock? 
                "invalidate was already locked" 
                : 
                "invalidate has just been locked") + 
            ".\n" + 
            "ViewState passed to online screen offset " + 
            Str(nViewPassed) + ", ");

    // round around?
    if (nViewPassed > nViewWidth)
    {
        rASSERT(nViewPassed == GetViewWidthSlices() + 1);
        nViewPassed = 1;

        TESTLOG(
            "",
            "and WRAPPING around at width=" + Str(nViewWidth) + "\n");

        //bPartialUpdateMode = false;

        // Profiling
        rViewCache->_x_tickPerScreen = 
            sys::GGetUtc() - rViewCache->_x_tickStartScreen;
        rViewCache->_x_tickStartScreen = sys::GGetUtc();
    }
    else
    {
        TESTLOG(
            "",
            "still not reached width=" + Str(nViewWidth) + "\n");
    }

    // save new offset
    SetViewPassedSlicesInternalAndForDebugUi(nViewPassed);

    // scroll one point if we already don't fit on the screen
    if (nStoredSlices > GetViewWidthSlices())
    {
        SetViewScrollSliceInternalPos(GetViewScrollSlice() + 1);
    }

    // fast update only one slice?
    if (bPartialUpdateMode)
    {
        // incrementally draw only new slice
        rViewCache->
            RenderOnlyNextOnlineSliceAndIgnoreInvalidates();

        // restore invalidate lock status
        rViewCache->
            LockCacheAndWindowInvalidates(
                bPrevInvalidateLock);
    }
    else
    {
        NOTEST;
        rFAIL("non-partial update is only for debug!");

        // restore invalidate lock status
        rViewCache->
            LockCacheAndWindowInvalidates(
                bPrevInvalidateLock);

        // slow invalidate
        rViewCache->
            InvalidateViewCache();
    }

    TESTLOG(
        "",
        "AdvanceViewPassed complete.\n");
}

void CViewState::SetViewScrollSliceFromUserAction(
        int iScrollAtSlice)
{
    bool bReallyChangingScroll = (m_iScrollSlice != iScrollAtSlice);

    SetViewScrollSliceInternalPos(iScrollAtSlice);

    // Update calculations
    ref<CImageImpl> rImage = 
        _x_pParentImage.Get().Valid();
    ref<CDocIface> rDoc = 
        rImage->GetParentDoc();
    ref<CCalcupSystemIface> rCalcupSystem = 
        rDoc->x_rCalcupSystem;

    // fix for auto-scroll killing restored indexes after playback
    if (bReallyChangingScroll)
    {
        rCalcupSystem->ResetManualRecalsPosOrUpdateAutomatic();
    }

    // Notify RASTER
    PrepareOrSendTimeScrollSync(true); // bSend

[...]