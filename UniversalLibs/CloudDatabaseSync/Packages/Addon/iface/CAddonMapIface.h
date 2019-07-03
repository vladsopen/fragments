// CAddonMapIface.h
#pragma once

#include "CAddonMapIfaceGp.h"

class CTableClassIface;
class CActionIfaceGp;
class CUdbConnectionIfaceGp;
class CSessionIface;

//
// CAddonMapIface - see CAddonMapIfaceGp
//
//ASSUME_IMPL_FOR_IFACE(CAddonMapImpl, CAddonMapIface)

class CAddonMapIface : public CAddonMapIfaceGp
{
public:
    CAddonMapIface();
    //~CAddonMapIface();
    NEW_LOCAL(CAddonMapImpl, CAddonMapIface)
    //NEW_GP(CloudSync, CAddonMapImpl, CAddonMapIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CSessionIface> _x_pSession
            xauto(Get, Set);

// Operations

    // Session calls this for its map upon init
    void InitAddonMapForSession()
            vhook;
    void HandleAddonInitAfterCloud()
            vhook;

    // Session calls this for its map upon close
    void CloseAddonMapForSession()
            vhook;

    // Specific for table registration
    void RegisterTableClassSession(
            classinfo classinfoDb,
            str sClassIdCore,
            ref<CTableClassIface> rTableClass)
            vhook;
        SEE_ALSO(RegisterAddonSession)

    //
    // Hook Callers
    //

    // Addons may add their table classes into the local cache db
    void HandleDbClassInfoListUpdate(
            ref<CUdbConnectionIfaceGp> rUdbConnection)
            vhook;

    // Called after Store init complete, before the rest of init
    //   Addons may want to make sure their system objects are added to db
    void HandleStoreReady(
            ref<CUdbConnectionIfaceGp> rUdbConnection)
            vhook;

    // At this point addons are allowed to lookup each
    //   other using FindSingletonAddonSession()
    //   and IterateAddonSessions() is available
    void HandleMapRegistrationsComplete()
            vhook;

    // A change is being applied, update your relational caches as appropriate
    void HandleActionInterpreter(
            ref<CActionIfaceGp> rAction)
            vhook;

// UI

protected:
    SEE_ALSO(CAddonMapImpl) // F12-lookup

    virtual void OnInitAddonMapForSession()
            v1pure;
    virtual void OnHandleAddonInitAfterCloud()
            v1pure;
    virtual void OnCloseAddonMapForSession()
            v1pure;
    virtual void OnRegisterTableClassSession(
            classinfo classinfoDb,
            str sClassIdCore,
            ref<CTableClassIface> rTableClass)
            v1pure;
    virtual void OnHandleDbClassInfoListUpdate(
            ref<CUdbConnectionIfaceGp> rUdbConnection)
            v1pure;
    virtual void OnHandleStoreReady(
            ref<CUdbConnectionIfaceGp> rUdbConnection)
            v1pure;
    virtual void OnHandleMapRegistrationsComplete()
            v1pure;
    virtual void OnHandleActionInterpreter(
            ref<CActionIfaceGp> rAction)
            v1pure;

private:
    //bool _m_bOneTimeInitAddonMapIfaceOk = false;
    //bool _m_bAddonMapIfaceOpened = false;

    void _init_AttachToAddonMapIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
