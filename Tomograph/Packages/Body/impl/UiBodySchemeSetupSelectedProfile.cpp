// UiBodySchemeSetupSelectedProfile.cpp

#include "CProject.h"
#include "CBodySchemeImpl.h"
#include "CBodyProfileImpl.h"
#include "CBodyPointImpl.h"

void UiBodySchemeSetupSelectedProfile::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rBodyScheme->_m_pUiBodySchemeSetupSelectedProfile = rUi;

        int nVerticalIndent = (pix) scale(40);
        rUi->SetMarginAtTop(nVerticalIndent);
        rUi->SetMarginAtBottom(nVerticalIndent);
    }

    //
    // Group Box
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetGroupBox(true);

        rUi->SetPadY(2);

        // always get a half
        rUi->SetPreferredX(oo);
        rUi->SetMinX(0);

        if not_null(
            ref<CBodyProfileImpl> rBodyProfile = 
                m_rBodyScheme->
                    x_linkSelectedBodyProfile.Get().
                        FindPtr(
                            m_rBodyScheme))
        {
            //
            // Group Caption
            //
            {
                ref<CUiStaticGp> rUi;

                rUi->SetText(
                    "" +
                        TRANSLATE(
                            "Profile Na[... UI-string-removed]");
            }

            //
            // Profile Name Input
            //
            {
                ref<CUiEditGp> rUi;

                rUi->SetText(rBodyProfile->x_sBodyProfileName);

                rUi->SetTip(
                    TRANSLATE(
                        "Profile na[... UI-string-removed]");

                rUi->OnChange = [=]
                {
                    rBodyProfile->x_sBodyProfileName =
                        rUi->InputPlainText();
                }
            }

            NL();

            //
            // Current Profile Setup Panel
            //
            {
                ref<UiContainer> rUi = 
                    rBodyProfile->
                        NewUiBodyProfileSetup();
            }
        }
        else
        {
            //
            // Not selected
            //
            {
                ref<CUiStaticGp> rUi;

                rUi->SetText(
                    TRANSLATE(
                        "Profile is[... UI-string-removed]");
            }
        }
    }
}
