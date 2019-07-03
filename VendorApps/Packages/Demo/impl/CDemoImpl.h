// CDemoImpl.h
#pragma once

#include "CDemoIface.h"

class CDemoExitType;
class CProtocolIface;

//
// CDemoImpl - see Iface
//

class CDemoImpl : public CDemoIface
{
public:
    CDemoImpl();
    ~CDemoImpl();
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

    // or ""
    path GetCommandLineIni();

    // Ini parser
    ref<CProtocolIface> x_rExternalProtocol
            xauto(Get);

    // Open|Create
    bool x_bCreatingNewSample = false
            xauto(Get, Set);

    // What we return as the result
    type<CDemoExitType> x_typeSelectedExitType
            xauto(Get, Set);

    // The data we save into a file
    str x_sDemoData = ""
            xauto(Get, Set)
            xassert(true);

    // Where we store
    path GetDemoDataFilePath(
            str sPrefix,
            str sAttachmentRevision,
            out str& out_sError);

// Operations

    // One-time opener
    //void InitDemo();

    // Cleanup
    //void CloseDemo();

    // Done
    void PrepareDemoResultsAndExit();

    // File I/O
    void LoadDemoDataFile(
            out str& out_sError);
    void SaveDemoDataFile(
            out str& out_sError);

    // Errors
    void ReportDemoError(
            sloc slocMain,
            str sDetails);

// UI

    // DemoData
    ref<UiContainer> NewUiDemoData();
    ptrmap<CUiPanelGp> _m_ptrmapUiDemoData;

    // DemoExitSetupFail
    ref<UiContainer> NewUiDemoExitSetupFail();
    ptrmap<CUiPanelGp> _m_ptrmapUiDemoExitSetupFail;

    // DemoExitSetupNop
    ref<UiContainer> NewUiDemoExitSetupNop();
    ptrmap<CUiPanelGp> _m_ptrmapUiDemoExitSetupNop;

    // DemoExitSetupUpdate
    ref<UiContainer> NewUiDemoExitSetupUpdate();
    ptrmap<CUiPanelGp> _m_ptrmapUiDemoExitSetupUpdate;

    // DemoExitSetupCreate
    ref<UiContainer> NewUiDemoExitSetupCreate();
    ptrmap<CUiPanelGp> _m_ptrmapUiDemoExitSetupCreate;

    // DemoExitRadio
    ref<UiContainer> NewUiDemoExitRadio(
            type<CDemoExitType> typeExitType);
    ptrmap<CUiPanelGp> _m_ptrmapUiDemoExitRadio;

    // DemoControl
    ref<UiContainer> NewUiDemoControl();
    ptrmap<CUiPanelGp> _m_ptrmapUiDemoControl;

    // DemoMain
    //ref<UiContainer> NewUiDemoMain();
    ptrmap<CUiPanelGp> _m_ptrmapUiDemoMain;

protected:
    SEE_ALSO(CDemoIface)

    virtual bool OnIsExeStartedForDemo()
            ;
    virtual bool OnIsDemoInitOk()
            ;
    virtual void OnInitDemo()
            ;
    virtual void OnCloseDemo()
            ;
    virtual ref<UiContainer> OnNewUiDemoMain()
            ;

private:
    bool _m_bOneTimeInitDemoImplOk = false;
    bool _m_bDemoImplOpened = false;

    void _init_AttachToDemoImpl();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiDemoMain.cpp)
class UiDemoMain : public UiContainer
{
public:
    virtual void Ui();

    ref<CDemoImpl> m_rDemo;

private:
};

// (UiDemoControl.cpp)
class UiDemoControl : public UiContainer
{
public:
    virtual void Ui();

    ref<CDemoImpl> m_rDemo;

private:
};

// (UiDemoExitRadio.cpp)
class UiDemoExitRadio : public UiContainer
{
public:
    virtual void Ui();

    ref<CDemoImpl> m_rDemo;
    type<CDemoExitType> m_typeExitType;

private:
};

// (UiDemoExitSetupCreate.cpp)
class UiDemoExitSetupCreate : public UiContainer
{
public:
    virtual void Ui();

    ref<CDemoImpl> m_rDemo;

private:
};

// (UiDemoExitSetupUpdate.cpp)
class UiDemoExitSetupUpdate : public UiContainer
{
public:
    virtual void Ui();

    ref<CDemoImpl> m_rDemo;

private:
};

// (UiDemoExitSetupNop.cpp)
class UiDemoExitSetupNop : public UiContainer
{
public:
    virtual void Ui();

    ref<CDemoImpl> m_rDemo;

private:
};

// (UiDemoExitSetupFail.cpp)
class UiDemoExitSetupFail : public UiContainer
{
public:
    virtual void Ui();

    ref<CDemoImpl> m_rDemo;

private:
};

// (UiDemoData.cpp)
class UiDemoData : public UiContainer
{
public:
    virtual void Ui();

    ref<CDemoImpl> m_rDemo;

private:
};
