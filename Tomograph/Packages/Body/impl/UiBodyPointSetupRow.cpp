// UiBodyPointSetupRow.cpp

#include "CProject.h"
#include "CBodyPointImpl.h"
#include "CBodyProfileImpl.h"

void UiBodyPointSetupRow::Ui()
{
    bool bSelected = 
        m_rBodyProfile->x_iSelectedBodyPoint == m_iBodyPoint;
    rASSERT(m_iBodyPoint >= 0);

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rBodyPoint->_m_pUiBodyPointSetupRow = rUi;

        rUi->SetIdleUpdate(true); // to simplify selection update

        rUi->SetSelect(bSelected);

        rUi->OnMouse = [=]
        {
            if (GetOnMouseEvent() == E_Mouse_LeftButtonDown)
            {
                m_rBodyProfile->x_iSelectedBodyPoint = m_iBodyPoint;
            }
        }
    }

    //
    // Point #
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetText(Str(m_iBodyPoint + 1));

        rUi->SetPreferredX(scale(35));
        rUi->SetAlignContentToCenterX();
    }

    //
    // Point Name Input
    //
    {
        ref<CUiEditGp> rUi;

        rUi->SetText(m_rBodyPoint->x_sBodyPointName);

        rUi->SetTip(
            TRANSLATE(
                "Body point[... UI-string-removed]");

        rUi->SetMaxX(oo);

        rUi->SetLookToUnderlined();

        rUi->OnChange = [=]
        {
            m_rBodyPoint->x_sBodyPointName =
                rUi->InputPlainText();
        }

        rUi->OnFocused = [=]
        {
            m_rBodyProfile->x_iSelectedBodyPoint = m_iBodyPoint;
        }
    }
}
