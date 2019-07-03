// CWriter.h
// (see "CWriter.cpp")
class DComplexityStats;

#include "CWriterOptions.h"

class CHookWriteIface;
class CHookWriteMap;

//
// CWriter - abstract base class for all types of regenerated output
//

class CWriter : public CCommon
{
public:
    CWriter();

// Simple Options

    void SetOptions(CWriterOptions* pOptions);
    CWriterOptions* GetOptions();

// Stream Control

    // Write char sequence
    void WriteTokenStr(
            CToken* pToken,
            str sNextFragment,
            ELineNumbering eLineNumbering);
    void WriteTokenMultiLineStr(
            CToken* pToken,
            str sNextFragment,
            ELineNumbering eLineNumbering);
    void WriteTokenZeroTerminatedString(
            CToken* pToken,
            const char* szNextFragment,
            ELineNumbering eLineNumbering);
    void WriteTokenBuffer(
            CToken* pToken,
            const char* pcNextFragment,
            int nLength,
            ELineNumbering eLineNumbering);
    void WriteCosmeticNewLine();
    void WriteDebugText(str sAnyText);
    void WriteDebugSubBranch(
            str sBranchKey,
            str sBranchIndexKey,
            str sAnyText);
    void HandleTokenStart(CToken* pToken);
    void HandleTokenEnd(CToken* pToken);
    void ForceLineResyncOnNextToken();

// Result

    str GetRegenerationResult();

// Helpers

    static str PreparePathForLineDirective(path pathRaw);
    static str PreparePathAsCppString(path pathRaw);

// Debug

    void SetDebugDumpMode(bool bDebugDumpMode);
    bool IsDebugDumpMode();
    void SetEmitExtraComments(bool bEmitExtraComments);
    bool IsEmitExtraComments();

    int GetActualCurrentOutputLineForDebug();

protected:
    virtual void OnWriteTokenStr(
            CToken* pToken,
            str sNextFragment,
            ELineNumbering eLineNumbering) = 0;
    virtual void OnWriteTokenMultiLineStr(
            CToken* pToken,
            str sNextFragment,
            ELineNumbering eLineNumbering) = 0;
    virtual void OnWriteTokenZeroTerminatedString(
            CToken* pToken,
            const char* szNextFragment,
            ELineNumbering eLineNumbering) = 0;
    virtual void OnWriteTokenBuffer(
            CToken* pToken,
            const char* pcNextFragment,
            int nLength,
            ELineNumbering eLineNumbering) = 0;
    virtual void OnWriteCosmeticNewLine() = 0;
    virtual void OnWriteDebugText(str sAnyText) = 0;
    virtual void OnWriteDebugSubBranch(
            str sBranchKey,
            str sBranchIndexKey,
            str sAnyText) = 0;
    virtual void OnHandleTokenStart(CToken* pToken) = 0;
    virtual void OnHandleTokenEnd(CToken* pToken) = 0;
    virtual int OnGetActualCurrentOutputLineForDebug() = 0;
    virtual str OnGetRegenerationResult();
    virtual void OnForceLineResyncOnNextToken() {}

private:
    // Output Mode
    bool m_bDebugDumpMode;
    bool m_bEmitExtraComments;
    array<CHookWriteIface*> _m_apHookWriteCache;

    void AppendHookWriteMapFrom(
            CToken* pToken);

    // Options
    CWriterOptions* m_pOptions;
};

