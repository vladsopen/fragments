// CMagicStrGenerator.h
// (see "CMagicStrGenerator.cpp")

//
// CMagicStrGenerator - concatenates String expression tokens into MagicStr macro.
//   Helps preallocate string constants as cache all kinds of static string assocs.
//

class CMagicStrGenerator : public CCommon
{
public:
    GENERIC(CMagicStrGenerator, CCommon);
    CMagicStrGenerator();

// Constants

// Attributes

// Operations

    // Simple wrapper
    static str GGenerateMagicStrFromString(
            sbuf sbufWrapThis,
            nullable<CToken*> npTokenForErrorContext);
    str GenerateMagicStrFromString(
            sbuf sbufWrapThis,
            nullable<CExpression*> npSourceExpressionForAssert,
            nullable<CExpression*> npGeneratedStringChainExpression,
            nullable<CToken*> npTokenForErrorContext);

    // Start sequence generation process
    void StartAtRootExpression(
            CExpression* pExpression,
            CWriter* pWriter);

    // Continue scan for subexpression
    void DiveIntoSubExpression(
            CExpression* pExpression,
            CWriter* pWriter);

private:
    bool m_bInsideMagicStrSequence;
    bool m_bMagicStrModeStartedInFile;
    array<CToken*> m_apMagicStrToken;
    bool m_bPlusSuppressed;
    bool m_bMidZeroExpected;
    bool m_bStrPrecalcRequested;
    sbuf m_sbufForceChallengeSource;

    void EmitMagicStrAtEndOfExpression(
            CExpression* pExpression,
            CWriter* pWriter,
            bool bPlusSuppressed);

    // Converts C-syntax string to binary buffer 
    //   (using str::GParseCeeStringConst())
    static sbuf GConvertCeeStringToBuffer(
            str sSource,
            CToken* pSourceToken);

    // Encryption support
    int PrecalcHash(
            sbuf sbufWrapThis,
            nullable<CToken*> npTokenForErrorContext);

    // Verification helper
    void ParseBigChallenge(
            CExpression* pExpression,
            out int& out_iToken,
            out str& out_sError);
};
