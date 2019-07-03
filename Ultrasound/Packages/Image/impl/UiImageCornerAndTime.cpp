// UiImageCornerAndTime.cpp

#include "CUsmainProject.h"
#include "CImageImpl.h"
#include "CDocIface.h"
#include "CRulerSystemIface.h"
#include "CRulerBarIface.h"

void UiImageCornerAndTime::Ui()
{
    ref<CDocIface> rDoc =
        m_rImage->GetParentDoc();

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rImage->_m_ptrmapUiImageCornerAndTime += rUi;
    }

    //
    // Corner Panel Size Control
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetAlikeX(m_rImage->_m_alikexScaleBar);

        rUi->SetAlignContentToCenterX();

        //
        // Corner
        //
        {
            ref<UiContainer> rUi = 
                m_rImage->
                    NewUiCornerScaleTime();
        }
    }

    //
    // Time Panel Size Control
    //
    {
        ref<CUiPanelGp> rUi;

        // it gets compressed!
        rUi->SetMinY(off);

        //
        // Time Bar
        //
        {
            ref<UiContainer> rUi = 
                rDoc->
                    _x_rRulerSystem->
                        x_rRulerBarForTime->
                            NewUiRulerBar();
        }
    }
}
