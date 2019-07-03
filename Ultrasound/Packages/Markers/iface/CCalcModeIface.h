// CCalcModeIface.h
#pragma once

class CUsoundCalcModeIfaceGp;

//
// CCalcModeIface -
//
//   Calc modes in Uscan project context.
//     Deeply related of course to CUsoundCalcModeIfaceGp.
//
ASSUME_IMPL_FOR_IFACE(CCalcModeImpl, CCalcModeIface)

class CCalcModeIface : public typeobject
{
public:
    CCalcModeIface();
    NEW_LOCAL(CCalcModeImpl, CCalcModeIface)
    //NEW_GP(MyProject, CCalcModeImpl, CCalcModeIface)

// Constants

// Attributes

    // Disable baseline move in calc mode
    bool IsBaselineMouseEnabled()
            vhook;

// Operations

    // Convert from shared Ducalc type
    type<CCalcModeIface> FromUsoundCalcMode(
            type<CUsoundCalcModeIfaceGp> typeUsoundCalcMode)
            vhook;

    // Convert to shared Ducalc type
    type<CUsoundCalcModeIfaceGp> ConvertToUsoundCalcMode()
            vhook;

// Derived Item Enumeration

    const typelist<CCalcModeIface> GetDerivedList()
            vhook;

    // Automatic: manual mode is OFF and we are free to recalculate by flow
    type<CCalcModeIface> AsCalcModeAutomatic()
            vhook;

    // Manual: manual mode UI is ON we use CalcMaking as the index source
    type<CCalcModeIface> AsCalcModeManual()
            vhook;

    // Locked: manual mode UI is OFF but we STILL use CalcMaking as the index source
    type<CCalcModeIface> AsCalcModeLocked()
            vhook;

// UI

protected:

    virtual bool OnIsBaselineMouseEnabled()
            v1pure;

    virtual type<CCalcModeIface> OnFromUsoundCalcMode(
            type<CUsoundCalcModeIfaceGp> typeUsoundCalcMode)
            v1pure;

    virtual type<CUsoundCalcModeIfaceGp> OnConvertToUsoundCalcMode()
            v1pure;

    virtual const typelist<CCalcModeIface> OnGetDerivedList()
            v1pure;

    virtual type<CCalcModeIface> OnAsCalcModeAutomatic()
            v1pure;

    virtual type<CCalcModeIface> OnAsCalcModeManual()
            v1pure;

    virtual type<CCalcModeIface> OnAsCalcModeLocked()
            v1pure;

private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

