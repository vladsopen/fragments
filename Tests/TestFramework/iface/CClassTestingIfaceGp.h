// CClassTestingIfaceGp.h
#pragma once

class CClassTestingTempIfaceGp;
class CTestingOptionsIfaceGp;

//
// CClassTestingIfaceGp -
//
//  ClassTest controller for CProject
//
ASSUME_IMPL_FOR_IFACE(CClassTestingImpl, CClassTestingIfaceGp)

class CClassTestingIfaceGp : public object
{
public:
    CClassTestingIfaceGp();
    //NEW_LOCAL(CClassTestingImpl, CClassTestingIfaceGp)
    NEW_GP(Debux, CClassTestingImpl, CClassTestingIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    static const str C_sClassTestsAborted = "Class tests aborted!";

    static const str C_sNoTestSwitch = "--disabletests";

// Attributes

    // Call only in ClassTest mode
    ref<CTestingOptionsIfaceGp> GetRunningTestingOptions()
            vhook;

    // Temporary app state existing only for a signgle class test run
    ref<CClassTestingTempIfaceGp> x_rClassTestingTemp
            xauto(Get);

// Operations

    // Main ClassTest entry
    void RunClassTests(
            ref<CTestingOptionsIfaceGp> rTestingOptions)
            vhook;

    // call debug::GRestartVirtualTime()!
    void _internal_RestartVirtualTime()
            vhook;

// UI

protected:
    virtual void OnRunClassTests(
            ref<CTestingOptionsIfaceGp> rTestingOptions)
            v1pure;
    virtual ref<CTestingOptionsIfaceGp> OnGetRunningTestingOptions()
            v1pure;
    virtual void On_internal_RestartVirtualTime()
            v1pure;

private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
