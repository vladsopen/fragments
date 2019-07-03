// CCompilerHook.h
// (see "CCompilerHook.cpp")
// (see "CCompilerHook_Procedure.cpp")

class CMultiXlangGlobals;
class CXlang;

// CCompilerHook - controls whole translation procedure

class CCompilerHook : public CCommon
{
public:
    GENERIC(CCompilerHook, CCommon);
    CCompilerHook();

    // Parses parameters and invokes other modules
    int TheMain();

private:

    // Compiler Extender Controller
    int RunCompilationProcedure(
            CCompilerOptions* pCompilerOptions,
            str sBeforeArgFile,
            str sAfterArgFile);
    int RunFileCompilationProcedure(
            CCompilerOptions* pCompilerOptions,
            path pathRootSourceFile,
            path pathRootOutputFile);
    int RunMetaprocessFile(
            path pathPrecopiedPreprocessedSource,
            path pathRootOutputFile,
            CCompilerOptions* pCompilerOptions);
    int MetaprocessFile(
            path pathRootSourceFile,
            path pathRootOutputFile,
            CCompilerOptions* pCompilerOptions,
            CMultiXlangGlobals* pMultiXlangGlobals);
    int TranslateFile(
            str sSource,
            out str& out_sOutput,
            path pathRootSourceFile,
            path pathRootOutputFile,
            CCompilerOptions* pCompilerOptions,
            CMultiXlangGlobals* pMultiXlangGlobals);

    // Utilites
    bool ClearAllTemporaryPreprocessorFiles(
            CCompilerOptions* pCompilerOptions);
    void CreateDebugCopy(
            path pathFile,
            str sCaption,
            path pathForceFolder = str(""));

    // Call the original CC.EXE
    int ExecuteOrigCompiler(str sArgs);
    int ExecuteOrigCompilerThroughTempFile(
            str sArgsToPutIntoTempFile,
            str sBeforeArgFile,
            str sAfterArgFile);

    // Generic Helper Functions
    path GetMyTempFolder();
    path GetTempFile(str prefix);
    int Execute(path pathExe, str sCmdline);

    // Command line processing
    str LoadCompleteArgs(
            path pathArgFile,
            str sBeforeArgFile,
            str sAfterArgFile);

    // Separates CC.EXE path\name from the application command line
    str RemoveClExeFromCommandLine(
            const char* pcCmdline,
            out str& out_sClStripReport);

    void SplitArgsAroundArgFile(
            int argc, char* argv[],
            out path& out_pathArgFile,
            out str& out_sBeforeArgFile,
            out str& out_sAfterArgFile);
    void CheckCtaVersion();

    bool CheckForValidSourcePath(
            path pathRootSourceFile);
    bool CheckForDuplicationsInSourcePath(
            path pathRootSourceFile,
            str sDuplication);

    bool CheckForValidTextEncoding(
            path pathRootSourceFile);
};
