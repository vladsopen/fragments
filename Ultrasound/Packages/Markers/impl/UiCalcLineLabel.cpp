// UiCalcLineLabel.cpp

#include "CUsmainProject.h"
#include "CCalcLineState.h"
#include "CCalcLineType.h"
#include "CUsoundAxisIfaceGp.h"
#include "CGlobalConfigIface.h"
#include "CGlobalPaletteConfigIface.h"
#include "CLocalPaletteConfigIface.h"
#include "VirtOs_ask.h"

void UiCalcLineLabel::Ui()
{
    type<CCalcLineType> typeCalcLineType = 
        m_rCalcLineState->GetKey();

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rCalcLineState->_m_ptrmapUiCalcLineLabel += rUi;

        // darker back to erase the media and the end of the marker
        rUi->SetBackColor(rgb(0, 0, 0));

        rUi->OnModifyShape = [=]
        {
            m_rCalcLineState->
                ShapeCalcLabel(rUi);
        }

        rUi->OnMouse = [=]
        {
            // Slider will initiate drag routine if reqired
            m_rCalcLineState->
                HandleLineOnMouse(
                    rUi, 
                    GetOnMouseEvent());
        }
    }

    //
    // Label
    //
    {
        ref<CUiButtonGp> rUi;

        rUi->SetText(
            typeCalcLineType->GetCalcLineName());

        rUi->SetTip(
            typeCalcLineType->GetCalcLineDescription());

        rUi->SetCursor(
            typeCalcLineType->
                GetCalcLineUsoundAxis()->
                    GetCalcMarkerCursor());

        rUi->SetAlignContentToCenterX();

        rUi->SetBackColor(
            typeCalcLineType->GetCalcLineColor());
        rUi->SetBackAlpha(
            percent(60));

        rUi->SetTextColor(rgb(255, 255, 255));
        rUi->SetFontBold(true);
        rUi->SetFontFace(E_FontFace_Arial);

        rUi->SetBorderAtForeground(true);
        rUi->SetBorderStyleToFlat();
        rUi->SetBorderDepth(1);
        rUi->SetBorderColor(typeCalcLineType->GetCalcLineColor());
    }
}
