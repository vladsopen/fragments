// UiNoteLineList.cpp

#include "CUsmainProject.h"
#include "CNoteLineList.h"
#include "CNoteLineTypeImpl.h"
#include "CNoteLineState.h"

void UiNoteLineList::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rNoteLineList->_m_ptrmapUiNoteLineList += rUi;

        rUi->SetMouseCursorTransparentLayer(true);

        rUi->SetMaxX(oo);
        rUi->SetMaxY(oo);

        // we have ModifyShape at play here
        rUi->SetPreferredX(0);
        rUi->SetPreferredY(0);
        rUi->SetMinX(0);
        rUi->SetMinY(0);
    }

    ref<CNoteLineState> rNoteLineState;
    for (
        iter i;
        m_rNoteLineList->
            IterateLinesInPaintOrder(
                out i, 
                out rNoteLineState);)
    {
        type<CNoteLineTypeImpl> typeIterNoteLineType = 
            rNoteLineState->GetKey();

        //
        // Next Line
        //
        {
            ref<UiContainer> rUi = 
                rNoteLineState->
                    NewUiNoteLine();
        }

        //
        // Next Label
        //
        {
            ref<UiContainer> rUi = 
                rNoteLineState->
                    NewUiNoteLineLabel();
        }
    }
}
