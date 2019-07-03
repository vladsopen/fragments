// CPatientLinkSampleIface.h
#pragma once

class CProxyIface;
class CPatientLinkIface;

//
// CPatientLinkSampleIface -
//
//   Patient sample context info attached to every Proxy.
//
ASSUME_IMPL_FOR_IFACE(CPatientLinkSampleImpl, CPatientLinkSampleIface)

class CPatientLinkSampleIface : public object
{
public:
    CPatientLinkSampleIface();
    NEW_LOCAL(CPatientLinkSampleImpl, CPatientLinkSampleIface)
    //NEW_GP(CustomApps, CPatientLinkSampleImpl, CPatientLinkSampleIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CPatientLinkIface> _x_pPatientLink
            xauto(Get, Set);
    ptr<CProxyIface> _x_pProxy
            xauto(Get, Set);

    // True if saved, opened, painted or viewed by Patients
    bool IsProxyAttachedToPatientSample()
            vhook;

    // True if not a new session but created for
    //   any PatientIo or PatientEmbed purpose
    bool IsProxyCreatedByPatientLink()
            vhook;

    // More specific Patients attachment cases
    bool IsPatientSampleOpenedByPatientIo()
            vhook;
    bool IsPatientSamplePaintedByPatientIo()
            vhook;
    bool IsPatientSampleViewedByPatientEmbed()
            vhook;

// Operations

    // Cleanup (close floaters)
    void HandleCloseProxy()
            vhook;

// UI

protected:
    virtual bool OnIsProxyAttachedToPatientSample()
            v1pure;
    virtual bool OnIsProxyCreatedByPatientLink()
            v1pure;
    virtual bool OnIsPatientSampleOpenedByPatientIo()
            v1pure;
    virtual bool OnIsPatientSamplePaintedByPatientIo()
            v1pure;
    virtual bool OnIsPatientSampleViewedByPatientEmbed()
            v1pure;
    virtual void OnHandleCloseProxy()
            v1pure;
private:
    //bool _m_bOneTimeInitPatientLinkSampleIfaceOk = false;

    void _init_AttachToPatientLinkSampleIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
