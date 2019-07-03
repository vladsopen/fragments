// UiDemoExitSetupNop.cpp

#include "CProject.h"
#include "CDemoImpl.h"

void UiDemoExitSetupNop::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rDemo->_m_ptrmapUiDemoExitSetupNop += rUi;
    }


    //
    // Label
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetText(
            "Not sending any data");

        //rUi->SetTip(
        //    "Used only to create a new sample");
    }
}
