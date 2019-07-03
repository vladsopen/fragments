// CPatientLinkSampleImpl.h
#pragma once

#include "CPatientLinkSampleIface.h"

//
// CPatientLinkSampleImpl - see Iface
//

class CPatientLinkSampleImpl : public CPatientLinkSampleIface
{
public:
    CPatientLinkSampleImpl();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // For viewing samples opened via PatientIo or in PatientEmbed
    str x_sOpenedPatientSampleKey = ""
            xauto(Get, Set)
            xassert(true);

    // PatientIo: OpenSample
    bool x_bProxyOpenedFromPatient = false
            xauto(Get, Set);

    // PatientIo: PaintSample
    bool x_bTempProxyForPatientPaint = false
            xauto(Get, Set);

    // PatientEmbed
    bool x_bProxyForPatientEmbed = false
            xauto(Get, Set);

    // Allows hiding Patients samples
    bool x_bShowProxyInList = false
            xauto(Get, Set);

    // Selection
    bool x_bProxySelectedInPatient = false
            xauto(Get, Set);

// Operations

// UI

    // PatientLinkSampleView
    void OpenUiPatientLinkSampleView();
    ref<UiContainer> NewUiPatientLinkSampleView();
    ptrmap<CUiPanelGp> _m_ptrmapUiPatientLinkSampleView;

protected:
    virtual bool OnIsProxyAttachedToPatientSample()
            ;
    virtual bool OnIsProxyCreatedByPatientLink()
            ;
    virtual bool OnIsPatientSampleOpenedByPatientIo()
            ;
    virtual bool OnIsPatientSamplePaintedByPatientIo()
            ;
    virtual bool OnIsPatientSampleViewedByPatientEmbed()
            ;
    virtual void OnHandleCloseProxy()
            ;
private:
    //bool _m_bOneTimeInitPatientLinkSampleImplOk = false;

    void _init_AttachToPatientLinkSampleImpl();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiPatientLinkSampleView.cpp)
class UiPatientLinkSampleView : public UiContainer
{
public:
    virtual void Ui();

    ref<CPatientLinkSampleImpl> m_rPatientLinkSample;

private:
};
