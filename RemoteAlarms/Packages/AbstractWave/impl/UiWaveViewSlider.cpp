// UiWaveViewSlider.cpp

#include "CProject.h"
#include "CWaveView.h"
#include "CRulerbarIfaceGp.h"

void UiWaveViewSlider::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rWaveView->_m_ptrmapUiWaveViewSlider += rUi;
    }

    //
    // Scroll Slider
    //
    {
        big iiMax = m_rWaveView->CalcShowMaxPos();
        rASSERT(iiMax < off);

        ref<CUiSliderGp> rUi;

        rUi->SetSliderMax(
            (int) iiMax);

        rUi->SetSliderStep(
            1);

        rUi->SetSliderPageSize(
            10);

        rUi->SetSliderPos(
            (int) m_rWaveView->x_iiShowBlock);

        rUi->SetTimerInTicks(500);

        rUi->OnTimer = [=]
        {
            if (m_iiLastMaxSegment != iiMax)
            {
                //m_rWaveView->x_iiShowBlock = iiMax;
                if (m_iiLastMaxSegment != -1)
                {
                    rUi->InvalidateState();
                }

                m_iiLastMaxSegment = iiMax;
            }
        }

        rUi->OnChange = [=]
        {
            int iNewInternalPos =
                rUi->GetSliderPos();

            m_rWaveView->x_iiShowBlock =
                iNewInternalPos;
        }
    }

}
