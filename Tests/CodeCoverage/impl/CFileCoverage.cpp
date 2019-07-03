// CFileCoverage.cpp

#include "CDebuxProject.h"
#include "CFileCoverage.h"
#include "VirtOs_virtfs.h"

CFileCoverage::CFileCoverage()
{
}

void CFileCoverage::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CFileCoverage!");
}

void CFileCoverage::TraceFileCoverage(
        coverage::ECoverageHook eCoverageHook,
        int iPoint,
        int nPoints,
        const int* _unsafe_anLines,
        const char* szFunction,
        const char* szSourceFile,
        int nLine)
{
    nLine;
    eCoverageHook;
    szFunction;

    rASSERT(nPoints > 0);

    // skip last meaningless zero point terminator
    if ok(
        nPoints > 0 &&
        _unsafe_anLines &&
        _unsafe_anLines[nPoints - 1] == 0)
    {
        // remove last zero
        nPoints--;
    }

    // autoinit the array
    if (_m_abHitPoints.GetCount() != nPoints)
    {
        rASSERT(_m_abHitPoints.GetCount() == 0);

        InitFileCoverageLines(
            nPoints,
            _unsafe_anLines,
            virtfs::GVirtualizeLocalOsPath((ospath) (str) szSourceFile));
    }

    _m_abHitPoints.SetAt(iPoint, true);
}

void CFileCoverage::InitFileCoverageLines(
        int nPoints,
        const int* _unsafe_anLines,
        path pathSourceFile)
{
    // Autoinit
    if ok(_m_abHitPoints.GetCount() != nPoints)
    {
        rASSERT(_m_abHitPoints.GetCount() == 0);

        // int flags
        _m_abHitPoints.SetCount(nPoints);

        // save the path
        this->x_pathCoveringFile = pathSourceFile;

        // save line ## list
        if ok(_unsafe_anLines)
        {
            rASSERT(_m_anSourceLines.GetCount() == 0);

            _m_anSourceLines.
                SetCount(nPoints);

            _m_anSourceLines.
                _unsafe_ImportRawBuffer(
                    _unsafe_anLines,
                    nPoints);
        }
    }
}

int CFileCoverage::DumpFileCoverageResultsGetMessageCount()
{
    int nMessageCount = 0;
    repeat(iPoint, _m_abHitPoints.GetCount())
    {
        if (!_m_abHitPoints.GetAt(iPoint))
        {
            int nLine = _m_anSourceLines.GetAt(iPoint);

            if (nLine >= 0)
            {
                sys::GOutputDebugStringToDebugger(
                    sys::GFormatMessageWithDebugSourceReference(
                        "block not covered",
                        x_pathCoveringFile,
                        nLine));

                nMessageCount++;
            }
        }
    }

    return nMessageCount;
}

int CFileCoverage::GetTotalInstrumentedPointsFound()
{
    return _m_abHitPoints.GetCount();
}

