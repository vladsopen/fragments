// UiJobFilterSetupMenu.cpp

#include "CProject.h"
#include "CJobFilterSetup.h"
#include "CJobFilterType.h"
#include "CJobFilterSwitch.h"

void UiJobFilterSetupMenu::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rJobFilterSetup->_m_ptrmapUiJobFilterSetupMenu += rUi;
    }

    //
    // Caption
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetText(
            TRANSLATE(
                "List[... UI-string-removed]");

        rUi->SetTip(
            TRANSLATE(
                "Package li[... UI-string-removed]");
    }

    NL();

    //
    // indent
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetText("  ");
    }

    //
    // indented list
    //
    {
        ref<CUiPanelGp> rUi;

        //
        // (o) All
        //
        {
            ref<CUiRadioGp> rUi;

            rUi->SetText(
                TRANSLATE(
                    "All[... UI-string-removed]");

            rUi->SetCheck(
                !m_rJobFilterSetup->IsAnyJobFilterActive());

            rUi->SetMaxX(oo);

            rUi->OnClick = [=]
            {
                m_rJobFilterSetup->
                    ClearAllJobFilters();
            }
        }

        NL();

        // iterate filters
        {
            type<CJobFilterType> typeIterFilter;
            for (
                iter i;
                type<CJobFilterType>()->GetDerivedList().
                    Iterate(
                        out i,
                        out typeIterFilter);)
            {
                //
                // Next Filrer = Normal
                //
                {
                    ref<UiContainer> rUi = 
                        m_rJobFilterSetup->
                            NewUiJobFilterSetupRow(
                                typeIterFilter,
                                type<CJobFilterSwitchIsOnNormal>());
                }

                NL();

                //
                // Next Filrer = Inverse
                //
                {
                    ref<UiContainer> rUi = 
                        m_rJobFilterSetup->
                            NewUiJobFilterSetupRow(
                                typeIterFilter,
                                type<CJobFilterSwitchIsOnInverse>());
                }

                NL();
            }
        }
    }
}
