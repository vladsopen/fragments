// UiDemoMain.cpp

#include "CProject.h"
#include "CDemoImpl.h"
#include "VirtOs_file.h"
#include "CProtocolIface.h"

void UiDemoMain::Ui()
{
    str sModality = m_rDemo->x_rExternalProtocol->x_sIniModality;

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rDemo->_m_ptrmapUiDemoMain += rUi;
    }

    //
    // Scroll Raw Info
    //
    {
        ref<CUiScrollablePanelGp> rUi;

        rUi->GetScrollSystemRoot()->SetMinY(scale(220));
        rUi->GetScrollSystemRoot()->SetMaxY(0);
        rUi->GetScrollSystemRoot()->SetPreferredY(0);

        //
        // Raw Info
        //
        {
            ref<CUiEditGp> rUi;

            path pathIni = 
                m_rDemo->
                    GetCommandLineIni();
            str sIni = "<no .ini file>";
            if (pathIni != "")
            {
                file(pathIni).Load(out sIni, out_IGNORE_sError);
            }

            rUi->SetText(
                pathIni + 
                    "\n\n" + 
                    sIni);

            rUi->SetTip(
                "Raw parameters from .ini file");

            rUi->SetWrapText(true);

            rUi->SetMinX(scale(250));
            rUi->SetPreferredX(0);
            rUi->SetMaxX(oo);
            rUi->SetMaxY(oo);

            rUi->SetFontSize(16);

            //rUi->SetCheck(m_rDemo->x_bXxx);

            /*
            rUi->OnEdit = [=]
            {
            }
            */
        }
    }

    NL();

    //
    // Emulator Panel
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetTip(
            "Parsed info:\n\n" + 
                m_rDemo->
                    x_rExternalProtocol->
                        ExposeOut().
                            LookupUnidef(C_szExposedRoot, unidef()).
                            ExportAsTextWithoutHeader());

        //
        // XXL icon
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetIcon(
                sModality == "Ecg"?
                    RES__SMDEV_XXL()
                    :
                sModality == "Press"?
                    RES__DON_XXL()
                    :
                    RES__PUZZLE_XXL());
        }

        //
        // Parsed Info and Control Panel
        //
        {
            ref<UiContainer> rUi = 
                m_rDemo->
                    NewUiDemoControl();
        }

        NL();

        //
        // [Exit]
        //
        {
            ref<CUiButtonGp> rUi;

            rUi->SetText(
                "Complete demo, "
                    "save results to .ini "
                    "and exit");

            rUi->SetIcon(RES__SAVE_L());
            rUi->SetIconSize(size(16, 16));

            rUi->SetTip(
                "Click when done");

            rUi->SetMaxX(oo);

            //rUi->SetCheck(m_rDemo->x_bXxx);

            rUi->OnClick = [=]
            {
                //sys::GPostQuitMessage();
                m_rDemo->
                    PrepareDemoResultsAndExit();
            }
        }

    }
}
