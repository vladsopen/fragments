// DComplexityStats.h
// (see "DComplexityStats.cpp")

//
// DComplexityStats - counts statement types in a given block
//

class DComplexityStats
{
public:
    DComplexityStats();

    int xx_nTotalTokens;
    int GetActualJoinedTokens();

    int xx_nStatements;
    int xx_nDeclarations;
    int xx_nClassBodyCount;
    int xx_nFunctionBodyCount;
    int xx_nExecutableStatements;
    int xx_nExpressions;
    int xx_nAngleBrackets;
    int xx_nParens;
    int xx_nCompounds;
    int xx_nUnqualifiedNames;
    int xx_nQualifiedNames;
    //int xx_nOperations; can't count
    int xx_nLoops;
    int xx_nIfs;
    int xx_nCatchBlocks;
    bool xx_bDisableHairyMethodHookFound;

    str GetStatsReport();
};