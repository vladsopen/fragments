// UiSequenceList.cpp

#include "CProject.h"
#include "CSequence.h"
#include "CStepAbstract.h"
#include "CProxyImpl.h"

void UiSequenceList::Ui()
{
    ref<CProxyImpl> rProxy = 
        m_rSequence->GetProxy();

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rSequence->_m_ptrmapUiSequenceList += rUi;
    }

    //
    // Scroll
    //
    {
        ref<CUiScrollablePanelGp> rUi;

        rUi->SetMaxY(oo);
        rUi->SetAlignContentToBottom();

        // iterate steps
        {
            ref<CStepAbstract> rIterStep;
            for (
                iter i;
                m_rSequence->
                    IterateSteps(
                        out i,
                        out rIterStep);)
            {
                //
                // Next Step
                //
                {
                    ref<UiContainer> rUi = 
                        rIterStep->
                            NewUiStepAbstractRow();
                }

                NL();
            }
        }
    }

    NL();

    { ref<CUiSeparateRowsGp> rUi; }

    NL();

    //
    // Status
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetText(
            m_rSequence->x_sDebugLastPollStatus);

        rUi->SetTip(
            "Current sequence execution status");
    }
}
