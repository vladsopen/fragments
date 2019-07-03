// UiNoteMarkingMenu.cpp

#include "CUsmainProject.h"
#include "CNoteMarkingImpl.h"
#include "CNoteLineTypeImpl.h"

void UiNoteMarkingMenu::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rNoteMarking->_m_ptrmapUiNoteMarkingMenu += rUi;
    }

    // add for all markers
    {
        type<CNoteLineTypeImpl> typeIterNoteLineType;
        for (
            iter i;
            type<CNoteLineTypeImpl>()->GetDerivedList().
                Iterate(out i, out typeIterNoteLineType);)
        {
            //
            // Next Add
            //
            {
                ref<CUiMenuItemGp> rUi;

                rUi->SetText(typeIterNoteLineType->GetNoteLineName(null()));

                rUi->SetTip(
                    "" + 
                        TRANSLATE(
                            "Place mark[... UI-string-removed]");

                rUi->OnClick = [=]
                {
                    m_rNoteMarking->
                        CreateOrPlaceNoteLineAtScreen(
                            typeIterNoteLineType,
                            m_pointAtScreen);
                }
            }

            NL();
        }
    }
}
