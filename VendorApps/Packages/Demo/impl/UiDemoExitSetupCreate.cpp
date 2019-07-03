// UiDemoExitSetupCreate.cpp

#include "CProject.h"
#include "CDemoImpl.h"
#include "CProtocolIface.h"

void UiDemoExitSetupCreate::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rDemo->_m_ptrmapUiDemoExitSetupCreate += rUi;
    }

    //
    // Label
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetText(
            "NewFilePrefix =");

        rUi->SetTip(
            "Used only to create a new sample");
    }

    //
    // _____prefix
    //
    {
        ref<CUiEditGp> rUi;

        rUi->SetText(
            m_rDemo->
                x_rExternalProtocol->
                    x_sIniNewFilePrefix);

        rUi->SetTipAsPrev();

        rUi->OnChange = [=]
        {
            m_rDemo->
                x_rExternalProtocol->
                    x_sIniNewFilePrefix = 
                        rUi->InputPlainText();
        }
    }

    NL();

    //
    // _______.ext
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetText(
            "FileExtension =");

        rUi->SetTip(
            "Optional file suffix");
    }
    {
        ref<CUiEditGp> rUi;

        rUi->SetText(
            m_rDemo->
                x_rExternalProtocol->
                    x_sIniNewFileExtension);

        rUi->SetTipAsPrev();

        rUi->OnChange = [=]
        {
            m_rDemo->
                x_rExternalProtocol->
                    x_sIniNewFileExtension = 
                        rUi->InputPlainText();
        }
    }

}
