// CDrawCoordAxis.h
#pragma once

#include "CDrawAnythingTimed.h"

class CRulersForTimed;
class CDrawCoords;
class CProcedureConfigIface;
class CRecordTypeIface;

//
// CDrawCoordAxis -
//
//   [X][Y] Graph
//
//ASSUME_IMPL_FOR_IFACE(CDrawCoordAxisImpl, CDrawCoordAxis)

class CDrawCoordAxis : public CDrawAnythingTimed
{
public:
    CDrawCoordAxis();
    //NEW_LOCAL(CDrawCoordAxisImpl, CDrawCoordAxis)
    //NEW_GP(MyProject, CDrawCoordAxisImpl, CDrawCoordAxis)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // Scaling
    static const num C_numInitialMaxCoordAxis = 1;

// Attributes

    // parent
    ptr<CDrawCoords> _x_pDrawCoords
            xauto(Get, Set);

    // Rulers
    ref<CRulersForTimed> x_rRulersForCoordAxis
            xauto(Get);

// Operations

    // Scale indicator
    str GetAmplitudeScaleStatus();

    // scaling indication
    num GetCurrentMaxCoordAxis();

    // Coordinate range
    //num GetRecordingMaxAmplitudeRangeMm();

    // Range to scale MM graph
    num GetVisibleAmplitudeRangeMm();

    // X or Y?
    str GetCoordinateName()
            vhook;

    // X or Y?
    num GetRelevantCoordinate(
            num numPosX,
            num numPosY,
            num numKilo)
            vhook;

    // Update everything
    void UpdateCoordRanges();

// UI

    // DrawCoordAxisGraph
    ref<UiContainer> NewUiDrawCoordAxisGraph();
    ptrmap<CUiPanelGp> _m_ptrmapUiDrawCoordAxisGraph;
    ptrmap<CUiPanelGp> _m_ptrmapUiMaxCoordAxisIndicator;

    // Grapher
    void DrawCoordAxisGraph(
            draw dc,
            ref<CUiPanelGp> rUi);

protected:
    // base
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
    virtual num OnGetAmplitudeRangeNegative()
            ;

    // new
    virtual str OnGetCoordinateName()
            v1pure;
    virtual num OnGetRelevantCoordinate(
            num numPosX,
            num numPosY,
            num numKilo)
            v1pure;

private:

    // scale to draw at
    //num m_numAmplitudeScale = C_numInitialMaxCoordAxis;

    // Drawing
    void DrawCoordAxisBack(
            draw dc,
            ref<CUiPanelGp> rUi);
    void DrawAllCoordAxisGraphs(
            draw dc,
            ref<CUiPanelGp> rUi);
    void DrawCoordAxisForRecordType(
            draw dc,
            ref<CUiPanelGp> rUi,
            type<CRecordTypeIface> typeRecordType,
            ref<CProcedureConfigIface> rProcedureConfig);

    void _init_AttachToDrawCoordAxis();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiDrawCoordAxisGraph.cpp)
class UiDrawCoordAxisGraph : public UiContainer
{
public:
    virtual void Ui();

    ref<CDrawCoordAxis> m_rDrawCoordAxis;

private:
    alikex _m_alikexVerticalRuler;
};
