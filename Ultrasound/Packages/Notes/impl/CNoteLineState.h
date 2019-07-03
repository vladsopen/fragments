// CNoteLineState.h
#pragma once

class CDbNoteIface;
class CUdbMirrorIfaceGp;
class CMarkerIface;
class CNoteLineList;
class CNoteLineTypeImpl;

//
// CNoteLineState - 
//
//   Current line state and UI conteroller.
//     Indexed by CNoteLineTypeImpl in CNoteLineList
//
//ASSUME_IMPL_FOR_IFACE(CNoteLineStateImpl, CNoteLineState)

class CNoteLineState : public object
{
public:
    CNoteLineState();
    //NEW_LOCAL(CNoteLineStateImpl, CNoteLineState)
    //NEW_GP(MyProject, CNoteLineStateImpl, CNoteLineState)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);
    bool MirrorDbNote(
            ref<CUdbMirrorIfaceGp> rUdbMirror,
            ref<CDbNoteIface> rDbNote);

// Constants

    // Line width
    static const int C_nNoteMarkerThickness = 4;
    //static const int C_nNoteMarkerThickness = 4;

    // Dark border around the marker
    static const int C_nNoteMarkerDimBorder = 1;

    // Label offset
    static const int C_nNoteLabelOffset = 0; //-10;
    static const int C_nNoteLabelIndent = 2;

    // Minimal indent from window edge we allow
    static const int C_nNoteLineMinWindowMargin = 0;

    // Historgam metric
    static const pix C_pixHistogramHorizontalRange = 128;
    static const pix C_pixHistogramLineThickness = 3;
    static const rgb C_rgbHistogram = rgb(255, 255, 255);

// Attributes

    // Backref to parent
    ptr<CNoteLineList> _x_pParentNoteLineList
            xauto(Get, Set);

    // Common marking
    ref<CMarkerIface> x_rMarker
            xauto(Get);

    // Key to Index Type Identifier
    type<CNoteLineTypeImpl> GetKey();
    void InitKey(
            type<CNoteLineTypeImpl> typeNoteLineType);

    // Current marker position on media image
    num x_numMarkerPosVirtual = 0
            xauto(Get)
            xassert(true);
    void x_numMarkerPosVirtual_Set(
            num numValue);

    // User defined text
    str x_sNoteComment = ""
            xauto(Get, Set)
            xassert(true);

    // Original value may need be restored
    num GetRestoreOriginalIndexValueRoundedOrOo();

    // see x_bRestrictToNoteMarkerRange
    bool IsLineUiControlsVisible();

// Operations

    // Handle mode change
    void UpdateNoteLine();

    // Place a marker from the context menu
    void PlaceNoteLineInParent(
            point pointAt,
            int nThickness);

    // Play preview if required
    void PlayPreviewAtMarker();

    // Input new comment
    void EditMarkerText();

    // Remove from list
    void DeleteMarker();

// UI
    // NoteLineMenu
    ref<UiContainer> NewUiNoteLineMenu();
    ptrmap<CUiPanelGp> _m_ptrmapUiNoteLineMenu;

    // NoteLine
    ref<UiContainer> NewUiNoteLine();
    ptrmap<CUiPanelGp> _m_ptrmapUiNoteLine;
    void ShapeNoteLine(
            ref<CUiPanelGp> rUi);

    // Move hander
    void HandleLineOnMouse(
            ref<CUiPanelGp> rUi,
            EMouse eMouse);

    // Paint handler
    void HandleLineOnPaint(
            draw dc,
            ref<CUiPanelGp> rUi);

    // NoteLineLabel
    ref<UiContainer> NewUiNoteLineLabel();
    ptrmap<CUiPanelGp> _m_ptrmapUiNoteLineLabel;
    void ShapeNoteLabel(
            ref<CUiPanelGp> rUi);

protected:
private:

    // identification key
    type<CNoteLineTypeImpl> m_typeNoteLineType;

    // they want original values to be restored on calc off
    num _m_numRestoreOriginalIndexValueRounded = OO;

    rect GetLineOrLabelShape(
            int nLabelOffset);

    rect GetMarkerScreenRext();

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiNoteLine.cpp)
class UiNoteLine : public UiContainer
{
public:
    virtual void Ui();

    ref<CNoteLineState> m_rNoteLineState;
};

// (UiNoteLineLabel.cpp)
class UiNoteLineLabel : public UiContainer
{
public:
    virtual void Ui();

    ref<CNoteLineState> m_rNoteLineState;
};

// (UiNoteLineMenu.cpp)
class UiNoteLineMenu : public UiContainer
{
public:
    virtual void Ui();

    ref<CNoteLineState> m_rNoteLineState;
};
