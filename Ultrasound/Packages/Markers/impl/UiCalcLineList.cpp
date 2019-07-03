// UiCalcLineList.cpp

#include "CUsmainProject.h"
#include "CCalcLineList.h"
#include "CCalcLineType.h"
#include "CCalcLineState.h"

void UiCalcLineList::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rCalcLineList->_m_ptrmapUiCalcLineList += rUi;

        rUi->SetMouseCursorTransparentLayer(true);

        rUi->SetMaxX(oo);
        rUi->SetMaxY(oo);

        // we have ModifyShape at play here
        rUi->SetPreferredX(0);
        rUi->SetPreferredY(0);
        rUi->SetMinX(0);
        rUi->SetMinY(0);
    }

    ref<CCalcLineState> rCalcLineState;
    for (
        iter i;
        m_rCalcLineList->
            IterateLinesInPaintOrder(
                out i, 
                out rCalcLineState);)
    {
        //type<CCalcLineType> typeIterCalcLineType;

        //
        // Next Line
        //
        {
            ref<UiContainer> rUi = 
                rCalcLineState->
                    NewUiCalcLine();
        }

        //
        // Next Label
        //
        {
            ref<UiContainer> rUi = 
                rCalcLineState->
                    NewUiCalcLineLabel();
        }
    }
}
