// UiFakeTomographConnectionControls.cpp

#include "CProject.h"
#include "CFakeTomograph.h"
#include "CSkinizerCommonIface.h"

void UiFakeTomographConnectionControls::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rFakeTomograph->_m_pUiFakeTomographConnectionControls = rUi;

        // Must at least include PC image, plugs and status info
        rUi->SetMinY(C_nFakeTomographUiFixedTopHeight);
        rUi->SetMaxY(off);

        rUi->SetAlignContentToTop();
    }

    //
    // Emulator Window Caption
    //
    {
        ref<CUiCaptionGp> rUi;

        //
        // App Logo
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText("Demo");
            rUi->SetIcon(RES__DEMO_PACIFIER_18x14());

            if (debug::GIsAllowed...())
            {
                rUi->SetTip(
                    CSkinizerCommonIface::GGetSkinReport(
                        rUi->GetSkinizer()));
            }

            //rUi->SetAlignContentToRight();

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

        /*
        //
        // Close Emulator (not application!)
        //
        { 
            ref<CUiCloseGp> rUi; 

            rUi->OnClick = [=]
            {
                CProject::GGetIt().x_rLoader->
                    x_rSession->
                        EnableTomographEmulator(
                            false);
            }
        }
        */
    }

    NL();

    //
    // PC Overlay
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetMaxY(oo);
        rUi->SetAlignContentToBottom();

        rUi->SetMarginAtLeft(10);

        //
        // Plug IN/OUT
        //
        {
            ref<CUiButtonGp> rUi;

            bool bConnected = m_rFakeTomograph->x_bUsbPlugedIn;

            rUi->SetText(
                bConnected?
                    TRANSLATE(
                        "Disconnect[... UI-string-removed]");

            rUi->SetCheck(bConnected);

            rUi->SetFontSize(20);
            rUi->SetFontBold(true);

            rUi->SetAlignContentToCenterX();

            // Some flashing
            rUi->SetBackColor(
                !m_rFakeTomograph->x_bUsbPlugedIn &&
                    m_rFakeTomograph->x_iBlinkCycle % 2 == 0 &&
                    m_rFakeTomograph->x_iBlinkCycle % 10 >= 5
                    ?
                    rgb(255, 0, 0)
                    :
                    (rgb) C_rgbSysNoUiFill);

            rUi->SetTextColor(
                !m_rFakeTomograph->x_bUsbPlugedIn?
                    rgb(200, 0, 0)
                    :
                    rgb(0, 0, 80));

            // Cover the corner image
            //rUi->SetMaxY(oo);
            rUi->SetMinX(
                GGetPictureInfo(RES_FAKE_PC_CORNER())->GetRasterSize().w);

            rUi->OnClick = [=]
            {
                m_rFakeTomograph->x_bUsbPlugedIn = 
                    !m_rFakeTomograph->x_bUsbPlugedIn;
            }
        }
    }

    //
    // Main Status Info
    //
    {
        ref<CUiStaticGp> rUi;

        str sStatus = m_rFakeTomograph->GetMainHelpStatusLine();
        str sErrors = m_rFakeTomograph->GetUsbClientErrorLog();
        if (sErrors != "")
            sStatus += RichSize(20, "\n" + sErrors);

        rUi->SetText(sStatus);

        // This is large and important
        rUi->SetFontSize(35);
        rUi->SetTextColor(rgb(110, 74, 8));

        // Remember we have a corner image
        rUi->SetMarginAtLeft(
            //GGetPictureInfo(RES_FAKE_PC_CORNER())->GetRasterSize().w + 
                (pix) scale(30));
    }
}
