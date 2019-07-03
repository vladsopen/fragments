// CUmoveImpl.cpp

#include "CGuiPlusProject.h"
#include "CUmoveImpl.h"
#include "CUmoveAxis.h"
#include "CSliderIfaceGp.h"

CUmoveImpl::CUmoveImpl()
{
}

void CUmoveImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("it may not be safe to expose this GuiPlus class!");
}

point CUmoveImpl::Onx_pointLogicalVehiclePos_Get()
{
    point pointPos = this->x_pointLogicalVehiclePos;
    size sizeZone = x_sizeLogicalUmoveRange;
    rect rectZone(0, 0, sizeZone.w - 1, sizeZone.h - 1);

    if (pointPos.x < rectZone.left)
        pointPos.x = rectZone.left;
    if (pointPos.x > rectZone.right)
        pointPos.x = rectZone.right;
    if (pointPos.y < rectZone.top)
        pointPos.y = rectZone.top;
    if (pointPos.y > rectZone.bottom)
        pointPos.y = rectZone.bottom;

    this->x_pointLogicalVehiclePos = pointPos;

    return pointPos;
}

point CUmoveImpl::OnGetVehiclePosInParentPanel(
        size sizeParentPanel)
{
    // Calc thumb offset in the restricted rail panel range

    size sizeVehicle = x_sizeVehicle;

    point pointResult = point();

    pointResult.x = 
        GetVehiclePosInParentPanelByAxis(
            sizeParentPanel,
            type<CUmoveAxisX>());

    pointResult.y = 
        GetVehiclePosInParentPanelByAxis(
            sizeParentPanel,
            type<CUmoveAxisY>());

    // allow client to shift move area within the parent panel
    pointResult.x += x_sizeOffsetInParentPanel.Get().w;
    pointResult.y += x_sizeOffsetInParentPanel.Get().h;

    return pointResult;
}

int CUmoveImpl::GetVehiclePosInParentPanelByAxis(
        size sizeParentPanel,
        type<CUmoveAxis> typeUmoveAxis)
{
    int nRailLength = 
        typeUmoveAxis->GetAxisSize(sizeParentPanel) 
            - 
            typeUmoveAxis->GetAxisSize(x_sizeVehicle);

    int nLogicalRange = 
        typeUmoveAxis->
            GetAxisSize(
                x_sizeLogicalUmoveRange);

    int iZeroBasedUmovePos = 
        typeUmoveAxis->
            GetAxisPoint(
                x_pointLogicalVehiclePos);

    int iVehiclePos = 0;
    if (nLogicalRange > 1)
    {
        // We don't want to lose +/- 0.5 precision with integer DIV
        num numVehiclePos = 
            math::GMulDiv<num>(
                iZeroBasedUmovePos, 
                nRailLength - 1, 
                nLogicalRange - 1);

        iVehiclePos = 
            math::GRound(numVehiclePos);

        // exact position at the end of range
        if (iZeroBasedUmovePos == nLogicalRange - 1)
        {
            iVehiclePos = nRailLength;
        }
    }

    return iVehiclePos;
}

bool CUmoveImpl::OnHandleVehicleOnMouse(
        size sizeParentPanel,
        ref<CUiPanelGp> rUiVehicle,
        EMouse eMouse)
{
    if (!rUiVehicle->IsEnabled())
    {
        return false;
    }

    // We usually drag with Left button
    bool bDrag = sys::GIsAccess(...);

    // Get vehicle size and pos
    size sizeVehicle = x_sizeVehicle;
    point pointCurrentVehicle = 
        //rUiVehicle->GetInstantPos();
        GetVehiclePosInParentPanel(
            sizeParentPanel);

    // allow client to shift move area within the parent panel
    pointCurrentVehicle.x -= x_sizeOffsetInParentPanel.Get().w;
    pointCurrentVehicle.y -= x_sizeOffsetInParentPanel.Get().h;

    // trace grag
    point pointDraggedTo = point();
    if (HookMouseDrag(
            rUiVehicle, // rUiToSetCapture
            eMouse, // mouse event to handle
            //sys::GGetMousePosAsynchronously(), // global mouse position
            sys::GGetMousePosForCurrentMessage(), // global mouse position
            E_Mouse_LeftButtonDown, // starts drag
            E_Mouse_LeftButtonUp, // stops drag
            bDrag, // dragging?
            pointCurrentVehicle, // current position
            out pointDraggedTo)) // desired drag target
    {
        //sys::GDisplayDebug(100, 200, Str(GetOnMouseEvent()));

        point pointNewLogicalPos = point();

        pointNewLogicalPos.x = 
            CalcNewLogicalPosByAxis(
                sizeParentPanel,
                pointDraggedTo,
                type<CUmoveAxisX>());

        pointNewLogicalPos.y = 
            CalcNewLogicalPosByAxis(
                sizeParentPanel,
                pointDraggedTo,
                type<CUmoveAxisY>());

        if (x_pointLogicalVehiclePos != pointNewLogicalPos)
        {
            debug("Umove./show.Moved:").
                FloatBlink(
                    "Delta " + 
                        Str(pointNewLogicalPos.x - x_pointLogicalVehiclePos.Get().x) + 
                        " x " + 
                        Str(pointNewLogicalPos.y - x_pointLogicalVehiclePos.Get().y) +
                        " to (" +
                        Str(pointNewLogicalPos.x) + 
                        " x " + 
                        Str(pointNewLogicalPos.y) + 
                        ")");

            x_pointLogicalVehiclePos = pointNewLogicalPos;

            return true; // moved!
        }
    }

    return false; // not moved
}

int CUmoveImpl::CalcNewLogicalPosByAxis(
        size sizeParentPanel,
        point pointDraggedTo,
        type<CUmoveAxis> typeUmoveAxis)
{
    // by default return current pos
    int iNewLogicalPos = 
        typeUmoveAxis->
            GetAxisPoint(
                x_pointLogicalVehiclePos);

    // Logical range
    int nLogicalRange = 
        typeUmoveAxis->
            GetAxisSize(
                x_sizeLogicalUmoveRange);
    if (nLogicalRange <= 0)
    {
        return iNewLogicalPos;
    }

    // Pixel range
    int nRailLength = 
        typeUmoveAxis->GetAxisSize(sizeParentPanel) 
            - 
            typeUmoveAxis->GetAxisSize(x_sizeVehicle);
    if (nRailLength <= 1)
    {
        return iNewLogicalPos;
    }

    // new pixel destination
    int iDraggedTo = 
        typeUmoveAxis->
            GetAxisPoint(
                pointDraggedTo);

    // We don't want to lose +/- 0.5 precision with integer DIV
    num numNewLogicalPos = 
        math::GMulDiv<num>(
            iDraggedTo, 
            nLogicalRange - 1, 
            nRailLength - 1);

    iNewLogicalPos = 
        math::GRound(numNewLogicalPos);

    /*
    // round may cause bogus +/- 1 change,
    //   so if we have NOT moved then do not change the pos
    if (pointDraggedTo.x == pointCurrentVehicle.x)
    {
        rASSERT(iCurrentZeroBasedUmovePos - iNewLogicalPos <= 1);
        iNewUmovePos = iCurrentZeroBasedUmovePos;
    }
    */

    return iNewLogicalPos;
}

res CUmoveImpl::OnGetUmoveCursorForVehicle()
{
    size sizeZone = x_sizeLogicalUmoveRange;

    if (sizeZone.w > 0 &&
        sizeZone.h > 0)
    {
        return IDC_STD_SIZEALL;
    }

    if (sizeZone.w > 0)
    {
        return IDC_STD_SIZEWE;
    }

    if (sizeZone.h > 0)
    {
        return IDC_STD_SIZENS;
    }

    return null();
}

res CUmoveImpl::OnGetUmoveCursorForParentRailPanel()
{
    FAIL("not implemented yet");

    return null();
}

bool CUmoveImpl::OnHookMouseDrag(
        ref<CUiPanelGp> rUiToSetCapture,
        EMouse eMouse,
        point pointMouseOnScreen,
        EMouse eMouseStartDrag,
        EMouse eMouseStopDrag,
        bool bMouseMoveDragCondition,
        point pointInstantPos,
        out point& out_pointDraggedTo)
{
    ASSERT(eMouseStartDrag != eMouseStopDrag);

    // No drag by default
    out_pointDraggedTo = pointInstantPos;

    // Decide what to do
    bool bContinueDrag = false;
    bool bStartDrag = (eMouse == eMouseStartDrag);
    bool bStopDrag = (eMouse == eMouseStopDrag);

    if (eMouse == E_Mouse_Move)
    {
        if (bMouseMoveDragCondition)
        {
            bContinueDrag = true;
            ASSERT(!bStopDrag);
        }
    }

    // 
    // Perform the chosen actions
    //

    if (bStartDrag)
    {
        ASSERT(!bStopDrag);
        s_pointScreenStart = pointMouseOnScreen;
        s_pointLocalStart = pointInstantPos;
        rUiToSetCapture->SetMouseInputCapture(true);
        s_pUiCapture = rUiToSetCapture;
    }

    if (bStopDrag)
    {
        ASSERT(!bStartDrag);
        rUiToSetCapture->SetMouseInputCapture(false);
        s_pUiCapture = null();
        s_pointScreenStart = point(off, off);
        s_pointLocalStart = point(off, off);
    }

    if (bContinueDrag)
    {
        ASSERT(!bStopDrag);

        // Did we receive LeftButtonDown message and ready to drag?
[...]