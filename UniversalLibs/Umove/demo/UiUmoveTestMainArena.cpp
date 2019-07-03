// UiUmoveTestMainArena.cpp

#include "CProject.h"
#include "CUmoveTest.h"
#include "CDemoVehicleList.h"
#include "CDemoVehicle.h"
#include "CSkinizerCommonIfaceGp.h"

void UiUmoveTestMainArena::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rUmoveTest->_m_ptrmapUiUmoveTestMainArena += rUi;
    }

    //
    // Umove Parent Zone where stuff moves around
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->GetSkinizer()->EqualizeGroupBorder(rUi);

        rUi->SetBackColor(rgb(255, 255, 240));

        // if Vehicles are ModifyShaped we have to suppress their 
        //   added size
        rUi->SetMinX(0);
        rUi->SetMinY(0);
        rUi->SetMaxX(oo);
        rUi->SetMaxY(oo);

        rUi->OnSize = [=]
        {
            //TODO: save parent window size Umove will need later
            //  as alternative you may have a ptr to this panel and 
            //  get its size when required by Umove
            m_rUmoveTest->x_sizeUmoveVehicleParent = 
                rUi->GetInstantSize();
        }

        // create all movable Vehicles we show for this demo
        {
            ref<CDemoVehicleList> rDemoVehicleList = 
                m_rUmoveTest->
                    x_rDemoVehicleList;

            ref<CDemoVehicle> rIterDemoVehicle;
            for (
                iter i;
                rDemoVehicleList->
                    IterateVehicles(
                        out i,
                        out rIterDemoVehicle);)
            {
                //
                // Next Vehicle 
                //
                {
                    ref<UiContainer> rUi = 
                        rIterDemoVehicle->
                            NewUiUmoveTestVehicle(
                                m_rUmoveTest);
                }
            }
        }
    }
}
