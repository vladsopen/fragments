// CDownloaderIface.h
#pragma once

class CTomogramIface;
class CSessionIface;
class CDeviceManagerIface;

//
// CDownloaderIface - controls download session for Tomo,
//   maintains main UI and coordinates Device, Tomogram and PatientLink 
//   efforts in this application mode
//
ASSUME_IMPL_FOR_IFACE(CDownloaderImpl, CDownloaderIface)

class CDownloaderIface : public object
{
public:
    CDownloaderIface();
    NEW_LOCAL(CDownloaderImpl, CDownloaderIface)
    //NEW_GP(MyProject, CDownloaderIfaceImpl, CDownloaderIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // We limit max sample count we will support
    static const int C_nMaxTomographSampleCount = 40;

// Attributes

    // Downloader MUST be linked to the session before device callback sends 
    //   its first Tomogram back (I could pass the ptr<> through UI and callback
    //   but this would not change anything - the callback code creates this 
    //   hard object link anyways).
    //   This is used by the callback to pass Tomogram back to Patients.
    ptr<CSessionIface> _x_pSessionTrusted
            xauto(Get, Set);

    // Total stored Tomogram count
    int GetStoredTomogramCount()
            vhook;

// Operations

    // Device read callback
    void AppendNextAmplitudePairFromTomograph(
            ref<CDeviceManagerIface> rDeviceManager,
            int nAmplitudeAt0,
            int nAmplitudeAt1)
            vhook;

    // Device read callback
    void HandleNextTomogramComplete(
            ref<CDeviceManagerIface> rDeviceManager)
            vhook;

    // Document Open Command
    void OpenTomogram(
            ref<CTomogramIface> rTomogram)
            vhook;

    // Profile change update
    void PrepareDownloaderToSelectedBodyProfile()
            vhook;

// UI

    ref<UiContainer> NewUiDownloaderMain(
            ref<CDeviceManagerIface> rDeviceManager)
            vhook;

    // Refresh error indicator
    void UpdateErrorReportingUi()
            vhook;

protected:
    virtual void OnAppendNextAmplitudePairFromTomograph(
            ref<CDeviceManagerIface> rDeviceManager,
            int nAmplitudeAt0,
            int nAmplitudeAt1)
            v1pure;
    virtual void OnHandleNextTomogramComplete(
            ref<CDeviceManagerIface> rDeviceManager)
            v1pure;
    virtual void OnOpenTomogram(
            ref<CTomogramIface> rTomogram)
            v1pure;
    virtual void OnUpdateErrorReportingUi()
            v1pure;
    virtual int OnGetStoredTomogramCount()
            v1pure;
    virtual void OnPrepareDownloaderToSelectedBodyProfile()
            v1pure;
    virtual ref<UiContainer> OnNewUiDownloaderMain(
            ref<CDeviceManagerIface> rDeviceManager)
            v1pure;

private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
