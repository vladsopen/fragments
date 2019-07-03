// CProxyImpl.h
#pragma once

#include "CProxyIface.h"
#include "CParaMapIface.h"

class CProtocolIface;
class CInterprocRunProcessIfaceGp;
class CSequence;
class CParaApiIface;
class CPatientLinkIface;

//
// CProxyImpl - see iface
//

class CProxyImpl : public CProxyIface
{
public:
    CProxyImpl();
    ~CProxyImpl();
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

    // An algorithm consisting of 'Steps'
    ref<CSequence> x_rSequence
            xauto(Get);

    // External app
    ref<CInterprocRunProcessIfaceGp> x_rRunExternalApp
            xauto(Get);

    // Comm formatting
    ref<CProtocolIface> x_rProtocol
            xauto(Get);

    // In/Out ini file
    path x_pathProtocolIni = path()
            xauto(Get, Set)
            xassert(true);

    // Protocol data location
    path GetSampleFolder();
    str GetSampleExternalFilePrefix();
    str GetSampleAttachmentRevision();
    str GetSampleFileExtension();

    // Exec
    path x_pathExternalExe = path()
            xauto(Get, Set)
            xassert(true);

    // Disable tracer for production mode
    bool x_bAutoExit = false
            xauto(Get, Set);

// Operations

    // Save to Patients
    void FinalizeStudy(
            str sExternalProtocolFilePrefix,
            str sAttachmentRevision,
            str sAttachmentExtension,
            str sIndicatorRecordingStart,
            str sIndicatorRecordingDuration,
            str sIndicatorEcgFragmentCount);

    // Errors
    void ReportProxyError(
            sloc slocMain,
            str sDetails);

// UI

    // ProxyMain
    //ref<UiContainer> NewUiProxyMain();
    ptrmap<CUiPanelGp> _m_ptrmapUiProxyMain;

protected:

    // IfaceGp:
    virtual bool OnIsProxyOpened()
            ;
    virtual void OnInitProxy(
            path pathOpenExistingAttachment,
            str sAttachmentRevision,
            str sAttachmentExtra)
            ;
    virtual void OnCloseProxy()
            ;
    virtual ref<UiContainer> OnNewUiProxyMain()
            ;
    virtual void OnHandleProxyIdle(
            out CProject::EIdleNext& out_eIdleNextMayBeUpgraded)
            ;

    // Iface:
    virtual ref_ret<CPatientLinkIface> OnGetPatientLink()
            ;

private:

    // init state
    bool _m_bOneTimeInitOk = false;
    bool m_bProxyOpen = false;
    path _m_pathSampleFolder;
    str _m_sSampleExternalFilePrefix;
    str _m_sSampleAttachmentRevision;
    str _m_sSampleExternalFileExtension;

    int m_nDemoCounter = 0;

    void _init_AttachToProxyImpl();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiProxyMain.cpp)
class UiProxyMain : public UiContainer
{
public:
    virtual void Ui();

    ref<CProxyImpl> m_rProxy;
};
