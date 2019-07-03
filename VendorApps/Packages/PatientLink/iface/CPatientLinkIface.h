// CPatientLinkIface.h
#pragma once

class CPatientSampleIfaceGp;
class CLoaderIface;
class CProxyIface;

//
// CPatientLinkIface - PatientIo encapsulation for this project
//
//   Quick Start Guide
//
//    1) Declare a ref<CPatientLinkIface> as an AppLoader attribute.
//       This connection object must exist while the application is running,
//       it is not tied to a particular session.
//       Although the Session class may want to encapsulate access to this global object.
//
//       Start
//   Patients:      [oooooooooooooooooooooo||ooooooooooooooooooo||oooooooooooooooooooooo][x]Exit
//                       \/ RunSampler       \/ Open/Activate     \/ Open/Activate       \/
//   Loader:        [ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo]
//                       \/ InitPatientLink  \/                   \/                     \/
//   PatientLink:   [oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo|CloseApp]
//                       \/                  \/
//   Sessions:      [oooooooooo][x]      [ooooooooo][x]                            time
//                                                                              ------->
//              (Main Frame is only visible when a session is active)
//
//    2) Separate Main Frame open from AppInit
//       2.1) in LoaderImpl::OnStartApplication()
//
//        // Init Patients Connecton
//        if (rPatientLink->InitPatientIsMainFrameHidden())
//        {
//            // do something for connected mode?
//        }
//        else
//        {
//            // Show main window only for standalone mode
//            EnsureOpenAndActivateMainWindow();
//        }
//
//       2.2) EnsureOpenAndActivateMainWindow() must not only create but also
//         REactivate existing Main Frame.
//
//    3) Loader::CloseProxyAndMainWindow() should:
//       3.1) Close Main Frame
//       3.2) Close Session
//       3.3) Start app termination only if Patients is NOT connected
//
//    4) UI changes regarding Main Frame [x]-button
//
//       4.1) Add rUi->OnCloseFrame = [=] handler to the Main Frame,
//          and call Loader->CloseProxyAndMainWindow(); from it
//
//       4.2) Add rUi->OnClick = [=] handler to ref<CUiCloseGp> rUi;
//          and call rUi->CloseFrame(); from it, it will not close app but
//          redirect to ---> OnCloseFrame() ---> CloseProxyAndMainWindow()
//          In PatientIo mode CUiCloseGp will not close anything automatically.
//          For Main Window [x]-button you should provide an explicit rUi->OnClick = [=]
//          handler:
//
//        //
//        // Close Window (not application!)
//        //
//        {
//            ref<CUiCloseGp> rUi;
//
//            rUi->OnClick = [=]
//            {
//                // For PatientIoed app this will not close main
//                //   window and exit automatically.
//                // see also OnCloseFrame() handler
//                rUi->CloseFrame();
//            }
//        }
//
//    5) Change C_pathMainProxyExternalFileExt.
//
//    6) Edit SendSampleProxyToPatient() for your "Proxy"
//       Call it from a [Save] button.
//
//    7) Edit PatientCallbackImpl::OnOpenSample()
//      7.1) Make sure it creates and activates UI: EnsureOpenAndActivateMainWindow()
//      7.2) Create a new document and import/load data from Patients
//
//    8) Implement PatientCallbackImpl::OnPaintSample() as appropriate to your doc.
//        (note: Paint must be operational regardless of current session and UI state)
//
ASSUME_IMPL_FOR_IFACE(CPatientLinkImpl, CPatientLinkIface)

class CPatientLinkIface : public object
{
public:
    CPatientLinkIface();
    NEW_LOCAL(CPatientLinkImpl, CPatientLinkIface)
    //NEW_GP(MyProject, CPatientLinkIfaceImpl, CPatientLinkIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // For CustomApps file format:
    static const path C_pathMainProxyExternalFileExt = ".CustomApps";

// Attributes

    // Check if we were launched from Patients
    bool IsConnectedToPatient()
            vhook;

    // Some demo info for window caption.
    //str GetCurrentPatientNameAgeSexAndId()
    //        vhook;

    // Query fields
    str GetCurrentPatientFieldValue(
            str sFieldKey,
            str sOrReturn)
            vhook;
    int GetCurrentPatientAge()
            vhook;

// Operations

    // One-time init. Called from StartApplication()
    bool InitPatientIsMainFrameHidden()
            vhook;

    // This is mainly for ASSERTs because DB cannot depend
    //   on close-handlers to save patient data!
    void ClosePatientLink()
            vhook;

    // Called to create a new empty checkup in Patients
    void HandleStartNewCheckup()
            vhook;

    // Uses PatientIo to get a unique name and appends our ext
    path GetNewFullAttachmentFilePath()
            vhook;

    // Called from OnCloseFrame()
    void SendSampleProxyToPatient(
            ref<CPatientSampleIfaceGp> rPatientSample,
            ref<CProxyIface> rProxy)
            vhook;

    // GotoPatientCard command
    //   Activates Patients or shows PatLite UI
    void ShowPatientCard()
            vhook;

    // 'CheckupStarted'/'CheckupFinished'
    void LockCurrentCheckup(
            bool bLockOn)
            vhook;

// UI

    // For embedded Pi-DB this must replace main window content which
    //   shall be moved to OnNewUiCurrentCheckupContent()
    ref<UiContainer> NewUiPatInfoBrowser()
            vhook;

    // Debug UI
    ref<UiContainer> NewUiPatientDebug()
            vhook;

protected:
    virtual bool OnIsConnectedToPatient()
            v1pure;
    //virtual str OnGetCurrentPatientNameAgeSexAndId()
    //        v1pure;
    virtual str OnGetCurrentPatientFieldValue(
            str sFieldKey,
            str sOrReturn)
            v1pure;
    virtual int OnGetCurrentPatientAge()
            v1pure;
    virtual bool OnInitPatientIsMainFrameHidden()
            v1pure;
    virtual void OnClosePatientLink()
            v1pure;
    virtual void OnHandleStartNewCheckup()
            v1pure;
    virtual path OnGetNewFullAttachmentFilePath()
            v1pure;
    virtual void OnSendSampleProxyToPatient(
            ref<CPatientSampleIfaceGp> rPatientSample,
            ref<CProxyIface> rProxy)
            v1pure;
    virtual void OnShowPatientCard()
            v1pure;
    virtual void OnLockCurrentCheckup(
            bool bLockOn)
            v1pure;
    virtual ref<UiContainer> OnNewUiPatInfoBrowser()
            v1pure;
    virtual ref<UiContainer> OnNewUiPatientDebug()
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
