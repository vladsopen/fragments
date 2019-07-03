// CCoverageTracerImpl.cpp

#include "CDebuxProject.h"
#include "CCoverageTracerImpl.h"
#include "CFileCoverage.h"
#include "lib__CClassRegistration.h"
#include "lib__classinfo_DClassAttr.h"
#include "VirtOs_virtfs.h"

CCoverageTracerImpl::CCoverageTracerImpl()
{
}

void CCoverageTracerImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CCoverageTracerImpl!");
}

void CCoverageTracerImpl::OnTraceCoverage(
        coverage::ECoverageHook eCoverageHook,
        int iPoint,
        int nPoints,
        int* _unsafe_anLines,
        const char* szFunction,
        const char* szSourceFile,
        int nLine)
{
    ref<CFileCoverage> rFileCoverage = 
        GetFileCoverage(
            virtfs::GVirtualizeLocalOsPath((ospath) (str) szSourceFile));

    rFileCoverage->
        TraceFileCoverage(
            eCoverageHook,
            iPoint,
            nPoints,
            _unsafe_anLines,
            szFunction,
            szSourceFile,
            nLine);
}

ref<CFileCoverage> CCoverageTracerImpl::GetFileCoverage(
        path pathFile)
{
    rASSERT(pathFile != "");

    ref<CFileCoverage> rFileCoverage;
    if (!_m_mapFileCoverage.LookupExisting(pathFile, out rFileCoverage))
    {
        // Create a new entry

        rFileCoverage = _m_mapFileCoverage[pathFile];
    }

    return rFileCoverage;
}

void CCoverageTracerImpl::OnDumpCoverageResults()
{
    rASSERT(!_m_bDumpCalled);
    _m_bDumpCalled = true;

    // if we dont have any logs, dont bother
    if (_m_mapFileCoverage.GetCount() == 0)
    {
        sys::GOutputDebugStringToDebugger(
            "\n" "::::: No Code Coverage Logs.\n\n");
        return;
    }

    _m_nDumpMessageCount = 0;
    _m_nDumpCheckedPointCount = 0;
    _m_nDumpCheckedCoverableFileCount = 0;
    _m_nDumpCheckedTotalFileCount = 0;
    _m_nDumpRemovedWholeFiles = 0;

    debug("Coverage./show.TracingActive:").
        FloatNote("Code coverage is active and it slows down DEBUG mode");

    sys::GOutputDebugStringToDebugger("\n" "::::: Code Coverage Tracer Dump:\n\n");

    ClearFilesWithDisabledCoverage();

    DumpCoverageLines();

    if (_m_nDumpMessageCount > 0)
    {
        sys::GOutputDebugStringToDebugger(
            "\n" "::::: Coverage for files never hit at all will follow:\n\n");
    }

    DumpCoverageMissingFiles();

    sys::GOutputDebugStringToDebugger(
        "\n" 
            "::::: " + 
            Str(_m_nDumpMessageCount) + 
            " points not covered in tests,\n"
            "::::: " + 
            Str(_m_nDumpCheckedPointCount) + 
            " instrumented points found,\n"
            "::::: " + 
            Str(_m_nDumpCheckedCoverableFileCount) + 
            " instrumented files out of " +
            Str(_m_nDumpCheckedTotalFileCount) + 
            " total Generic class files,\n"
            "::::: " + 
            Str(_m_nDumpRemovedWholeFiles) + 
            " files removed from log due to global NOTEST but partially tested.\n"
            "\n");
}

void CCoverageTracerImpl::DumpCoverageLines() 
{
    ref<CFileCoverage> rIterFileCoverage;
    for (
        iter i;
        _m_mapFileCoverage.
            Iterate(
                out i,
                out rIterFileCoverage);)
    {
        LogFileCoverage(rIterFileCoverage);
    }
}

void CCoverageTracerImpl::LogFileCoverage(
        ref<CFileCoverage> rFileCoverage)
{
    _m_nDumpMessageCount += 
        rFileCoverage->
            DumpFileCoverageResultsGetMessageCount();

    _m_nDumpCheckedPointCount += 
        rFileCoverage->
            GetTotalInstrumentedPointsFound();
}

void CCoverageTracerImpl::ClearFilesWithDisabledCoverage()
{
    // Scan all visible classes
    classinfo classinfoIter;
    bool bMapChangeBreak = false;
    for (
        iter i; 
        classinfo::GGetClassRegistration().
            IterateRegisteredClassesBreakIfRemapped(
                out i, 
                out classinfoIter,
                out bMapChangeBreak);)
    {
        DClassAttr* pdataClassAttr = classinfoIter._internal_GetStaticClassAttr();
        if ok(pdataClassAttr)
        {
            _m_nDumpCheckedTotalFileCount++;

            path pathSource = pdataClassAttr->m_pathImplementedInSource;
            if (pdataClassAttr->m_anCoverageLines.GetCount() == 0 &&
                _m_mapFileCoverage.IsIn(pathSource))
            {
                _m_nDumpRemovedWholeFiles++;
                _m_mapFileCoverage.RemoveAtKey(pathSource);
            }
        }
    }

    rASSERT(!bMapChangeBreak);
}

void CCoverageTracerImpl::DumpCoverageMissingFiles()
{
    // Scan all visible classes
    classinfo classinfoIter;
    bool bMapChangeBreak = false;
    for (
        iter i; 
        classinfo::GGetClassRegistration().
            IterateRegisteredClassesBreakIfRemapped(
                out i, 
                out classinfoIter,
                out bMapChangeBreak);)
    {
        DClassAttr* pdataClassAttr = classinfoIter._internal_GetStaticClassAttr();
        if ok(pdataClassAttr)
        {
            _m_nDumpCheckedTotalFileCount++;

            if (pdataClassAttr->m_anCoverageLines.GetCount() > 0)
            {
                _m_nDumpCheckedCoverableFileCount++;

                // Check if any coverage logs recorded for the file
                CheckIfFileCoverageLogged(
                    pdataClassAttr->m_pathImplementedInSource,
                    pdataClassAttr->m_anCoverageLines);
            }
        }
    }

    rASSERT(!bMapChangeBreak);
}

void CCoverageTracerImpl::CheckIfFileCoverageLogged(
        path pathToBeCovered,
        const array<int>& anCoverageLines)
{
    if (!_m_mapFileCoverage.IsIn(pathToBeCovered))
    {
        if (!coverage::GIsFileNameExcludedFromCoverage(pathToBeCovered))
        {
            // Register as a new file in the map
            ref<CFileCoverage> rFileCoverage = 
                GetFileCoverage(
                    pathToBeCovered);

            rASSERT(anCoverageLines.GetCount() > 0);
            rASSERT(anCoverageLines._unsafe_GetPointer() != NULL);

            // Init with coverage info from the classinfo
            rFileCoverage->
                InitFileCoverageLines(
                    anCoverageLines.GetCount(),
                    anCoverageLines._unsafe_GetPointer(),
                    pathToBeCovered);

            // Trace everything as uncovered
            LogFileCoverage(rFileCoverage);

            /*
            sys::GOutputDebugStringToDebugger(
                sys::GFormatMessageWithDebugSourceReference(
                    "file not covered at all",
                    pathToBeCovered,
                    1));
                    */

            _m_nDumpMessageCount++;
        }
    }
}

