// CAxisTypeIfaceGp.h
#pragma once

//
// CAxisTypeIfaceGp - 
//
//   X or Y
//
ASSUME_IMPL_FOR_IFACE(CAxisTypeIface, CAxisTypeIfaceGp)
ASSUME_IMPL_FOR_IFACE(CAxisTypeImpl, CAxisTypeIface)

class CAxisTypeIfaceGp : public typeobject
{
public:
    CAxisTypeIfaceGp();
    //NEW_LOCAL(CAxisTypeImpl, CAxisTypeIfaceGp)
    NEW_GP(Balance, CAxisTypeImpl, CAxisTypeIfaceGp)

// Constants

// Attributes

// Operations

// Derived Item Enumeration

    const typelist<CAxisTypeIfaceGp> GetDerivedList()
            vhook;

    type<CAxisTypeIfaceGp> AsAxisTypeX()
            vhook;

    type<CAxisTypeIfaceGp> AsAxisTypeY()
            vhook;

// UI

protected:
    virtual const typelist<CAxisTypeIfaceGp> OnGetDerivedList()
            v1pure;
    virtual type<CAxisTypeIfaceGp> OnAsAxisTypeX()
            v1pure;
    virtual type<CAxisTypeIfaceGp> OnAsAxisTypeY()
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
