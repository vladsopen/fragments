// CClassTestGlobalsIface.h
#pragma once

#include "CClassTestGlobalsIfaceGp.h"

//
// CClassTestGlobalsIface - see CClassTestGlobalsIfaceGp
//
//ASSUME_IMPL_FOR_IFACE(CClassTestGlobalsImpl, CClassTestGlobalsIface)

class CClassTestGlobalsIface : public CClassTestGlobalsIfaceGp
{
public:
    CClassTestGlobalsIface();
    //~CClassTestGlobalsIface();
    NEW_LOCAL(CClassTestGlobalsImpl, CClassTestGlobalsIface)
    //NEW_GP(Debux, CClassTestGlobalsImpl, CClassTestGlobalsIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

    // One-time opener
    //void InitClassTestGlobals();

    // Cleanup
    //void CloseClassTestGlobals();

    // Support for run-all-starters
    void GuardInsideDebuxStartersTest(
            bool bInside)
            vhook;
    bool IsInsideDebuxStartersTest()
            vhook;

// UI

    // CCES
    ref<UiContainer> NewUiClassTestGlobalsCces()
            vhook;

protected:
    SEE_ALSO(CClassTestGlobalsImpl)

    virtual void OnGuardInsideDebuxStartersTest(
            bool bInside)
            v1pure;
    virtual bool OnIsInsideDebuxStartersTest()
            v1pure;
    virtual ref<UiContainer> OnNewUiClassTestGlobalsCces()
            v1pure;

private:
    //bool _m_bOneTimeInitClassTestGlobalsIfaceOk = false;
    //bool _m_bClassTestGlobalsIfaceOpened = false;

    void _init_AttachToClassTestGlobalsIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
