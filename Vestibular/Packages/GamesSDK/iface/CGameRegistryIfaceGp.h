// CGameRegistryIfaceGp.h
#pragma once

class CGameTypeIfaceGp;
class CGameFactoryIfaceGp;

//
// CGameRegistryIfaceGp -
//
//   External game modules register themselves via this service.
//
ASSUME_IMPL_FOR_IFACE(CGameRegistryIface, CGameRegistryIfaceGp)
ASSUME_IMPL_FOR_IFACE(CGameRegistryImpl, CGameRegistryIface)

class CGameRegistryIfaceGp : public object
{
public:
    CGameRegistryIfaceGp();
    //NEW_LOCAL(CGameRegistryImpl, CGameRegistryIfaceGp)
    NEW_GP(Balance, CGameRegistryImpl, CGameRegistryIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

// Operations

    // This is called only once per module to let Balance
    //   know that an external Game factory exists.
    //   Balance will save the ref<>.
    void AddGlobalGameFactory(
            ref<CGameFactoryIfaceGp> rGameFactory)
            vhook;

    // This switches Balance into servicing a Game sampler instead of its
    //   primary checkup functions.
    //   typeForceSingleGameType can be null()
    void SetToGameMode(
            bool bGame,
            type<CGameTypeIfaceGp> typeForceSingleGameType)
            vhook;

    // Until we have a dedicated device controller as IfageGp
    //   this essentially global flag can live here.
    void EnableKeyboardSimulation(
            bool bSimulate)
            vhook;

// UI

    // Balance UI for game control
    ref<UiContainer> NewUiMainForGames()
            vhook;

    // Balance games UI for online/offline mode
    ref<UiContainer> NewUiGamesForOffline()
            vhook;


protected:
    virtual void OnAddGlobalGameFactory(
            ref<CGameFactoryIfaceGp> rGameFactory)
            v1pure;
    virtual void OnSetToGameMode(
            bool bGame,
            type<CGameTypeIfaceGp> typeForceSingleGameType)
            v1pure;
    virtual void OnEnableKeyboardSimulation(
            bool bSimulate)
            v1pure;
    virtual ref<UiContainer> OnNewUiMainForGames()
            v1pure;

    virtual ref<UiContainer> OnNewUiGamesForOffline()
            v1pure;

private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToGameRegistryIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
