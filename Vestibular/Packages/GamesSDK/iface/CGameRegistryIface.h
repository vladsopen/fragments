// CGameRegistryIface.h
#pragma once

#include "CGameRegistryIfaceGp.h"

class CCallGameIfaceGp;
class CGameTypeIface;

//
// CGameRegistryIface - see ifacegp
//

class CGameRegistryIface : public CGameRegistryIfaceGp
{
public:
    CGameRegistryIface();
    NEW_LOCAL(CGameRegistryImpl, CGameRegistryIface)
    //NEW_GP(MyProject, CGameRegistryImpl, CGameRegistryIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

    // Find a game instantiator
    ref<CCallGameIfaceGp> NewCallGameFromFactories(
            type<CGameTypeIface> typeGameType)
            vhook;

    // For auto-sterting a command-line selected game
    void HandleAppMainWindowInitComplete()
            vhook;

// UI

protected:
    virtual ref<CCallGameIfaceGp> OnNewCallGameFromFactories(
            type<CGameTypeIface> typeGameType)
            v1pure;
    virtual void OnHandleAppMainWindowInitComplete()
            v1pure;
private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToGameRegistryIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
