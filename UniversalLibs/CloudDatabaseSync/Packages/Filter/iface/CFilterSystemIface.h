// CFilterSystemIface.h
#pragma once

#include "CFilterSystemIfaceGp.h"

class CSessionIface;
class CUdbConnectionIfaceGp;

//
// CFilterSystemIface - see CFilterSystemIfaceGp
//
//ASSUME_IMPL_FOR_IFACE(CFilterSystemImpl, CFilterSystemIface)

class CFilterSystemIface : public CFilterSystemIfaceGp
{
public:
    CFilterSystemIface();
    //~CFilterSystemIface();
    NEW_LOCAL(CFilterSystemImpl, CFilterSystemIface)
    //NEW_GP(CloudSync, CFilterSystemImpl, CFilterSystemIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CSessionIface> _x_pSession
            xauto(Get, Set);
    ref<CSessionIface> GetSession()
            return
                _x_pSession.Get().Valid();

    // debug
    bool x_bDisableFilterSystemForTest = false
            xauto(Get, Set);

// Operations

    // One-time opener
    void InitFilterSystemAfterAllModulesReady()
            vhook;

    // Cleanup
    void CloseFilterSystem()
            vhook;

    // A hook for adding our class into DBs
    void HandleApplicationLevelDbClassInfoListUpdate(
            ref<CUdbConnectionIfaceGp> rUdbConnection)
            vhook;

// UI

    // Small (1-button?) insertion opening filter menu
    ref<UiContainer> NewUiFilterSystemBar()
            vhook;

protected:
    SEE_ALSO(CFilterSystemImpl) // F12-lookup

    virtual void OnInitFilterSystemAfterAllModulesReady()
            v1pure;
    virtual void OnCloseFilterSystem()
            v1pure;
    virtual void OnHandleApplicationLevelDbClassInfoListUpdate(
            ref<CUdbConnectionIfaceGp> rUdbConnection)
            v1pure;
    virtual ref<UiContainer> OnNewUiFilterSystemBar()
            v1pure;

private:
    //bool _m_bOneTimeInitFilterSystemIfaceOk = false;
    //bool _m_bFilterSystemIfaceOpened = false;

    void _init_AttachToFilterSystemIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
