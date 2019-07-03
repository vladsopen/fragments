// UiDemoData.cpp

#include "CProject.h"
#include "CDemoImpl.h"

void UiDemoData::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rDemo->_m_ptrmapUiDemoData += rUi;
    }

    /*
    //
    // Input label
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetText();
    }
    */

    //
    // ______data
    //
    {
        ref<CUiEditGp> rUi;

        rUi->SetText(
            m_rDemo->x_sDemoData);

        rUi->SetTip(
            "Demo file test data");

        rUi->SetMaxX(oo);
        rUi->SetMinX(30);
        rUi->SetWrapText(true);

        rUi->OnChange = [=]
        {
            m_rDemo->x_sDemoData =
                rUi->InputPlainText();
        }
    }

    //
    // History
    //
    {
        ref<CUiHistoryGp> rUi;

        rUi->SetConfigKey(
            "DemoDataTxt");
    }
}
