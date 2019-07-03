// CSyncTypeIface.h
#pragma once

//
// CSyncTypeIface - 
//
//   Action history info
//
//ASSUME_IMPL_FOR_IFACE(CSyncTypeImpl, CSyncTypeIface)

class CSyncTypeIface : public typeobject
{
public:
    CSyncTypeIface();
    //NEW_LOCAL(CSyncTypeImpl, CSyncTypeIface)
    NEW_GP(CloudSync, CSyncTypeImpl, CSyncTypeIface)

// Constants

// Attributes

// Operations

// Derived Item Enumeration

    const typelist<CSyncTypeIface> GetDerivedList()
            vhook;

// UI

protected:
    virtual const typelist<CSyncTypeIface> OnGetDerivedList()
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
