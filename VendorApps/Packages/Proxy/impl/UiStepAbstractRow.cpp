// UiStepAbstractRow.cpp

#include "CProject.h"
#include "CStepAbstract.h"
#include "CSequence.h"

void UiStepAbstractRow::Ui()
{
    ref<CSequence> rSequence = 
        m_rStep->GetSequence();

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rStep->_m_ptrmapUiStepAbstractRow += rUi;

        rUi->SetMarginAtTop(scale(6));
    }

    //
    // Step info
    //
    {
        ref<CUiStaticGp> rUi;

        str sText = 
            m_rStep->x_sReplaceStepTitle;
        if (sText == "")
        {
            sText = 
                m_rStep->GetFixedStepTitle();
        }

        rUi->SetText(
            sText);

        rUi->SetTip(
            "A step in checkup sequence");

        rUi->SetIcon(
            m_rStep->x_bStepComplete?
                RES__OK_ICON()
                :
                RES__NEXT_ICON()
                //RES__NEXT_ICON()
                //RES__BALL_M()
                //RES__CANCEL_ICON()
                );

        rUi->SetSelect(
            m_rStep == rSequence->_x_pLastPolledStep);

        rUi->SetMarginAtLeft(scale(8));
        rUi->SetMarginAtRight(scale(8));
        rUi->SetMarginAtTop(scale(6));
        rUi->SetMarginAtBottom(scale(6));
    }

    NL();

    //
    // Step internals
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetMarginAtLeft(scale(30));

        //
        // Step details
        //
        {
            ref<CUiStaticGp> rUi;

            str sText = 
                m_rStep->x_sStepDetails;

            sText = sText.GetStripBoth(CHARMAP(" \n"));

            rUi->SetText(
                sText);

            rUi->SetTip(
                "Step details");
        }

        NL();

        //
        // Virtual add-ons
        //
        {
            ref<UiContainer> rUi = 
                m_rStep->
                    NewUiStepAdvanced();
        }

    }

}
