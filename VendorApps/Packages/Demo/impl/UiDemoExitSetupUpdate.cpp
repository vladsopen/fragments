// UiDemoExitSetupUpdate.cpp

#include "CProject.h"
#include "CDemoImpl.h"
#include "CProtocolIface.h"

void UiDemoExitSetupUpdate::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rDemo->_m_ptrmapUiDemoExitSetupUpdate += rUi;
    }

    //
    // Label
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetText(
            "UpdatedFileRevision =");

        rUi->SetTip(
            "Used only to update an existing sample");
    }

    //
    // _____ver
    //
    {
        ref<CUiEditGp> rUi;

        rUi->SetText(
            m_rDemo->
                x_rExternalProtocol->
                    x_sIniUpdatedFileRevision);

        rUi->SetTipAsPrev();

        rUi->OnChange = [=]
        {
            m_rDemo->
                x_rExternalProtocol->
                    x_sIniUpdatedFileRevision = 
                        rUi->InputPlainText();
        }
    }
}
