// UiTunerSetup.cpp

#include "CDucalcProject.h"
#include "CMediaDocIfaceGp.h" //(dont_uninclude)
#include "CTunerProfileList.h"
#include "CTunerProfile.h"

void UiTunerSetup::Ui()
{
    {
        ref<CUiPanelGp> rUi = this;

        m_rTunerProfileList->_m_pUiTunerSetup = rUi;
    }

    //
    // List Caption
    //
    {
        ref<CUiCaptionGp> rUi;

        //
        // Title
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText("Flow Tuner Profile Setup");
            rUi->SetMaxX(oo);
        }

        //
        // Spacer
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText("");
        }

        { ref<CUiCloseGp> rUi; }
    }

    NL();

    //
    // Main Back
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetDialogRootPanel();

        //
        // Slit Setup Panel
        //
        {
            ref<CUiPanelGp> rUi;

            //
            // Left Side - List Box
            //
            {
                ref<CUiPanelGp> rUi;

                //
                // Profile List
                //
                {
                    ref<UiContainer> rUi = 
                        m_rTunerProfileList->NewTunerProfileListBoxUi();
                }
            }

            //
            // Setup Splitter
            //
            {
                ref<CUiSplitterGp> rUi;

                rUi->SetConstPrevPanel();
                rUi->SetConfigKey("Usound.Flow.TunerSetup");
            }

            //
            // Right Side - Edit Form
            //
            {
                ref<CUiPanelGp> rUi;

                //
                // The Selected Profile
                //
                {

                    ptr<CTunerProfile> pSelectedTunerProfile = 
                        m_rTunerProfileList->x_linkSelectedTunerProfile.Get().
                            FindPtr(m_rTunerProfileList);

                    if not_null(
                        ref<CTunerProfile> rSelectedTunerProfile = pSelectedTunerProfile)
                    {
                        //
                        // Profile Edit Form
                        //
                        {
                            ref<UiContainer> rUi = 
                                rSelectedTunerProfile->NewTunerProfileEditFormUi(
                                    m_rTunerProfileList);
                        }
                    }
                    else
                    {
                        //
                        // No Selection
                        //
                        {
                            ref<CUiStaticGp> rUi;

                            rUi->SetText("No profile selected");
                            rUi->SetDisableInfo("Select a profile in the list box");
                        }
                    }
                }
            }
        }

        NL();

        /*
        //
        // Add New
        //
        {
            ref<CUiButtonGp> rUi;

            rUi->SetText("Add New Tuner Profile");

            rUi->OnClick = [=]
            {
                m_rTunerProfileList->AddNewTunerProfile();
            }
        }
        */

        //
        // Save
        //
        {
            ref<CUiButtonGp> rUi;

            rUi->SetText("Save");

            rUi->OnClick = [=]
            {
                m_rTunerProfileList->SaveFlowTunerSetup();
            }
        }

        //
        // Load
        //
        {
            ref<CUiButtonGp> rUi;

            rUi->SetText("Load");

            rUi->OnClick = [=]
            {
                m_rTunerProfileList->LoadFlowTunerSetup();
            }
        }
    }
}
