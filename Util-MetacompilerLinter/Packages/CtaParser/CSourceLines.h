// CSourceLines.h
// (see "CSourceLines.cpp")
// (see "CSourceLines_CLineInfo.cpp")

// CSourceLines - source file line info.
//  (preserves original numbering and preprocessor directives)

class CSourceLines : public object
{
public:
    CSourceLines();

// Line Info

    class CLineInfo : public CCommon
    {
    public:
        GENERIC(CLineInfo, CCommon);
        CLineInfo();

        void SetOriginalLineNumber(int iOriginalLineNumber);
        void SetOriginalSourceFile(path pathOriginalSourceFile);
        void SetPreservedDirective(str sPreservedDirective);

        int GetOriginalLineNumber();
        path GetOriginalSourceFile();
            // see also GetInitialSourceFile()
        str GetPreservedDirective();

    private:
        int m_iOriginalLineNumber;
        path m_pathOriginalSourceFile;
        str m_sPreservedDirective;
    };

// Initialization

    // Prepare array size
    void PrepareToSourceText(
            int nSourceTextLength,
            path pathInitialSourceFile);

    // Query parsed line information
    nullable<CLineInfo*> GetLineInfo(int iParsedLine);

// Parsing

    // Another line parsed
    void HandleAnotherNewLine(
            int iSourceTextPos);

    // Change location reporting
    void SetCurrentOriginalLineNumber(int iOriginalLineNumber);
    void SetCurrentOriginalSourceFile(path pathOriginalSourceFile);
    int GetCurrentOriginalLineNumber();

    // Preprocessor
    void PreservePreprocessorDirective(str sPreprocessorDirective);

    // Error reporting
    str GetSourceFileLineReference(int iLineInfo);

private:

    enum { C_nAverageCharsPerLine = 50 };

    array<CLineInfo*> m_aLineInfo;

    int m_iMaxSourceTextPosReached;

    int m_iCurrentOriginalLineNumber;
    path m_pathCurrentOriginalSourceFile;

    void AddAnotherLine();
    CLineInfo* GetLastLineInfo();
};

