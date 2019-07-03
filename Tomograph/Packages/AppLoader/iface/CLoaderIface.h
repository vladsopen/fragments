// CLoaderIface.h
#pragma once

class CSessionIface;
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

    // Global Tomo session (linked to Main Window and Patients)
    ref<CSessionIface> x_rSession
            xabstract;
    ref<CSessionIface> x_rSession_Get()
            vhook;

    // Check session status
    bool IsActiveSessionPresent()
            vhook;

    // PatientLink object must exist right here along with the main application
    ref<CPatientLinkIface> x_rPatientLink
            xauto(Get);

// Operations

    // Create main window and init for standalone mode
    void StartApplication()
            vhook;

    // Session terminated, decide if app needs to wait another Patient callback
    void CloseSessionAndMainWindow()
            vhook;

    void StopSessionAfterUiClosed()
            vhook;

    // Main app Idle
    void HandleLoaderIdle(
            out CProject::EIdleNext& out_eIdleNextMayBeUpgraded)
            vhook;

// UI

    // Main window existence depends on Patients connection
    void EnsureOpenAndActivateMainWindow()
            vhook;

protected:
    virtual ref<CSessionIface> Onx_rSession_Get()
            v1pure;
    virtual bool OnIsActiveSessionPresent()
            v1pure;
    virtual void OnStartApplication()
            v1pure;
    virtual void OnCloseSessionAndMainWindow()
            v1pure;
    virtual void OnStopSessionAfterUiClosed()
            v1pure;
    virtual void OnHandleLoaderIdle(
            out CProject::EIdleNext& out_eIdleNextMayBeUpgraded)
            v1pure;
    virtual void OnEnsureOpenAndActivateMainWindow()
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
