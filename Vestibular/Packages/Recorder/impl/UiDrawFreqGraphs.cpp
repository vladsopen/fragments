// UiDrawFreqGraphs.cpp

#include "CProject.h"
#include "CDrawFreq.h"
#include "CSkinizerCommonIfaceGp.h"
#include "CDrawFreqAxis.h"

void UiDrawFreqGraphs::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rDrawFreq->_m_ptrmapUiDrawFreqGraphs += rUi;

        rUi->SetPadX(4);
    }

    // X, Y
    {
        repeat(i, E_RouteAxis_count)
        {
            ref<CDrawFreqAxis> rIterDrawFreqAxis = 
                m_rDrawFreq->
                    GetDrawFreqAxis(
                        ERouteAxis(i));

            //
            // Next Axis
            //
            {
                ref<UiContainer> rUi = 
                    rIterDrawFreqAxis->
                        NewUiDrawFreqAxisGraph();
            }
        }
    }

}
