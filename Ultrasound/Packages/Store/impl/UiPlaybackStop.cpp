// UiPlaybackStop.cpp

#include "CUsmainProject.h"
#include "CPlayback.h"
#include "CMultiPlayIfaceGp.h"
#include "CDocImpl.h"
#include "CSessionIface.h"
#include "CHardwareIfaceGp.h"

void UiPlaybackStop::Ui()
{
    ref<CDocImpl> rDoc = 
        m_rPlayback->_x_pParentDoc.Get().Valid();

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rPlayback->_m_ptrmapUiPlaybackStop += rUi;
    }

    //
    // [Stop]
    //
    {
        ref<CUiButtonGp> rUi;

        rUi->SetText("Stop");

        rUi->SetTip("Stop playback");

        rUi->SetMaxX(oo);
        rUi->SetAlignContentToCenterX();

        rUi->SetBackColor(rgb(255, 0, 0));
        rUi->SetFontBold(true);

        rUi->OnClick = [=]
        {
            ref<CMultiPlayIfaceGp> rMultiPlay = 
                rDoc->
                    GetSessionForInitedDoc()->
                        GetHardware()->
                            x_rMultiPlay;

            //m_rPlayback->StopPlayback();

            rMultiPlay->
                EndMultiPlay();
        }
    }
}
