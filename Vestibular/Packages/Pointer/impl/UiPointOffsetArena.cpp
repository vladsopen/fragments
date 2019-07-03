// UiPointOffsetArena.cpp

#include "CProject.h"
#include "CPointOffset.h"
#include "CUmoveIfaceGp.h"

void UiPointOffsetArena::Ui()
{
    // current position of the main vehicle
    point pointVirtual = 
        m_rPointOffset->x_rUmove->x_pointLogicalVehiclePos;

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rPointOffset->_m_ptrmapUiPointOffsetArena += rUi;
    }

    //
    // Umove Parent Zone where stuff moves around
    //
    {
        ref<CUiPanelGp> rUi;

        //rUi->SetBackColor(rgb(255, 255, 240));

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
            m_rPointOffset->x_sizeUmoveVehicleParent = 
                rUi->GetInstantSize();
        }

        //
        // The Vehicle 
        //
        {
            ref<UiContainer> rUi = 
                m_rPointOffset->
                    NewUiPointOffsetVehicle();
        }
    }
}
