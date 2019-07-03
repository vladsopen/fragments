// CSessionIface.h
#pragma once

class CPatientSampleIfaceGp;
class CPatientLinkSampleIface;
class CProIface;
class CTomogramIface;
class CPatientLinkIface;
class CDownloaderIface;
class CDeviceManagerIface;

//
// CSessionIface - interactive Tomo user session, initiated when 
//   the main window is created and terminated on [x] button click
//

class CSessionIface : public object //CSessionIfaceGp
{
public:
    CSessionIface();
    NEW_LOCAL(CSessionImpl, CSessionIface)
    //NEW_GP(MyProject, CSessionImpl, CSessionIface)

// Constants

// Attributes

//    // Patients sample context
//    ref<CPatientLinkSampleIface> _x_rPatientLinkSample
//            xauto(Get);

    SEE_ALSO(CTomogramIface)

    // Our device
    ref<CDeviceManagerIface> x_rDeviceManager
            xauto(Get);

    // Our downloader
    ref<CDownloaderIface> x_rDownloader
            xauto(Get);

    // Tomo Pro 
    ref<CProIface> x_rPro
            xauto(Get);

    // Patients gateway or its emulator
    ref<CPatientLinkIface> x_rPatientLink
            xauto(Get);

// Operations

    // Preliminary init just after new session created
    void PrepareToNewSession(
            ref<CPatientLinkIface> rNewPatientLink)
            vhook;

    // Convert and post an echo sample to PatientLink
    void SendSampleTomogramToPatient(
            ref<CPatientSampleIfaceGp> rPatientSample,
            ref<CTomogramIface> rTomogram,
            big nnAddAttachmentBytes,
            bool bAddAttachmentHardRandom,
            bool bAddAttachmentRandomWave)
            vhook;

    // Final cleanup
    void TerminateSession()
            vhook;

    // Final cleanup
    void EnableTomographEmulator(
            bool bOn)
            vhook;

// UI

    ref<UiContainer> NewMainUi()
            vhook;

protected:
    virtual void OnPrepareToNewSession(
            ref<CPatientLinkIface> rNewPatientLink)
            v1pure;
    virtual void OnSendSampleTomogramToPatient(
            ref<CPatientSampleIfaceGp> rPatientSample,
            ref<CTomogramIface> rTomogram,
            big nnAddAttachmentBytes,
            bool bAddAttachmentHardRandom,
            bool bAddAttachmentRandomWave)
            v1pure;
    virtual void OnTerminateSession()
            v1pure;
    virtual void OnEnableTomographEmulator(
            bool bOn)
            v1pure;
    virtual ref<UiContainer> OnNewMainUi()
            v1pure;
private:
};

