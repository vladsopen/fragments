// CNotePlayPreview.cpp

#include "CUsmainProject.h"
#include "CNotePlayPreview.h"
#include "CNoteLineState.h"
#include "CNoteLineList.h"
#include "CNoteMarkingImpl.h"
#include "CDocIface.h"
#include "CSessionIfaceGp.h"
#include "CAppEventFloaterIfaceGp.h"
#include "CCustomizeDocIfaceGp.h"
#include "CDocMetricsIfaceGp.h"
#include "CImageIface.h"
#include "CSessionIface.h"
#include "CMultiPlayIface.h"
#include "CHardwareIface.h"
#include "CStoreIface.h"
#include "CAudioSliceGp.h"

CNotePlayPreview::CNotePlayPreview()
{
}

CNotePlayPreview::~CNotePlayPreview()
{
    HAIRY; // searching for slow destructors

    x_rDocCopy->CloseDoc();
}

void CNotePlayPreview::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CNotePlayPreview!");
}

void CNotePlayPreview::RunMarkerPreviewPlayback(
        num numMarkerPosVirtualStart,
        num numMarkerPosVirtualStop,
        rect rectScreen)
{
    // degraded
    return;

    CopyOriginalDoc(
        numMarkerPosVirtualStart,
        numMarkerPosVirtualStop);

    CreatePreviewFrame(
        rectScreen);
}

void CNotePlayPreview::CopyOriginalDoc(
        num numMarkerPosVirtualStart,
        num numMarkerPosVirtualStop)
{
    //
    // Save current doc
    //

    ref<CNoteLineState> rNoteLineState = 
        _x_pNoteLineState.Get().Valid();
    ref<CNoteLineList> rNoteLineList = 
        rNoteLineState->_x_pParentNoteLineList.Get().Valid();
    ref<CNoteMarkingImpl> rNoteMarking = 
        rNoteLineList->_x_pParentNoteMarking.Get().Valid();
    ref<CDocIface> rOriginalDoc = 
        rNoteMarking->_x_pParentDoc.Get().Valid();

    path pathTempDoc = 
        sys::GGetTempFolder().GetAppendedPath("MarkerPreview.tmp");

    // save faster without media
    bool bPreserveEnableMirrorDbMatrixList = 
        rOriginalDoc->x_rStore->x_bEnableMirrorDbMatrixList;
    //rOriginalDoc->x_rStore->x_bEnableMirrorDbMatrixList = false;

    //
    // Make audio copy
    //

    rOriginalDoc->
        x_rStore->
            SaveAudioForDoc(
                rOriginalDoc->
                    x_rStore->
                        NewAudioCompositeLinearCopy(0),
                pathTempDoc,
                out sError);

    if (sError != "")
    {
        CProject::GGetProjectWarningFloater()->
            LogNewEvent(
                TRANSLATE(
                    "Cannot cre[... UI-string-removed]");
        return
    }

    //
    // Load the copy
    //

    ref<CSessionIface> rSession = 
        rOriginalDoc->GetSessionForInitedDoc();

    x_rDocCopy->x_sDebugId = "NotePlay";

    if (!x_rDocCopy->
            InitAndLoadDocAndReportErrors(
                pathTempDoc,
                rSession,
                out sError))
    {
        CProject::GGetProjectWarningFloater()->
            LogNewEvent(
                TRANSLATE(
                    "Cannot loa[... UI-string-removed]");
        return
    }

    //
    // Setup the doc
    //

    /*
    // fake fourier
    x_rDocCopy->
        ForceMediaSliceCount(
            rOriginalDoc->
                x_rStore->
                    GetStoreLastSavedRipeMatrixSliceCount());
                    */

    // modes
    x_rDocCopy->_x_bDocExcludedFromList = true;
    x_rDocCopy->_x_bDocIndependentOnlineMode = true;
    ref<CCustomizeDocIfaceGp> rCustomizeDoc = 
        x_rDocCopy->_x_rCustomizeDoc;
    rCustomizeDoc->x_bCompactRulerBars = true;
    rCustomizeDoc->x_bIndependentRecorderTimeBars = true;

    // width
    //num numSetSeconds = 3;
        //numMarkerPosVirtualStop - numMarkerPosVirtualStart;
    //rASSERT(numSetSeconds > 0);
    int nViewWidthInMatrixSlices = 
        int(numMarkerPosVirtualStop - numMarkerPosVirtualStart);
    rASSERT(nViewWidthInMatrixSlices > 0);
        /*
        x_rDocCopy->
            GetDocMetrics()->
                ConvertDocViewWidthFromSecondsToSlices(
                    numSetSeconds);
                    */
    x_rDocCopy->
        x_rImage->
            SetViewWidthInMatrixSlices(
                nViewWidthInMatrixSlices);
    x_rDocCopy->
        x_rImage->
            SetViewScrollInMatrixSlicesWuu(
                (int) numMarkerPosVirtualStart);

    //
    // Start Playback
    //

    ref<CMultiPlayIface> rMultiPlay = 
        rSession->x_rHardware->x_rMultiPlay;

    rMultiPlay->
        InitiateDocIntoPlayback(
            x_rDocCopy,
            E_UsoundPlaybackRange_VisibleView,
            //E_UsoundPlaybackRange_WholeRecording
            //TODO: global Volume level from your UI
            percent(100));

    rMultiPlay->
        StartMultiPlay();

    //
    // Restore Scroll
    //

    rOriginalDoc->
        x_rImage->
            SetViewScrollInMatrixSlicesWithSliderUpdate(
                iSaveScroll);
}

void CNotePlayPreview::EndPreviewPlayback()
{
    ref<CSessionIface> rSession = 
        x_rDocCopy->GetSessionForInitedDoc();

    ref<CMultiPlayIface> rMultiPlay = 
        rSession->x_rHardware->x_rMultiPlay;

    rMultiPlay->
        EndMultiPlay();

    x_rDocCopy->CloseDoc();
    this->x_rDocCopy = null();
}

void CNotePlayPreview::CreatePreviewFrame(
        rect rectScreen)
{
    //
    // Create UI
    //

    ref<UiContainer> rUiFrame = 
        NewUiNotePlayPreviewFrame();

    rUiFrame->CreateModal();

    ref<CUiPanelGp> rUiPanel = rUiFrame->thispanel();

    rUiPanel->SetUnlimitedShape(true);
    rUiPanel->SetIndependentShape(true);

    /*
    size sizeMonitor = sys::GGetPrimaryMonitorResolution();
    if (rectScreen.right > sizeMonitor.w)
    {
        rectScreen.right = sizeMonitor.w;
    }
    */

    rUiPanel->
        SetInstantPos(
            rectScreen.left,
            rectScreen.top);
    rUiPanel->
        SetInstantSize(
            rectScreen.Width(),
            rectScreen.Height());
}

ref<UiContainer> CNotePlayPreview::NewUiNotePlayPreviewFrame()
{
    ref<UiNotePlayPreviewFrame> rUi;

    rUi->m_rNotePlayPreview = this;

    return rUi;
}


