// CAddonMapIfaceGp.h
#pragma once

class CTableClassIfaceGp;
class CAddonSessionIfaceGp;

//
// CAddonMapIfaceGp -
//
//   Handles all CloudSync Addon modules for a Session.
//
ASSUME_IMPL_FOR_IFACE(CAddonMapIface, CAddonMapIfaceGp)
ASSUME_IMPL_FOR_IFACE(CAddonMapImpl, CAddonMapIface)

class CAddonMapIfaceGp : public object
{
public:
    CAddonMapIfaceGp();
    //~CAddonMapIfaceGp();
    //NEW_LOCAL(CAddonMapImpl, CAddonMapIfaceGp)
    NEW_GP(CloudSync, CAddonMapImpl, CAddonMapIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

    // Add another Plugin class
    void RegisterAddonSession(
            ref<CAddonSessionIfaceGp> rAddonSession)
            vhook;
        SEE_ALSO(RegisterTableClassSession())

    // List all registrations
    bool IterateAddonSessions(
            out iter& out_i,
            out ref<CAddonSessionIfaceGp>& out_rAddonSession)
            vhook;

    // Locate a required plug-in session
    ptr<CAddonSessionIfaceGp> FindSingletonAddonSession(
            classinfo classinfoAddonSession)
            vhook;

    // For TableClass addons only
    queryorcount QueryTableClasses(
            out iter& out_i,
            out ref<CTableClassIfaceGp>& out_rTableClass,
            str sClassIdCore,
            classinfo classinfoAppDb)
            vhook;

// UI

protected:
    SEE_ALSO(CAddonMapImpl) // F12-lookup

    virtual void OnRegisterAddonSession(
            ref<CAddonSessionIfaceGp> rAddonSession)
            v1pure;
    virtual bool OnIterateAddonSessions(
            out iter& out_i,
            out ref<CAddonSessionIfaceGp>& out_rAddonSession)
            v1pure;
    virtual ptr<CAddonSessionIfaceGp> OnFindSingletonAddonSession(
            classinfo classinfoAddonSession)
            v1pure;
    virtual queryorcount OnQueryTableClasses(
            out iter& out_i,
            out ref<CTableClassIfaceGp>& out_rTableClass,
            str sClassIdCore,
            classinfo classinfoAppDb)
            v1pure;

private:
    //bool _m_bOneTimeInitAddonMapIfaceGpOk = false;
    //bool _m_bAddonMapIfaceGpOpened = false;

    void _init_AttachToAddonMapIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
