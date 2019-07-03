// UiNoteLineMenu.cpp

#include "CUsmainProject.h"
#include "CNoteLineState.h"
#include "CNoteLineTypeImpl.h"
#include "VirtOs_ask.h"

void UiNoteLineMenu::Ui()
{
    type<CNoteLineTypeImpl> typeNoteLineType = 
        m_rNoteLineState->GetKey();

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rNoteLineState->_m_ptrmapUiNoteLineMenu += rUi;
    }

    if (typeNoteLineType->HasUserEnteredText())
    {
        //
        // [Edit]
        //
        {
            ref<CUiMenuItemGp> rUi;

            rUi->SetText(
                TRANSLATE(
                    "Edit Comme[... UI-string-removed]");

            rUi->SetTip(
                TRANSLATE(
                    "Edit marke[... UI-string-removed]");

            rUi->OnClick = [=]
            {
                m_rNoteLineState->EditMarkerText();
            }
        }

        NL();
    }

    //
    // [Delete]
    //
    {
        ref<CUiMenuItemGp> rUi;

        rUi->SetText(
            TRANSLATE(
                "Delete Mar[... UI-string-removed]");

        rUi->SetTip(
            TRANSLATE(
                "Delete mar[... UI-string-removed]");

        rUi->OnClick = [=]
        {
            if (ask(CSlocCommon::L__Delete() + "?")._unsafe_ask_Yes()._unsafe_ask_No() == 
                    ask::Compare()._unsafe_ask_Yes())
            {
                m_rNoteLineState->DeleteMarker();
            }
        }
    }

    NL();
}