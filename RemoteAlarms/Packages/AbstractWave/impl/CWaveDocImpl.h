// CWaveDocImpl.h
#pragma once

#include "CWaveDocIface.h"
#include "VirtOs_ontimer.h"

class CWaveLaunch;
class CWaveView;
class CPatientLinkIface;
class CParseEcgFile;
class CWaveDb;
class CWaveChannelImpl;

//
// CWaveDocImpl - see Iface
//

class CWaveDocImpl : public CWaveDocIface
{
public:
    CWaveDocImpl();
    ~CWaveDocImpl();
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

    ref<CPatientLinkIface> GetPatientLink();

    // Data file
    path x_pathWaveDoc = ""
            xauto(Get, Set)
            xassert(value != "");

    // Array count
    int GetActiveWaveChannels();

    // Segment storage for all channels
    ref<CWaveDb> x_rWaveDb
            xauto(Get);

    // Viewport/scroll state
    ref<CWaveView> x_rWaveView
            xauto(Get);

    // Feeds AnotherCompany SMDEV5 format to the db
    ref<CParseEcgFile> x_rParseEcgFile
            xauto(Get);

    // Runs external processor via PatInfoContact
    ref<CWaveLaunch> x_rWaveLaunch
            xauto(Get);

    // Debug
    bool x_bWaveHackMode = false
            xauto(Get, Set);

    // Online
    bool x_bEnableOnlineIncrementalLoad = true
            xauto(Get, Set);
    bool x_bIdleOnlineIncrementalLoadDetected = false
            xauto(Get);

// Operations

    // One-time opener
    //void InitWaveDoc();

    // Cleanup
    //void CloseWaveDoc();

    // All active channels
    bool IterateWaveChannels(
            out iter& out_i,
            out ref<CWaveChannelImpl>& out_value);

// UI

    // WaveDocMenu
    //ref<UiContainer> NewUiWaveDocMenu();
    ptrmap<CUiPanelGp> _m_ptrmapUiWaveDocMenu;

    // WaveDocMain
    //ref<UiContainer> NewUiWaveDocMain();
    ptrmap<CUiPanelGp> _m_ptrmapUiWaveDocMain;

protected:
    SEE_ALSO(CWaveDocIface)

    virtual bool OnIsWaveDocActive()
            ;
    virtual void OnOpenWaveDoc(
            path pathWave)
            ;
    virtual void OnCloseWaveDoc()
            ;
    virtual ref<UiContainer> OnNewUiWaveDocMain()
            ;
    virtual ref<UiContainer> OnNewUiWaveDocMenu()
            ;

private:
    bool _m_bOneTimeInitWaveDocImplOk = false;
    bool _m_bWaveDocImplOpened = false;

    array< ref<CWaveChannelImpl> > m_arChannel;

    void HandleWaveReceptionTimer();
    ontimer<HandleWaveReceptionTimer> _m_ontimerWaveReception;

    void _init_AttachToWaveDocImpl();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiWaveDocMain.cpp)
class UiWaveDocMain : public UiContainer
{
public:
    virtual void Ui();

    ref<CWaveDocImpl> m_rWaveDoc;
};

// (UiWaveDocMenu.cpp)
class UiWaveDocMenu : public UiContainer
{
public:
    virtual void Ui();

    ref<CWaveDocImpl> m_rWaveDoc;

private:
};
