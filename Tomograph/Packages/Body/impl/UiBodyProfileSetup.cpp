// UiBodyProfileSetup.cpp

#include "CProject.h"
#include "CBodyProfileImpl.h"
#include "CBodyPointImpl.h"
#include "CSkinizerCommonIfaceGp.h"

void UiBodyProfileSetup::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rBodyProfile->_m_pUiBodyProfileSetup = rUi;

        rUi->SetPadX(3);
    }

    //
    // List Controls before Reorder Buttons
    //
    {
        ref<CUiPanelGp> rUi;

        //
        // Point List
        //
        {
            ref<CUiScrollablePanelGp> rUi;

            // Standard-List: Border and Background
            rUi->GetSkinizer()->EqualizeListScrollablePanel(rUi);

            // all points
            {
                ref<CBodyPointImpl> rIterBodyPoint;
                for (
                    iter i;
                    m_rBodyProfile->IterateBodyPoints(
                        out i, 
                        out rIterBodyPoint);)
                {
                    //
                    // Next Point Row
                    //
                    {
                        ref<UiContainer> rUi = 
                            rIterBodyPoint->
                                NewUiBodyPointSetupRow(
                                    m_rBodyProfile,
                                    i);
                    }

                    NL();
                }
            }
        }

        // WARNING: for better performance the following should be moved into a separate Ui()

        NL();

        //
        // Add New Point Button
        //
        {
            ref<CUiButtonGp> rUi;

            // Standard-List: [ Add ]
            rUi->GetSkinizer()->EqualizeListAddNewButton(rUi);

            rUi->OnClick = [=]
            {
                m_rBodyProfile->AppendNewPoint();
            }
        }

        //
        // Delete Selected Point Button
        //
        {
            ref<CUiButtonGp> rUi;

            // Standard-List: [ Delete ]
            rUi->GetSkinizer()->EqualizeListDeleteButton(rUi);

            if (m_rBodyProfile->x_iSelectedBodyPoint < 0 ||
                m_rBodyProfile->x_iSelectedBodyPoint >= 
                    m_rBodyProfile->GetBodyPointCount())
            {
                rUi->SetDisableInfo(
                    TRANSLATE(
                        "No point s[... UI-string-removed]");
            }

            rUi->OnClick = [=]
            {
                // Standard-List: Confirm Delete
                if (rUi->GetSkinizer()->
                        AskConfirmListDelete(
                            TRANSLATE(
                                "Delete the[... UI-string-removed]");
                }
            }
        }

    }

    if (debug("Body.ShowProfilePointReorder?").InputBool())
    {
        //
        // Order Controls
        //
        {
            ref<CUiPanelGp> rUi;

            rUi->SetAlignContentToCenterX();

            rUi->SetPadY(3);

            //
            // Up Button
            //
            {
                ref<CUiButtonGp> rUi;

                // Standard-List: [ Up ]
                rUi->GetSkinizer()->EqualizeListOrderUp(rUi);

                rUi->OnClick = [=]
                {
                    m_rBodyProfile->ReorderUp();
                }
            }

            NL();

            //
            // Down Button
            //
            {
                ref<CUiButtonGp> rUi;

                // Standard-List: [ Down ]
                rUi->GetSkinizer()->EqualizeListOrderDown(rUi);

                rUi->OnClick = [=]
                {
                    m_rBodyProfile->ReorderDown();
                }
            }
        }
    }
}
