// CSourceInput.h
// (see "CSourceInput.cpp")

// CSourceInput - low level source character stream.
//  (designed to be used by CTokenizer)

class CSourceInput : public object
{
public:
    CSourceInput();

// Initialization

    // Prepare source text
    void SetSourceText(str sSourceText);

// Stream Control

    //int GetCurrentChar();
    const char* GetCurrentSourcePointer();
    void AdvanceCurrentPosition(int nMoveForward);
    void RestoreCurrentPositionTo(const char* szNewCurrentPos);
    int GetRemainedLength();
    int ConvertSourcePointerToIndex(const char* szSourcePos);
    bool IsWithinProcessedInput(const char* szSourcePos);
    bool IsWithinSourceText(const char* szSourcePos);

// Debug

    // Reports some lines around given source position
        enum { C_nMinCharsToGoBack = 20 };
        enum { C_nMinLinesToGoBack = 1 };
        enum { C_nMinCharsToGoForward = 20 };
        enum { C_nMinLinesToGoForward = 2 };
    str GetSourceContextQuote(const char* szAroundPoint);

private:
    str m_sText;
    int m_iCurrentPos;
};

