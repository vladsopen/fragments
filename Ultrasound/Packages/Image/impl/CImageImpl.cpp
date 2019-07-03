// CImageImpl.cpp

#include "CUsmainProject.h"
#include "CImageImpl.h"
#include "CDocIface.h"
#include "CViewState.h"
#include "CViewCache.h"
#include "CGlobalConfigIface.h"
#include "CGlobalPaletteConfigIface.h"
#include "CLocalPaletteConfigIface.h"
#include "CColoring.h"
#include "CSessionIface.h"
#include "CHardwareIface.h"
#include "CGeneratorListIface.h"
#include "CDbDocIface.h"
#include "CStoreIface.h"
#include "CDocMetricsIfaceGp.h"
#include "CRawSlotGp.h"
#include "CPaletteVirtualizerIface.h"

CImageImpl::CImageImpl()
{
    _init_AttachProperties();
}

void CImageImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachProperties();

    //rFAIL("make sure it is OK to expose CImageImpl!");
}

void CImageImpl::_init_AttachProperties()
{
    x_rViewState->_x_pParentImage = this;
    x_rViewCache->_x_pParentImage = this;
    x_rColoring->_x_pParentImage = this;
}

ref<CDocIface> CImageImpl::GetParentDoc()
{
    return _x_pParentDoc.Get().Valid();
}

/*
ref<CSessionIface> CImageImpl::GetRelatedSession()
{
    return GetParentDoc()->GetSessionForInitedDoc();
}
*/

ref<CStoreIface> CImageImpl::GetRelatedStore()
{
    return GetParentDoc()->x_rStore;
}

ref<CGeneratorListIface> CImageImpl::DigGeneratorList()
{
    NOTEST;

    // Get active signal generator
    ref<CDocIface> rDoc = 
        GetParentDoc();
    ref<CSessionIface> rSession =
        rDoc->GetSessionForInitedDoc();
    ref<CHardwareIface> rHardware = 
        rSession->x_rHardware;
    ref<CGeneratorListIface> rGeneratorList = 
        rHardware->x_rGeneratorList;

    return rGeneratorList;
}

ref<UiContainer> CImageImpl::OnNewUiImageMain()
{
    ref<UiImageMain> rUi;

    rUi->m_rImage = this;

    return rUi;
}

void CImageImpl::OnSetViewWidthInMatrixSlices(
        int nSlices)
{
    x_rViewState->SetViewWidthSlices(nSlices);
}

int CImageImpl::OnGetViewWidthInMatrixSlices()
{
    return x_rViewState->GetViewWidthSlices();
}

int CImageImpl::OnGetViewScrollInMatrixSlices()
{
    return x_rViewState->GetViewScrollSlice();
}

int CImageImpl::OnGetCycledScreenSliceCounterForIndexRecalc()
{
    return x_rViewState->GetViewPassedSlices();
}

void CImageImpl::OnForceBroadcastMyTimebarSyncToEveryone()
{
    x_rViewState->ForceViewStateBroadcastMyTimebarSyncToEveryone();
}

void CImageImpl::OnPrepareToViewRecordingAtWidth(
        int nDefaultViewWidthSlices)
{
    TESTLOG(
        "", 
        "Initializing Image for recording, setting width = " + 
            Str(nDefaultViewWidthSlices) + 
            "...\n");

    // Init view states
    x_rViewState->SetViewWidthSlices(nDefaultViewWidthSlices);

    // Palette
    x_rColoring->LoadDefaultPaletteFromConfig()
}

void CImageImpl::OnSetViewScrollInMatrixSlicesWuu(
        int nSlices)
{
    x_rViewState->SetViewScrollSliceInternalPos(nSlices);
}

void CImageImpl::OnSetViewScrollInMatrixSlicesWithSliderUpdate(
        int nSlices)
{
    x_rViewState->SetViewScrollSliceWithSliderUpdate(nSlices);
}

num CImageImpl::OnPrepareOrSendViewScrollRange(
        bool bSend)
{
    num numScrollRangeSeconds = -1;

    // Synchronize final slice count for RASTER scroll
    ref<CDocIface> rDoc = 
        GetParentDoc();
    if (!rDoc->_x_pDupomDoc)
    {
        ref<CSessionIface> rSession = 
            rDoc->GetSessionForInitedDoc();

        int nScrollRangeSlice = 
            rDoc->
                x_rStore->
                    //GetStoreRipeMatrixSliceCount();
                    GetStoreLastSavedRipeMatrixSliceCount();

        nScrollRangeSlice = 
            math::GMax(
                0,
                nScrollRangeSlice - GetViewWidthInMatrixSlices());

        numScrollRangeSeconds = 
            rDoc->
                GetDocMetrics()->
                    ConvertDocViewWidthFromSlicesToSeconds(
                        nScrollRangeSlice);

        // during init conversion may not be done
        if (!math::GIsNumUnreal(numScrollRangeSeconds) &&
            numScrollRangeSeconds > 0 &&
            bSend)
        {
            rSession->
                SynchronizeTimebarWidthInSeconds(
                    rDoc,
                    -1, // numWidthSeconds,
                    numScrollRangeSeconds, // numScrollRangeSeconds,
                    -1, // numScrollPosSeconds,
                    rDoc->x_rRawSlot->x_iScannerChannel,
                    rDoc->x_rRawSlot->x_iScannerCluster);

            // pos update
            SetViewScrollInMatrixSlicesWithSliderUpdate(
                GetViewScrollInMatrixSlices());
        }
    }

    return numScrollRangeSeconds;
}

void CImageImpl::OnPrepareOfflineScroll()
{
    // Init view states
    x_rViewState->ScrollToEndScreenReset();
}

void CImageImpl::OnUpdateAfterMediaEditInStore()
{
    NOTEST;

    // Full invalidate
    x_rViewCache->InvalidateViewCache();
}

void CImageImpl::OnUpdateAfterMediaFormatChanged()
{
    NOTEST;

    // Resize bitmap
    x_rViewCache->ResizeViewCacheAfterFormatChange();
}

void CImageImpl::OnUpdateAfterPaletteChange()
{
[...]
}

void CImageImpl::OnEmulateViewRedrawToMarkVisibleSlices()
{
    TESTLOG("", "\n" "Emulate View Redraw To Mark Visible Slices...\n");

    x_rViewCache->InvalidateViewCache();
    x_rViewCache->EnsureViewCacheUpdated();

    TESTLOG("", "Done Emulate View Redraw To Mark Visible Slices.\n\n");
}

void CImageImpl::OnResetForClearStore()
{
    // this is called twice: before and after storage reset

    x_rViewState->SetViewScrollSliceInternalPos(0);
    x_rViewState->SetViewPassedSlicesInternalAndForDebugUi(0);

    x_rViewCache->InvalidateViewCache();
    x_rViewCache->EnsureViewCacheUpdated();

    // reset sync scroll make it forgwr prev state
    x_rViewState->ResetScrollForRestart()

    //PrepareOrSendViewScrollRange(
        //true); // bSend
        // no, no need to resync/update scroll range, pos is enough
}

void CImageImpl::OnResetImagePaletteToFactoryDefaults()
{
    ref<CMatrixFormatGp> rMatrixFormat =
        x_rColoring->GetRelatedMatrixFormat();

    return 
        x_rColoring->
            x_rLocalPaletteConfig->
                ResetLocalPaletteConfigToFactoryDefaults(
                    rMatrixFormat);
}

ref<UiContainer> CImageImpl::OnNewUiImagePaletteBar()
{
    return 
        x_rColoring->
            NewUiColoringPaletteBar(
                E_PaletteBarMode_MainUiFull);
}
