// CLoaderIface.h
#pragma once

class CSenderSessionIface;
class CPatientLinkIface;
class CSessionIface;

//
// CLoaderIface -
//
//   This standard singleton controls app lifetime and
//     ONLY USED IN MAIN APPLICATION MODULE or in case of standalone DLL startup.
//
//   NO APPLICATION LOGIC here please! This is system-level global service,
//     while all your app objects must happily support multiple simultaneous
//     instances of themselves!
//   You may place here your app session controller, or whatever object(s)
//     manage(s) your app in standalone mode.
//
ASSUME_IMPL_FOR_IFACE(CLoaderImpl, CLoaderIface)

class CLoaderIface : public object
{
public:
    CLoaderIface();
    NEW_LOCAL(CLoaderImpl, CLoaderIface)
    //NEW_GP(MyProject, CLoaderImpl, CLoaderIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Here you place root application objects you need to control
    //   standalone project execution or some application session manager if
    //   this is intended to be the main application module.
    // (Always remember: these app objects may also need IfaceGp level
    //  access so that this module can be attached as a secondary DLL to another app)
    //

    // If you implement the main session context you may need this
    ref<CSessionIface> x_rSession
            xauto(Get, Set);

    // Alaem Sender
    ref<CSessionIface> x_rEmulatorSession
            xauto(Get, Set);

    // PatientLink object must exist right here along with the main application
    ref<CPatientLinkIface> x_rPatientLink
            xauto(Get);

// Operations

    // Create main window and init for standalone mode
    void StartApplication()
            vhook;

    // Main Frame open for a new session
    void EnsureSessionOpenAndActivateMainWindow()
            vhook;

    // Session terminated, decide if app needs to wait another Patient callback
    void CloseSessionAndMainWindow()
            vhook;

    // Reinit
    void RestartSession()
            vhook;

    // Reopen session without killing main window
    void EnsureActiveMainSession()
            vhook;

    // Main app Idle
    void HandleLoaderIdle(
            out CProject::EIdleNext& out_eIdleNextMayBeUpgraded)
            vhook;

// UI

protected:
    virtual void OnStartApplication()
            v1pure;
    virtual void OnEnsureSessionOpenAndActivateMainWindow()
            v1pure;
    virtual void OnCloseSessionAndMainWindow()
            v1pure;
    virtual void OnRestartSession()
            v1pure;
    virtual void OnEnsureActiveMainSession()
            v1pure;
    virtual void OnHandleLoaderIdle(
            out CProject::EIdleNext& out_eIdleNextMayBeUpgraded)
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
