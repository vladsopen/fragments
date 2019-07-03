// CWriteForStats.h
// (see "CWriteForStats.cpp")

// CWriteForStats - only calculates DComplexityStats

class CWriteForStats : public CWriter
{
public:
    GENERIC(CWriteForStats, CWriter);
    CWriteForStats();

// Stream Control

    // (virtual)

// Result

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

private:
};

