// CClassTestGlobalsImpl.h
#pragma once

#include "CClassTestGlobalsIface.h"

//
// CClassTestGlobalsImpl - see Iface
//

class CClassTestGlobalsImpl : public CClassTestGlobalsIface
{
public:
    CClassTestGlobalsImpl();
    //~CClassTestGlobalsImpl();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Log to view in CCES
    str x_sClassTestTimingLog = ""
            xauto(Get, Set)
            xassert(true);

    // For internal UI to consolidate running with profiler
    bool x_bSkipRunningClassTests = false
            xauto(Get, Set);

    //// Test Dashboard
    //ref<CDebuxSessionImpl> x_rDebuxSession
    //        xauto(Get);

// Operations

    // One-time opener
    //void InitClassTestGlobals();

    // Cleanup
    //void CloseClassTestGlobals();

// UI

    // ClassTestGlobalsCces
    //ref<UiContainer> NewUiClassTestGlobalsCces();
    ptrmap<CUiPanelGp> _m_ptrmapUiClassTestGlobalsCces;

protected:
    SEE_ALSO(CClassTestGlobalsIfaceGp)

    // ... your virtuals from IfaceGp here

    SEE_ALSO(CClassTestGlobalsIface)

    virtual void OnGuardInsideDebuxStartersTest(
            bool bInside)
            ;
    virtual bool OnIsInsideDebuxStartersTest()
            ;
    virtual ref<UiContainer> OnNewUiClassTestGlobalsCces()
            ;

private:
    //bool _m_bOneTimeInitClassTestGlobalsImplOk = false;
    //bool _m_bClassTestGlobalsImplOpened = false;

    bool* GetInsideDebuxStartersTestGuardFlag();

    void _init_AttachToClassTestGlobalsImpl();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiClassTestGlobalsCces.cpp)
class UiClassTestGlobalsCces : public UiContainer
{
public:
    virtual void Ui();

    ref<CClassTestGlobalsImpl> m_rClassTestGlobals;

private:
};
