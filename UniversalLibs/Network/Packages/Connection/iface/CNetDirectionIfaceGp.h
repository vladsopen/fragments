// CNetDirectionIfaceGp.h
#pragma once

//
// CNetDirectionIfaceGp - 
//
//   In- or Out- connection direction.
//
//ASSUME_IMPL_FOR_IFACE(CNetDirectionImpl, CNetDirectionIfaceGp)

class CNetDirectionIfaceGp : public typeobject
{
public:
    CNetDirectionIfaceGp();
    //NEW_LOCAL(CNetDirectionImpl, CNetDirectionIfaceGp)
    NEW_GP(Net, CNetDirectionImpl, CNetDirectionIfaceGp)

// Constants

// Attributes

    // "In" or "Out"
    str GetNetDirectionName()
            vhook;

    // Locator options relevance (UI cleanup)
    bool IsChaseLatestAppliedToDirection()
            vhook;
    bool IsSingleReceiverClearsServerAppliedToDirection()
            vhook;

// Operations

// Derived Item Enumeration

    const typelist<CNetDirectionIfaceGp> GetDerivedList()
            vhook;

    type<CNetDirectionIfaceGp> AsNetDirectionOut()
            vhook;
    type<CNetDirectionIfaceGp> AsNetDirectionIn()
            vhook;

// UI

protected:
    virtual str OnGetNetDirectionName()
            v1pure;
    virtual bool OnIsChaseLatestAppliedToDirection()
            v1pure;
    virtual bool OnIsSingleReceiverClearsServerAppliedToDirection()
            v1pure;
    virtual const typelist<CNetDirectionIfaceGp> OnGetDerivedList()
            v1pure;
    virtual type<CNetDirectionIfaceGp> OnAsNetDirectionOut()
            v1pure;
    virtual type<CNetDirectionIfaceGp> OnAsNetDirectionIn()
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
