// UiDemoExitSetupFail.cpp

#include "CProject.h"
#include "CDemoImpl.h"
#include "CProtocolIface.h"

void UiDemoExitSetupFail::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rDemo->_m_ptrmapUiDemoExitSetupFail += rUi;
    }

    //
    // Label
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetText(
            "Failed =");

        rUi->SetTip(
            "Report application error");
    }

    //
    // _____error
    //
    {
        ref<CUiEditGp> rUi;

        rUi->SetText(
            m_rDemo->
                x_rExternalProtocol->
                    x_sIniResultFailed);

        rUi->SetTipAsPrev();

        rUi->OnChange = [=]
        {
            m_rDemo->
                x_rExternalProtocol->
                    x_sIniResultFailed = 
                        rUi->InputPlainText();
        }
    }
}
