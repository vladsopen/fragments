// CSessionIface.h
#pragma once

#include "CSessionIfaceGp.h"

class CFilterSystemIface;
class CFileClassIface;
class CBasicClassIface;
class CAddonMapIface;
class CActionIface;
class CNetIface;
class CStoreIface;

//
// CSessionIface - see ifacegp
//

class CSessionIface : public CSessionIfaceGp
{
public:
    CSessionIface();
    NEW_LOCAL(CSessionImpl, CSessionIface)
    //NEW_GP(CloudSync, CSessionImpl, CSessionIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Manages access to the journalled database
    ref<CStoreIface> x_rStore
            xauto(Get);

    // Network sync
    ref<CNetIface> x_rNet
            xauto(Get);

    // Core singleton class sessions
    ref<CBasicClassIface> x_rSessionBasicClass
            xauto(Get);
    ref<CFileClassIface> x_rSessionFileClass
            xauto(Get);

    // Addon registrations for this session
    ref<CAddonMapIface> x_rAddonMap
            xauto(Get);

    // Filters
    ref<CFilterSystemIface> x_rFilterSystem
            xauto(Get);

// Operations

// UI

protected:
private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToSessionIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
