// UiWaveViewSystem.cpp

#include "CProject.h"
#include "CWaveView.h"

void UiWaveViewSystem::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rWaveView->_m_ptrmapUiWaveViewSystem += rUi;
    }

    //
    // Graph + Ruler
    //
    {
        ref<CUiPanelGp> rUi;

        m_rWaveView->_m_ptrmapUiWaveGraphAndRulers += rUi;

        //
        // All Graphs
        //
        {
            ref<UiContainer> rUi = 
                m_rWaveView->
                    NewUiWaveViewZone();
        }

        //
        // All Markers
        //
        {
            ref<UiContainer> rUi = 
                m_rWaveView->
                    NewUiWaveViewMakers();
        }

        NL();

        //
        // Ruler
        //
        {
            ref<UiContainer> rUi = 
                m_rWaveView->
                    NewUiWaveViewRulerTime();
        }
    }

    NL();

    //
    // Slider
    //
    {
        ref<UiContainer> rUi = 
            m_rWaveView->
                NewUiWaveViewSlider();
    }
}
