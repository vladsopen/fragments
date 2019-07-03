// UiUmoveTestDialog.cpp

#include "CProject.h"
#include "CUmoveTest.h"
#include "CSkinizerCommonIfaceGp.h"
#include "CColorizerCommonIfaceGp.h"

// see //TODO: markers for code you must have in your app

void UiUmoveTestDialog::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rUmoveTest->_m_ptrmapUiUmoveTestDialog += rUi;

        rUi->SetSkinizerAndDefaultColorizer(ref<CSkinizerCommonIfaceGp>());

        rUi->SetPreferredX(scale(800));
        rUi->SetPreferredY(scale(600));
    }

    //
    // Window Caption
    //
    {
        ref<CUiCaptionGp> rUi;

        //
        // Caption Text
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetIdleUpdate(true);

            rUi->SetText(
                "Umove Demo");

            rUi->SetMaxX(oo); // stretch to the right Frame edge
            rUi->SetMinX(0);
        }

        { ref<CUiCloseGp> rUi; }
    }

    NL();

    //
    // Client
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetDialogRootPanel(2.0);

        //
        // "Simple Demo"
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText("Simple Demo");
            rUi->SetFontSize(30);
            rUi->SetFontBold(true);
            rUi->SetTextColor(rUi->GetColorizer()->GetButtonShadowAndSoftTextColor());
        }

        NL();

        //
        // Simple Demo Version
        //
        {
            ref<UiContainer> rUi = 
                m_rUmoveTest->NewUiUmoveSimpleDemo();
        }

        //
        // Simple Demo Setup
        //
        {
            ref<UiContainer> rUi = 
                m_rUmoveTest->NewUiUmoveSimpleDemoSetup();
        }

        NL();

        { ref<CUiSeparateRowsGp> rUi; }

        NL();

        //
        // "Advanced Demo"
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText("Advanced Demo");
            rUi->SetFontSize(30);
            rUi->SetFontBold(true);
            rUi->SetTextColor(rUi->GetColorizer()->GetButtonShadowAndSoftTextColor());
        }

        NL();

        //
        // Slider-style Additional External Controls around Umove parent
        //
        {
            ref<UiContainer> rUi = 
                m_rUmoveTest->x_bShowSliderWrapper?
                    m_rUmoveTest->NewUiUmoveTestSliderWrapper()
                    :
                    m_rUmoveTest->NewUiUmoveTestMainArena();
        }

        //
        // Controls
        //
        {
            ref<UiContainer> rUi = 
                m_rUmoveTest->NewUiUmoveTestDemoSetup();
        }
    }
}
