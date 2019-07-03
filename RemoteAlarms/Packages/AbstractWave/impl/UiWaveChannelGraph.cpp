// UiWaveChannelGraph.cpp

#include "CProject.h"
#include "CWaveChannelImpl.h"
#include "CWaveDocImpl.h"
#include "CWaveDb.h"
#include "CWaveSegment.h"
#include "CWaveView.h"
#include "CWaveLayerType.h"
#include "CWaveChannelType.h"

void UiWaveChannelGraph::Ui()
{
    ref<CWaveDocImpl> rWaveDoc = 
        m_rWaveChannel->GetWaveDoc();
    ref<CWaveDb> rWaveDb = 
        rWaveDoc->x_rWaveDb;
    ref<CWaveView> rWaveView = 
        rWaveDoc->x_rWaveView;

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rWaveChannel->_m_ptrmapUiWaveChannelGraph += rUi;

        rUi->SetMaxY(oo);
        rUi->SetMinY(0);
        rUi->SetPreferredY(
            m_rWaveChannel->
                GetWaveChannelType()->
                    GetRelativeChannelHeight());
    }

    // iterate channels segments
    {
        int iChannel = m_rWaveChannel->x_iWaveChannel;

        ref<CWaveSegment> rIterWaveSegment;
        big nnCountZeroTimeSegments = 0;
        for (
            iter i;
            rWaveDb->
                IterateWaveSegmentsForChannel(
                    out i,
                    out rIterWaveSegment,
                    iChannel,
                    rWaveView->x_iiShowBlock);)
        {
            if (rIterWaveSegment->x_iiWaveSegment > 
                    rWaveView->x_iiLastShownSegment)
            {
                //
                // Gray Stub
                //
                {
                    ref<UiContainer> rUi = 
                        m_rWaveChannel->
                            NewUiWaveChannelBusy();
                }

                break;
            }

            if (rIterWaveSegment->x_tickSegmentDuration <= 0)
            {
                nnCountZeroTimeSegments++;
            }
            else
            {
                nnCountZeroTimeSegments = 0;
            }

            if (nnCountZeroTimeSegments < 
                    CWaveView::C_nMaxMarkersOverlayed)
            {
                //
                // Next Segment
                //
                {
                    ref<UiContainer> rUi = 
                        rIterWaveSegment->
                            NewUiWaveSegmentGraph(
                                type<CWaveLayerType>()->
                                    AsWaveLayerIsWave());
                }
            }
        }
    }
}
