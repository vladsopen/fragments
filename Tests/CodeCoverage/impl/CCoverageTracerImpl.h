// CCoverageTracerImpl.h
#pragma once

#include "CCoverageTracerIfaceGp.h"

class CFileCoverage;

//
// CCoverageTracerImpl - 
//
//   Global code coverage stats.
//

class CCoverageTracerImpl : public CCoverageTracerIfaceGp
{
public:
    CCoverageTracerImpl();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

// Operations

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
            ;
    virtual void OnDumpCoverageResults()
            ;

private:

    // all encountered files
    map<path, ref<CFileCoverage> > _m_mapFileCoverage;

    // can only be use once
    bool _m_bDumpCalled = false;

    // for info
    int _m_nDumpMessageCount = 0;
    int _m_nDumpCheckedPointCount = 0;
    int _m_nDumpCheckedCoverableFileCount = 0;
    int _m_nDumpCheckedTotalFileCount = 0;
    int _m_nDumpRemovedWholeFiles = 0;

    // Lokkpu file context
    ref<CFileCoverage> GetFileCoverage(
            path pathFile);

    // Log file and update stats
    void LogFileCoverage(
            ref<CFileCoverage> rFileCoverage);

    // Dump lines
    void DumpCoverageLines();

    // Dump totally missing files
    void DumpCoverageMissingFiles();

    // Remove NOTEST makerd files from log
    void ClearFilesWithDisabledCoverage();

    // Dump unlogged but covered files
    void CheckIfFileCoverageLogged(
            path pathToBeCovered,
            const array<int>& anCoverageLines);

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
