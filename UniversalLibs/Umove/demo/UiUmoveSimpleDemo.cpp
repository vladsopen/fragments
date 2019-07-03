// UiUmoveSimpleDemo.cpp

#include "CProject.h"
#include "CUmoveTest.h"
#include "CUmoveIfaceGp.h"
#include "CSkinizerCommonIfaceGp.h"

void UiUmoveSimpleDemo::Ui()
{
    // we store CUmoveIfaceGp controller in the doc
    ref<CUmoveIfaceGp> rUmove = 
        m_rUmoveTest->
            x_rUmoveForSimpleDemo;

    //
    // init/update rUmove attributes every time we enter Ui()
    //

    // allow only horizontal movements for this test
    rUmove->x_sizeLogicalUmoveRange = 
        size(
            m_rUmoveTest->x_nSimpleDemoRange,
            0); // no Y movement

    // update current virtual vehicle position
    //   (if the position changes outside of this Ui don't forget
    //    to invalidate TreeLayout to call OnIndependentShape)
    rUmove->x_pointLogicalVehiclePos = 
        point(
            m_rUmoveTest->x_iSimpleDemoPos,
            0); // no Y movement

    // it is important for rUmove to know the exact current vehicle 
    //   extent (in pixels) for any direction (X/Y) in which
    //   the vehicle can move.
    //   (you must set this size to your Ui button or panel yoursef!
    //    see below)
    rUmove->x_sizeVehicle = 
        size(
            (pix) scale(20), // may be variable, but don't forget to relayout
            0); // no Y movement

    // rarely used
    //rUmove->x_sizeOffsetInParentPanel = ...

    //
    // this
    //
    //   (the background panel often in another Ui so 
    //    in your app you would have to make 
    //        x_sizeUmoveVehicleParent
    //    a doc's property or use pUi to get the parent size)
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rUmoveTest->_m_ptrmapUiUmoveSimpleDemo += rUi;

        // for demo only
        rUi->SetMaxX(oo);
        rUi->SetPreferredY(scale(20));
        rUi->GetSkinizer()->EqualizeGroupBorder(rUi);
        rUi->SetBackColor(rgb(255, 255, 240));

        rUi->OnSize = [=]
        {
            //TODO: you either save the parent size here or get it from pUi
            m_sizeUmoveVehicleParent = rUi->GetInstantSize();
        }
    }

    //
    // The Movable Vehicle
    //
    {
        ref<CUiPanelGp> rUi;

        // we position the vehicle as Umove directs us

        // 1. Either IndependentShape
        //rUi->SetIndependentShape(true);

        // 2. Or ModifyShape
        rUi->SetPreferredX(rUmove->x_sizeVehicle.Get().w);
        rUi->SetPreferredY(scale(30)); // no Y movement in this demo
        rUi->SetMaxX(off); // do not stretch even if content wants this
        rUi->SetMaxY(off);

        //rUi->OnIndependentShape = [=] - if we don't need any auto-size
        rUi->OnModifyShape = [=]
        {
            //TODO: you must ask Umove the Vehicle position
            point pointVehicle = 
                rUmove->
                    GetVehiclePosInParentPanel(
                        m_sizeUmoveVehicleParent);

            rUi->SetInstantPos(pointVehicle.x, pointVehicle.y);

            //TODO: you must make sure you told Umove the correct Vehicle size
            //  if you use OnIndependentShape()
            //size sizeVehicle = rUmove->x_sizeVehicle;
            //rUi->SetInstantSize(sizeVehicle.w, sizeVehicle.h);
        }

        rUi->OnMouse = [=]
        {
            // save prev pos if we would need to force restore it later
            point pointOldPos = 
                rUmove->
                    x_pointLogicalVehiclePos;

            //TODO: you must allow Umove to hook mouse drag and drop
            bool bMoved = 
                rUmove->
                    HandleVehicleOnMouse(
                        m_sizeUmoveVehicleParent,
                        rUi,
                        GetOnMouseEvent());

            if (bMoved)
            {
                // here we can restrict new position
                point pointNewPos = 
                    rUmove->
                        x_pointLogicalVehiclePos;

                // force previous position if you have a forbidden zone
                //if (pointNewPos is invalid)
                //    rUmove->x_pointLogicalVehiclePos = pointOldPos;

                // send new virtualized position to the application
                //   (you don't have to actually duplicate its value
                //    in your app - it is always safe in x_pointLogicalVehiclePos)
                m_rUmoveTest->x_iSimpleDemoPos = pointNewPos.x;

                // we've got a new position. OnModifyShape will take care
                m_rUmoveTest->UpdateForNewSimepleDemoPositions();

                // DO NOT INVALIDATE CONTENT HERE you'll break mouse capture!
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
        }
    }
}
