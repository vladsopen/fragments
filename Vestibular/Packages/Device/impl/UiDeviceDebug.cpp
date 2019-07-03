// UiDeviceDebug.cpp

#include "CProject.h"
#include "CDeviceImpl.h"

void UiDeviceDebug::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rDevice->_m_ptrmapUiDeviceDebug += rUi;
    }

    //
    // [Scan]
    //
    {
        ref<CUiButtonGp> rUi;

        rUi->SetText("Rescan");

        rUi->SetMinX(off);

        rUi->OnClick = [=]
        {
            m_rDevice->RedetectDevice();
        }
    }

    //
    // "Status"
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetIdleUpdate(true);

        rUi->SetText(
            "Status: " + 
                m_rDevice->GetPosturalDeviceStatus());

        rUi->SetTip(
            "Status: " + 
                m_rDevice->GetPosturalDeviceStatus());

        rUi->SetMinX(30);
    }
}
