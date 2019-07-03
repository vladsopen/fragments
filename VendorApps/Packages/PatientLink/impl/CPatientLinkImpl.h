// CPatientLinkImpl.h
#pragma once

#include "CPatientLinkIface.h"

class CPatientEmbedLinkImpl;
class CPatientCheckupIfaceGp;
class CPatientBaseConnectionIfaceGp;
class CPatientCallbackImpl;

//
// CPatientLinkImpl - PatientIo encapsulation for this project
//

class CPatientLinkImpl : public CPatientLinkIface
{
public:
    CPatientLinkImpl();
    //NEW_LOCAL(CPatientLinkImplImpl, CPatientLinkImpl)
    //NEW_GP(MyProject, CPatientLinkImplImpl, CPatientLinkImpl)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Main connection controller
    ref<CPatientBaseConnectionIfaceGp> x_rPatientBaseConnection
            xauto(Get);

    // PatientEmbed connection
    ref<CPatientEmbedLinkImpl> x_rPatientEmbedLink
            xauto(Get);

    // Proxy list for PatientIo-OpenSample 
    //ref<CPatientSampleMap> x_rOpenedPatientSampleMap
    //        xauto(Get);

// Operations

// UI

protected:
    virtual bool OnIsConnectedToPatient()
            ;
    //virtual str OnGetCurrentPatientNameAgeSexAndId()
    //        ;
    virtual str OnGetCurrentPatientFieldValue(
            str sFieldKey,
            str sOrReturn)
            ;
    virtual int OnGetCurrentPatientAge()
            ;
    virtual bool OnInitPatientIsMainFrameHidden()
            ;
    virtual void OnClosePatientLink()
            ;
    virtual void OnHandleStartNewCheckup()
            ;
    virtual path OnGetNewFullAttachmentFilePath()
            ;
    virtual void OnSendSampleProxyToPatient(
            ref<CPatientSampleIfaceGp> rPatientSample,
            ref<CProxyIface> rProxy)
            ;
    virtual void OnShowPatientCard()
            ;
    virtual void OnLockCurrentCheckup(
            bool bLockOn)
            ;
    virtual ref<UiContainer> OnNewUiPatInfoBrowser()
            ;
    virtual ref<UiContainer> OnNewUiPatientDebug()
            ;

private:
    ref<CPatientCheckupIfaceGp> m_rPatientCheckup;
    ref<CPatientCallbackImpl> m_rPatientCallback;
    bool _m_bPatientLinkInitOk = false;

    // Error reporting
    void ReportPatientCommunicationError(
            str sReport);

    void _init_AttachToPatientLinkImpl();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
