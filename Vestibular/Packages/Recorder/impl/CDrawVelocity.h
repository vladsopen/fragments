// CDrawVelocity.h
#pragma once

#include "CDrawAnythingTimed.h"

class CProcedureConfigIface;
class CRecordTypeIface;

//
// CDrawVelocity - 
//
//   [V] Graph
//
//ASSUME_IMPL_FOR_IFACE(CDrawVelocityImpl, CDrawVelocity)

class CDrawVelocity : public CDrawAnythingTimed
{
public:
    CDrawVelocity();
    //NEW_LOCAL(CDrawVelocityImpl, CDrawVelocity)
    //NEW_GP(MyProject, CDrawVelocityImpl, CDrawVelocity)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // Scaling
    static const num C_numInitialScale = 3000;
    static const num C_numScaleGoDownSlow1From = 1000;
    static const num C_numScaleGoDownSlow2From = 100;
    static const num C_numScaleGoDownModerateStep = 10;

// Attributes

    // parent - see base

// Operations

    // Scale indicator
    str GetAmplitudeScaleStatus();

    // scaling indication
    num GetCurrentAmplitudeScale();

    // Coordinate range 
    num GetRecordingMaxAmplitudeRangeMm();

    // Range to scale MM graph
    num GetVisibleAmplitudeRangeMm();

// UI

    // DrawVelocityGraph
    ref<UiContainer> NewUiDrawVelocityGraph();
    ptrmap<CUiPanelGp> _m_ptrmapUiDrawVelocityGraph;
    ptrmap<CUiPanelGp> _m_ptrmapUiVelocityScaleIndicator;

    // Grapher
    void DrawVelocityGraph(
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
    virtual num OnGetAmplitudeRange()
            ;

private:

    // scale to draw at
    //num m_numAmplitudeScale = C_numInitialScale;

    // Drawing
    void DrawVelocityBack(
            draw dc,
            ref<CUiPanelGp> rUi);
    void DrawAllVelocityGraphs(
            draw dc,
            ref<CUiPanelGp> rUi);
    void DrawVelocityForRecordType(
            draw dc,
            ref<CUiPanelGp> rUi,
            type<CRecordTypeIface> typeRecordType,
            ref<CProcedureConfigIface> rProcedureConfig);

    void _init_AttachToDrawVelocity();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiDrawVelocityGraph.cpp)
class UiDrawVelocityGraph : public UiContainer
{
public:
    virtual void Ui();

    ref<CDrawVelocity> m_rDrawVelocity;

private:
    alikex _m_alikexVerticalRuler;
};
