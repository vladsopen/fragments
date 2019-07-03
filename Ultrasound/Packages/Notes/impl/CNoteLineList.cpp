// CNoteLineList.cpp

#include "CUsmainProject.h"
#include "CNoteLineList.h"
#include "CNoteLineTypeImpl.h"
#include "CNoteLineState.h"
#include "CUdbConnectionIfaceGp.h"
#include "CUdbMirrorIfaceGp.h"
#include "CDbDocIface.h"
#include "CDbNoteIface.h"
#include "CNoteMarkingImpl.h"

CNoteLineList::CNoteLineList()
{
    _init_RefillArray();
    _init_AttachProperties();
}

void CNoteLineList::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // old versions may miss items for some new type<>s
    _init_RefillArray();

    // you may want to kill some obsolete data saved by an old version
    RemoveNullKeyedItems();

    // update child pointers
    _init_AttachProperties();
}

void CNoteLineList::AddNoteLine(
        ref<CNoteLineState> rNoteLineState,
        type<CNoteLineTypeImpl> typeNoteLineType)
{
    rASSERT(m_aNoteLineState.Find(rNoteLineState) == -1);

    m_aNoteLineState.Add(rNoteLineState);
    rNoteLineState->InitKey(typeNoteLineType);
    _init_AttachProperties();
}

void CNoteLineList::DeleteNoteLine(
        ref<CNoteLineState> rNoteLineState)
{
    int iAt = m_aNoteLineState.Find(rNoteLineState);
    if ok(iAt != -1)
    {
        m_aNoteLineState.RemoveAt(iAt);
    }

    rASSERT(m_aNoteLineState.Find(rNoteLineState) == -1);

    // invalidate map cache
    _m_ptrcacheKeyMap = null();

    // update UI
    ref<CNoteMarkingImpl> rNoteMarking = 
        _x_pParentNoteMarking.Get().Valid();
    rNoteMarking->UpdateNoteMarking();
}

ref<UiContainer> CNoteLineList::NewUiNoteLineList()
{
    ref<UiNoteLineList> rUi;

[...]