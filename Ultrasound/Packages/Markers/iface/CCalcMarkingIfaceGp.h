// CCalcMarkingIfaceGp.h
#pragma once

#include "CCalcMarkingIfaceGp.h"

class CUsoundCalcModeIfaceGp;
class CCalcupSystemIfaceGp;

//
// CCalcMarkingIfaceGp -
//
//   Manual calculation mode for USB Doc.
//
ASSUME_IMPL_FOR_IFACE(CCalcMarkingImpl, CCalcMarkingIfaceGp)

class CCalcMarkingIfaceGp : public CCalcMarkingIfaceGp
{
public:
    CCalcMarkingIfaceGp();
    //NEW_LOCAL(CCalcMarkingImpl, CCalcMarkingIfaceGp)
    NEW_GP(Usmain, CCalcMarkingImpl, CCalcMarkingIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // see iface
    //// Backref to parent
    //ptr<CCalcupSystemIface> _x_pParentCalcupSystem
    //        xauto(Get, Set);

// Operations

    // Update after mode change
    void PrepareCalcMarkingForCalcModeWuu(
            type<CUsoundCalcModeIfaceGp> typeUsoundCalcMode)
            vhook;

// UI

    // Overlay over media image
    ref<UiContainer> NewUiCalcMarkingMainOverlay()
            vhook;

protected:

    virtual ref<UiContainer> OnNewUiCalcMarkingMainOverlay()
            v1pure;

    virtual void OnPrepareCalcMarkingForCalcModeWuu(
            type<CUsoundCalcModeIfaceGp> typeUsoundCalcMode)
            v1pure;

private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
