// CDizzyPaintTypeIface.h
#pragma once

//
// CDizzyPaintTypeIface -
//
//   The image type to be rendened to dizzyfy the patient.
//
ASSUME_IMPL_FOR_IFACE(CDizzyPaintTypeImpl, CDizzyPaintTypeIface)

class CDizzyPaintTypeIface : public typeobject
{
public:
    CDizzyPaintTypeIface();
    NEW_LOCAL(CDizzyPaintTypeImpl, CDizzyPaintTypeIface)
    //NEW_GP(Balance, CDizzyPaintTypeImpl, CDizzyPaintTypeIface)

// Constants

// Attributes

    // Image type
    bool IsStripesPainted()
            vhook;
    bool IsFocusDotPainted()
            vhook;

    // (x|y) -1 | 0 | +1
    int GetStripeDirectionHorizontal()
            vhook;
    int GetStripeDirectionVertical()
            vhook;

    // Report to UI
    sloc GetPaintTypeAsString()
            vhook;

    // Large icon
    res GetPaintTypeIcon()
            vhook;

// Operations

// Derived Item Enumeration

    const typelist<CDizzyPaintTypeIface> GetDerivedList()
            vhook;

// UI

protected:
    virtual bool OnIsStripesPainted()
            v1st
            return
                false;

    virtual bool OnIsFocusDotPainted()
            v1st
            return
                false;

    virtual int OnGetStripeDirectionHorizontal()
            v1st
            return
                0;

    virtual int OnGetStripeDirectionVertical()
            v1st
            return
                0;

    virtual sloc OnGetPaintTypeAsString()
            v1st
            return
                "";

    virtual res OnGetPaintTypeIcon()
            v1st
            return
                res();

    virtual const typelist<CDizzyPaintTypeIface> OnGetDerivedList()
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
