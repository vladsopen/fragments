// CAddonSessionIfaceGp.h
#pragma once

class CAddonMapIfaceGp;
class CNetIfaceGp;
class CStoreIfaceGp;
class CActionIfaceGp;
class CUdbConnectionIfaceGp;
class CSessionIfaceGp;

//
// CAddonSessionIfaceGp -
//
//   Abstract app-defined Add-on callback created and closed with a Session.
//

class CAddonSessionIfaceGp : public object
{
public:
    CAddonSessionIfaceGp();
    ~CAddonSessionIfaceGp();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CSessionIfaceGp> _x_pCoreSession
            xauto(Get, Set);
    ref<CSessionIfaceGp> GetCoreSession()
            return
                _x_pCoreSession.Get().Valid("no Session for addon");

    // store
    ref<CStoreIfaceGp> GetCoreStore()
            return
                GetCoreSession()->x_rStoreGp;

    // db
    ref<CUdbConnectionIfaceGp> GetCoreUdbConnection()
            return
                GetCoreStore()->GetStoreLowLevelUdbConnection();

    // net
    ref<CNetIfaceGp> GetCoreNet()
            return
                GetCoreSession()->x_rNetGp;

    // addon map
    ref<CAddonMapIfaceGp> GetCoreAddonMap()
            return
                GetCoreSession()->x_rAddonMapGp;

// Operations

    // One-time opener
    void InitAddonSessionBeforeCloud()
            vhooked;
    void InitAddonSessionAfterCloud()
            vhook;

    // Cleanup
    void CloseAddonSession()
            vhooked;

    //
    // Hook Handlers
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

    // Status bar addition
    ref<UiContainer> NewUiAddonStatusBarBottomRow()
            vhook;

protected:
    virtual void OnInitAddonSessionBeforeCloud()
            v1pure;
    virtual void OnInitAddonSessionAfterCloud()
            v1pure;
    virtual void OnCloseAddonSession()
            v1pure;
    virtual void OnHandleDbClassInfoListUpdate(
            ref<CUdbConnectionIfaceGp> rUdbConnection)
            v1null;
    virtual void OnHandleStoreReady(
            ref<CUdbConnectionIfaceGp> rUdbConnection)
            v1null;
    virtual void OnHandleMapRegistrationsComplete()
            v1null;
    virtual void OnHandleActionInterpreter(
            ref<CActionIfaceGp> rAction)
            v1null;
    virtual ref<UiContainer> OnNewUiAddonStatusBarBottomRow()
            v1null;
private:
    bool _m_bOneTimeInitAddonSessionBeforeIfaceGpOk = false;
    bool _m_bAddonSessionIfaceGpOpened = false;

    void _init_AttachToAddonSessionIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
