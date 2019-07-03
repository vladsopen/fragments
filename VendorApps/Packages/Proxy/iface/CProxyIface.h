// CProxyIface.h
#pragma once

#include "CProxyIfaceGp.h"

class CPatientLinkIface;
class CPatientLinkSampleIface;

//
// CProxyIface - see iface
//

class CProxyIface : public CProxyIfaceGp
{
public:
    CProxyIface();
    NEW_LOCAL(CProxyImpl, CProxyIface)
    //NEW_GP(CustomApps, CProxyImpl, CProxyIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Patients sample context
    ref<CPatientLinkSampleIface> _x_rPatientLinkSample
            xauto(Get);

    // Global object access encapsulation
    ref_ret<CPatientLinkIface> GetPatientLink()
            vhook;

// Operations

// UI

protected:
    virtual ref_ret<CPatientLinkIface> OnGetPatientLink()
            v1pure;
private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToProxyIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
