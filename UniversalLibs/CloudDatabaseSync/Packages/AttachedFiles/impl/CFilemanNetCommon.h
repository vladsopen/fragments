// CFilemanNetCommon.h
#pragma once

class CNetEnablePumpIfaceGp;
class CNetLocatorIfaceGp;
class CNetDriverIfaceGp;
class CFileClassImpl;
class CFilemanNetIn;
class CFilemanNetOut;

//
// CFilemanNetCommon - 
//
//   Common stuff for file upload and download
//
//ASSUME_IMPL_FOR_IFACE(CFilemanNetCommonImpl, CFilemanNetCommon)

class CFilemanNetCommon : public object
{
public:
    CFilemanNetCommon();
    ~CFilemanNetCommon();
    //NEW_LOCAL(CFilemanNetCommonImpl, CFilemanNetCommon)
    //NEW_GP(CloudSync, CFilemanNetCommonImpl, CFilemanNetCommon)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CFileClassImpl> _x_pFileClass
            xauto(Get, Set);

    // Uploader
    ref<CFilemanNetOut> x_rFilemanNetOut
            xauto(Get);

    // Downloader
    ref<CFilemanNetIn> x_rFilemanNetIn
            xauto(Get);

    // Duplicates local folder we send to Net locator
    path x_pathFilemanNetTempRoot = ""
            xauto(Get)
            xassert(value.Len() > 3);

    // supports test ping pong
    ref<CNetDriverIfaceGp> NewFilemanNetDriver();

// Operations

    // One-time opener
    void InitFilemanNetCommon(
            ref<CNetEnablePumpIfaceGp> rNetEnablePumpSync);

    // Cleanup
    void CloseFilemanNetCommon();

    // Init common communication channel locator
    void InitFilemanNetLocator(
            ref<CNetLocatorIfaceGp> rNetLocator);

    // Error reporting
    void HandleFilemanNetError(
            str sError);

    // non-timer external poll
    void ForcePollFilemanNetCommon(
            out bool& out_bHaveJobs);

// UI

    // FilemanNetCommonStatus
    ref<UiContainer> NewUiFilemanNetCommonStatus();
    ptrmap<CUiPanelGp> _m_ptrmapUiFilemanNetCommonStatus;

protected:
private:
    bool _m_bOneTimeInitFilemanNetCommonOk = false;
    bool _m_bFilemanNetCommonOpened = false;

    static ptr<CNetDriverIfaceGp> s_pForceNetDriverForTests;
        friend class CTestAllNodes;
        friend class CTestAll;

    void _init_AttachToFilemanNetCommon();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiFilemanNetCommonStatus.cpp)
class UiFilemanNetCommonStatus : public UiContainer
{
public:
    virtual void Ui();

    ref<CFilemanNetCommon> m_rFilemanNetCommon;

private:
};
