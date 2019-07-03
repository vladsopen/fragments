// DebugDiffPos.h

// Diff current position
//  This object by design can scan in forward direction only,
//    assignment copies the parsing context enabling client to save a position
//    and start scanning from the saved position later.
class CDiffPos
{
public:
    enum EInitialPos {};
    CDiffPos(EInitialPos, const char* szSource);
    CDiffPos(const CDiffPos& oOtherDiffPos);
    void operator = (const CDiffPos& oOtherDiffPos);

    char GetNextEssentialChar();
    void SkipChars(int nChars);

    const char* FindLineStart();
    int GetLineNumber();
    str GetSomeFirstCharReport(int nChars);
    str GetClearText(int nMaxChars = INT_MAX);
    str GetRemainedClearText();

private:
    const char* m_szPos;
    const char* m_szOrigin;
    bool m_bInsideString;
    bool m_bEscapeNextChar;

    void SkipWhiteSpaceChunk();
    //char GetNextNonWhiteSpaceChar();
};

