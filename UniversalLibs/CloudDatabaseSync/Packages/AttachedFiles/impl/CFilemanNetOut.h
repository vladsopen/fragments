// CFilemanNetOut.h
#pragma once

#include "VirtOs_ontimer.h"

class CNetEnablePumpIfaceGp;
class CNetSyncOutIfaceGp;
class CFilemanNetCommon;
class CFileClassImpl;

//
// CFilemanNetOut - 
//
//   Controls file uploading
//
//ASSUME_IMPL_FOR_IFACE(CFilemanNetOutImpl, CFilemanNetOut)

class CFilemanNetOut : public object
{
public:
    CFilemanNetOut();
    ~CFilemanNetOut();
    //NEW_LOCAL(CFilemanNetOutImpl, CFilemanNetOut)
    //NEW_GP(CloudSync, CFilemanNetOutImpl, CFilemanNetOut)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CFilemanNetCommon> _x_pFilemanNetCommon
            xauto(Get, Set);

    // Net Uploader
    ref<CNetSyncOutIfaceGp> x_rFilemanNetSyncOut
            xauto(Get);

// Operations

    // One-time opener
    void InitFilemanNetOut(
            ref<CNetEnablePumpIfaceGp> rNetEnablePumpSync);

    // Cleanup
    void CloseFilemanNetOut();

    // Put file contents into upload channel queue
    void StartUploadingLocalFile(
            path pathLocalFile,
            str sSubdirs);

    // non-timer external poll
    void ForcePollFilemanNetOut(
            out bool& out_bHaveJobs);

// UI

    // FilemanNetOutStatus
    ref<UiContainer> NewUiFilemanNetOutStatus();
    ptrmap<CUiPanelGp> _m_ptrmapUiFilemanNetOutStatus;

protected:
private:
    bool _m_bOneTimeInitFilemanNetOutOk = false;
    bool _m_bFilemanNetOutOpened = false;

    int m_nNextUniqueFile = 0;

    void HandleFilemanUploadTimer();
    ontimer<HandleFilemanUploadTimer> _m_ontimerFilemanUploadTimer;

    friend class CTestAllNodes;

    void _init_AttachToFilemanNetOut();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiFilemanNetOutStatus.cpp)
class UiFilemanNetOutStatus : public UiContainer
{
public:
    virtual void Ui();

    ref<CFilemanNetOut> m_rFilemanNetOut;

private:
};
