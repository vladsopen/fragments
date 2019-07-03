// CSessionImpl.h
#pragma once

#include "CSessionIface.h"
#include "CParaMapIface.h"

class CParaApiIface;
class CPatientLinkIface;

//
// CSessionImpl - see iface
//

class CSessionImpl : public CSessionIface
{
public:
    CSessionImpl();
    ~CSessionImpl();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Parallel processes serving this session.
    //   Move this to Loader if multiple sessions must share
    //   parallel process(es) in your app.
    ref<CParaMapIface> x_rParaMap
            xauto(Get);

    // debug
    ptr<CPatientLinkIface> _x_pForcePatientLink
            xauto(Get, Set);

// Operations

    // Save to Patients
    //void FinalizeStudy();
        // see CReceiverSessionImpl

// UI

    // SessionMain
    //ref<UiContainer> NewUiSessionMain();
    ptrmap<CUiPanelGp> _m_ptrmapUiSessionMain;

    // SessionPatientLink
    //ref<UiContainer> NewUiSessionPatientLink();
    ptrmap<CUiPanelGp> _m_ptrmapUiSessionPatientLink;

    // SessionCornerPopup
    //ref<UiContainer> NewUiSessionCornerPopup();
    ptrmap<CUiPanelGp> _m_ptrmapUiSessionCornerPopup;

protected:

    // IfaceGp:
    virtual bool OnIsSessionOpened()
            ;
    virtual bool OnJustIsAdded()
            ;
    virtual ref<CJobListIfaceGp> OnGetJobList()
            ;
    virtual utc OnGetLastGoodEventUtc()
            ;
    virtual utc OnGetLastBadEventUtc()
            ;
    virtual utc OnGetLastDownloadReceivedUtc()
            ;
    virtual bool OnIsDownloadListGoodAndComplete()
            ;
    virtual void OnInitSession()
            ;
    virtual void OnCloseSession()
            ;
    virtual void OnHandleSessionIdle(
            out CProject::EIdleNext& out_eIdleNextMayBeUpgraded)
            ;
    virtual ref<CSendParcelIfaceGp> OnNewSendParcel()
            ;
    virtual ref<UiContainer> OnNewUiSessionMain()
            ;
    virtual ref<UiContainer> OnNewUiReceiverSessionDisplay()
            ;
    virtual bool OnIsUnwelcomeIn()
            ;
    virtual ref<UiContainer> OnNewUiUnwelcome()
            ;

    // Iface:
    virtual ref<CPatientLinkIface> OnGetPatientLink()
            ;
    virtual ref<UiContainer> OnNewUiSessionPatientLink()
            ;
    virtual ref<UiContainer> OnNewUiSessionCornerPopup()
            ;

private:

    // init state
    bool _m_bOneTimeInitOk = false;
    bool m_bSessionOpen = false;
    path _m_pathDatabase;

    void _init_AttachToSessionImpl();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiSessionMain.cpp)
class UiSessionMain : public UiContainer
{
public:
    virtual void Ui();

    ref<CSessionImpl> m_rSession;
};

// (UiSessionPatientLink.cpp)
class UiSessionPatientLink : public UiContainer
{
public:
    virtual void Ui();

    ref<CSessionImpl> m_rSession;

private:
};

// (UiSessionCornerPopup.cpp)
class UiSessionCornerPopup : public UiContainer
{
public:
    virtual void Ui();

    ref<CSessionImpl> m_rSession;

private:
    int m_nLastJobBoxWidth = 0;
};
