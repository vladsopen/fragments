// CDrawDisposition.h
#pragma once

#include "CDrawAnythingRouted.h"

class CRangesIface;
class CRecordTypeIface;

//
// CDrawDisposition - 
//
//   Paints the recorded path.
//
//ASSUME_IMPL_FOR_IFACE(CDrawDispositionImpl, CDrawDisposition)

class CDrawDisposition : public CDrawAnythingRouted
{
public:
    CDrawDisposition();
    //NEW_LOCAL(CDrawDispositionImpl, CDrawDisposition)
    //NEW_GP(MyProject, CDrawDispositionImpl, CDrawDisposition)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent - see base

    // Feet scaling area
    size x_sizeLastGraphBack = size()
            xauto(Get, Set)
            xassert(true);

// Operations

// UI

    // DrawDispositionGraph
    ref<UiContainer> NewUiDrawDispositionGraph();
    ptrmap<CUiPanelGp> _m_ptrmapUiDrawDispositionGraph;

    // Grapher
    void DrawDispositionGraph(
            draw dc,
            ref<CUiPanelGp> rUi);

    // Feet outside of gtaph
    void DrawDispositionFeet(
            draw dc,
            ref<CUiPanelGp> rUi);

protected:
    virtual void OnHandleRecordingChange()
            ;
    virtual void OnResetScalling()
            vnull;
    virtual ref<UiContainer> OnNewUiDrawMainGraph()
            ;
    virtual bool OnIsCoordinateOffsetDrawnInGrid()
            ;
    virtual num OnGetRouteGridStepMm()
            ;
private:

    // Drawing
    void DrawDispositionBack(
            draw dc,
            ref<CUiPanelGp> rUi);
    void DrawAllRoutes(
            draw dc,
            ref<CUiPanelGp> rUi);
    void DrawDispositionForRecordType(
            draw dc,
            ref<CUiPanelGp> rUi,
            type<CRecordTypeIface> typeRecordType);
    void DrawIdealPressurePoint(
            draw dc,
            ref<CUiPanelGp> rUi);

    void _init_AttachProperties();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiDrawDispositionGraph.cpp)
class UiDrawDispositionGraph : public UiContainer
{
public:
    virtual void Ui();

    ref<CDrawDisposition> m_rDrawDisposition;

private:
    alikex _m_alikexVerticalRuler;
    alikex _m_alikexHorizontalRuler;
};
