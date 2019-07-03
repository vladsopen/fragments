// CDrawRoute.h
#pragma once

#include "CDrawAnythingRouted.h"

class CRectIface;
class CRangesIface;
class CRecordTypeIface;

//
// CDrawRoute - 
//
//   Paints the recorded path.
//
//ASSUME_IMPL_FOR_IFACE(CDrawRouteImpl, CDrawRoute)

class CDrawRoute : public CDrawAnythingRouted
{
public:
    CDrawRoute();
    //NEW_LOCAL(CDrawRouteImpl, CDrawRoute)
    //NEW_GP(MyProject, CDrawRouteImpl, CDrawRoute)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // Scaling
    //static const num C_numInitialScale = 10;
    static const num C_numMinVisibleMm = 5;
    static const num C_numMinMarginMm = 2;
    static const num C_numStepMarginMm = 5;

// Attributes

    // parent - see base

    // scaling indication
    ref<CRectIface> GetCurrentVisibleRect();

// Operations

// UI

    // DrawRouteGraph
    ref<UiContainer> NewUiDrawRouteGraph();
    ptrmap<CUiPanelGp> _m_ptrmapUiDrawRouteGraph;
    ptrmap<CUiActionGp> _m_ptrmapUiDrawRouteScaleIndicator;

    // Grapher
    void DrawRouteGraph(
            draw dc,
            ref<CUiPanelGp> rUi);

protected:
    virtual void OnHandleRecordingChange()
            ;
    virtual void OnUpdateGraphNow()
            ;
    virtual void OnResetScalling()
            ;
    virtual ref<UiContainer> OnNewUiDrawMainGraph()
            ;
    virtual ref<CRectIface> OnGetRouteGraphVisibleRect()
            ;
    virtual bool OnIsCoordinateOffsetDrawnInGrid()
            ;
    virtual num OnGetRouteGridStepMm()
            ;
private:

    // scale to draw at
    //num m_numDrawScale = 10.;

    // Drawing
    void DrawRouteBack(
            draw dc,
            ref<CUiPanelGp> rUi,
            ref<CRectIface> rRect);
    void DrawAllRoutes(
            draw dc,
            ref<CUiPanelGp> rUi,
            ref<CRectIface> rRect);
    int DrawRouteForRecordType(
            draw dc,
            ref<CUiPanelGp> rUi,
            type<CRecordTypeIface> typeRecordType,
            ref<CRectIface> rRect,
            ref<CRectIface> rNewRect);

    void _init_AttachProperties();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiDrawRouteGraph.cpp)
class UiDrawRouteGraph : public UiContainer
{
public:
    virtual void Ui();

    ref<CDrawRoute> m_rDrawRoute;

private:
    alikex _m_alikexVerticalRuler;
};
