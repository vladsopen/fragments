// CWriteRegeneratedText.h
// (see "CWriteRegeneratedText.cpp")

// CWriteRegeneratedText - text-based output stream for regenerated source code

class CWriteRegeneratedText : public CWriter
{
public:
    GENERIC(CWriteRegeneratedText, CWriter);
    CWriteRegeneratedText();
    void InitRegeneratedText(
            // Source line numbers syncronization with CTokenizer
            //CSourceLines* pSourceLinesFromTokenizer,
            // Performance optimization
            int nEstimatedOutputLength);

// Stream Control

    // (virtual)

protected:
    virtual void OnWriteTokenStr(
            CToken* pToken,
            str sNextFragment,
            ELineNumbering eLineNumbering);
    virtual void OnWriteTokenMultiLineStr(
            CToken* pToken,
            str sNextFragment,
            ELineNumbering eLineNumbering);
    virtual void OnWriteTokenZeroTerminatedString(
            CToken* pToken,
            const char* szNextFragment,
            ELineNumbering eLineNumbering);
    virtual void OnWriteTokenBuffer(
            CToken* pToken,
            const char* pcNextFragment,
            int nLength,
            ELineNumbering eLineNumbering);
    virtual void OnWriteCosmeticNewLine();
    virtual void OnWriteDebugText(str sAnyText);
    virtual void OnWriteDebugSubBranch(
            str sBranchKey,
            str sBranchIndexKey,
            str sAnyText);
    virtual void OnHandleTokenStart(CToken* pToken);
    virtual void OnHandleTokenEnd(CToken* pToken);
    virtual int OnGetActualCurrentOutputLineForDebug();
    virtual str OnGetRegenerationResult();
    virtual void OnForceLineResyncOnNextToken();

private:

    enum { C_nMaxLinesToSyncWithoutLineDirective = 5 };

    bool m_bInitCalled;

    // Result
    str m_sAppendOutput;

    // Line count
    //CSourceLines* m_pSourceLines;
    int m_iPreviousLineInfoNumber;
    int m_iLastOriginalLineNumberWritten;
    int m_iActualCurrentOutputLine;
    int m_iMaxLineInfoForWhichPragmasWritten;
    str m_pathPreviousSourceFile;
    bool m_bEnablePreservedDirectives;
    bool m_bEnableLineDirectives;
    int m_iLastOriginalLineIndent;

    // Init
    void _init_SetTextSizeEstimation(int nEstimatedOutputLength);

    // New line
    void HandleTokenLineSync(
            CToken* pToken);

    // #line writer
    void EnsureSyncLineStart(
            int iOriginalLineNumber,
            str pathOriginalSourceFile,
            CToken* pToken);

    void WritePreservedDirectivesForLineRange(
            int iLineStart,
            int iLineEnd,
            CToken* pToken);

    void HandleLineNumbering(
            CToken* pToken,
            ELineNumbering eLineNumbering);
    void HandleSpaceSeparation(
            CToken* pToken,
            const char* pcNextFragment);

    enum EEmitNewLine
    {
        E_EmitNewLine_Always,
        E_EmitNewLine_SuppressForDebugDumpMode,
    };
    void EmitNewLineChars(
            EEmitNewLine eEmitNewLine = E_EmitNewLine_Always);

    // Write
    void EmitAnyBuffer(const char* pcNextFragment, int nLength);
    void EmitSingleLineBuffer(const char* pcNextFragment, int nLength);
    void EmitMultiLineBuffer(const char* pcNextFragment, int nLength);
    void EmitZeroTerminated(const char* pcNextFragment);

    // Low level Xlang hookable
    void DoAppendBuffer(
            const char* pcAppend,
            int nAppendLength);
    void DoAppendBufferSz(
            const char* szAppend);
    void DoAppendBufferStr(
            const str& sAppend);

    // Current context
    bool IsAtNewLine();
    bool IsAfterSpace();

//    // Prevent duplicate ';;' killing if...else
//    const char* m_szLastWritten;
//    bool ShouldCancelWrite(
//            CToken* pToken,
//            const char* szNextFragment);

    // does not work: if(x) t; /*exec*/; else {
    
};

