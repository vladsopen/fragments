// UiMainWindow.cpp

#include "CProject.h"
#include "CLoaderImpl.h"
#include "CProxyIface.h"
#include "CUvOptionsIfaceGp.h"
#include "CUvSkinOptionsIfaceGp.h"
#include "CSkinizerCommonIfaceGp.h"
#include "CGlobalConfigIface.h"
#include "VirtOs_CAppGp.h"
#include "VirtOs_CGlobalUi.h"
#include "CPatientLinkIface.h"
#include "CDemoIface.h"

void UiMainWindow::Ui()
{
    bool bAsDemoDialog = 
        CProject::GGetIt().
            x_rLoader->
                x_rDemo->
                    IsExeStartedForDemo();

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rLoader->_m_pUiMainWindow = rUi;

        rUi->SetSkinizerAndDefaultColorizer(
            CAppGp::GGet()->
                x_rUvOptions->
                    x_rUvSkinOptions->
                        NewSelectedSkinizerForMainFrame());

        // Update taskbar text
        rUi->SetTaskbarText(CAppGp::GGet()->x_rUi->x_sAppLogoForUi);

        if (bAsDemoDialog)
        {
            // 1st Option:
            // Make MainWindow dialog like - centered and sized to content
            //  (remove this to turn it into classic full-desktop main window)
            rUi->SetIndependentShape(false);
        }
        else
        {
            // 2nd Option:
            // Maximized by default:
            rUi->SetPreferredX(oo);
            rUi->SetPreferredY(oo);
        }

        rUi->OnCloseFrame = [=]
        {
            // PatientIo compatible logic
            m_rLoader->
                CloseProxyAndMainWindow();
        }
    }

    //
    // Main Window Caption
    //
    {
        ref<CUiCaptionGp> rUi;

        //
        // App Logo
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                "" +
                    CAppGp::GGet()->x_rUi->x_sAppLogoForUi + " " +
                    RichBoldOff(CProject::GGetUniVersion()) + " " +
                    " " +
                    //"" + 
                    (bAsDemoDialog?
                        RichSuccessBold("Modality Demo")
                        :
                        /*
                        m_rLoader->
                            x_rPatientLink->
                                GetCurrentPatientNameAgeSexAndId())
                        */
                        "PatientIo-Protocol Proxy Debugger"));

            rUi->SetIcon(RES_APP_MAIN());
            rUi->SetMinX(0);
            rUi->SetMaxX(oo);
        }

        //
        // Minimize
        //
        { 
            ref<CUiMinimizeGp> rUi; 

            rUi->SetAlikeY(_m_alikeyCaptionButtons);
        }

        //
        // Maximize/Restore
        //
        { 
            ref<CUiMaximizeGp> rUi; 

            rUi->SetAlikeY(_m_alikeyCaptionButtons);
        }

        //
        // [Options]
        //
        {
            ref<CUiButtonGp> rUi;

            rUi->SetTip(
                TRANSLATE(
                    "Program se[... UI-string-removed]");

            rUi->SetIcon(
                RES__OPTIONS_16x16());

            //rUi->SetIconLocationAtTop();
            //rUi->SetAlignInParentToRight();

            rUi->OnClick = [=]
            {
                CProject::GGetIt().
                    x_rGlobalConfig->
                        OpenUiGlobalConfigDialog();
            }
        }

        //
        // Spacer
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText("");
            rUi->SetPreferredX(2);
            rUi->SetMaxX(off);
        }

        //
        // Close Window (not application in PatientIoed mode!)
        //
        { 
            ref<CUiCloseGp> rUi; 

            rUi->SetText(CSlocCommon::L__Exit());

            rUi->SetIcon(RES__EXIT());

            rUi->SetAlikeY(_m_alikeyCaptionButtons);

            rUi->OnClick = [=]
            {
                // For PatientIoed app this will not close main 
                //   window and exit automatically.
                // see also OnCloseFrame() handler
                rUi->CloseFrame();
            }
        }
    }

    NL();

    //
    // CustomApps Root MainWindow Client Area
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetAlignContentToTop();

        rUi->SetDialogRootPanel(); // some padding

        if (m_rLoader->
                x_rDemo->
                    IsDemoInitOk())
        {
            rASSERT(!m_rLoader->x_rPatientLink->IsConnectedToPatient());
            rASSERT(!m_rLoader->x_rProxy->IsProxyOpened());

            //
            // Demo Mode
            //
            {
                ref<UiContainer> rUi =
                    m_rLoader->
                        x_rDemo->
                            NewUiDemoMain();
            }
        }
        else
        if (m_rLoader->
                x_rPatientLink->
                    IsConnectedToPatient())
        {
            //
            // Pi-DB Browser Area
            //
            {
                ref<UiContainer> rUi =
                    m_rLoader->
                        x_rPatientLink->
                            NewUiPatInfoBrowser();
            }
        }
        else
        {
            //
            // Standalone Session without any Pi-DB
            //
            {
                ref<UiContainer> rUi =
                    m_rLoader->
                        x_rProxy->
                            NewUiProxyMain();
            }
        }
    }
}
