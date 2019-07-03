// CCodeMarking.h

class CAlterCtaMode;
class CWriter;

//
// CCodeMarking - creates a code marking list for test code coverage
//

class CCodeMarking : public CCommon
{
public:
    GENERIC(CCodeMarking, CCommon);
    CCodeMarking();

// Constants

    #define C_szCodeCoverageCounterId "Cover"
                // "s_anCtaLnCover"

// Attributes

    // Set to false inside 'NOTEST' blocks
    bool x_bEnableCoverageHooks;

// Operations

    // 'Counter' handler called from all translation modes
    void HandleCounterEncounterGetExtendModeToken(
            str sFlagName,
            ETranslationMode eTranslationMode,
            CExpression* pArgExpression,
            CAlterCtaMode* pParentAlterCtaMode);

    // Automatic coverage decision maker
    str DecideTokenCoverageGetCode(
            CToken* pSourceToken,
            out int& out_iLineEntry);

    // Lower level counter code (for COVERAGE functions)
    str GenerateCounterEncounterCode(
            str sPrefixedCounterId,
            int nLine,
            out int& out_iLineEntry);

    // Wraps all transform logic for FN/BRACE
    enum EInsertCoverage { E_InsertCoverage_AtStart, E_InsertCoverage_AfterEnd };
    void HandleEnterCoverageHook(
            str sInsertMacroName, // CTA_FN/CTA_BRACE
            CStatementList* pStatementListToInsertHooks,
            EInsertCoverage eInsertCoverage);

    // Remove line number from coverage report array
    void CancelLineCoverageAt(
            int iLineEntry,
            str sPrefixedCounterId);

    // Intro code generator
    str WriteCodeMarkingIntroBeforeRegenerate();

    // Check and remove CTA_NOCOVERAGE marker
    bool CheckIfCoverableCode(
            CStatementList* pStatementList);

protected:

private:

    // counter stats
    map<str, array<int> > m_mapPrefixedCounterIdToLineList;

    // Name for Max constant
    str GetCounterLineListName(
            str sPrefixedFlagName);

    // Arg parser helper
    str ParseCodeMarkingCounterId(
            str sFlagName,
            ETranslationMode eTranslationMode,
            CExpression* pArgExpression,
            CToken* pParentAlterCtaMode);    
};

