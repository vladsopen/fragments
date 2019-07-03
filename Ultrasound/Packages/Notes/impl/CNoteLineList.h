// CNoteLineList.h
#pragma once

class CDbDocIface;
class CUdbMirrorIfaceGp;
class CNoteMarkingImpl;
class CNoteLineState;
class CNoteLineTypeImpl;

//
// CNoteLineList - 
//
//   Note markers state, selection and order.
//
//ASSUME_IMPL_FOR_IFACE(CNoteLineListImpl, CNoteLineList)

class CNoteLineList : public object
{
public:
    CNoteLineList();
    //NEW_LOCAL(CNoteLineListImpl, CNoteLineList)
    //NEW_GP(MyProject, CNoteLineListImpl, CNoteLineList)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);
    bool MirrorDbNoteList(
            ref<CUdbMirrorIfaceGp> rUdbMirror,
            ref<CDbDocIface> rDbDoc);

// Constants

// Attributes

    // Backref to parent
    ptr<CNoteMarkingImpl> _x_pParentNoteMarking
            xauto(Get, Set);

    // Current selection
    type<CNoteLineTypeImpl> x_typeSelectedNoteLineType
            xauto(Get);
    void x_typeSelectedNoteLineType_Set(
            type<CNoteLineTypeImpl> value);

// Data Item Access

    // Optimized item search
    ptr<CNoteLineState> LookupSingularNoteByKey(
            type<CNoteLineTypeImpl> typeNoteLineType);

// Operations

    // Ordered iteration - from FOREGROUND to BACKGPROUND lines
    bool IterateLinesInPaintOrder(
            out iter& out_i, 
            out ref<CNoteLineState>& out_rNoteLineState);

    // Marker pos update
    void UpdateMarkersAfterMove();

    // Add
    void AddNoteLine(
            ref<CNoteLineState> rNoteLineState,
            type<CNoteLineTypeImpl> typeNoteLineType);

    // Remove
    void DeleteNoteLine(
            ref<CNoteLineState> rNoteLineState);

// UI

    // NoteLineList
    ref<UiContainer> NewUiNoteLineList();
    ptrmap<CUiPanelGp> _m_ptrmapUiNoteLineList;
    size GetUiNoteLineListInstantSize();

protected:
private:

    // array indexed by type<>
    array< ref<CNoteLineState> > m_aNoteLineState;
    ptrcache<type<CNoteLineTypeImpl>, CNoteLineState> _m_ptrcacheKeyMap;

    // Init/Update array
    void _init_RefillArray();

    // Remove obsolete items for depreciated type<>s
    void RemoveNullKeyedItems();

    // init parent back pointers in our members
    void _init_AttachProperties();

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiNoteLineList.cpp)
class UiNoteLineList : public UiContainer
{
public:
    virtual void Ui();

    ref<CNoteLineList> m_rNoteLineList;
};
