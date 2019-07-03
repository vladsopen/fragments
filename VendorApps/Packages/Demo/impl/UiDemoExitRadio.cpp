// UiDemoExitRadio.cpp

#include "CProject.h"
#include "CDemoImpl.h"
#include "CDemoExitType.h"

void UiDemoExitRadio::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rDemo->_m_ptrmapUiDemoExitRadio += rUi;
    }

    //
    // (o) Exit type
    //
    {
        ref<CUiRadioGp> rUi;

        rUi->SetIdleUpdate(true);

        rUi->SetText(
            m_typeExitType->GetDemoExitDescription());

        rUi->SetTip(
            "Select demo exit type");

        rUi->SetCheck(
            m_rDemo->x_typeSelectedExitType
                ==
                m_typeExitType);

        rUi->OnClick = [=]
        {
            m_rDemo->x_typeSelectedExitType = 
                m_typeExitType;
        }
    }

    NL();

    //
    // Setup indented
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetIdleUpdate(true);

        rUi->SetMarginAtLeft(scale(30));

        if (m_rDemo->x_typeSelectedExitType
                !=
                m_typeExitType)
        {
            rUi->SetDisableInfo("This exit type is not selected");
        }

        //
        // Setup
        //
        {
            ref<UiContainer> rUi = 
                m_typeExitType->
                    NewUiExitTypeSetup(
                        m_rDemo);
        }

    }
}
