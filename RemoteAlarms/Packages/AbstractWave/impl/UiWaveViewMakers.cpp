// UiWaveViewMakers.cpp

#include "CProject.h"
#include "CWaveView.h"
#include "CParseEcgFile.h"
#include "CWaveDocImpl.h"
#include "CWaveDb.h"
#include "CWaveSegment.h"
#include "CWaveLayerType.h"

void UiWaveViewMakers::Ui()
{
    ref<CWaveDocImpl> rWaveDoc = 
        m_rWaveView->GetWaveDoc();
    ref<CWaveDb> rWaveDb = 
        rWaveDoc->x_rWaveDb;

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rWaveView->_m_ptrmapUiWaveViewMakers += rUi;

        rUi->SetOverlayPrevSibling(true);
    }

    // iterate marker segments
    {
        int iChannel = 
            CParseEcgFile::C_nEcgMarkerChannel;

        ref<CWaveSegment> rIterWaveSegment;
        for (
            iter i;
            rWaveDb->
                IterateWaveSegmentsForChannel(
                    out i,
                    out rIterWaveSegment,
                    iChannel,
                    m_rWaveView->x_iiShowBlock);)
        {
            if (rIterWaveSegment->x_iiWaveSegment > 
                    m_rWaveView->x_iiLastShownSegment)
            {
                /*
                //
                // Gray stub for missing data
                //
                */

                break;
            }

            //
            // Next Segment
            //
            {
                ref<UiContainer> rUi = 
                    rIterWaveSegment->
                        NewUiWaveSegmentGraph(
                            type<CWaveLayerType>()->
                                AsWaveLayerIsMarkers());
            }
        }
    }
}
