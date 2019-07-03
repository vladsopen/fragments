// CWizardStepType.h
#pragma once

class CDeviceManagerIface;
class CDownloaderImpl;

//
// CWizardStepType - 
//
//   Tomo Downloader UI phases and device communication states.
//
//ASSUME_IMPL_FOR_IFACE(CWizardStepTypeImpl, CWizardStepType)

class CWizardStepType : public typeobject
{
public:
    CWizardStepType();
    //NEW_LOCAL(CWizardStepTypeImpl, CWizardStepType)
    //NEW_GP(MyProject, CWizardStepTypeImpl, CWizardStepType)

// Constants

// Attributes

    // A stage where we accept new data and provide all progress indication
    bool IsTomographPollingInProgress()
            vhook;

    // Short status info
    sloc GetShortDownloadStatusForUi(
            ref<CDownloaderImpl> rDownloader)
            vhook;

    // We can only change profile at the initial stage
    bool IsProfileChangeEnabled()
            vhook;

// Operations

    // Step preparation logic
    bool TryEnterStep(
            ref<CDownloaderImpl> rDownloader,
            ref<CDeviceManagerIface> rDeviceManager)
            vhook;

// Derived Item Enumeration

    const typelist<CWizardStepType> GetDerivedList();

// UI

    // Different UIs for wizard stages
    ref<UiContainer> NewUiForStep(
            ref<CDownloaderImpl> rDownloader,
            ref<CDeviceManagerIface> rDeviceManager)
            vhook;

protected:

    virtual bool OnIsTomographPollingInProgress()
            v1st
            return
                false;

    virtual sloc OnGetShortDownloadStatusForUi(
            ref<CDownloaderImpl> rDownloader)
            v1pure;

    virtual bool OnIsProfileChangeEnabled()
            v1st
            return 
                false;

    virtual bool OnTryEnterStep(
            ref<CDownloaderImpl> rDownloader,
            ref<CDeviceManagerIface> rDeviceManager)
            v1pure;

    virtual ref<UiContainer> OnNewUiForStep(
            ref<CDownloaderImpl> rDownloader,
            ref<CDeviceManagerIface> rDeviceManager)
            v1pure;

private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

    // Welcome - before everything else started
    class CWizardStepTypeWelcome : public CWizardStepType
    {
    public:
        void flag_List_as_WizardStepType();

        virtual sloc OnGetShortDownloadStatusForUi(
                ref<CDownloaderImpl> rDownloader)
                return
                    TRANSLATE(
                        "Ready to D[... UI-string-removed]");

        virtual bool OnIsProfileChangeEnabled()
                return 
                    true;

        virtual ref<UiContainer> OnNewUiForStep(
                ref<CDownloaderImpl> rDownloader,
                ref<CDeviceManagerIface> rDeviceManager)
                return
                    rDownloader->
                        NewUiStepWelcome(
                            rDeviceManager);

        virtual bool OnTryEnterStep(
                ref<CDownloaderImpl> rDownloader,
                ref<CDeviceManagerIface> rDeviceManager);

    protected:
    };

    // Downloading - from first to last data sample reception
    class CWizardStepTypeDownloading : public CWizardStepType
    {
    public:
        void flag_List_as_WizardStepType();

        virtual bool OnIsTomographPollingInProgress()
                return
                    true;

        virtual sloc OnGetShortDownloadStatusForUi(
                ref<CDownloaderImpl> rDownloader)
                return
                    "" + 
                        TRANSLATE(
                            "Downloadin[... UI-string-removed]");

        virtual ref<UiContainer> OnNewUiForStep(
                ref<CDownloaderImpl> rDownloader,
                ref<CDeviceManagerIface> rDeviceManager)
                return
                    rDownloader->
                        NewUiStepDownloading(
                            rDeviceManager);

        virtual bool OnTryEnterStep(
                ref<CDownloaderImpl> rDownloader,
                ref<CDeviceManagerIface> rDeviceManager);

    protected:
    };

    // FinalInfo - after last data sample reception
    class CWizardStepTypeFinalInfo : public CWizardStepType
    {
    public:
        void flag_List_as_WizardStepType();

        virtual sloc OnGetShortDownloadStatusForUi(
                ref<CDownloaderImpl> rDownloader)
                return
                    TRANSLATE(
                        "Download i[... UI-string-removed]");

        virtual ref<UiContainer> OnNewUiForStep(
                ref<CDownloaderImpl> rDownloader,
                ref<CDeviceManagerIface> rDeviceManager)
                return
                    rDownloader->
                        NewUiStepFinalInfo(
                            rDeviceManager);

        virtual bool OnTryEnterStep(
                ref<CDownloaderImpl> rDownloader,
                ref<CDeviceManagerIface> rDeviceManager);

    protected:
    };

    // DoneAndOut - a stub for final state when downloader UI is closed
    class CWizardStepTypeDoneAndOut : public CWizardStepType
    {
    public:
        void flag_List_as_WizardStepType();

        virtual sloc OnGetShortDownloadStatusForUi(
                ref<CDownloaderImpl> rDownloader)
                return
                    " ";

        virtual ref<UiContainer> OnNewUiForStep(
                ref<CDownloaderImpl> rDownloader,
                ref<CDeviceManagerIface> rDeviceManager)
                return
                    rDownloader->
                        NewUiStepDoneAndOut(
                            rDeviceManager);

        virtual bool OnTryEnterStep(
                ref<CDownloaderImpl> rDownloader,
                ref<CDeviceManagerIface> rDeviceManager);

    protected:
    };
