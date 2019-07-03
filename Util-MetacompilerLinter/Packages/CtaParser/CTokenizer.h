// CTokenizer.h
// (see "CTokenizer.cpp")
// (see "CTokenizer_CharHandlers.cpp")

class CNameHookMap;
class CTokenizerMacroMap;

//
// CTokenizer - turns source characters from CSourceInput into CToken objects
//

class CTokenizer : public CCommon
{
public:
    GENERIC_WITHOUT_NewOfTheKind(CTokenizer, CCommon);
    CTokenizer(CProcessing* pProcessing);

// Initialization

    void InitTransformers();
    CKeywordMapping* GetKeywordMapping();

// Translation Stage Control

    enum EStage
    {
        E_Stage_Parsing,
        E_Stage_Transforming,
        E_Stage_Writing,
    };

    static void SetStage(EStage eStage);
    static EStage GetStage();

// Processing Control

    CProcessing* GetProcessing();

// Parsing

    // Read next token
    CToken* ReadNextToken(CToken* pParentToken);
    CToken* ReadNextTokenInLowLevelMode(CToken* pParentToken);
    CToken* PeekNextToken(CToken* pParentToken);
    void RollbackToToken(CToken* pAlreadyReadToken);

    // Low level peeking functions,
    //   don't affect current input position or lookup token
    CToken* PeekTempTokenOrSpaceAfter(
            CToken* pPrecedingToken,
            CToken* pParentToken);
    CToken* PeekTempTokenSkippingSpaceAfter(
            CToken* pPrecedingToken,
            CToken* pParentToken);

    // Prepare source text
    void SetSourceInfo(
            str sSourceText,
            path pathInitialSourceFile);
    CSourceLines* GetSourceLines();
    path GetInitialSourceFile();

// Preprocessor

    CNameHookMap* GetNameHookMap();
    CNameHookMap* GetInternalPrefixHookMap();

    void SetPreprocessorMode(bool bPreprocessorMode);
    bool IsPreprocessorMode();

    // Source location reporting
    void SetOriginalLineNumber(int iOriginalLineNumber);
    void SetOriginalSourceFile(path pathOriginalSourceFile);
    int GetCurrentOriginalLineNumber();

    // Preprocessor
    void PreservePreprocessorDirective(str sPreprocessorDirective);
    void RegisterTemplateName(CUnqualifiedName* pUnqualifiedName);
    void RegisterTemplateNameStr(str sName);
    bool IsTemplateNameOnlyByDeclaration(str sName);
    bool IsTemplateName(CUnqualifiedName* pUnqualifiedName);
    nullable<CToken*> FindKeywordKind(CCharWord* pCharWord);

// Final Phases Context Control
//   (not available for CTokenizer)

    void SetContextToken(nullable<CToken*> npContextToken);
    nullable<CToken*> GetContextToken();

// Transformation

    CTransformContext* GetLinkedTransformContext();

// Error Reporting

    // Non-ignorable error
    #define C_szNoSuppressByCtaIgnoreMarker "[NON-IGNORABLE]"

    // Source reference
    str GetSourceFileLineReference(
            nullable<CToken*> npReferenceToken,
            nullable<CToken*> npContextToken,
            out nullable<CToken*>& out_npFoundToken,
            out str& out_sSourceLocationReport);
    str GetSourceContextQuote(CToken* pAroundToken);

    // Reporting routines
    bool ExpectCharToken(
            errn errnCode,
            CToken* pKindOfToken,
            const char* szStatementDescription,
            CToken* pParentToken);
    nullable<CToken*> ExpectCharTokenGetToken(
            errn errnCode,
            CToken* pKindOfToken,
            const char* szStatementDescription,
            CToken* pParentToken);
    void ReportErrorExpecting(
            errn errnCode,
            CToken* pKindOfToken,
            str sStatementDescription,
            nullable<CToken*> npContextToken);
    void ReportErrorExpectingClosing(
            errn errnCode,
            CToken* pKindOfToken,
            str sStatementDescription,
            nullable<CToken*> npContextToken);
    void ReportErrorUnexpectedToken(
            errn errnCode,
            CToken* pToken,
            str sStatementDescription,
            nullable<CToken*> npContextToken);
    void ReportInternalError(
            errn errnCode,
            str sErrorReport,
            nullable<CToken*> npContextToken);
    void ReportInternalTransformError(
            errn errnCode,
            str sErrorReport,
            nullable<CToken*> npContextToken);
    void ReportTransformClientError(
            errn errnCode,
            str sErrorReport,
            nullable<CToken*> npContextToken);
    void ReportError(
            errn errnCode,
            str sErrorReport,
            nullable<CToken*> npContextToken);
    void ReportWarning(
            errn errnCode,
            str sErrorReport,
            nullable<CToken*> npContextToken);
    void FailOnInternalCheck(
            bool bCheckOk);
    void ReportAppendix(
            errn errnCode,
            str sErrorReport,
            nullable<CToken*> npContextToken);
    #define C_szOnlyCollectMessages "<OnlyCollect>"
    bool ReportMessage(
            errn errnCode,
            str sReport,
            nullable<CToken*> npContextToken);
    bool SuppressSimilarMessage(
            str sReport);
    void ReturnErrorCode(int nExitCode = 1);
    int GetReturnedErrorCode();
    void SetErrorsAsWarnings(bool bYes);
    bool IsErrorsAsWarnings();

    // Temporary flag must be properly guarded!
    void EnableMessages(bool bEnable);
    bool IsMessagesEnabled();

// Implementation

    // Eof marker is used for table indexing (zero char is supported in source)
    //enum { C_nEof = 0 };

private:
    CSourceInput* m_pSourceInput;
    CSourceLines* m_pSourceLines;
    CKeywordMapping* m_pKeywordMapping;
    CNameHookMap* m_pNameHookMap;
    CNameHookMap* m_pInternalPrefixHookMap;
    nullable<CToken*> m_npNextToken;
    bool m_bNextTokenExtended;
    int m_iCurrentLineInfo;
    bool m_bPreprocessorMode;
    CProcessing* m_pProcessing;
    int m_nReturnErrorCode;
    bool m_bErrorsAsWarnings;
    bool m_bEnableMessages;
    path m_pathInitialSourceFile;
    int m_nCurrentLineIndent;
    bool m_bAtStartOfLineIndent;

    CTokenizerMacroMap* m_pTokenizerMacroMap;

    // Creates a token for a given input position
    CCharToken* CreateTokenOrSpaceAt(
            const char* szInput,
            CToken* pParentToken,
            out int& out_nSkipInSource);

    // Gets a token including CWhiteSpace
    CCharToken* ExtractNextTokenOrSpace(
            CToken* pParentToken);

    // Filters out only meaningful tokens
    CCharToken* ExtractNextToken(
            CToken* pParentToken);

    // Source Lines proxy
    void HandleNewLine(const char* pcSourceTextPos);

    // Change current / context token
    void ImplCurrentTokenChange(
            CToken* pToken,
            out str& out_sRollbackError);

// Quick first token char translation.

    // Translation function pointer
    struct DArgTokenLookup
    {
        DArgTokenLookup(
                CTokenizer* pTokenizer,
                const char* szInput,
                CToken* pContextToken);

        // [object]
        CTokenizer* m_pTokenizer;

        // [const] initial input pointer
        const char* m_szInputStart;

        // [in] stream input position at the start of the token
        const char* m_szInput;

        // [in] Context token
        CToken* m_pContextToken;

        // [out] informs tokenizer of the total token length
        int m_out_nTokenLength;

        // overriden in CTokenizer::OnCharWhiteSpace()
        bool m_bExtractedTokenAtStartOfLineIndent;
    };

    typedef CCharToken* (*PFN_TokenLookup) (DArgTokenLookup& dataArgTokenLookup);

    // Handlers called for the first token char.
    //   These functions are expected to collect as much following input chars
    //   into a token as possible.
    static CCharToken* OnCharUninitialized(DArgTokenLookup& dataArgTokenLookup);
    static CCharToken* OnCharNull(DArgTokenLookup& dataArgTokenLookup);
    static CCharToken* OnCharWhiteSpace(DArgTokenLookup& dataArgTokenLookup);
    static CCharToken* OnCharNewLine(DArgTokenLookup& dataArgTokenLookup);
    static CCharToken* OnCharExclamation(DArgTokenLookup& dataArgTokenLookup);
    static CCharToken* OnCharDoubleQuote(DArgTokenLookup& dataArgTokenLookup);
    static CCharToken* OnCharSharp(DArgTokenLookup& dataArgTokenLookup);
    //static CCharToken* OnCharDollar(DArgTokenLookup& dataArgTokenLookup);
    static CCharToken* OnCharPercent(DArgTokenLookup& dataArgTokenLookup);
    static CCharToken* OnCharAnd(DArgTokenLookup& dataArgTokenLookup);
    static CCharToken* OnCharSingleQuote(DArgTokenLookup& dataArgTokenLookup);
    static CCharToken* OnCharLeftParen(DArgTokenLookup& dataArgTokenLookup);
    static CCharToken* OnCharRightParen(DArgTokenLookup& dataArgTokenLookup);
    static CCharToken* OnCharStar(DArgTokenLookup& dataArgTokenLookup);
    static CCharToken* OnCharPlus(DArgTokenLookup& dataArgTokenLookup);
    static CCharToken* OnCharComma(DArgTokenLookup& dataArgTokenLookup);
    static CCharToken* OnCharMinus(DArgTokenLookup& dataArgTokenLookup);
    static CCharToken* OnCharDot(DArgTokenLookup& dataArgTokenLookup);
    static CCharToken* OnCharDivide(DArgTokenLookup& dataArgTokenLookup);
    static CCharToken* OnCharDigit(DArgTokenLookup& dataArgTokenLookup);
    
[...]