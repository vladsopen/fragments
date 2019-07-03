// CClassTestingTempIface.h
#pragma once

#include "CClassTestingTempIfaceGp.h"

//
// CClassTestingTempIface - see CClassTestingTempIfaceGp
//
//ASSUME_IMPL_FOR_IFACE(CClassTestingTempImpl, CClassTestingTempIface)

class CClassTestingTempIface : public CClassTestingTempIfaceGp
{
public:
    CClassTestingTempIface();
    NEW_LOCAL(CClassTestingTempImpl, CClassTestingTempIface)
    //NEW_GP(VirtOs, CClassTestingTempImpl, CClassTestingTempIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

// UI

protected:
private:
    //bool _m_bOneTimeInitClassTestingTempIfaceOk = false;

    void _init_AttachToClassTestingTempIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
