// CFileCoverage.h
#pragma once

//
// CFileCoverage - 
//
//  Trace code coverage stats for a single source file.
//
//ASSUME_IMPL_FOR_IFACE(CFileCoverageImpl, CFileCoverage)

class CFileCoverage : public object
{
public:
    CFileCoverage();
    //NEW_LOCAL(CFileCoverageImpl, CFileCoverage)
    //NEW_GP(MyProject, CFileCoverageImpl, CFileCoverage)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    path x_pathCoveringFile = ""
            xauto(Get)
            xassert(value != "");

    int GetTotalInstrumentedPointsFound();

// Operations

    // Called for both code hooks
    void TraceFileCoverage(
            coverage::ECoverageHook eCoverageHook,
            int iPoint,
            int nPoints,
            const int* _unsafe_anLines,
            const char* szFunction,
            const char* szSourceFile,
            int nLine);

    // Called only for files missing any hits with TraceFileCoverage()
    void InitFileCoverageLines(
            int nPoints,
            const int* _unsafe_anLines,
            path pathSourceFile);

    // After tests done
    int DumpFileCoverageResultsGetMessageCount();

// UI

protected:
private:

    // hit points
    array<bool> _m_abHitPoints;
    array<int> _m_anSourceLines;

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

