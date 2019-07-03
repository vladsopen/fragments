// UiCalcLine.cpp

#include "CUsmainProject.h"
#include "CCalcLineState.h"
#include "CCalcLineType.h"
#include "CUsoundAxisIfaceGp.h"
#include "CMarkerIface.h"

void UiCalcLine::Ui()
{
    type<CCalcLineType> typeCalcLineType = 
        m_rCalcLineState->GetKey();

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rCalcLineState->_m_ptrmapUiCalcLine += rUi;

        // for the histogram
        rUi->SetClippingPaintToSiblings(false);

        rUi->SetIndependentShape(true);

        rUi->OnIndependentShape = [=]
        {
            m_rCalcLineState->
                ShapeCalcLine(rUi);
        }

        rUi->OnMouse = [=]
        {
            // Slider will initiate drag routine if reqired
            m_rCalcLineState->
                HandleLineOnMouse(
                    rUi, 
                    GetOnMouseEvent());
        }

        rUi->OnPaint = [=]
        {
            draw dc;
            dc.HandleDrawUi(this);

            rUi->PaintContent(GetOnPaintDc());

            m_rCalcLineState->
                HandleLineOnPaint(
                    dc,
                    rUi);

        }
    }

    /*
    //
    // Back
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetCursor(
            typeCalcLineType->
                GetCalcLineUsoundAxis()->
                    GetCalcMarkerCursor());

        // Dark border around the marker
        rUi->SetBackColor(
            rgb(0, 0, 0));
        rUi->SetBackAlpha(
            percent(50));

        rUi->SetMarginAtLeft(CCalcLineState::C_nCalcMarkerDimBorder);
        rUi->SetMarginAtTop(CCalcLineState::C_nCalcMarkerDimBorder);
        rUi->SetMarginAtRight(CCalcLineState::C_nCalcMarkerDimBorder);
        rUi->SetMarginAtBottom(CCalcLineState::C_nCalcMarkerDimBorder);

        rUi->SetMaxX(oo);
        rUi->SetMaxY(oo);
        */

        //
        // Button Line
        //
        {
            ref<CUiButtonGp> rUi;

            rUi->SetTip(
                typeCalcLineType->GetCalcLineDescription());

            rUi->SetCursor(
                typeCalcLineType->
                    GetCalcLineUsoundAxis()->
                        GetCalcMarkerCursor());

            m_rCalcLineState->
                x_rMarker->
                    EqualizeMarkerLine(
                        rUi,
                        // grinko: not visible on media
                        //typeCalcLineType->GetCalcLineColor());
                        rgb(255, 255, 255));

            rUi->SetMaxX(oo);
            rUi->SetMaxY(oo);
        }
    //}
}
