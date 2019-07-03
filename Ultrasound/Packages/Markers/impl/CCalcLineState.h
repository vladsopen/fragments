// CCalcLineState.h
#pragma once

class CMarkerIface;
class CUsoundCalcModeIfaceGp;
class CCalcLineList;
class CCalcLineType;

//
// CCalcLineState - 
//
//   Current line state and UI conteroller.
//     Indexed by CCalcLineType in CCalcLineList
//
//ASSUME_IMPL_FOR_IFACE(CCalcLineStateImpl, CCalcLineState)

class CCalcLineState : public object
{
public:
    CCalcLineState();
    //NEW_LOCAL(CCalcLineStateImpl, CCalcLineState)
    //NEW_GP(MyProject, CCalcLineStateImpl, CCalcLineState)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // Line width
    static const int C_nCalcMarkerThickness = 2;
    //static const int C_nCalcMarkerThickness = 4;

    // Dark border around the marker
    static const int C_nCalcMarkerDimBorder = 1;

    // Label offset
    static const int C_nCalcLabelOffset = -10;

    // Minimal indent from window edge we allow
    static const int C_nCalcLineMinWindowMarginLeft = 
            1;
    static const int C_nCalcLineMinWindowMarginTop = 
            1;
    // MedX-14214&
    static const int C_nCalcLineMinWindowMarginRight = 
            3;
    static const int C_nCalcLineMinWindowMarginBottom = 
            3;
    //static const int C_nCalcLineMinWindowMarginRight = 
    //        10;
    //static const int C_nCalcLineMinWindowMarginBottom = 
    //        10;
    //static const int C_nCalcLineMinWindowMargin = 
            //1;
            //10;

    // Historgam metrics
    static const pix C_pixHistogramHorizontalRange = 128;
    static const pix C_pixHistogramLineThickness = 3;
    static const rgb C_rgbHistogram = rgb(255, 255, 255);

// Attributes

    // Backref to parent
    ptr<CCalcLineList> _x_pParentCalcLineList
            xauto(Get, Set);

    // Common marking
    ref<CMarkerIface> x_rMarker
            xauto(Get);

    // Key to Index Type Identifier
    type<CCalcLineType> GetKey();
    void InitKey(
            type<CCalcLineType> typeCalcLineType);

    // Current marker position on media image
    num x_numMarkerPosInDib = OO
            xauto(Get)
            xassert(true);
    void x_numMarkerPosInDib_Set(
            num numValue);

    // Original value may need be restored
    num GetRestoreOriginalIndexValueRoundedOrOo();

// Operations

    // Handle mode change
    void PrepareCalcLineForCalcModeWuu(
            type<CUsoundCalcModeIfaceGp> typeUsoundCalcMode);

// UI

    // CalcLine
    ref<UiContainer> NewUiCalcLine();
    ptrmap<CUiPanelGp> _m_ptrmapUiCalcLine;
    void ShapeCalcLine(
            ref<CUiPanelGp> rUi);

    // Move hander
    void HandleLineOnMouse(
            ref<CUiPanelGp> rUi,
            EMouse eMouse);

    // Paint handler
    void HandleLineOnPaint(
            draw dc,
            ref<CUiPanelGp> rUi);

    // CalcLineLabel
    ref<UiContainer> NewUiCalcLineLabel();
    ptrmap<CUiPanelGp> _m_ptrmapUiCalcLineLabel;
    void ShapeCalcLabel(
            ref<CUiPanelGp> rUi);

protected:
private:

    // identification key
    type<CCalcLineType> m_typeCalcLineType;

    // they want original values to be restored on calc off
    num _m_numRestoreOriginalIndexValueRounded = OO;

    rect CalcLineOrLabelShape(
            int nLabelOffset);

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiCalcLine.cpp)
class UiCalcLine : public UiContainer
{
public:
    virtual void Ui();

    ref<CCalcLineState> m_rCalcLineState;
};

// (UiCalcLineLabel.cpp)
class UiCalcLineLabel : public UiContainer
{
public:
    virtual void Ui();

    ref<CCalcLineState> m_rCalcLineState;
};
