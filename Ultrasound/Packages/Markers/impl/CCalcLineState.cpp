// CCalcLineState.cpp

#include "CUsmainProject.h"
#include "CCalcLineState.h"
#include "CCalcLineType.h"
#include "CCalcMarkingImpl.h"
#include "CCalcupSystemIface.h"
#include "CCalcLineList.h"
#include "CUsoundAxisIfaceGp.h"
#include "CUsoundIndexTypeIfaceGp.h"
#include "CImageIface.h"
#include "CDocIface.h"
#include "CMatrixFormatGp.h"
#include "CBaselineIface.h"
#include "CStoreIface.h"
#include "CSliderIfaceGp.h"
#include "CCalcModeIface.h"
#include "VirtOs_ask.h"
#include "CMatrixSliceIface.h"
#include "CUmoveIfaceGp.h"
#include "CVelocityUnitsIfaceGp.h"
#include "CMarkerIface.h"

CCalcLineState::CCalcLineState()
{
}

void CCalcLineState::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CCalcLineState!");
}

type<CCalcLineType> CCalcLineState::GetKey()
{
    ASSERT(m_typeCalcLineType != null());
    return m_typeCalcLineType;
}

void CCalcLineState::InitKey(
        type<CCalcLineType> typeCalcLineType)
{
    ASSERT(m_typeCalcLineType == null());
    ASSERT(typeCalcLineType != null());

    m_typeCalcLineType = typeCalcLineType;

    /*
    type<CCalcModeIface> typeCalcMode;
    if (m_typeCalcLineType.IsSelecting(typeCalcMode))
    {
        ask("==");
    }
    else
    {
        ask("!=");
    }
    */
}

ref<UiContainer> CCalcLineState::NewUiCalcLine()
{
    ref<UiCalcLine> rUi;

    rUi->m_rCalcLineState = this;

    return rUi;
}

ref<UiContainer> CCalcLineState::NewUiCalcLineLabel()
{
    ref<UiCalcLineLabel> rUi;

    rUi->m_rCalcLineState = this;

    return rUi;
}

void CCalcLineState::PrepareCalcLineForCalcModeWuu(
        type<CUsoundCalcModeIfaceGp> typeUsoundCalcMode)
{
    typeUsoundCalcMode;

    // parents
    ref<CCalcLineList> rCalcLineList = 
        _x_pParentCalcLineList.Get().Valid();
    ref<CCalcMarkingImpl> rCalcMarking = 
        rCalcLineList->_x_pParentCalcMarking.Get().Valid();
    ref<CCalcupSystemIface> rCalcupSystem = 
        rCalcMarking->_x_pParentCalcupSystem.Get().Valid();
    type<CVelocityUnitsIfaceGp> typeVelocityUnits = 
        rCalcupSystem->
            GetCurrentCalcupSystemVelocityUnits();
    ref<CDocIface> rDoc = 
        rCalcupSystem->_x_pParentDoc.Get().Valid();

    // our type
    type<CCalcLineType> typeCalcLineType = GetKey();
    type<CUsoundAxisIfaceGp> typeUsoundAxis = 
        typeCalcLineType->GetCalcLineUsoundAxis();

    // index value
    num numIndexValue = 
        rCalcupSystem->
            GetIndexValueRoundedInUiUnits(
                typeCalcLineType->GetCalcLineUsoundIndex());

    num numDib = 
        rCalcupSystem->
            ConvertBasicIndexValueToDib(
                numIndexValue,
                typeCalcLineType->GetCalcLineUsoundIndex());

    // init line pos
    x_numMarkerPosInDib = math::GIsNumUnreal(numDib)? 0 : int(numDib);

    if (typeVelocityUnits->IsRestorableOriginalIndexValueRounded())
    {
        _m_numRestoreOriginalIndexValueRounded = numIndexValue;
    }
}

void CCalcLineState::x_numMarkerPosInDib_Set(
        num numValue)
{
    if (this->x_numMarkerPosInDib == numValue)
    {
        return;
    }

    this->x_numMarkerPosInDib = numValue;

    // disable original value auto-restore
    _m_numRestoreOriginalIndexValueRounded = OO;
}

num CCalcLineState::GetRestoreOriginalIndexValueRoundedOrOo()
{
    return _m_numRestoreOriginalIndexValueRounded;
}

void CCalcLineState::ShapeCalcLine(
        ref<CUiPanelGp> rUi)
{
    rect rectLine = 
        CalcLineOrLabelShape(
            0); // nLabelOffset but this is a line

    rUi->SetInstantPos(
        rectLine.left,
        rectLine.top);

    rUi->SetInstantSize(
        rectLine.Width(),
        rectLine.Height());
}

void CCalcLineState::ShapeCalcLabel(
        ref<CUiPanelGp> rUi)
{
    rect rectLine = 
        CalcLineOrLabelShape(
            C_nCalcLabelOffset);

    rUi->SetInstantPos(
        rectLine.left,
        rectLine.top);
}

rect CCalcLineState::CalcLineOrLabelShape(
        int nLabelOffset)
{
    // parents
    ref<CCalcLineList> rCalcLineList = 
        _x_pParentCalcLineList.Get().Valid();
    ref<CCalcMarkingImpl> rCalcMarking = 
        rCalcLineList->_x_pParentCalcMarking.Get().Valid();
    ref<CCalcupSystemIface> rCalcupSystem = 
        rCalcMarking->_x_pParentCalcupSystem.Get().Valid();
    ref<CDocIface> rDoc = 
        rCalcupSystem->_x_pParentDoc.Get().Valid();

    // our type
    type<CCalcLineType> typeCalcLineType = GetKey();
    type<CUsoundAxisIfaceGp> typeUsoundAxis = 
        typeCalcLineType->GetCalcLineUsoundAxis();

    int nDib = (int) x_numMarkerPosInDib;

    // media (Dib) extent
    ref<CImageIface> rImage = 
        rDoc->x_rImage;
    int nDibViewWidth =
        rImage->GetViewWidthInMatrixSlices();
    int nDibMatrixHeight = 
        rDoc->x_rMatrixFormat->x_nMatrixLength;

    // add thickness in screen coordinates
    rectMarker = 
        typeUsoundAxis->
            AddCalcMarkerThickness(
                rectMarker,
                C_nCalcMarkerThickness);

    if (nLabelOffset < 0)
    {
        if (typeUsoundAxis == 
                type<CUsoundAxisIfaceGp>()->AsUsoundAxisForVelocityScaleY())
        {
            // keep label from sinking below bottom
            int nBelowBottom = 
                rectMarker.bottom - nLabelOffset - sizeImageUi.h;

            if (nBelowBottom > 0)
            {
                rectMarker.top -= nBelowBottom;
                rectMarker.bottom -= nBelowBottom;
            }
        }

        if (typeUsoundAxis == 
                type<CUsoundAxisIfaceGp>()->AsUsoundAxisForTimeX())
        {
            // keep label from breaking to the right
            int nRightBreak = 
                rectMarker.right - nLabelOffset - sizeImageUi.w;

            if (nRightBreak > 0)
            {
                rectMarker.left -= nRightBreak;
                rectMarker.right -= nRightBreak;
            }
        }
    }

    return 
        rect(
            rectMarker.left,
            rectMarker.top,
            rectMarker.right,
            rectMarker.bottom);
}

void CCalcLineState::HandleLineOnMouse(
        ref<CUiPanelGp> rUi,
        EMouse eMouse)
{
    // bring active marker to the front
    if (eMouse == E_Mouse_Enter)
    {
        ref<CCalcLineList> rCalcLineList = 
            _x_pParentCalcLineList.Get().Valid();

        // destroys UI!
        //rCalcLineList->x_typeSelectedCalcLineType = 
            //GetKey();

        return;
    }

    rect rectPrevLineShape =
        CalcLineOrLabelShape(
            0); // no need for offset to compare

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
        // our type
        type<CCalcLineType> typeCalcLineType = GetKey();
        type<CUsoundAxisIfaceGp> typeUsoundAxis = 
            typeCalcLineType->GetCalcLineUsoundAxis();

        // get thickness
        size sizeHandle = rUi->GetInstantSize();
        int nThickness = (int)
            typeUsoundAxis->
                ExtractCoordinateFromMarkerNumPoint(
                    sizeHandle.w,
                    sizeHandle.h);

        pointDraggedTo.x += nThickness / 2;
        pointDraggedTo.y += nThickness / 2;

        // we are interested in the center marker position
        //CASSERT(C_nCalcMarkerThickness % 2 == 0);

        // parents
        ref<CCalcLineList> rCalcLineList = 
            _x_pParentCalcLineList.Get().Valid();
        ref<CCalcMarkingImpl> rCalcMarking = 
            rCalcLineList->_x_pParentCalcMarking.Get().Valid();
        ref<CCalcupSystemIface> rCalcupSystem = 
            rCalcMarking->_x_pParentCalcupSystem.Get().Valid();
        ref<CDocIface> rDoc = 
            rCalcupSystem->_x_pParentDoc.Get().Valid();

        //
        // convert from screen coords
        //

        // find parent panel size == media image size
        size sizeImageUi = 
            rCalcLineList->GetUiCalcLineListInstantSize();

        // media (Dib) extent
        ref<CImageIface> rImage = 
            rDoc->x_rImage;
        int nDibViewWidth =
            rImage->GetViewWidthInMatrixSlices();
        int nDibMatrixHeight = 
            rDoc->x_rMatrixFormat->x_nMatrixLength;

        // convert from screen
        int nDibX = 
            math::GMulDiv(
                pointDraggedTo.x,
                nDibViewWidth,
                sizeImageUi.w);
        int nDibY = 
            math::GMulDiv(
                pointDraggedTo.y,
                nDibMatrixHeight,
                sizeImageUi.h);

        // media (Dib) height
        int nMatrixLength = 
            rDoc->x_rMatrixFormat->x_nMatrixLength;

        // select axis
        num numNewPos = 
            typeUsoundAxis->
                ExtractCoordinateFromMarkerNumPoint(
                    nDibX,
                    nDibY);

        // save new pos
        num numPrevMarkerPosInDib = x_numMarkerPosInDib;
        x_numMarkerPosInDib = numNewPos;

        // do not change logical position if screen pos cannot follow

        rect rectNewLineShape =
            CalcLineOrLabelShape(
                0); // no need for offset to compare

        if (rectNewLineShape == rectPrevLineShape)
        {
            x_numMarkerPosInDib = numPrevMarkerPosInDib;
        }

        debug("Calc.Marking./show.Drag:").
            FloatNote(
                "Drag to " + 
                    Str(pointDraggedTo.x) + 
                    "x" + 
                    Str(pointDraggedTo.y));

        if (GetKey() != type<CCalcLineTypeFlowMeanVelocity>())
        {
            rCalcMarking->
                UpdateMeanVelocity();
        }

        // initiate updates
        rCalcupSystem->
            UpdateUsoundIndexValuesAfterManualCalcChange();
        rCalcLineList->
            UpdateMarkersAfterMove();
    }
}

void CCalcLineState::HandleLineOnPaint(
        draw dc,
        ref<CUiPanelGp> rUi)
{
    type<CCalcLineType> typeCalcLineType = GetKey();

    // only for T1
    if (!typeCalcLineType->HasHistogramPainted())
    {
        return;
    }

    // parents
    ref<CCalcLineList> rCalcLineList = 
        _x_pParentCalcLineList.Get().Valid();
    ref<CCalcMarkingImpl> rCalcMarking = 
        rCalcLineList->_x_pParentCalcMarking.Get().Valid();
    ref<CCalcupSystemIface> rCalcupSystem = 
        rCalcMarking->_x_pParentCalcupSystem.Get().Valid();
    ref<CDocIface> rDoc = 
        rCalcupSystem->_x_pParentDoc.Get().Valid();

    // media (Dib) extent
    ref<CImageIface> rImage = 
        rDoc->x_rImage;
    //int nDibViewWidth =
        //rImage->GetViewWidthInMatrixSlices();
    int nScrollSlices = 
        rImage->GetViewScrollInMatrixSlices();
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
    int iSlice = nScrollSlices + (int) x_numMarkerPosInDib;
    ref<CMatrixSliceIface> rMatrixSlice = 
        rStore->
            GetMatrixSliceAt(
                iSlice,
                ref<CMatrixSliceIface>());

    // window metrics
    size sizeUi = rUi->GetInstantSize();

    // prepare DC
    dc.SetPen(PS_SOLID, C_pixHistogramLineThickness, C_rgbHistogram);

    // All slice points scaled
    bool bFlipped = false;
    // restart while calclines visible
    if (rMatrixSlice->GetMatrixSlicePointCount() > 0)
    repeat(iPoint, nDibMatrixHeight)
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

