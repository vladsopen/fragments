// CDemoVehicleList.cpp

#include "CProject.h"
#include "CDemoVehicleList.h"
#include "CDemoVehicle.h"
#include "CUmoveIfaceGp.h"

CDemoVehicleList::CDemoVehicleList()
{
}

void CDemoVehicleList::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CDemoVehicleList!");
}

void CDemoVehicleList::RenderDemoVehicles()
{
    m_aDemoVehicle.RemoveAll();

    // Init demo Umove vehicle list
    {
        ref<CDemoVehicle> rDemoVehicle;

        // First define range then pos
        rDemoVehicle->x_rUmove->x_sizeLogicalUmoveRange = size(100, 0);
        rDemoVehicle->x_rUmove->x_pointLogicalVehiclePos = point(60, 0);
        rDemoVehicle->x_rUmove->x_sizeVehicle = size(20, 0);
        rDemoVehicle->x_bStretchHeightAsForVerticalMarker = true;

        AddVehicle(rDemoVehicle);
    }
    {
        ref<CDemoVehicle> rDemoVehicle;

        // First define range then pos
        rDemoVehicle->x_rUmove->x_sizeLogicalUmoveRange = size(0, 100);
        rDemoVehicle->x_rUmove->x_pointLogicalVehiclePos = point(0, 8);
        rDemoVehicle->x_rUmove->x_sizeVehicle = size(0, 20);
        rDemoVehicle->x_bStretchWidthAsForHorizontalMarker = true;

        AddVehicle(rDemoVehicle);
    }
    {
        ref<CDemoVehicle> rDemoVehicle;

        // First define range then pos
        rDemoVehicle->x_rUmove->x_sizeLogicalUmoveRange = size(10, 5);
        rDemoVehicle->x_rUmove->x_pointLogicalVehiclePos = point(2, 1);
        rDemoVehicle->x_rUmove->x_sizeVehicle = size(30, 30);
        rDemoVehicle->x_pointForbiddenPlace = point(7, 3);

        AddVehicle(rDemoVehicle);

        // mark forbidden place with unmovable vehicle
        {
            ref<CDemoVehicle> rVehicleForbiden;

            // First define range then pos
            rVehicleForbiden->x_rUmove->x_sizeLogicalUmoveRange = 
                rDemoVehicle->x_rUmove->x_sizeLogicalUmoveRange;
            rVehicleForbiden->x_rUmove->x_pointLogicalVehiclePos = 
                rDemoVehicle->x_pointForbiddenPlace;
            rVehicleForbiden->x_rUmove->x_sizeVehicle = 
                rDemoVehicle->x_rUmove->x_sizeVehicle;
            rVehicleForbiden->x_bUnmovable = true;

            AddVehicle(rVehicleForbiden);
        }
    }
}

ref<CDemoVehicle> CDemoVehicleList::GetGuineaPigVehicle()
{
    return m_aDemoVehicle.GetLast(-2);
}

