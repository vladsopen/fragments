// CEyeModeIface.h
#pragma once

class CRecordTypeIface;

//
// CEyeModeIface -
//
//   Do we record for open, closed eyes or both in Classic
//
ASSUME_IMPL_FOR_IFACE(CEyeModeImpl, CEyeModeIface)

class CEyeModeIface : public typeobject
{
public:
    CEyeModeIface();
    NEW_LOCAL(CEyeModeImpl, CEyeModeIface)
    //NEW_GP(Balance, CEyeModeImpl, CEyeModeIface)

// Constants

// Attributes

    // Short description
    sloc GetEyeModeNameForUi()
            vhook;

    // Long description
    sloc GetEyeModeTip()
            vhook;

    // Link eye modes and record types
    bool IsRecordTypeForEyeMode(
            type<CRecordTypeIface> typeRecordType)
            vhook;

    // Internal?
    bool IsEyeModeAvailableForEndUser()
            vhook;

// Operations

// Derived Item Enumeration

    const typelist<CEyeModeIface> GetDerivedList()
            vhook;

    type<CEyeModeIface> AsEyeModeForOpenAndClosed()
            vhook;
    type<CEyeModeIface> AsEyeModeForOpenOnly()
            vhook;
    type<CEyeModeIface> AsEyeModeForNeutral()
            vhook;

// UI

protected:

    virtual sloc OnGetEyeModeNameForUi()
            v1pure;

    virtual sloc OnGetEyeModeTip()
            v1pure;

    virtual bool OnIsRecordTypeForEyeMode(
            type<CRecordTypeIface> typeRecordType)
            v1pure;

    virtual bool OnIsEyeModeAvailableForEndUser()
            v1st
            return
                true;

    virtual const typelist<CEyeModeIface> OnGetDerivedList()
            v1pure;
    virtual type<CEyeModeIface> OnAsEyeModeForOpenAndClosed()
            v1pure;
    virtual type<CEyeModeIface> OnAsEyeModeForOpenOnly()
            v1pure;
    virtual type<CEyeModeIface> OnAsEyeModeForNeutral()
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
