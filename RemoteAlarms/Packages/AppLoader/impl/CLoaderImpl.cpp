// CLoaderImpl.cpp

#include "CProject.h"
#include "CLoaderImpl.h"
#include "CSessionIface.h"
#include "CPatientLinkIface.h"
#include "VirtOs_CAppGp.h"
#include "CInterprocParaMapIfaceGp.h"
#include "CSenderSessionIface.h"
#include "CPatientEmbedLinkIface.h"
#include "CGlobalConfigIface.h"

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
            InitPatientIsMainFrameHidden(
                ptr<CPatInfoSessionIfaceGp>()))
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
        EnsureSessionOpenAndActivateMainWindow();
    }
}

void CLoaderImpl::OnEnsureSessionOpenAndActivateMainWindow()
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
            //TODO: Ralarm splash screen?
            //x_rHelp->x_rAbout->OpenSplashDialog();
        }

        // Open main session
        //rASSERT(!x_rSession->IsSessionOpened());
        EnsureActiveMainSession();

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

void CLoaderImpl::OnRestartSession()
{
    CloseAllSessions();

    // reInit Patients Connecton
    x_rPatientLink->ClosePatientLink();

    // Close MainWindow (and its session) always
    if not_null(
        ref<CUiPanelGp> rUiMainWindow = _m_pUiMainWindow)
    {
        rUiMainWindow->InvalidateContent();
        rUiMainWindow->UpdateContentAndLayoutNow();
    }

    StartApplication();

    /*
    x_rPatientLink->
        InitPatientIsMainFrameHidden(
            ptr<CPatInfoSessionIfaceGp>());
    this->x_rPatientLink = null();

    EnsureActiveMainSession();
    */
}

void CLoaderImpl::OnCloseSessionAndMainWindow()
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
    CloseAllSessions();

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

void CLoaderImpl::CloseAllSessions()
{
    // Destroy the session
    if (x_rSession->IsSessionOpened())
    {
        x_rSession->CloseSession();
        this->x_rSession = null();

        x_rEmulatorSession->CloseSession();
        this->x_rEmulatorSession = null();
    }
}

void CLoaderImpl::OnEnsureActiveMainSession()
{
    // Clear session (which should not exist yet)
    if (!x_rSession->IsSessionOpened())
    {
        // Main init
        x_rSession->
            InitSession();
    }

    if (!x_rEmulatorSession->IsSessionOpened())
    {
        // Sender
        x_rEmulatorSession->
            InitSession();
    }

    // refresh it in the main window
    _m_pUiMainWindow.DefaultIfNull()->InvalidateContent();
}

void CLoaderImpl::OnHandleLoaderIdle(
        out CProject::EIdleNext& out_eIdleNextMayBeUpgraded)
{
    x_rSession->
        HandleSessionIdle(
            out out_eIdleNextMayBeUpgraded);

    x_rEmulatorSession->
        HandleSessionIdle(
            out out_eIdleNextMayBeUpgraded);
}

ref<UiContainer> CLoaderImpl::NewUiMainWindow()
{
    ref<UiMainWindow> rUi;

    rUi->m_rLoader = this;

    return rUi;
}

