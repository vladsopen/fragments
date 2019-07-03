// CLoaderImpl.h
#pragma once

#include "CLoaderIface.h"

class CPatientLinkIface;

//
// CLoaderImpl - see iface (no application-domain logic here!)
//

class CLoaderImpl : public CLoaderIface
{
public:
    CLoaderImpl();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

// Operations

// UI

    // MainWindow
    ref<UiContainer> NewUiMainWindow();
    ptr<CUiPanelGp> _m_pUiMainWindow;
    ptr<CUiPanelGp> _m_pUiLoader;

protected:
    virtual ref<CSessionIface> Onx_rSession_Get()
            ;
    virtual bool OnIsActiveSessionPresent()
            ;
    virtual void OnStartApplication()
            ;
    virtual void OnCloseSessionAndMainWindow()
            ;
    virtual void OnStopSessionAfterUiClosed()
            ;
    virtual void OnHandleLoaderIdle(
            out CProject::EIdleNext& out_eIdleNextMayBeUpgraded)
            ;
    virtual void OnEnsureOpenAndActivateMainWindow()
            ;

private:

    ref<CSessionIface> m_rActiveSession;
    bool _m_bSessionActive = false;

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiMainWindow.cpp)
class UiMainWindow : public UiContainer
{
public:
    virtual void Ui();

    ref<CLoaderImpl> m_rLoader;

private:
    alikey _m_alikeyCaptionButtons;
};
