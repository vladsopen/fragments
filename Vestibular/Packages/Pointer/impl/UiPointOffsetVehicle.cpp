// UiPointOffsetVehicle.cpp

#include "CProject.h"
#include "CPointOffset.h"
#include "CUmoveIfaceGp.h"

void UiPointOffsetVehicle::Ui()
{
    ref<CUmoveIfaceGp> rUmove = 
        m_rPointOffset->
            x_rUmove;

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rPointOffset->_m_ptrmapUiPointOffsetVehicle += rUi;

        rUi->SetIndependentShape(true);

        rUi->SetWindowAlpha(percent(70));

        rUi->OnIndependentShape = [=]
        {
            //TODO: you must ask Umove the Vehicle position
            point pointVehicle = 
                rUmove->
                    GetVehiclePosInParentPanel(
                        m_rPointOffset->x_sizeUmoveVehicleParent);

            rUi->SetInstantPos(pointVehicle.x, pointVehicle.y);

            //TODO: you must make sure you told Umove the correct Vehicle size
            size sizeVehicle = rUmove->x_sizeVehicle;

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
                        m_rPointOffset->x_sizeUmoveVehicleParent,
                        rUi,
                        GetOnMouseEvent());

            if (bMoved)
            {
                // here we can restrict new position
                point pointNewPos = 
                    rUmove->
                        x_pointLogicalVehiclePos;

                /*
                if (pointNewPos.x == m_rPointOffset->x_pointForbiddenPlace.Get().x &&
                    pointNewPos.y == m_rPointOffset->x_pointForbiddenPlace.Get().y)
                {
                    // force previous position
                    rUmove->
                        x_pointLogicalVehiclePos = 
                            pointOldPos;
                }
                */

                // we've got a new position. OnModifyShape will take care
                rUi->InvalidateLayout();
                m_rPointOffset->UpdateForNewVehiclePositions();
                //rUi->UpdateTreeLayout();
                //rUi->UpdatePaint();

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

        rUi->SetMinX(0);
        rUi->SetMinY(0);

        //TODO: if you don't want to invent the wheel ask Umove cursor
        rUi->SetCursor(rUmove->GetUmoveCursorForVehicle());

        rUi->SetTip(
            TRANSLATE(
                "Target cen[... UI-string-removed]");

        /*
        if (m_rPointOffset->x_bUnmovable)
        {
            rUi->SetIcon(RES__CANCEL_ICON());
            rUi->SetAlignContentToCenterX();
            rUi->SetAlignContentToCenterY();
        }
        */
    }
}
