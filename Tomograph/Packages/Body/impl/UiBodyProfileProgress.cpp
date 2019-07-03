// UiBodyProfileProgress.cpp

#include "CProject.h"
#include "CBodyProfileImpl.h"
#include "CDownloaderIface.h"
#include "CSkinizerCommonIfaceGp.h"
#include "CBodyPointImpl.h"

void UiBodyProfileProgress::Ui()
{
    //int nTomograms = m_rDownloader->GetStoredTomogramCount();

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rBodyProfile->_m_pUiBodyProfileProgress = rUi;
    }

    //
    // List Controls before Reorder Buttons
    //
    {
        ref<CUiPanelGp> rUi;

        //
        // Point List
        //
        {
            ref<CUiScrollablePanelGp> rUi;

            // Standard-List: Border and Background
            rUi->GetSkinizer()->EqualizeListScrollablePanel(rUi);

            // all points
            {
                ref<CBodyPointImpl> rIterBodyPoint;
                for (
                    iter i;
                    m_rBodyProfile->
                        IterateBodyPoints(
                            out i, 
                            out rIterBodyPoint);)
                {
                    //
                    // Next Point Row
                    //
                    {
                        ref<UiContainer> rUi = 
                            rIterBodyPoint->
                                NewUiBodyPointSetupRow(
                                    m_rBodyProfile,
                                    i);
                    }

                    NL();
                }
            }
        }

        NL();

    }
}
