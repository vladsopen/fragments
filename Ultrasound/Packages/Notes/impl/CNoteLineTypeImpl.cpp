// CNoteLineTypeImpl.cpp

#include "CUsmainProject.h"
#include "CNoteLineTypeImpl.h"
#include "CUsoundAxisIfaceGp.h"
#include "CNoteLineState.h"
#include "CCustomizeDocIface.h"

CNoteLineTypeImpl::CNoteLineTypeImpl()
{
}

const typelist<CNoteLineTypeImpl> CNoteLineTypeImpl::GetDerivedList()
{
    typelist<CNoteLineTypeImpl> typelistDerived;
    if (typelistDerived.IsNeedInit(__super::GetDerivedListStorage()))
    {
        typelistDerived.AddDerivedClassesWithFlag("flag_List_as_NoteLineType");
    }

    return typelistDerived;
}

str CNoteLineTypeImpl::GetNoteLineName(
        ptr<CNoteLineState> pNoteLineState)
{
    // custom comment has priority
    if not_null(
        ref<CNoteLineState> rNoteLineState = pNoteLineState)
    {
        str sComment = rNoteLineState->x_sNoteComment;
        if (sComment != "")
        {
            return sComment;
        }
    }

    str sCustomName = OnGetNoteLineName(pNoteLineState);

    rASSERT(sCustomName != "");
    return
        sCustomName;
}

