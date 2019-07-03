// UiViewScrollAlikeUnderScale.cpp

#include "CUsmainProject.h"
#include "CViewState.h"
#include "CImageImpl.h"

void UiViewScrollAlikeUnderScale::Ui()
{
    ref<CImageImpl> rImage = 
        m_rViewState->_x_pParentImage.Get().Valid();

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rViewState->_m_pUiViewScrollAlikeUnderScale = rUi;

        rUi->SetAlikeX(rImage->_m_alikexScaleBar);
        rUi->SetAlikeY(m_rViewState->_m_alikeyViewScroll);

        rUi->SetMinY(off);

        //rUi->SetMaxY(oo);
    }

    /*
    if (!typeOnlineType->IsImageScrollEnabled())
    {
        return;
    }

    //
    // Padding
    //
    {
        ref<CUiPanelGp> rUi;

    }
    */
}
