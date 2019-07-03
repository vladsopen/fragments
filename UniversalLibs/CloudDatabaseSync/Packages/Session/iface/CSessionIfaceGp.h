// CSessionIfaceGp.h
#pragma once

class CUbroRefineIfaceGp;
class CNotifyOnChangeIfaceGp;
class CUdbInitConnectIfaceGp;
class CFileClassIfaceGp;
class CBasicClassIfaceGp;
class CFilterSystemIfaceGp;
class CLoginIfaceGp;
class CUbroSessionIfaceGp;
class CNetIfaceGp;
class CStoreIfaceGp;
class CAddonMapIfaceGp;

//
// CSessionIfaceGp -
//
//   CloudSync global context.
//
ASSUME_IMPL_FOR_IFACE(CSessionIface, CSessionIfaceGp)
ASSUME_IMPL_FOR_IFACE(CSessionImpl, CSessionIface)

class CSessionIfaceGp : public object
{
public:
    CSessionIfaceGp();
    //NEW_LOCAL(CSessionImpl, CSessionIfaceGp)
    NEW_GP(CloudSync, CSessionImpl, CSessionIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Addon context registrations
    ref<CAddonMapIfaceGp> x_rAddonMapGp
            xabstract;
    ref<CAddonMapIfaceGp> x_rAddonMapGp_Get()
            vhook;

    // Data storage
    ref<CStoreIfaceGp> x_rStoreGp
            xabstract;
    ref<CStoreIfaceGp> x_rStoreGp_Get()
            vhook;

    // Network
    ref<CNetIfaceGp> x_rNetGp
            xabstract;
    ref<CNetIfaceGp> x_rNetGp_Get()
            vhook;

    // Filters
    ref<CFilterSystemIfaceGp> x_rFilterSystemGp
            xabstract;
    ref<CFilterSystemIfaceGp> x_rFilterSystemGp_Get()
            vhook;

    // Basic class
    ref<CBasicClassIfaceGp> x_rSessionBasicClassGp
            xabstract;
    ref<CBasicClassIfaceGp> x_rSessionBasicClassGp_Get()
            vhook;

    // Files
    ref<CFileClassIfaceGp> x_rSessionFileClassGp
            xabstract;
    ref<CFileClassIfaceGp> x_rSessionFileClassGp_Get()
            vhook;

    // Ubro
    ref<CUbroSessionIfaceGp> GetUbroSessionForCloud()
            vhook;

    // Data change callback
    ptr<CNotifyOnChangeIfaceGp> _x_pNotifyOnChange
            xauto(Get, Set);

    // Init ok?
    bool IsSessionOpened()
            vhook;

// Operations

    // Optional pre init
    void PreInitSessionSetFileSyncFolderName(
            str sFileSyncAttach)
            vhook;

    // Open CloudSync context
    void InitSession(
            str sDatabaseChannel,
            str sLocalDatabase,
            ref<CLoginIfaceGp> rLogin,
            ref<CUdbInitConnectIfaceGp> rUdbInitConnect)
            vhook;

    // Close CloudSync context
    void CloseSession()
            vhook;

    // External polling
    void ForcePollSession(
            out bool& out_bHaveJobs)
            vhook;

    // Called from Udb during app table class init.
    //   Created for inserting Filter tables as Nodes at application level
    void HandleInternalStorageDbClassInfoListUpdate(
            ref<CUdbConnectionIfaceGp> rUdbConnection)
            vhook;

// UI

    // Main UI
    ref<UiContainer> NewUiSessionMain(
            ref<CUbroRefineIfaceGp> rUbroRefine)
            vhook;

    // Status bar for all internals and Addons
    ref<UiContainer> NewUiSessionStatusBar(
            ref<CUbroRefineIfaceGp> rUbroRefine)
            vhook;

protected:
    virtual ref<CAddonMapIfaceGp> Onx_rAddonMapGp_Get()
            v1pure;
    virtual ref<CStoreIfaceGp> Onx_rStoreGp_Get()
            v1pure;
    virtual ref<CNetIfaceGp> Onx_rNetGp_Get()
            v1pure;
    virtual ref<CFilterSystemIfaceGp> Onx_rFilterSystemGp_Get()
            v1pure;
    virtual ref<CBasicClassIfaceGp> Onx_rSessionBasicClassGp_Get()
            v1pure;
    virtual ref<CFileClassIfaceGp> Onx_rSessionFileClassGp_Get()
            v1pure;
    virtual ref<CUbroSessionIfaceGp> OnGetUbroSessionForCloud()
            v1pure;
    virtual bool OnIsSessionOpened()
            v1pure;
    virtual void OnPreInitSessionSetFileSyncFolderName(
            str sFileSyncAttach)
            v1pure;
    virtual void OnInitSession(
            str sDatabaseChannel,
            str sLocalDatabase,
            ref<CLoginIfaceGp> rLogin,
            ref<CUdbInitConnectIfaceGp> rUdbInitConnect)
            v1pure;
    virtual void OnCloseSession()
            v1pure;
    virtual void OnForcePollSession(
            out bool& out_bHaveJobs)
            v1pure;
    virtual void OnHandleInternalStorageDbClassInfoListUpdate(
            ref<CUdbConnectionIfaceGp> rUdbConnection)
            v1pure;
    virtual ref<UiContainer> OnNewUiSessionMain(
            ref<CUbroRefineIfaceGp> rUbroRefine)
            v1pure;
    virtual ref<UiContainer> OnNewUiSessionStatusBar(
            ref<CUbroRefineIfaceGp> rUbroRefine)
            v1pure;
private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToSessionIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
