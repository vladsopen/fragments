// CClassTestingTempIfaceGp.h
#pragma once

#include "VirtOs_text.h"

//
// CClassTestingTempIfaceGp -
//
//   Temporary app state existing only for a single class test run
//
//ASSUME_IMPL_FOR_IFACE(CClassTestingTempIface, CClassTestingTempIfaceGp)
//ASSUME_IMPL_FOR_IFACE(CClassTestingTempImpl, CClassTestingTempIface)

class CClassTestingTempIfaceGp : public object
{
public:
    CClassTestingTempIfaceGp();
    //NEW_LOCAL(CClassTestingTempImpl, CClassTestingTempIfaceGp)
    NEW_GP(Debux, CClassTestingTempImpl, CClassTestingTempIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Force sys::GGetCommandLine emulation
    text _x_textTestCommandLine
            xauto(Get)
            xassert(true);
    void AddToTestCommandLine(
            str sAddOption)
            vhook;
    str GetTestCommandLineAsString()
            vhook;

    // see DELAY_TEST_UNTIL_PROJECT_LOADED()
    bool x_bCancelAndDelayCurrentTest = false
            xauto(Get, Set);

    // ONLYPORT cannot reproduce exact packed sizes
    bool x_bPleaseDontReallyCompress = false
            xauto(Get, Set);

// Operations

// UI

protected:
    virtual void OnAddToTestCommandLine(
            str sAddOption)
            v1pure;
    virtual str OnGetTestCommandLineAsString()
            v1pure;
private:
    //bool _m_bOneTimeInitClassTestingTempIfaceGpOk = false;

    void _init_AttachToClassTestingTempIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
