// CProcessing.h
// (see "CProcessing.cpp")
// (see "CProcessing_MainRoutines.cpp")
// (see "CProcessing_MiscDebug.cpp")

class CMultiXlangGlobals;
class CCompilationUnit;

///////////////////////////////////////////////////////
// CProcessing manages C++ source translation logic

class CProcessing : public CCommon
{
public:
    GENERIC(CProcessing, CCommon);
    CProcessing();

    str RunMainParsingLoop(
            str sSourceText,
            path pathInitialSourceFile,
            path pathRootOutputFile,
            CCompilerOptions* pCompilerOptions,
            CMultiXlangGlobals* pMultiXlangGlobals,
            out int& out_nErrorCode);

    path GetCurrentOutputFilePath();
    CCompilerOptions* GetCompilerOptions();

// Debug

    // see "DebugDiff.cpp"
    void DumpResultDiff(
            str sSource,
            str sOutput);

private:
    path m_pathRootOutputFile;
    CCompilerOptions* m_pCompilerOptions;

    void ParseSource(
            str sSourceText,
            CCompilationUnit* pCompilationUnit);
    void Transform(
            str sSourceText,
            CCompilationUnit* pCompilationUnit);
    str Regenerate(
            str sSourceText,
            CCompilationUnit* pCompilationUnit);
    void DebugDiff(
            str sSourceText,
            str sOutput,
            CCompilationUnit* pCompilationUnit);
    void TextDump(
            str sSourceText,
            CCompilationUnit* pCompilationUnit);
    void TreeDump(
            str sSourceText,
            CCompilationUnit* pCompilationUnit);
    void DirectoryDump(
            str sSourceText,
            CCompilationUnit* pCompilationUnit);
};

