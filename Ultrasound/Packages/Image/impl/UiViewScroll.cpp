// UiViewScroll.cpp

#include "CUsmainProject.h"
#include "CViewState.h"
#include "CImageImpl.h"
#include "CDocIface.h"
#include "COnlineModeIface.h"
#include "CStoreIface.h"
#include "CViewCache.h"
#include "COnlineTypeIface.h"
#include "CInitTypeIface.h"
#include "CCustomizeDocIfaceGp.h"
#include "CGlobalConfigIface.h"
#include "CDebugConfigIface.h"
#include "CTimeScrollConfigIface.h"
#include "CLoaderIface.h"
#include "CNoteMarkingIface.h"

void UiViewScroll::Ui()
{
    ref<CImageImpl> rImage = 
        m_rViewState->_x_pParentImage.Get().Valid();
    ref<CStoreIface> rStore = 
        rImage->GetRelatedStore();
    ref<CViewCache> rViewCache = 
        rImage->x_rViewCache;
    ref<CDocIface> rDoc = 
        rImage->_x_pParentDoc.Get().Valid();
    ref<COnlineModeIface> rOnlineMode = 
        rDoc->_x_rOnlineMode;
    type<COnlineTypeIface> typeOnlineType = 
        rOnlineMode->x_typeOnlineType;
    ref<CNoteMarkingIface> rNoteMarking = 
        rDoc->x_rNoteMarking;

    int nTotalVirtualSlices = 
        rStore->GetStoreRipeMatrixSliceCount();
    int nTotalSavedSlices = 
        rStore->GetStoreLastSavedRipeMatrixSliceCount();
    int nLostInitialSlices = 
        nTotalVirtualSlices - nTotalSavedSlices;
    rASSERT(nLostInitialSlices >= 0);

    int nRange = 
        nTotalSavedSlices;

    rNoteMarking->
        ModifyMediaScrollRanges(
            out nRange,
            out nLostInitialSlices,
            nTotalVirtualSlices);

    nRange = 
        math::GMax(
            0,
            nRange - m_rViewState->GetViewWidthSlices());

    int nSliderPos = 
        m_rViewState->GetViewScrollSlice() - 
            nLostInitialSlices;

    bool bHackMode = 
        CUsmainProject::GGetIt().
            x_rUsmainGlobalConfig->
                x_rDebugConfig->
                    _x_bHackMode;

    bool bCompactRulerBars = 
        rDoc->_x_rCustomizeDoc->x_bCompactRulerBars;

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rViewState->_m_pUiViewScroll = rUi;

        rUi->SetAlikeY(m_rViewState->_m_alikeyViewScroll);
    }

    sloc slocScrollDisableInfo = "";

    if (!bHackMode)
    {
        // online?
        if (!typeOnlineType->IsImageScrollEnabled())
        {
            slocScrollDisableInfo = 
                TRANSLATE(
                    "Scroll in [... UI-string-removed]");
        }

        // saved cut?
        if (!rDoc->_x_typeInitType->IsViewScrollingEnabled() 
            &&
            !rDoc->_x_rCustomizeDoc->x_bShowScrollBarForSaved)
            //!CUsmainProject::GGetIt().
            //    x_rLoader->
            //        x_bUscanViewerApp)
        {
            return;
        }

        // no data?
        if (rDoc->x_rStore->GetStoreRipeMatrixSliceCount() <= 0)
        {
            slocScrollDisableInfo = 
                TRANSLATE(
                    "No data to[... UI-string-removed]");
        }

        // Compact mode?
        if (bCompactRulerBars)
        {
            return;
        }
    }

    //
    // Media Scroll
    //
    {
        ref<CUiSliderGp> rUi;

        m_rViewState->_m_pUiSlider = rUi;

        rUi->SetIdleUpdate(true);

        if (slocScrollDisableInfo == "")
        {
            rUi->SetSliderMax(nRange);

            rUi->SetSliderStep(10);

            rUi->SetSliderPageSize(
                m_rViewState->GetViewWidthSlices());

            rUi->SetSliderPos(
                nSliderPos);
        }
        else
        {
            // prevent it from jumping online
            rUi->SetSliderMax(0);
            //rUi->SetSliderStep(10);
            rUi->SetSliderPageSize(0);
            rUi->SetSliderPos(0);
        }

        rUi->SetTip(
            TRANSLATE(
                "Media reco[... UI-string-removed]");

        if (slocScrollDisableInfo != "")
        {
            rUi->SetDisableInfo(slocScrollDisableInfo);
        }
        else
        if (nTotalVirtualSlices <= 0)
        {
            rUi->SetDisableInfo(
                TRANSLATE(
                    "There is n[... UI-string-removed]");
        }
        else
        if (nRange <= 0)
        {
            rUi->SetDisableInfo(
                TRANSLATE(
                    "All record[... UI-string-removed]");
        }

        rUi->OnChange = [=]
        {
            m_rViewState->
                SetViewPassedSlicesInternalAndForDebugUi(
                    0);

            m_rViewState->
                SetViewScrollSliceFromUserAction(
                    nLostInitialSlices + rUi->GetSliderPos());
        }
    }
}
