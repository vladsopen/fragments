// CBodySchemeIface.h
#pragma once

class CDownloaderIface;

//
// CBodySchemeIface - 
//
//   Body point to sample association for the downloader.
//
ASSUME_IMPL_FOR_IFACE(CBodySchemeImpl, CBodySchemeIface)

class CBodySchemeIface : public object
{
public:
    CBodySchemeIface();
    NEW_LOCAL(CBodySchemeImpl, CBodySchemeIface)
    //NEW_GP(MyProject, CBodySchemeImpl, CBodySchemeIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

// Operations

    // Create an association with an Tomogram
    str QueryBodyPointNameFromCurrentProfile(
            int iTomogram,
            out str& out_sError)
            vhook;

    // Current profile name or ""
    str GetCurrentBodyProfileNameForIndication()
            vhook;

    // Current profile point count >= 0
    int GetCurrentBodyProfilePointCount()
            vhook;

// UI

    // Main setup panel for config dialog
    ref<UiContainer> NewUiBodySchemeSetup()
            vhook;

    // Profile selector for the main downloader window
    ref<UiContainer> NewUiBodySchemeProfileSelector(
            ref<CDownloaderIface> rDownloader)
            vhook;

protected:
    virtual str OnQueryBodyPointNameFromCurrentProfile(
            int iTomogram,
            out str& out_sError)
            v1pure;
    virtual str OnGetCurrentBodyProfileNameForIndication()
            v1pure;
    virtual int OnGetCurrentBodyProfilePointCount()
            v1pure;
    virtual ref<UiContainer> OnNewUiBodySchemeSetup()
            v1pure;
    virtual ref<UiContainer> OnNewUiBodySchemeProfileSelector(
            ref<CDownloaderIface> rDownloader)
            v1pure;

private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
