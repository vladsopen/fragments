// CUmoveIfaceGp.h
#pragma once

//
// CUmoveIfaceGp -
//
//   Helps you drag stuff around.
//     Moving markers, scrollbar/slider thumbs,
//     or even MDI-like child windows.
//
//   Umove does not create its own UI. It gives you maximum flexibility
//     in its use.
//

class CUmoveIfaceGp : public object
{
public:
    CUmoveIfaceGp();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);
    //NEW_LOCAL(CUmoveImpl, CUmoveIfaceGp)
    NEW_GP(GuiPlus, CUmoveImpl, CUmoveIfaceGp)

// Constants

// Attributes

    // Current Umove position in logical application units
    //   Auto-limited within x_sizeLogicalUmoveRange
    point x_pointLogicalVehiclePos = point(0, 0)
            xauto(Set)
            xassert(true);
    point x_pointLogicalVehiclePos_Get()
            vhook;

    // Current Umove max positions in logical application units
    size x_sizeLogicalUmoveRange = size()
            xauto(Get, Set)
            xassert(true);

    // The thing we move (in a slider/scrollbar this is called "thumb")
    size x_sizeVehicle = size(1, 1)
            xauto(Get, Set)
            xassert(true);

    // Optional offset in parent panel
    size x_sizeOffsetInParentPanel = size(0, 0)
            xauto(Get, Set)
            xassert(true);

    // Recommended mouse pointer showing possible directions
    //   (arrow direction depends on x_sizeLogicalUmoveRange)
    res GetUmoveCursorForVehicle()
            vhook;

    // Recommended mouse pointer for slider rail panel
    res GetUmoveCursorForParentRailPanel()
            vhook;

// Operations

    // The pixel position for Vehicle panel OnModifyShape handler
    point GetVehiclePosInParentPanel(
            size sizeParentPanel)
            vhook;

    // Vehicle panel OnMouse handler returns true if
    //   x_pointLogicalVehiclePos is chaned and the app needs to invalidate
    //   Vehicle parent layout
    //   (DO NOT INVALIDATE CONTENT while dragging is in progress!)
    // rUiVehicle is used to set mouse capture and get actual Vehicle pos
    bool HandleVehicleOnMouse(
            size sizeParentPanel,
            ref<CUiPanelGp> rUiVehicle,
            EMouse eMouse)
            vhook;

    // Low level drag helper
    //   This drag procedure was designed to be used by the Slider internally
    //   but in the future I may want to employ it for some other similar tasks
    bool HookMouseDrag(
            ref<CUiPanelGp> rUiToSetCapture,
            EMouse eMouse,
            point pointMouseOnScreen,
            EMouse eMouseStartDrag,
            EMouse eMouseStopDrag,
            bool bMouseMoveDragCondition,
            point pointInstantPos,
            out point& out_pointDraggedTo)
            vhook;

// Advanced Service

// UI

protected:
    // (see CUmoveImpl)
    virtual res OnGetUmoveCursorForVehicle()
            v1pure;
    virtual res OnGetUmoveCursorForParentRailPanel()
            v1pure;
    virtual point Onx_pointLogicalVehiclePos_Get()
            v1pure;
    virtual point OnGetVehiclePosInParentPanel(
            size sizeParentPanel)
            v1pure;
    virtual bool OnHandleVehicleOnMouse(
            size sizeParentPanel,
            ref<CUiPanelGp> rUiVehicle,
            EMouse eMouse)
            v1pure;
    virtual bool OnHookMouseDrag(
            ref<CUiPanelGp> rUiToSetCapture,
            EMouse eMouse,
            point pointMouseOnScreen,
            EMouse eMouseStartDrag,
            EMouse eMouseStopDrag,
            bool bMouseMoveDragCondition,
            point pointInstantPos,
            out point& out_pointDraggedTo)
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
