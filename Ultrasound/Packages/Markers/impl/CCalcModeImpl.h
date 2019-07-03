// CCalcModeImpl.h
#pragma once

#include "CCalcModeIface.h"

//
// CCalcModeImpl - see iface
//

class CCalcModeImpl : public CCalcModeIface
{
public:
    CCalcModeImpl();

// Constants

// Attributes

// Operations

// Derived Item Enumeration

    //const typelist<CCalcModeImpl> GetDerivedList();

// UI

protected:

    virtual type<CCalcModeIface> OnFromUsoundCalcMode(
            type<CUsoundCalcModeIfaceGp> typeUsoundCalcMode)
            ;

    virtual const typelist<CCalcModeIface> OnGetDerivedList()
            ;

    virtual type<CCalcModeIface> OnAsCalcModeAutomatic()
            vnull;

    virtual type<CCalcModeIface> OnAsCalcModeManual()
            vnull;

    virtual type<CCalcModeIface> OnAsCalcModeLocked()
            vnull;

private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    virtual void OnTestClass();
};

    // Automatic - by flow
    class CCalcModeAutomatic : public CCalcModeImpl
    {
    public:
        void flag_List_as_CalcModeImpl();

        virtual bool OnIsBaselineMouseEnabled()
                return
                    true;

        virtual type<CUsoundCalcModeIfaceGp> OnConvertToUsoundCalcMode()
                return
                    type<CUsoundCalcModeIfaceGp>()->
                        AsUsoundCalcModeAutomatic();

    protected:
    };

    // Manual - by user
    class CCalcModeManual : public CCalcModeImpl
    {
    public:
        void flag_List_as_CalcModeImpl();

        virtual bool OnIsBaselineMouseEnabled()
                return
                    false;

        virtual type<CUsoundCalcModeIfaceGp> OnConvertToUsoundCalcMode()
                return
                    type<CUsoundCalcModeIfaceGp>()->
                        AsUsoundCalcModeManual();

    protected:
    };

    // Locked - post-manual
    class CCalcModeLocked : public CCalcModeImpl
    {
    public:
        void flag_List_as_CalcModeImpl();

        virtual bool OnIsBaselineMouseEnabled()
                return
                    true;

        virtual type<CUsoundCalcModeIfaceGp> OnConvertToUsoundCalcMode()
                return
                    type<CUsoundCalcModeIfaceGp>()->
                        AsUsoundCalcModeLocked();

    protected:
    };
