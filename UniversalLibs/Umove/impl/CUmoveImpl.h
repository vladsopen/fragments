// CUmoveImpl.h
#pragma once

#include "CUmoveIfaceGp.h"

class CUmoveAxis;

//
// CUmoveImpl - CUmoveIfaceGp implementation
//

class CUmoveImpl : public CUmoveIfaceGp
{
public:
    CUmoveImpl();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

// Operations

// UI

protected:
    // (see CUmoveIfaceGp)
    virtual res OnGetUmoveCursorForVehicle()
            ;
    virtual res OnGetUmoveCursorForParentRailPanel()
            ;
    virtual point Onx_pointLogicalVehiclePos_Get()
            ;
    virtual point OnGetVehiclePosInParentPanel(
            size sizeParentPanel)
            ;
    virtual bool OnHandleVehicleOnMouse(
            size sizeParentPanel,
            ref<CUiPanelGp> rUiVehicle,
            EMouse eMouse)
            ;
    virtual bool OnHookMouseDrag(
            ref<CUiPanelGp> rUiToSetCapture,
            EMouse eMouse,
            point pointMouseOnScreen,
            EMouse eMouseStartDrag,
            EMouse eMouseStopDrag,
            bool bMouseMoveDragCondition,
            point pointInstantPos,
            out point& out_pointDraggedTo)
            ;

private:

    int GetVehiclePosInParentPanelByAxis(
            size sizeParentPanel,
            type<CUmoveAxis> typeUmoveAxis);

    int CalcNewLogicalPosByAxis(
            size sizeParentPanel,
            point pointDraggedTo,
            type<CUmoveAxis> typeUmoveAxis);

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

