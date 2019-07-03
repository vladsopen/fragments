// CLoaderImpl.h
#pragma once

#include "CLoaderIface.h"

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

protected:
    virtual void OnStartApplication()
            ;
    virtual void OnEnsureProxyOpenAndActivateMainWindow()
            ;
    virtual void OnCloseProxyAndMainWindow()
            ;
    virtual void OnHandleLoaderIdle(
            out CProject::EIdleNext& out_eIdleNextMayBeUpgraded)
            ;

private:

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
