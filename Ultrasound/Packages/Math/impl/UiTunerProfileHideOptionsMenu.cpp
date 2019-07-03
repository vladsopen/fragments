// UiTunerProfileHideOptionsMenu.cpp

#include "CDucalcProject.h"
#include "CTunerProfile.h"
#include "VirtOs_LegacyResource.h"

void UiTunerProfileHideOptionsMenu::Ui()
{
    {
        ref<CUiPanelGp> rUi = this;

        m_rTunerProfile->_m_pUiTunerProfileHideOptionsMenu = rUi;
    }

    //
    // Hide Rare Values
    //
    {
        ref<CUiCheckGp> rUi;

        rUi->SetText("Hide rare");
        rUi->SetCheck(m_rTunerProfile->x_bHideRareValuesUi);

        rUi->OnClick = [=]
        {
            m_rTunerProfile->x_bHideRareValuesUi = 
                !m_rTunerProfile->x_bHideRareValuesUi;
        }
    }

    NL();

    { ref<CUiSeparateRowsGp> rUi; }

    NL();

    //
    // Mark Unused Hidden
    //
    {
        ref<CUiMenuItemGp> rUi;

        rUi->SetText("Mark All Unused as Hidden");

        rUi->OnClick = [=]
        {
            m_rTunerProfile->MarkAllUnusedValuesAsHidden();
        }
    }

    NL();

    //
    // Hide All
    //
    {
        ref<CUiMenuItemGp> rUi;

        rUi->SetText("Hide All");

        rUi->OnClick = [=]
        {
            m_rTunerProfile->MarkAllAsHidden();
        }
    }

    NL();

    //
    // Hide All
    //
    {
        ref<CUiMenuItemGp> rUi;

        rUi->SetText("Unhide All");

        rUi->OnClick = [=]
        {
            m_rTunerProfile->MarkAllAsUnhidden();
        }
    }

    NL();

    { ref<CUiSeparateRowsGp> rUi; }

    NL();

    //
    // Force Show 'Settings' Values
    //
    {
        ref<CUiCheckGp> rUi;

        rUi->SetText("Enable Viewing for 'Setting'-values");
        rUi->SetCheck(m_rTunerProfile->x_bForceViewSettings);
        rUi->SetIcon((res) RES_TINY_SETTING);

        rUi->OnClick = [=]
        {
            m_rTunerProfile->x_bForceViewSettings = 
                !m_rTunerProfile->x_bForceViewSettings;
        }
    }


    NL();

    //
    // Force Show 'Mutators' Values
    //
    {
        ref<CUiCheckGp> rUi;

        rUi->SetText("Enable Viewing for 'Mutator'-values");
        rUi->SetCheck(m_rTunerProfile->x_bForceViewMutators);
        rUi->SetIcon((res) RES_TINY_MUTATOR);

        rUi->OnClick = [=]
        {
            m_rTunerProfile->x_bForceViewMutators = 
                !m_rTunerProfile->x_bForceViewMutators;
        }
    }

    NL();

    //
    // Force Show 'Indicators' Values
    //
    {
        ref<CUiCheckGp> rUi;

        rUi->SetText("Enable Viewing for 'Indicator'-values");
        rUi->SetCheck(m_rTunerProfile->x_bForceViewIndicators);
        rUi->SetIcon((res) RES_TINY_INDICATOR);

        rUi->OnClick = [=]
        {
            m_rTunerProfile->x_bForceViewIndicators = 
                !m_rTunerProfile->x_bForceViewIndicators;
        }
    }
}
