// CFilemanNetIn.h
#pragma once

#include "VirtOs_ontimer.h"

class CNetEnablePumpIfaceGp;
class CNetSyncInIfaceGp;
class CFilemanNetCommon;

//
// CFilemanNetIn - 
//
//   Controls file downloading
//
//ASSUME_IMPL_FOR_IFACE(CFilemanNetInImpl, CFilemanNetIn)

class CFilemanNetIn : public object
{
public:
    CFilemanNetIn();
    ~CFilemanNetIn();
    //NEW_LOCAL(CFilemanNetInImpl, CFilemanNetIn)
    //NEW_GP(CloudSync, CFilemanNetInImpl, CFilemanNetIn)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CFilemanNetCommon> _x_pFilemanNetCommon
            xauto(Get, Set);

    // Net Downloader
    ref<CNetSyncInIfaceGp> x_rFilemanNetSyncIn
            xauto(Get);

    // Where complete downloads are stored for importing
    //path GetFilemanDownloadFolder();

    //// Ready files
    //int x_nRecentInboxFileCount = 0
    //        xauto(Get),
    //        xassert(true);

// Operations

    // One-time opener
    void InitFilemanNetIn(
            ref<CNetEnablePumpIfaceGp> rNetEnablePumpSync);

    // Cleanup
    void CloseFilemanNetIn();

    // Apply downloaded action files
    void GoImportFiles();

    // non-timer external poll
    void ForcePollFilemanNetIn(
            out bool& out_bHaveJobs);

// UI

    // FilemanNetInStatus
    ref<UiContainer> NewUiFilemanNetInStatus();
    ptrmap<CUiPanelGp> _m_ptrmapUiFilemanNetInStatus;

protected:
private:
    bool _m_bOneTimeInitFilemanNetInOk = false;
    bool _m_bFilemanNetInOpened = false;

    void HandleFilemanDownloadTimer();
    ontimer<HandleFilemanDownloadTimer> _m_ontimerFilemanDownloadTimer;

    friend class CTestAllNodes;

    void _init_AttachToFilemanNetIn();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiFilemanNetInStatus.cpp)
class UiFilemanNetInStatus : public UiContainer
{
public:
    virtual void Ui();

    ref<CFilemanNetIn> m_rFilemanNetIn;

private:
};
