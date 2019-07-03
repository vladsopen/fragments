// CCalcMarkingIface.h
#pragma once

#include "CCalcMarkingIfaceGp.h"

class CCalcupSystemIface;

//
// CCalcMarkingIface - see ifacegp
//
//ASSUME_IMPL_FOR_IFACE(CCalcMarkingImpl, CCalcMarkingIface)

class CCalcMarkingIface : public CCalcMarkingIfaceGp
{
public:
    CCalcMarkingIface();
    NEW_LOCAL(CCalcMarkingImpl, CCalcMarkingIface)
    //NEW_GP(Usmain, CCalcMarkingImpl, CCalcMarkingIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Backref to parent
    ptr<CCalcupSystemIface> _x_pParentCalcupSystem
            xauto(Get, Set);

// Operations

// UI

protected:
private:
    //bool _m_bOneTimeInitCalcMarkingIfaceOk = false;

    void _init_AttachToCalcMarkingIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
