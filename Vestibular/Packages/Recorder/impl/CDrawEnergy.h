// CDrawEnergy.h
#pragma once

#include "CDrawAnythingTimed.h"

class CProcedureConfigIface;
class CRecordTypeIface;

//
// CDrawEnergy - 
//
//   [A] Graph
//
//ASSUME_IMPL_FOR_IFACE(CDrawEnergyImpl, CDrawEnergy)

class CDrawEnergy : public CDrawAnythingTimed
{
public:
    CDrawEnergy();
    //NEW_LOCAL(CDrawEnergyImpl, CDrawEnergy)
    //NEW_GP(MyProject, CDrawEnergyImpl, CDrawEnergy)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // Scaling
    static const num C_numInitialMaxEnergy = 0.01;

// Attributes

    // parent - see base

// Operations

    // Scale indicator
    str GetAmplitudeScaleStatus();

    // scaling indication
    num GetCurrentMaxEnergy();

    // Coordinate range 
    //num GetRecordingMaxAmplitudeRangeMm();

    // Range to scale MM graph
    num GetVisibleAmplitudeRangeMm();

// UI

    // DrawEnergyGraph
    ref<UiContainer> NewUiDrawEnergyGraph();
    ptrmap<CUiPanelGp> _m_ptrmapUiDrawEnergyGraph;
    ptrmap<CUiPanelGp> _m_ptrmapUiMaxEnergyIndicator;

    // Grapher
    void DrawEnergyGraph(
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
    //num m_numAmplitudeScale = C_numInitialMaxEnergy;

    // Drawing
    void DrawEnergyBack(
            draw dc,
            ref<CUiPanelGp> rUi);
    void DrawAllEnergyGraphs(
            draw dc,
            ref<CUiPanelGp> rUi);
    void DrawEnergyForRecordType(
            draw dc,
            ref<CUiPanelGp> rUi,
            type<CRecordTypeIface> typeRecordType,
            ref<CProcedureConfigIface> rProcedureConfig);

    void _init_AttachToDrawEnergy();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiDrawEnergyGraph.cpp)
class UiDrawEnergyGraph : public UiContainer
{
public:
    virtual void Ui();

    ref<CDrawEnergy> m_rDrawEnergy;

private:
    alikex _m_alikexVerticalRuler;
};
