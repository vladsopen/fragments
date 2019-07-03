// UiTunerProfileHighlightDifferenceMenu.cpp

#include "CDucalcProject.h"
#include "CTunerProfileList.h"
#include "CTunerProfile.h"

void UiTunerProfileHighlightDifferenceMenu::Ui()
{
    {
        ref<CUiPanelGp> rUi = this;

        m_rTunerProfile->_m_pUiTunerProfileHighlightDifferenceMenu = rUi;
    }

    //
    // Default Option: use current profile as the source
    //
    {
        ref<CUiMenuItemGp> rUi;

        rUi->SetText("<don't compare>");

        rUi->OnClick = [=]
        {
            m_rTunerProfile->x_linkHighlightDifferenceTunerProfile = null();
        }
    }

    NL();

    // List all options
    {
        ref<CTunerProfile> rIterTunerProfile;
        for (iter iOption; 
            m_rTunerProfileList->Iterate(
                out iOption, 
                out rIterTunerProfile);)
        {
            //
            // Next Item
            //
            {
                ref<CUiMenuItemGp> rUi;

                rUi->SetText(
                    rIterTunerProfile->x_sProfileName);
                if (rIterTunerProfile == m_rTunerProfile)
                {
                    rUi->SetDisableInfo("This is the current profile");
                }

                rUi->OnClick = [=]
                {
                    m_rTunerProfile->x_linkHighlightDifferenceTunerProfile = 
                        rIterTunerProfile;
                }
            }

            NL();
        }
    }
}
