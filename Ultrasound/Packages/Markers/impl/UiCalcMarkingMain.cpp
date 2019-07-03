// UiCalcMarkingMain.cpp

#include "CUsmainProject.h"
#include "CCalcMarkingImpl.h"
#include "VirtOs_LegacyResource.h"
#include "CPatientLinkIface.h"
#include "CCalcLineList.h"
#include "CCalcModeIface.h"
#include "CCalcupSystemIface.h"
#include "CUsoundCalcModeIfaceGp.h"

void UiCalcMarkingMain::Ui()
{
    ref<CCalcupSystemIface> rCalcupSystem = 
        m_rCalcMarking->_x_pParentCalcupSystem.Get().Valid();
    type<CCalcModeIface> typeCalcMode = 
        rCalcupSystem->GetCurrentCalcMode();

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rCalcMarking->_m_pUiCalcMarkingMain = rUi;

        // Overlay media Image
        rUi->SetOverlayPrevSibling(true);

        // Only this panel but not the markers and the button itself 
        //   should not interfere with underlaying cursoer settings
        rUi->SetMouseCursorTransparentLayer(true);
        //rUi->SetMouseInsensitive(true);

        
        /*
        // for demo only
        rUi->SetBorderColor(rgb(0, 100, 0));
        rUi->SetBorderDepth(3);
        rUi->SetBorderStyleToFlat();
        rUi->SetDialogRootPanel(); // some padding
        */
    }


    /*
        //
        // TEMP ----- CALC
        //
        {
            ref<CUiButtonGp> rUi;

            //rUi->SetText(
            //    TRANSLATE(
            //       [... UI-string-removed]");

            rUi->SetIcon(
                //!bCalcActive?
                    RES_CALC_ON()
                    //:
                    //RES_CALC_OFF());
                    );

            //rUi->SetMenuArrowSideward();

            rUi->SetIconLocationAtTop();

            //rUi->SetCheck(rDevice->x_bDeviceUiActivated);

            //rUi->SetFontSize(14);

            //rUi->SetAlignInParentToRight();
            //rUi->SetAlignInParentToTop();

            rUi->SetAlignContentToCenterX();

            rUi->OnClick = [=]
            {
            }
        }
    }
    */

    // Activated?
    if (!typeCalcMode->
            ConvertToUsoundCalcMode()->
                IsCalcMarkingUiActive())
    {
        return;
    }

    //
    // Line List
    //
    {
        ref<UiContainer> rUi = 
            m_rCalcMarking->
                _x_rCalcLineList->
                    NewUiCalcLineList();
    }

    //
    // Other Controls
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetOverlayPrevSibling(true);

        rUi->SetMouseCursorTransparentLayer(true);
        //rUi->SetMouseInsensitive(true);

        rUi->SetMarginAtLeft(scale(10));
        rUi->SetMarginAtBottom(scale(10));
        rUi->SetAlignContentToBottom();

        rUi->SetMaxX(oo);
        rUi->SetMaxY(oo);

        //
        // [Calculator]
        //
        {
            ref<CUiButtonGp> rUi;

            rUi->SetText(
                TRANSLATE(
                    "Formulas[... UI-string-removed]");

            rUi->SetIcon(
                m_rCalcMarking->GetFormulaEditorLargeIcon());

            rUi->OnClick = [=]
            {
                m_rCalcMarking->
                    OpenFormulaEditor();
            }
        }
    }
}
