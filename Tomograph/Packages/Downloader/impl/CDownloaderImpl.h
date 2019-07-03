// CDownloaderImpl.h
#pragma once

#include "CDownloaderIface.h"

class CGlobalConfigIface;
class CWizardStepType;
class CTomogramIface;
class CTimerForTomographDownloader;
class CDeviceManagerIface;

//
// CDownloaderImpl - controls download session for Tomo,
//   maintains main UI and coordinates Device, Tomogram and PatientLink 
//   efforts in this application mode
//

class CDownloaderImpl : public CDownloaderIface
{
public:
    CDownloaderImpl();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // We don't want to repaint on every amplitude received!
    static const tick C_tickOnlineRepaintTimeout = 300;

// Attributes

    // Device data callback
    ref<CTimerForTomographDownloader> x_rTimerForTomograph
            xauto(Get);

    // Wizard phase
    type<CWizardStepType> x_typeWizardStepType = 
            (type<CWizardStepType>) type<CWizardStepTypeWelcome>()
            xauto(Get);

    // Current sample being downloaded
    int GetTomogramCount();

    // The number of samples we want to download
    int x_nWantDownloadTomogramCount = -1
            xauto(Get, Set)
            xassert(value >= -1 && value < oo);

    // Currenly shown Tomogram
    int x_iPreviewTomogram = -1
            xauto(Get)
            xassert(value >= -1 && value < oo);
    void x_iPreviewTomogram_Set(
            int value);

    // For testing we may not want to erase all every time
    bool x_bDoEraseAllWhenDone = 
            !debug("Downloader.NoEraseByDefault?").InputBool()
            xauto(Get, Set);

    // Body point error log
    str x_sBodyPointsFailed = ""
            xauto(Get, Set)
            xassert(true);

    // Gets filled upon download completion
    rich x_richFinalResultInfo = ""
            xauto(Get, Set)
            xassert(true);

    // Extra file size 
    big x_nnAddAttachmentBytes = 0
            xauto(Get, Set)
            xassert(value >= 0);
    bool x_bAddAttachmentHardRandom = true
            xauto(Get, Set);
    bool x_bAddAttachmentRandomWave = true
            xauto(Get, Set);

    // Download status
    bool IsDownloadInProgress();
    
    // Total stored Tomogram count
    //int GetStoredTomogramCount();

    // Total stored Tomogram count
    ptr<CTomogramIface> GetStoredTomogramAt(
            int iPosInArray);

// Operations

    // Open the device and request the data
    bool GotoNextStep(
            type<CWizardStepType> typeCheckExpectedWizardStepType,
            ref<CDeviceManagerIface> rDeviceManager);

    // Append a new Tomogram to store incoming data
    void AddNewTomogram();

    // Clear opened Tomograms
    void RemoveAllTomograms();

    // Encapsulating access to a global entity
    ref<CGlobalConfigIface> GetGlobalConfig();

    // External processor launcher
    void RunExternalProcessor(
            ref<CTomogramIface> rTomogram);

// UI

    // DownloaderMain
    //ref<UiContainer> NewUiDownloaderMain();
    ptr<CUiPanelGp> _m_pUiDownloaderMain;
    void UpdateDownloaderUiCompletelyOnline();
    void ShutdownDownloaderUi();

    // WizardImageArea
    ref<UiContainer> NewUiWizardImageArea();
    ptr<CUiPanelGp> _m_pUiWizardImageArea;

    //
    // Wizard Steps
    //

    // StepWelcome
    ref<UiContainer> NewUiStepWelcome(
            ref<CDeviceManagerIface> rDeviceManager);
    ptr<CUiPanelGp> _m_pUiStepWelcome;

    // StepDownloading
    ref<UiContainer> NewUiStepDownloading(
            ref<CDeviceManagerIface> rDeviceManager);
    ptr<CUiPanelGp> _m_pUiStepDownloading;

    // StepFinalInfo
    ref<UiContainer> NewUiStepFinalInfo(
            ref<CDeviceManagerIface> rDeviceManager);
    ptr<CUiPanelGp> _m_pUiStepFinalInfo;

    // StepDoneAndOut
    ref<UiContainer> NewUiStepDoneAndOut(
            ref<CDeviceManagerIface> rDeviceManager);
    ptr<CUiPanelGp> _m_pUiStepDoneAndOut;

    //
    // more UIs...
    //

    // DownloaderOptions
    void OpenUiDownloaderOptionsDialog();
    ref<UiContainer> NewUiDownloaderOptions();
    ptr<CUiPanelGp> _m_pUiDownloaderOptions;

    // DownloaderList
    ref<UiContainer> NewUiDownloaderList();
    ptr<CUiPanelGp> _m_pUiDownloaderList;

    // DownloaderPointRow
    ref<UiContainer> NewUiDownloaderPointRow(
            int iPointOrTomogram);
    ptr<CUiPanelGp> _m_pUiDownloaderPointRow;

    // EmergencyMenu
    ref<UiContainer> NewUiEmergencyMenu(
            ref<CDeviceManagerIface> rDeviceManager);
    ref<UiContainer> OpenUiEmergencyMenu(
            ref<UiContainer> rUiParent,
            ref<CDeviceManagerIface> rDeviceManager);
    ptr<CUiPanelGp> _m_pUiEmergencyMenu;

protected:
    virtual void OnAppendNextAmplitudePairFromTomograph(
            ref<CDeviceManagerIface> rDeviceManager,
            int nAmplitudeAt0,
            int nAmplitudeAt1)
            ;
    virtual void OnHandleNextTomogramComplete(
            ref<CDeviceManagerIface> rDeviceManager)
            ;
    virtual void OnOpenTomogram(
            ref<CTomogramIface> rTomogram)
            ;
    virtual void OnUpdateErrorReportingUi()
            ;
    virtual int OnGetStoredTomogramCount()
            ;
    virtual void OnPrepareDownloaderToSelectedBodyProfile()
            ;
    virtual ref<UiContainer> OnNewUiDownloaderMain(
            ref<CDeviceManagerIface> rDeviceManager)
            ;

private:

    array< ref<CTomogramIface> > m_aReceivedTomogram;
    tick _m_tickLastOnlineRepaint = 0;

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    virtual void OnTestClass();
};

// (UiDownloaderMain.cpp)
class UiDownloaderMain : public UiContainer
{
public:
    virtual void Ui();

    ref<CDownloaderImpl> m_rDownloader;
    ref<CDeviceManagerIface> m_rDeviceManager;

private:
    void EqualizePrevNextButtons(
            ref<CUiButtonGp> rUi);
};

// (UiWizardImageArea.cpp)
class UiWizardImageArea : public UiContainer
{
public:
    virtual void Ui();

    ref<CDownloaderImpl> m_rDownloader;

private:
    ptr<CTomogramIface> _m_pRecentSelectedTomogram;
};

// (UiStepWelcome.cpp)
class UiStepWelcome : public UiContainer
{
public:
    virtual void Ui();

    ref<CDownloaderImpl> m_rDownloader;
    ref<CDeviceManagerIface> m_rDeviceManager;
};

// (UiStepDownloading.cpp)
class UiStepDownloading : public UiContainer
{
public:
    virtual void Ui();

    ref<CDownloaderImpl> m_rDownloader;
    ref<CDeviceManagerIface> m_rDeviceManager;
};

// (UiStepFinalInfo.cpp)
class UiStepFinalInfo : public UiContainer
{
public:
    virtual void Ui();

    ref<CDownloaderImpl> m_rDownloader;
    ref<CDeviceManagerIface> m_rDeviceManager;
};

// (UiStepDoneAndOut.cpp)
class UiStepDoneAndOut : public UiContainer
{
public:
    virtual void Ui();

    ref<CDownloaderImpl> m_rDownloader;
    ref<CDeviceManagerIface> m_rDeviceManager;
};

// (UiDownloaderOptions.cpp)
class UiDownloaderOptions : public UiContainer
{
public:
    virtual void Ui();

    ref<CDownloaderImpl> m_rDownloader;
};

// (UiDownloaderList.cpp)
class UiDownloaderList : public UiContainer
{
public:
    virtual void Ui();

    ref<CDownloaderImpl> m_rDownloader;

private:
    str m_sRecentProfileName = "";
};

// (UiDownloaderPointRow.cpp)
class UiDownloaderPointRow : public UiContainer
{
public:
    virtual void Ui();

    ref<CDownloaderImpl> m_rDownloader;
    int m_iPointOrTomogram = -1;
};

// (UiEmergencyMenu.cpp)
class UiEmergencyMenu : public UiContainer
{
public:
    virtual void Ui();

    ref<CDownloaderImpl> m_rDownloader;
    ref<CDeviceManagerIface> m_rDeviceManager;
};
