// CCoverageTracerIfaceGp.h
#pragma once

//
// CCoverageTracerIfaceGp -
//
//   Global code coverage stats.
//
ASSUME_IMPL_FOR_IFACE(CCoverageTracerImpl, CCoverageTracerIfaceGp)

class CCoverageTracerIfaceGp : public object
{
public:
    CCoverageTracerIfaceGp();
    //NEW_LOCAL(CCoverageTracerImpl, CCoverageTracerIfaceGp)
    NEW_GP(Debux, CCoverageTracerImpl, CCoverageTracerIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

// Operations

    // Called for every function or { block[iPoint] }
    void TraceCoverage(
            coverage::ECoverageHook eCoverageHook,
            int iPoint,
            int nPoints,
            int* _unsafe_anLines,
            const char* szFunction,
            const char* szSourceFile,
            int nLine)
            vhook;

    // Called after class tests
    void DumpCoverageResults()
            vhook;

// UI

protected:
    virtual void OnTraceCoverage(
            coverage::ECoverageHook eCoverageHook,
            int iPoint,
            int nPoints,
            int* _unsafe_anLines,
            const char* szFunction,
            const char* szSourceFile,
            int nLine)
            v1pure;
    virtual void OnDumpCoverageResults()
            v1pure;

private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
