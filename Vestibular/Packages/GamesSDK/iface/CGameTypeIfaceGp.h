// CGameTypeIfaceGp.h
#pragma once

#include "CModusTypeIfaceGp.h"

class CCallGameIfaceGp;

//
// CGameTypeIfaceGp -
//
//   SP Game definitions.
//
ASSUME_IMPL_FOR_IFACE(CGameTypeIface, CGameTypeIfaceGp)
ASSUME_IMPL_FOR_IFACE(CGameTypeImpl, CGameTypeIface)

class CGameTypeIfaceGp : public CModusTypeIfaceGp
{
public:
    CGameTypeIfaceGp();
    //NEW_LOCAL(CGameTypeImpl, CGameTypeIfaceGp)
    NEW_GP(Balance, CGameTypeImpl, CGameTypeIfaceGp)

// Constants

// Attributes

    // Internal key for Patients
    str GetGameKey()
            vhooked;

    // Name
    sloc GetShortGameNameForUi()
            vhook;

    // Icon
    res GetLargeGameIcon()
            vhook;

    // Is really implemented?
    bool IsGamePublic()
            vhook;

    // Impl
    ref<CCallGameIfaceGp> NewCallGame()
            vhook;

// Operations

// Derived Item Enumeration

    const typelist<CGameTypeIfaceGp> GetDerivedList()
            vhook;

// UI

protected:
    virtual str OnGetGameKey()
            v1pure;
    virtual sloc OnGetShortGameNameForUi()
            v1pure;
    virtual res OnGetLargeGameIcon()
            v1pure;
    virtual bool OnIsGamePublic()
            v1st
            return
                true;
    virtual ref<CCallGameIfaceGp> OnNewCallGame()
            v1pure;
    virtual const typelist<CGameTypeIfaceGp> OnGetDerivedList()
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
