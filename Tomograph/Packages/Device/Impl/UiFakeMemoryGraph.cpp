// UiFakeMemoryGraph.cpp

#include "CProject.h"
#include "CFakeTomographMemory.h"
#include "CTomogramIface.h"
#include "CFakeTomograph.h"

void UiFakeMemoryGraph::Ui()
{
    ref<CTomogramIface> rTomogram = 
        m_rFakeTomographMemory->GetTomogramAtOr(
            m_rFakeTomographMemory->x_iEditTomogram,
            ref<CTomogramIface>());

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rFakeTomographMemory->_m_pUiFakeMemoryGraph = rUi;

        //rUi->SetPreferredX(scale(400));
        //rUi->SetPreferredY(scale(200));

        rUi->SetMaxX(oo);
        rUi->SetMaxY(oo);
    }

    //
    // Tomogram
    //
    {
        ref<UiContainer> rUi = 
            rTomogram->NewUiTomogramGraphAndStuff();
    }

    //
    // Edit-overlay
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetOverlayPrevSibling(true);

        rUi->OnMouse = [=]
        {
            EMouse eMouse = GetOnMouseEvent();

            m_rFakeTomograph->
                HandleMouseOverTomogram(
                    rTomogram,
                    eMouse,
                    rUi->GetClientMousePos(),
                    rUi->GetInstantSize());
        }
    }
}
