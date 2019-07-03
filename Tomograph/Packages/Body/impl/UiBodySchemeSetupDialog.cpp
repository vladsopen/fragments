// UiBodySchemeSetupDialog.cpp

#include "CProject.h"
#include "CBodySchemeImpl.h"
#include "CSkinizerCommonIface.h"
#include "CBodyProfileImpl.h"
#include "CBodyPointImpl.h"

void UiBodySchemeSetupDialog::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rBodyScheme->_m_pUiBodySchemeSetupDialog = rUi;

        rUi->SetSkinizerAndDefaultColorizer(
            ref<CSkinizerCommonIface>()
            //ref<CSkinizerDividoIfaceGp>()
            );
    }

    //
    // Back
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetBackColor(CSkinizerCommonIface::C_rgbOptionsWindowBack);

        //
        // Window Caption
        //
        {
            ref<CUiCaptionGp> rUi;

            //
            // App Logo
            //
            {
                ref<CUiStaticGp> rUi;

                rUi->SetText(
                    TRANSLATE(
                        "Body Point[... UI-string-removed]");

                //rUi->SetAlignContentToRight();

                if (debug::GIsAllowed...())
                {
                    rUi->SetTip(
                        CSkinizerCommonIface::GGetSkinReport(
                            rUi->GetSkinizer()));
                }

                //rUi->SetIcon((res) RES_SAMPLER_TOMO);

                rUi->SetMinX(0);
                rUi->SetMaxX(oo);
            }

            //
            // Spacer
            //
            {
                ref<CUiStaticGp> rUi;

                rUi->SetText("");
                rUi->SetPreferredX(2);
            }

            //
            // Close 
            //
            { 
                ref<CUiCloseGp> rUi; 
            }
        }

        NL();

        //
        // Options Dialog Client Area
        //
        {
            ref<CUiPanelGp> rUi;

            rUi->SetDialogRootPanel(2);

            rUi->SetMaxY(oo);
            rUi->SetAlignContentToTop();

            //
            // Profiles
            //
            {
                ref<UiContainer> rUi = 
                    m_rBodyScheme->NewUiBodySchemeSetup();
            }

            NL();

            //
            // Close
            //
            {
                ref<CUiDialogCloseGp> rUi;

                rUi->OnClick = [=]
                {
                    rUi->CloseFrame();
                }
            }
        }
    }
}
