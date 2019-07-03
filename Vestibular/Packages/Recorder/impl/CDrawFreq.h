// CDrawFreq.h
#pragma once

#include "CDrawAnythingRecorded.h"

class CDrawFreqAxis;
class CProcedureConfigIface;
class CRecordTypeIface;

//
// CDrawFreq - 
//
//   FFT Graphs
//
//ASSUME_IMPL_FOR_IFACE(CDrawFreqImpl, CDrawFreq)

class CDrawFreq : public CDrawAnythingRecorded
{
public:
    CDrawFreq();
    //NEW_LOCAL(CDrawFreqImpl, CDrawFreq)
    //NEW_GP(MyProject, CDrawFreqImpl, CDrawFreq)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent - see base

    ref<CDrawFreqAxis> GetDrawFreqAxis(
            ERouteAxis eRouteAxis);

// Operations

// UI

    // DrawFreqGraph
    ref<UiContainer> NewUiDrawFreqGraphs();
    ptrmap<CUiPanelGp> _m_ptrmapUiDrawFreqGraphs;

protected:
    virtual void OnHandleRecordingChange()
            ;
    virtual void OnResetScalling()
            ;
    virtual void OnFinalizeAtRecordingStop()
            ;
    virtual void OnUpdateGraphNow()
            ;
    virtual ref<UiContainer> OnNewUiDrawMainGraph()
            ;
private:

    array< ref<CDrawFreqAxis> > m_aDrawFreqAxis;

    void _init_AttachToDrawFreq();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiDrawFreqGraphs.cpp)
class UiDrawFreqGraphs : public UiContainer
{
public:
    virtual void Ui();

    ref<CDrawFreq> m_rDrawFreq;
};
