// CSequence.h
#pragma once

#include "VirtOs_ontimer.h"

class CStepAbstract;
class CProxyImpl;

//
// CSequence - 
//
//   An abstract algorithm consisting of 'Steps'
//
//ASSUME_IMPL_FOR_IFACE(CSequenceImpl, CSequence)

class CSequence : public object
{
public:
    CSequence();
    ~CSequence();
    //NEW_LOCAL(CSequenceImpl, CSequence)
    //NEW_GP(CustomApps, CSequenceImpl, CSequence)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CProxyImpl> _x_pProxy
            xauto(Get, Set);
    ref_ret<CProxyImpl> GetProxy()
            return
                _x_pProxy.Get().Valid();

    // Cur step
    str x_sDebugLastPollStatus = "No polling."
            xauto(Get, Set)
            xassert(true);

    // Current step
    ptr<CStepAbstract> _x_pLastPolledStep = null()
            xauto(Get, Set);

// Operations

    // One-time opener
    void InitSequence();

    // Cleanup
    void CloseSequence();

    // Steps (for UI)
    bool IterateSteps(
            out iter& out_i,
            out ref<CStepAbstract>& out_rStep);

// UI

    // SequenceList
    ref<UiContainer> NewUiSequenceList();
    ptrmap<CUiPanelGp> _m_ptrmapUiSequenceList;

protected:
private:
    bool _m_bOneTimeInitSequenceOk = false;
    bool _m_bSequenceOpened = false;

    array< ref<CStepAbstract> > m_arStep;

    // a timer for demo animation
    void HandleSequenceTimer();
    ontimer<HandleSequenceTimer> _m_ontimerSequence;

    void _init_AttachToSequence();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiSequenceList.cpp)
class UiSequenceList : public UiContainer
{
public:
    virtual void Ui();

    ref<CSequence> m_rSequence;

private:
};
