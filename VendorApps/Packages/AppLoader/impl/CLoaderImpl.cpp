// CLoaderImpl.cpp

#include "CProject.h"
#include "CLoaderImpl.h"
#include "CProxyIface.h"
#include "CPatientLinkIface.h"
#include "VirtOs_CAppGp.h"
#include "CInterprocParaMapIfaceGp.h"
#include "CDemoIface.h"

CLoaderImpl::CLoaderImpl()
{
}

void CLoaderImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("why would you want to expose a Loader object itself?!");
}

void CLoaderImpl::OnStartApplication()
{
    TESTLOG(
        "",
        "Loader is called to start the application.\n");

    rASSERTM(
        !CAppGp::GGet()->x_rInterprocParaMap->IsParaServiceProcess(),
        "Never use Loader in a ParaService process!");

    // Init Patients Connecton
    if (x_rPatientLink->
            InitPatientIsMainFrameHidden())
    {
        // do something for connected mode?
        TESTLOG(
            "",
            "Loader detected Patients connection.\n");
    }
    else
    {
        // do something for connected mode?
        TESTLOG(
            "",
            "Loader detected no Patients connection, will show window now.\n");

        // Show main window only for standalone mode
        EnsureProxyOpenAndActivateMainWindow();
    }
}

void CLoaderImpl::OnEnsureProxyOpenAndActivateMainWindow()
{
    TESTLOG(
        "",
        "Loader is making sure MainFrame is visible.\n");

    rASSERTM(
        !CAppGp::GGet()->x_rInterprocParaMap->IsParaServiceProcess(),
        "Never use Loader in a ParaService process!");

    // Create if does not exist
    if (_m_pUiMainWindow == null())
    {
        TESTLOG(
            "",
            "Loader sees no MainFrame, new session is going to be created.\n");

        // Show splash
        if (!debug::GIsTest())
        {
            //TODO: CustomApps splash screen?
            //x_rHelp->x_rAbout->OpenSplashDialog();
        }

        // Clear session (which should not exist yet)
        //rASSERT(!x_rProxy->IsProxyOpened());
        //this->x_rProxy = null();
            // can come from PatientIo OpenSample

        // Clear alternalte session (which should not exist yet)
        rASSERT(!x_rDemo->IsDemoInitOk());
        this->x_rDemo = null();

        // try demo session first
        if (x_rDemo->IsExeStartedForDemo())
        {
            x_rDemo->
                InitDemo();
        }
        else
        {
            // Main init
            if (!x_rProxy->IsProxyOpened())
            {
                x_rProxy->
                    InitProxy(
                        path(), // pathOpenExistingAttachment,
                        str(), // sAttachmentRevision);
                        str()); // sAttachmentExtra
            }
        }

        // Open the main window 
        ref<UiContainer> rUiMainFrame = NewUiMainWindow();
        rUiMainFrame->CreateMainFrame();
    }

    // Activate/Reactivate
    if not_null(
        ref<CUiPanelGp> rUiMainWindow = _m_pUiMainWindow)
    {
        TESTLOG(
            "",
            "Loader forces the MainFrame on the top.\n");

        rUiMainWindow->SetHidden(false); // just in case...

        // Handle Patients activation properly
        rUiMainWindow->MakeForegroundFrameSystemWide();
    }
    else
    {
        NOTEST;

        rFAIL("Main Window failed to open");
    }
}

void CLoaderImpl::OnCloseProxyAndMainWindow()
{
    TESTLOG(
        "",
        "Loader called to stop session.\n");

    // Close MainWindow (and its session) always
    if not_null(
        ref<CUiPanelGp> rUiMainWindow = _m_pUiMainWindow)
    {
        TESTLOG(
            "",
            "Loader stops existing session.\n");

        // Close window
        rUiMainWindow->CloseFrame();
    }
    else
    {
        NOTEST;

        TESTLOG(
            "",
            "Loader sees no visible Main Frame.\n");
    }

    TESTLOG(
        "",
        "Loader resets session object.\n");

    // Destroy the session
    if (x_rProxy->IsProxyOpened())
    {
        x_rProxy->CloseProxy();
        this->x_rProxy = null();
    }

    // Destroy the alternate session
    if (x_rDemo->IsDemoInitOk())
    {
        x_rDemo->CloseDemo();
        this->x_rDemo = null();
    }

    // Exit app if no Patients link
    if (x_rPatientLink->IsConnectedToPatient())
    {
        TESTLOG(
            "",
            "Loader will not quit application because connected to Patients.\n");
    }
    else
    {
        TESTLOG(
            "",
            "Standalone mode loader will exit app after session end.\n");

        sys::GPostQuitMessage();
    }
}

void CLoaderImpl::OnHandleLoaderIdle(
        out CProject::EIdleNext& out_eIdleNextMayBeUpgraded)
{
    x_rProxy->
        HandleProxyIdle(
            out out_eIdleNextMayBeUpgraded);
}

ref<UiContainer> CLoaderImpl::NewUiMainWindow()
{
    ref<UiMainWindow> rUi;

    rUi->m_rLoader = this;

    return rUi;
}

