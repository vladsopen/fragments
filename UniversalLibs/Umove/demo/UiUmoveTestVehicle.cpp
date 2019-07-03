// UiUmoveTestVehicle.cpp

#include "CProject.h"
#include "CDemoVehicle.h"
#include "CUmoveIfaceGp.h"
#include "CUmoveTest.h"

void UiUmoveTestVehicle::Ui()
{
    ref<CUmoveIfaceGp> rUmove = 
        m_rDemoVehicle->
            x_rUmove;

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rDemoVehicle->_m_ptrmapUiUmoveTestVehicle += rUi;

        if (m_rDemoVehicle->x_bUnmovable)
        {
            rUi->SetDisableInfo(
                "Disabled Vehicle cannot be moved. "
                    "Also the demo does not allow another vehicle box at this "
                    "position to show you how to restrict movements.");
        }

        rUi->SetIndependentShape(true);

        rUi->OnIndependentShape = [=]
        {
            //TODO: you must ask Umove the Vehicle position
            point pointVehicle = 
                rUmove->
                    GetVehiclePosInParentPanel(
                        m_rUmoveTest->x_sizeUmoveVehicleParent);

            rUi->SetInstantPos(pointVehicle.x, pointVehicle.y);

            //TODO: you must make sure you told Umove the correct Vehicle size
            size sizeVehicle = rUmove->x_sizeVehicle;

            // horizontal marker logic
            if (m_rDemoVehicle->x_bStretchWidthAsForHorizontalMarker)
            {
                sizeVehicle.w = m_rUmoveTest->x_sizeUmoveVehicleParent.Get().w;
                rUmove->x_sizeVehicle = sizeVehicle;
            }

            // vertical marker logic
            if (m_rDemoVehicle->x_bStretchHeightAsForVerticalMarker)
            {
                sizeVehicle.h = m_rUmoveTest->x_sizeUmoveVehicleParent.Get().h;
                rUmove->x_sizeVehicle = sizeVehicle;
            }

            rUi->SetInstantSize(sizeVehicle.w, sizeVehicle.h);
        }

        rUi->OnMouse = [=]
        {
            // save prev pos 
            point pointOldPos = 
                rUmove->
                    x_pointLogicalVehiclePos;

            //TODO: you must allow Umove hook mouse drag and drop
            bool bMoved = 
                rUmove->
                    HandleVehicleOnMouse(
                        m_rUmoveTest->x_sizeUmoveVehicleParent,
                        rUi,
                        GetOnMouseEvent());

            if (bMoved)
            {
                // here we can restrict new position
                point pointNewPos = 
                    rUmove->
                        x_pointLogicalVehiclePos;

                if (pointNewPos.x == m_rDemoVehicle->x_pointForbiddenPlace.Get().x &&
                    pointNewPos.y == m_rDemoVehicle->x_pointForbiddenPlace.Get().y)
                {
                    // force previous position
                    rUmove->
                        x_pointLogicalVehiclePos = 
                            pointOldPos;
                }

                // we've got a new position. OnModifyShape will take care
                rUi->InvalidateLayout();
                m_rUmoveTest->UpdateForNewVehiclePositions();

                // DO NOT INVALIDATE CONTENT HERE you'll break mouse capture!
            }
        }
    }

    //
    // The content which is being Moved
    //
    {
        ref<CUiButtonGp> rUi;

        // stretch to Vehicle panel independent shape
        rUi->SetMaxX(oo);
        rUi->SetMaxY(oo);

        //TODO: if you don't want to invent the wheel ask Umove cursor
        rUi->SetCursor(rUmove->GetUmoveCursorForVehicle());

        if (m_rDemoVehicle->x_bUnmovable)
        {
            rUi->SetIcon(RES__CANCEL_ICON());
            rUi->SetAlignContentToCenterX();
            rUi->SetAlignContentToCenterY();
        }
    }
}


