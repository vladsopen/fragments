// CNetTestSyncGui.h
#pragma once

#include "VirtOs_ontimer.h"

class CNetLocatorIfaceGp;
class CNetSyncInImpl;
class CNetSyncOutImpl;

//
// CNetTestSyncGui - interactive test
//
//ASSUME_IMPL_FOR_IFACE(CNetTestSyncGuiImpl, CNetTestSyncGui)

class CNetTestSyncGui : public object
{
public:
    CNetTestSyncGui();
    //~CNetTestSyncGui();
    //NEW_LOCAL(CNetTestSyncGuiImpl, CNetTestSyncGui)
    //NEW_GP(Net, CNetTestSyncGuiImpl, CNetTestSyncGui)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);
    //ref<> Get ()
    //        return
    //            _x_p .Get().Valid();

    // Root
    path x_pathTop
            xauto(Get, Set)
            xassert(true);

    // Test source
    path x_pathSendFile
            xauto(Get, Set)
            xassert(true);

    // Test source
    path x_pathSendFolder
            xauto(Get, Set)
            xassert(true);

    // Multifile upload mode
    ref<CPathList> x_rPathListTodoSend
            xauto(Get);

    // Upload queue
    path x_pathUploadTemp
            xauto(Get, Set)
            xassert(true);

    // Download parts
    path x_pathDownloadTemp
            xauto(Get, Set)
            xassert(true);

    // Reception location
    path x_pathReceptionFolder
            xauto(Get, Set)
            xassert(true);

    // Out
    ref<CNetSyncOutImpl> x_rNetSyncOut
            xauto(Get);

    // In
    ref<CNetSyncInImpl> x_rNetSyncIn
            xauto(Get);

// Operations

    // One-time opener
    void InitNetTestSyncGui();

    // Cleanup
    //void CloseNetTestSyncGui();

    // [Send]
    void SendFile(
            path pathSendFile);
    void SendFolder();

// UI

    // NetTestSyncGuiFile
    ref<UiContainer> NewUiNetTestSyncGuiFile(
            path pathListedFile);
    ptrmap<CUiPanelGp> _m_ptrmapUiNetTestSyncGuiFile;

    // NetTestSyncGuiDir
    ref<UiContainer> NewUiNetTestSyncGuiDir(
            path pathListFolder);
    ptrmap<CUiPanelGp> _m_ptrmapUiNetTestSyncGuiDir;

    // NetTestSyncGuiMain
    ref<UiContainer> NewUiNetTestSyncGuiMain();
    ptrmap<CUiPanelGp> _m_ptrmapUiNetTestSyncGuiMain;

protected:
private:
    bool _m_bInited = false;
    bool _m_bOpened = false;

    utc _m_utcLastSend;

    // Timer handler 
    void HandleNetTestSync();
    ontimer<HandleNetTestSync> _m_ontimerTestSync;

    ref<CNetLocatorIfaceGp> NewCommonLocator();

    void _init_AttachToNetTestSyncGui();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiNetTestSyncGuiMain.cpp)
class UiNetTestSyncGuiMain : public UiContainer
{
public:
    virtual void Ui();

    ref<CNetTestSyncGui> m_rNetTestSyncGui;

private:
};

// (UiNetTestSyncGuiDir.cpp)
class UiNetTestSyncGuiDir : public UiContainer
{
public:
    virtual void Ui();

    ref<CNetTestSyncGui> m_rNetTestSyncGui;
    path m_pathListFolder;

private:
};

// (UiNetTestSyncGuiFile.cpp)
class UiNetTestSyncGuiFile : public UiContainer
{
public:
    virtual void Ui();

    ref<CNetTestSyncGui> m_rNetTestSyncGui;
    path m_pathListedFile;

private:
};
