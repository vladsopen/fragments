// DComplexityStats.cpp
// (see "DComplexityStats.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "DComplexityStats.h"

DComplexityStats::DComplexityStats()
{
    ZeroMemory(this, sizeof(*this));
}

int DComplexityStats::GetActualJoinedTokens()
{
    return
        xx_nTotalTokens -
            xx_nAngleBrackets * 2 -
            xx_nParens;
}

str DComplexityStats::GetStatsReport()
{
    str sReport;

    // Report
    sReport += Str(xx_nTotalTokens);
    sReport += " tokens, ";
    sReport += "(";
    sReport += Str(GetActualJoinedTokens());
    sReport += " joined)";
    sReport += " in ";
    sReport += Str(xx_nStatements);
    sReport += " statms (";
    sReport += Str(xx_nExpressions);
    sReport += " exprs), with ";
    sReport += Str(xx_nUnqualifiedNames);
    sReport += " names (";
    sReport += Str(xx_nQualifiedNames);
    sReport += " qualif), ";
    sReport += Str(xx_nLoops);
    sReport += " loops, ";
    sReport += Str(xx_nIfs);
    sReport += " ifs";

    return sReport;
}