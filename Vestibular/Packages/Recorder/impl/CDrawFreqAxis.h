// CDrawFreqAxis.h
#pragma once

#include "CDrawAnythingRecorded.h"

class CRulersForFreq;
class CDrawFreq;
class CProcedureConfigIface;
class CRecordTypeIface;

//
// CDrawFreqAxis -
//
//   FFT Graphs
//
//ASSUME_IMPL_FOR_IFACE(CDrawFreqAxisImpl, CDrawFreqAxis)

class CDrawFreqAxis : public object
{
public:
    CDrawFreqAxis();
    //NEW_LOCAL(CDrawFreqAxisImpl, CDrawFreqAxis)
    //NEW_GP(MyProject, CDrawFreqAxisImpl, CDrawFreqAxis)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    static num GGetMaxDrawnFreq()
        return
            debug("Calc.Freq.MaxHz?/4e").InputNumOrSet(6); // Hz

// Attributes

    // parent
    ptr<CDrawFreq> _x_pDrawFreq
            xauto(Get, Set);

    // Overridable axis ID
    ERouteAxis GetRouteAxis()
            vhook;

    num GetMaxFreqAmplitude();

    str GetAmplitudeScaleStatus();

    // Rulers
    ref<CRulersForFreq> x_rRulersForFreq
            xauto(Get);

// Operations

    void ResetScalling();

    void ForceMaxAmplitude(
            num numSuperMaxAmplitude);

// UI

    // DrawFreqAxisGraph
    ref<UiContainer> NewUiDrawFreqAxisGraph();
    ptrmap<CUiPanelGp> _m_ptrmapUiDrawFreqAxisGraph;
    ptrmap<CUiPanelGp> _m_ptrmapUiFreqScaleIndicator;

    // Grapher
    void DrawFreqAxisGraph(
            draw dc,
            ref<CUiPanelGp> rUi);

protected:
    virtual ERouteAxis OnGetRouteAxis()
            v1st
            return
                ERouteAxis(-1);
private:

    // scale to draw at
    //num m_numAmplitudeScale = C_numInitialScale;

    num m_numLastMaxAmplitudeForRulers = 0;

    // Drawing
    void DrawFreqAxisBack(
            draw dc,
            ref<CUiPanelGp> rUi);
    void DrawAllFreqGraphs(
            draw dc,
            ref<CUiPanelGp> rUi);
    void DrawFreqAxisForRecordType(
            draw dc,
            ref<CUiPanelGp> rUi,
            type<CRecordTypeIface> typeRecordType,
            ref<CProcedureConfigIface> rProcedureConfig);

    void _init_AttachToDrawFreqAxis();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiDrawFreqAxisGraph.cpp)
class UiDrawFreqAxisGraph : public UiContainer
{
public:
    virtual void Ui();

    ref<CDrawFreqAxis> m_rDrawFreqAxis;

private:
    alikex _m_alikexVerticalRuler;
};
