// CLoaderImpl.cpp

#include "CProject.h"
#include "CLoaderImpl.h"
#include "CPatientLinkIface.h"
#include "CSessionIface.h"
#include "CTotalTest.h"
#include "VirtOs_CAppGp.h"
#include "VirtOs_CGlobalUi.h"

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

/*
void CLoaderImpl::OnStartApplication()
{
    // Open the main window 
    ref<UiContainer> rUiMainFrame = NewUiMainWindow();
    rUiMainFrame->CreateMainFrame();
}
*/

void CLoaderImpl::OnStartApplication()
{
    // Init Patients Connecton
    x_rPatientLink->InitPatientIsMainFrameHidden();

    // Handle connection mode
    if (x_rPatientLink->IsConnectedToPatient())
    {
        // do something for connected mode?
    }
    else
    {
        // Show main window only for standalone mode
        EnsureOpenAndActivateMainWindow();
    }
}

ref<CSessionIface> CLoaderImpl::Onx_rSession_Get()
{
    ASSERT(_m_bSessionActive);

    return m_rActiveSession;
}

bool CLoaderImpl::OnIsActiveSessionPresent()
{
    return _m_bSessionActive;
}

void CLoaderImpl::OnEnsureOpenAndActivateMainWindow()
{
    StopSessionAfterUiClosed();

    /*
    // Close old mode if open
    if (_m_bSessionActive && 
        m_rActiveSession->x_bProMode != bProMode)
    {
        StopSessionAfterUiClosed();
    }

    // Create if does not exist
    if (_m_pUiMainWindow == null())
    */
    {
        // Ensure NEW session is starting
        ASSERT(!_m_bSessionActive);
        this->m_rActiveSession = null();
        _m_bSessionActive = true;

        //x_rSession->x_bProMode = bProMode;

        if (_m_pUiMainWindow == null())
        {
            // Open the main window and attach global loader object to it
            ref<UiMainWindow> rUiFrame;
            rUiFrame->m_rLoader = this;
            rUiFrame->CreateMainFrame();
        }
        else
        {
            CAppGp::GGet()->x_rUi->RefreshAllFrames(&CUiPanelGp::InvalidateContent);
        }

        // Init
        x_rSession->PrepareToNewSession(x_rPatientLink);

        x_rSession->
            EnableTomographEmulator(
                true);
    }

    // Activate
    if not_null(
        ref<CUiPanelGp> rUiMainWindow = _m_pUiMainWindow)
    {
        rUiMainWindow->SetHidden(false); // just in case...

        // Handle Patients activation properly
        rUiMainWindow->MakeForegroundFrameSystemWide();
    }
    else
    {
        rFAIL("Main Window failed to open");
    }
}

void CLoaderImpl::OnStopSessionAfterUiClosed()
{
    if (_m_bSessionActive)
    {
        // Abort the session
        x_rSession->TerminateSession();
        m_rActiveSession = null();
        _m_bSessionActive = false;
    }
}

void CLoaderImpl::OnCloseSessionAndMainWindow()
{
    StopSessionAfterUiClosed();
}

/*
void CLoaderImpl::CloseMainWindow()
{
    if not_null(
        ref<CUiPanelGp> rUiMainWindow = _m_pUiMainWindow)
    {
        rUiMainWindow->CloseFrame();
        // OnCloseFrame() handler handles everything else

        ASSERT(_m_pUiMainWindow == null());
    }
}
*/

void CLoaderImpl::OnHandleLoaderIdle(
        out CProject::EIdleNext& out_eIdleNextMayBeUpgraded)
{
    out_eIdleNextMayBeUpgraded;
    //x_rSession->
    //    HandleSessionIdle(
    //        out out_eIdleNextMayBeUpgraded);
}

ref<UiContainer> CLoaderImpl::NewUiMainWindow()
{
    ref<UiMainWindow> rUi;

    rUi->m_rLoader = this;

    return rUi;
}

