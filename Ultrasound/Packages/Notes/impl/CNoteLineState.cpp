// CNoteLineState.cpp

#include "CUsmainProject.h"
#include "CNoteLineState.h"
#include "CNoteLineTypeImpl.h"
#include "CNoteMarkingImpl.h"
#include "CNoteLineList.h"
#include "CUsoundAxisIfaceGp.h"
#include "CImageIface.h"
#include "CDocIface.h"
#include "CMatrixFormatGp.h"
#include "CBaselineIface.h"
#include "CStoreIface.h"
#include "CSliderIfaceGp.h"
#include "VirtOs_ask.h"
#include "CMatrixSliceIface.h"
#include "CUmoveIfaceGp.h"
#include "CVelocityUnitsIfaceGp.h"
#include "CNotePlayPreview.h"
#include "CMarkerIface.h"
#include "CUdbMirrorIfaceGp.h"
#include "CDbNoteIface.h"
#include "CCustomizeDocIfaceGp.h"

CNoteLineState::CNoteLineState()
{
}

void CNoteLineState::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CNoteLineState!");
}

bool CNoteLineState::MirrorDbNote(
        ref<CUdbMirrorIfaceGp> rUdbMirror,
        ref<CDbNoteIface> rDbNote)
{
    //
    // Attributes
    //

    MIRROR_DB(
        rUdbMirror,
        rDbNote->x_typeNoteLineType,
        m_typeNoteLineType);
    MIRROR_DB(
        rUdbMirror,
        rDbNote->x_numNotePosVirtual,
        x_numMarkerPosVirtual);
    MIRROR_DB(
        rUdbMirror,
        rDbNote->x_sNoteComment,
        x_sNoteComment);

    return true;
}

type<CNoteLineTypeImpl> CNoteLineState::GetKey()
{
    ASSERT(m_typeNoteLineType != null());
    return m_typeNoteLineType;
}

void CNoteLineState::InitKey(
        type<CNoteLineTypeImpl> typeNoteLineType)
{
    ASSERT(m_typeNoteLineType == null());
    ASSERT(typeNoteLineType != null());

    m_typeNoteLineType = typeNoteLineType;
}

ref<UiContainer> CNoteLineState::NewUiNoteLine()
{
    ref<UiNoteLine> rUi;

    rUi->m_rNoteLineState = this;

    return rUi;
}

ref<UiContainer> CNoteLineState::NewUiNoteLineLabel()
{
    ref<UiNoteLineLabel> rUi;

    rUi->m_rNoteLineState = this;

    return rUi;
}

void CNoteLineState::x_numMarkerPosVirtual_Set(
        num numValue)
{
    if (this->x_numMarkerPosVirtual == numValue)
    {
        return;
    }

    this->x_numMarkerPosVirtual = numValue;

    // disable original value auto-restore
    _m_numRestoreOriginalIndexValueRounded = OO;
}

void CNoteLineState::ShapeNoteLine(
        ref<CUiPanelGp> rUi)
{
    rect rectLine = 
        GetLineOrLabelShape(
            0); // nLabelOffset but this is a line

    rUi->SetInstantPos(
        rectLine.left,
        rectLine.top);

    rUi->SetInstantSize(
        rectLine.Width(),
        rectLine.Height());
}

void CNoteLineState::ShapeNoteLabel(
        ref<CUiPanelGp> rUi)
{
    // our type
    type<CNoteLineTypeImpl> typeNoteLineType = GetKey();
    type<CUsoundAxisIfaceGp> typeUsoundAxis = 
        typeNoteLineType->GetNoteLineUsoundAxis();

    rect rectLine = 
        GetLineOrLabelShape(
            C_nNoteLabelOffset);

    size sizeUi = rUi->GetInstantSize();

    if (!IsLineUiControlsVisible())
    {
        rUi->SetInstantSize(0, 0);
        return;
    }

    if (rectLine.left > -sizeUi.w &&
        rectLine.top > -sizeUi.h)
    {
        rUi->SetInstantPos(
            math::GMax(C_nNoteLabelIndent, rectLine.left - sizeUi.w / 2),
            math::GMax(C_nNoteLabelIndent, rectLine.top - sizeUi.h / 2));
    }
    else
    {
        rUi->SetInstantPos(-sizeUi.w, -sizeUi.h);
    }
}

void CNoteLineState::HandleLineOnMouse(
        ref<CUiPanelGp> rUi,
        EMouse eMouse)
{
    // bring active marker to the front
    if (eMouse == E_Mouse_Enter)
    {
        ref<CNoteLineList> rNoteLineList = 
            _x_pParentNoteLineList.Get().Valid();

        return;
    }

    // our type
    type<CNoteLineTypeImpl> typeNoteLineType = GetKey();
    type<CUsoundAxisIfaceGp> typeUsoundAxis = 
        typeNoteLineType->GetNoteLineUsoundAxis();

    point pointDraggedTo = point();
    if (ref<CUmoveIfaceGp>()->
            HookMouseDrag(
                rUi, // rUiToSetCapture
                eMouse, // mouse event to handle
                sys::GGetMousePosForCurrentMessage(), // global mouse position
                E_Mouse_LeftButtonDown, // starts drag
                E_Mouse_LeftButtonUp, // stops drag
                sys::GIsAccess(...), // dragging?
                rUi->GetInstantPos(), // current position
                out pointDraggedTo)) // desired drag target
    {
        // get thickness
        size sizeHandle = rUi->GetInstantSize();
        int nThickness = (int)
            typeUsoundAxis->
                ExtractCoordinateFromMarkerNumPoint(
                    sizeHandle.w,
                    sizeHandle.h);

        PlaceNoteLineInParent(
            pointDraggedTo,
            nThickness);
    }

    // Playback preview
    if (eMouse == E_Mouse_LeftButtonUp)
    {
        PlayPreviewAtMarker();
    }
}

void CNoteLineState::PlayPreviewAtMarker()
{
    // parents
    ref<CNoteLineList> rNoteLineList = 
        _x_pParentNoteLineList.Get().Valid();
    ref<CNoteMarkingImpl> rNoteMarking = 
        rNoteLineList->_x_pParentNoteMarking.Get().Valid();
    ref<CDocIface> rDoc = 
        rNoteMarking->_x_pParentDoc.Get().Valid();

    // our type
    type<CNoteLineTypeImpl> typeNoteLineType = GetKey();
    type<CUsoundAxisIfaceGp> typeUsoundAxis = 
        typeNoteLineType->GetNoteLineUsoundAxis();

    if (!typeNoteLineType->HasPlaybackAssociated())
    {
        return;
    }

    //
    // Start Coords
    //

    rect rectStartScreen = GetMarkerScreenRext();

    //
    // End Coords
    //

    if not_null(
        ref<CNoteLineState> rEndNoteLineState = 
            rNoteLineList->
                LookupSingularNoteByKey(
                    type<CNoteLineTypeImplAudioTo>()))
    {
        rect rectEndScreen = 
            rEndNoteLineState->GetMarkerScreenRext();

        //
        // Run preview controller
        //

        ref<CNotePlayPreview> rNotePlayPreview;
        rNotePlayPreview->_x_pNoteLineState = this;
        rNotePlayPreview->
            RunMarkerPreviewPlayback(
                x_numMarkerPosVirtual,
                rEndNoteLineState->x_numMarkerPosVirtual,
                rect(
                    rectStartScreen.left + C_nNoteMarkerThickness,
                    rectStartScreen.top,
                    rectEndScreen.left,
                    rectEndScreen.bottom));
    }
}

void CNoteLineState::HandleLineOnPaint(
        draw dc,
        ref<CUiPanelGp> rUi)
{
    //return;///!!!

    type<CNoteLineTypeImpl> typeNoteLineType = GetKey();

    // only for T1
    if (!typeNoteLineType->HasHistogramPainted())
    {
        return;
    }

    // parents
    ref<CNoteLineList> rNoteLineList = 
        _x_pParentNoteLineList.Get().Valid();
    ref<CNoteMarkingImpl> rNoteMarking = 
        rNoteLineList->_x_pParentNoteMarking.Get().Valid();
    ref<CDocIface> rDoc = 
        rNoteMarking->_x_pParentDoc.Get().Valid();

    // media (Dib) extent
    ref<CImageIface> rImage = 
        rDoc->x_rImage;
    //int nDibViewWidth =
        //rImage->GetViewWidthInMatrixSlices();
    //int nScrollSlices = 
        //rImage->GetViewScrollInMatrixSlices();
    int nDibMatrixHeight = 
        rDoc->x_rMatrixFormat->x_nMatrixLength;
    int nMatrixSliceAmplitudeRange = 
        rDoc->x_rMatrixFormat->x_nMatrixSliceAmplitudeRange;

    // Store data
    ref<CStoreIface> rStore = rDoc->x_rStore;

    // Baseline
    ref<CBaselineIface> rBaseline = rStore->x_rBaseline;
    int nBaseline = 
        rBaseline->
            GetBaselinePos(
                nDibMatrixHeight);

    // Our slice
    int iSlice = 
        //nScrollSlices + 
            (int) x_numMarkerPosVirtual;

    ref<CMatrixSliceIface> rMatrixSlice = 
        rStore->
            GetMatrixSliceAt(
                iSlice,
                ref<CMatrixSliceIface>());

    // window metrics
    size sizeUi = rUi->GetInstantSize();

    // All slice points scaled
    bool bFlipped = false;
    repeat(iPoint, rMatrixSlice->GetMatrixSlicePointCount()) // nDibMatrixHeight)
    {
        // Read a point
        int nAmplitude = 
            rMatrixSlice->
                GetSlicePointAmplitudeAt(
                    iPoint);
    
        pix pixHorizonal = 
            math::GMulDiv(
                nAmplitude,
                (int) C_pixHistogramHorizontalRange,
                nMatrixSliceAmplitudeRange);

        pix pixVertical = 
            math::GMulDiv(
                iPoint + nBaseline,
                sizeUi.h,
                nDibMatrixHeight);

        if (pixVertical >= sizeUi.h)
        {
            if (!bFlipped)
            {
                bFlipped = true;
                dc.MoveTo(0, 0);
            }

            pixVertical = pixVertical - sizeUi.h;
        }

        if (iPoint == 0)
        {
            dc.MoveTo(pixHorizonal, pixVertical);
        }
        else
        {
            dc.LineTo(pixHorizonal, pixVertical);
        }
    }
}

bool CNoteLineState::IsLineUiControlsVisible()
{

[...]