// UiTunerProfileEditForm.cpp

#include "CDucalcProject.h"
/*
*/
#include "CTunerProfileList.h"
#include "CTunerValue.h"
#include "CTunerProfile.h"

void UiTunerProfileEditForm::Ui()
{
    {
        ref<CUiPanelGp> rUi = this;

        m_rTunerProfile->_m_pUiTunerProfileEditForm = rUi;
    }

    //
    // Name Input Label
    //
    {
        ref<CUiStaticGp> rUi;

        EqualizeControls(rUi);

        rUi->SetText("Name:");
    }

    //
    // Name Edit
    //
    {
        ref<CUiEditGp> rUi;

        EqualizeControls(rUi);

        rUi->SetText(m_rTunerProfile->x_sProfileName);
        
        rUi->OnChange = [=]
        {
            m_rTunerProfile->x_sProfileName.
                Set(
                    rUi->InputPlainText(), 
                    m_rTunerProfileList);
        }
    }

    { ref<CUiSeparateColumnsGp> rUi; }

    //
    // Selected for Compare?
    //
    {
        bool bCompare = 
            m_rTunerProfileList->x_linkCompareTunerProfile.Get().
                FindPtr(m_rTunerProfileList) == m_rTunerProfile;

        ref<CUiCheckGp> rUi;

        EqualizeControls(rUi);

        rUi->SetText("Compare Curve");
        rUi->SetCheck(bCompare);
        if (bCompare)
            rUi->SetDisableInfo("The profile is selected for compare.");
        
        rUi->OnClick = [=]
        {
            m_rTunerProfileList->x_linkCompareTunerProfile = m_rTunerProfile;
        }
    }

    { ref<CUiSeparateColumnsGp> rUi; }

    //
    // Compare Selection Label
    //
    {
        ref<CUiStaticGp> rUi;

        EqualizeControls(rUi);

        rUi->SetText("Highlight Diff:");
    }

    //
    // Compare Profile Selector
    //
    {
        ref<CUiDropDownGp> rUi;

        EqualizeControls(rUi);

        //rUi->SetBorderDepth(0);
        //rUi->SetBorderStyleToFlat();

        str sText;
        ptr<CTunerProfile> pHighlightDifferenceTunerProfile = 
            m_rTunerProfile->x_linkHighlightDifferenceTunerProfile.Get().
                FindPtr(m_rTunerProfileList);
        if not_null(
            ref<CTunerProfile> rHighlightDifferenceTunerProfile = 
                pHighlightDifferenceTunerProfile)
        {
            sText = rHighlightDifferenceTunerProfile->x_sProfileName;
            rUi->SetBackColor(CTunerValue::C_rgbDifferenceHighlightBack);
        }
        else
        {
            sText = "<not selected>";
            rUi->SetBackColor(rgb(192, 192, 192));
        }

        rUi->SetText(sText);

        rUi->OnClick = [=]
        {
            ref<UiContainer> rUiFrame = 
                m_rTunerProfile->NewUiTunerProfileHighlightDifferenceMenu(
                    m_rTunerProfileList);

            rUiFrame->CreateDropDownAnchoredTo(this);
        }
    }

    { ref<CUiSeparateColumnsGp> rUi; }

    //
    // Hide Options Menu
    //
    {
        ref<CUiMenuItemGp> rUi;

        EqualizeControls(rUi);

        rUi->SetText("Hide");
        rUi->SetFontSize(16);
        rUi->SetCheck(m_rTunerProfile->x_bHideRareValuesUi);
        rUi->SetMenuArrowDownward();
        rUi->SetMaxX(off);

        rUi->OnClick = [=]
        {
            ref<UiContainer> rUiFrame = 
                m_rTunerProfile->NewUiTunerProfileHideOptionsMenu();

            rUiFrame->CreateMenuAnchoredTo(this);
        }
    }

    NL();

    { ref<CUiSeparateRowsGp> rUi; }

    NL();

    //
    // List
    //
    {
        ref<CUiScrollablePanelGp> rUi;

        //rUi->SetBackColor(rgb(180, 180, 200));
        rUi->SetBackAlpha(percent(30));
        rUi->SetPadY(0); // the value itself ensures proper (colored) padding

        // List all items
        {
            ref<CTunerValue> rIterTunerValue;
            for (iter i; 
                m_rTunerProfile->Iterate(
                    out i, 
                    out rIterTunerValue);)
            {
                //
                // Next Value
                //
                {
                    ref<UiContainer> rUi =
                        rIterTunerValue->NewTunerValueEditorUi(
                            m_rTunerProfile,
                            m_rTunerProfileList);
                }

                NL();
            }
        }
    }
}

void UiTunerProfileEditForm::EqualizeControls(
        ref<CUiTextBasedGp> rUi)
{
    rUi->SetFontSize(18);
    rUi->SetMarginAtTop(0);
    rUi->SetMarginAtBottom(0);
}

