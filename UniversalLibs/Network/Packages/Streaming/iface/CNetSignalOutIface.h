// CNetSignalOutIface.h
#pragma once

#include "CNetSignalOutIfaceGp.h"

class CNetSupervisorIface;

//
// CNetSignalOutIface - see ifacegp
//
ASSUME_IMPL_FOR_IFACE(CNetSignalOutImpl, CNetSignalOutIface)

class CNetSignalOutIface : public CNetSignalOutIfaceGp
{
public:
    CNetSignalOutIface();
    NEW_LOCAL(CNetSignalOutImpl, CNetSignalOutIface)
    //NEW_GP(Net, CNetSignalOutImpl, CNetSignalOutIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CNetSupervisorIface> _x_pNetSupervisor
            xauto(Get, Set);

// Operations

// UI

protected:
private:
    //bool _m_bOneTimeInitNetSignalOutIfaceOk = false;

    void _init_AttachToNetSignalOutIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
