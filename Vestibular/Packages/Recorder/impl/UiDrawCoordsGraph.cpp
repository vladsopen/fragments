// UiDrawCoordsGraph.cpp

#include "CProject.h"
#include "CDrawCoords.h"
#include "CSkinizerCommonIfaceGp.h"
#include "CRulersForTimed.h"
#include "CDrawCoordAxis.h"

void UiDrawCoordsGraph::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rDrawCoords->_m_ptrmapUiDrawCoordsGraph += rUi;
    }

    // X, Y
    {
        repeat(i, E_RouteAxis_count)
        {
            ref<CDrawCoordAxis> rIterDrawCoordAxis = 
                m_rDrawCoords->
                    GetDrawCoordAxis(
                        ERouteAxis(i));

            //
            // Next Axis
            //
            {
                ref<UiContainer> rUi = 
                    rIterDrawCoordAxis->
                        NewUiDrawCoordAxisGraph();
            }

            NL();
        }
    }

}
