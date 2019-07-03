// CLogin.h
#pragma once

#include "CDividoBasicDialogIfaceGp.h"

class CSessionImpl;

//
// CLogin - 
//
//   Login dialog
//
//ASSUME_IMPL_FOR_IFACE(CLoginImpl, CLogin)

class CLogin : public CDividoBasicDialogIfaceGp
{
public:
    CLogin();
    //~CLogin();
    //NEW_LOCAL(CLoginImpl, CLogin)
    //NEW_GP(Ralarm, CLoginImpl, CLogin)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CSessionImpl> _x_pSession
            xauto(Get, Set);
    ref<CSessionImpl> GetSession()
            return
                _x_pSession.Get().Valid();

// Operations

// UI

    // LoginDialogContent
    ref<UiContainer> NewUiLoginDialogContent();
    ptrmap<CUiPanelGp> _m_ptrmapUiLoginDialogContent;

protected:

    virtual res OnGetDialogIcon()
            return
                RES__BROWSER_L();

    virtual sloc OnGetDialogCaption()
            return
                TRANSLATE(
                    "Logi[... UI-string-removed]");

    virtual ref<UiContainer> OnNewUiDividoDialogContent()
            return
                NewUiLoginDialogContent();

    //virtual ref<UiContainer> OnNewUiDividoDialogButtons()
    //        v1st;

    virtual void OnHandleDialogClosed()
            ;

private:
    //bool _m_bOneTimeInitLoginOk = false;
    //bool _m_bLoginOpened = false;

    void _init_AttachToLogin();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiLoginDialogContent.cpp)
class UiLoginDialogContent : public UiContainer
{
public:
    virtual void Ui();

    ref<CLogin> m_rLogin;

private:
    alikex _m_alikexEditLabel;
    bool m_bExtra = false;
    //key m_keyLastSelectedFrom;
};
