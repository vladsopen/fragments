// UiBodySchemeSetup.cpp

#include "CProject.h"
#include "CBodySchemeImpl.h"
#include "CBodyProfileImpl.h"
#include "CBodyPointImpl.h"

void UiBodySchemeSetup::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rBodyScheme->_m_pUiBodySchemeSetup = rUi;

        rUi->SetPreferredX(scale(800));
        rUi->SetPreferredY(scale(600));
        rUi->SetMinX(0);
        rUi->SetMinY(0);
    }

    //
    // Left column
    //
    {
        ref<CUiPanelGp> rUi;

        //int nVerticalIndent = (pix) scale(40);
        //rUi->SetMarginAtTop(nVerticalIndent);
        //rUi->SetMarginAtBottom(nVerticalIndent);

        //
        // Left Section with Profile List
        //
        {
            ref<CUiPanelGp> rUi;

            rUi->SetGroupBox(true);

            // always get a half
            rUi->SetPreferredX(oo);
            rUi->SetMinX(0);

            //
            // Group Caption
            //
            {
                ref<CUiStaticGp> rUi;

                rUi->SetText(
                    TRANSLATE(
                        "Body Point[... UI-string-removed]");
            }

            NL();

            //
            // Profile List
            //
            {
                ref<UiContainer> rUi = 
                    m_rBodyScheme->
                        NewUiBodySchemeSetupProfileList(
                            CBodySchemeImpl::E_ProfileListUi_List);
            }
        }
    }

    //{ ref<CUiSeparateColumnsGp> rUi; }

    //
    // Right Section with Selected Profile Setup
    //
    {
        ref<UiContainer> rUi = 
            m_rBodyScheme->
                NewUiBodySchemeSetupSelectedProfile();
    }

    NL();
}
