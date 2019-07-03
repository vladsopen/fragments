// CDebuxTestVirtFs.cpp

#include "CDebuxProject.h"
#include "CDebuxTestVirtFs.h"
#include "VirtOs_loci.h"
#include "VirtOs_file.h"
#include "VirtOs_dir.h"
#include "VirtOs_CPathList.h"
#include "VirtOs_text.h"

CDebuxTestVirtFs::CDebuxTestVirtFs()
{
    _init_AttachToDebuxTestVirtFs();
}

void CDebuxTestVirtFs::_init_AttachToDebuxTestVirtFs()
{
    //x_r->_x_p = this;
}

void CDebuxTestVirtFs::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToDebuxTestVirtFs();

    rFAIL("make sure it is OK to expose CDebuxTestVirtFs!");
}

void CDebuxTestVirtFs::CheckPermit(
        virtfs::FPermit fPermit,
        path pathSource,
        path pathMustChangeTo,
        bool bExpectVirtfsPermissionOk)
{
    path pathOriginal = pathSource;
    ospath ospathReal;
    str sError;
    if (virtfs::GPermitPath(
            fPermit,
            virtfs::F_Permit_none,
            virtfs::F_Permit_none,
            pathSource,
            out ospathReal,
            out sError))
    {
        rASSERT(bExpectVirtfsPermissionOk);
    }
    else
    {
        rASSERT(!bExpectVirtfsPermissionOk);
    }

    str sPermit =
        EnumInfo(virtfs::FPermit).ToString(fPermit);

    // cleanup for test consistency
    pathSource =
        pathSource.
            GetReplaced(
                sys::GGetTempFolder(), 
                "<Temp>").
            GetReplaced(
                CProject::GGetIt().
                    //GetStoragePath().
                    GetSharedStoragePath(),
                "<Storage>");

    TESTLOG(
        "",
        "" + 
            (bExpectVirtfsPermissionOk? "OK" : "Fail") + ": " +
            sPermit + " <" +
            pathSource +
            ">"
            "\n  --> " +
            pathMustChangeTo +
            "\n  (" +
            sError +
            ")"
            "\n\n");
}

void CDebuxTestVirtFs::TestBypassActivation(
        bool bVirtfsBypassActive)
{
    virtfsbypass virtfsbypassTest(bVirtfsBypassActive);

    path pathBadFile = 
        loci::GGetMyProjects().GetAppendedPath("virtfsbypass.test");

    str sError;

    str sSaveRandom = Str(GTickToBig(sys::GGetUtcPhysical()));
    sError = "";
    if (!file(pathBadFile).Save(sSaveRandom, out sError))
    {
        rASSERT(!bVirtfsBypassActive);
        rASSERT(sError.FindPos("/ERR:VF") != -1);
    }
    else
    {
        // Success! Expected?
        rASSERT(bVirtfsBypassActive);
    }

    str sLoadSaved;
    sError = "";
    if (!file(pathBadFile).Load(out sLoadSaved, out sError))
    {
        rASSERT(!bVirtfsBypassActive);
        return;
    }

    // Success! Expected?
    rASSERT(bVirtfsBypassActive);

    if (sLoadSaved != sSaveRandom)
    {
        rFAIL("unexpected save/load virtfs check failure (race?)");
    }

    sError = "";
    if (!file(pathBadFile).EraseFile(out sError))
    {
        rFAIL("cannot delete temp test file");
    }
}

void CDebuxTestVirtFs::OnTestClass()
{
    {
        //virtfsbypass virtfsbypassListAnywhere;

        assertoff aoDenied(
            "Path access is denied /ERR:VFDE/");

        ref<CPathList> rReadPathList;
        path pathDriveOrFolder = 
            loci::GGetMyProjects().GetDrive().GetAppendedPath("Windows");
            // must fail

        dir(pathDriveOrFolder).
            GetFilesIncludingSubfolders();

        TESTLOG(
            "",
            "\n"
                "Unallowed folder tree listing: <<<\n" +
                //"- error: " + sError + "\n"
                //"- listing:\n" +
                rReadPathList->ToText().CatenateTextSeparatingWith("\n") +
                ">>>\n\n");
    }

    assertoff aoAccess("Path access is denied");

    CheckPermit(
        virtfs::F_Permit_ReadContent,
        virtfs::GVirtualizeLocalOsPath((ospath) "c:\\invalid"),
        path(""),
        false);

    assertoff aoSemicolon("semicolon is not allowed");

    CheckPermit(
        virtfs::F_Permit_ReadContent,
        path("c:\\invalid"),
        path(""),
        false);

    CheckPermit(
        virtfs::F_Permit_ReadContent,
        sys::GGetTempFolder(),
        path(""),
        true);

    CheckPermit(
        virtfs::F_Permit_ReadContent,
        CProject::GGetIt().
            //GetStoragePath().
            GetSharedStoragePath(),
        path(""),
        true);

    CheckPermit(
        virtfs::F_Permit_WriteContent,
        CProject::GGetIt().
            //GetStoragePath().
            GetSharedStoragePath(),
        path(""),
        false);

    CheckPermit(
        virtfs::F_Permit_CreateFile,
        "||MyLan/any folder",
        path(""),
        true);

    CheckPermit(
        virtfs::F_Permit_CreateFile,
        "\\\\LLLan/unvirtualized",
        path(""),
        false);

    CheckPermit(
        virtfs::F_Permit_ReadContent,
        "||?sysLL",
        path(""),
        false);

    {
        assertoff aoError("Path traversal denied");
        CheckPermit(
            virtfs::F_Permit_ReadContent,
            sys::GGetTempFolder().GetUnslashed() + 
                // (GetAppendedPath() will refuse traversal creation)
                path::GGetPathSeparationSlash() +
                ".." +
                path::GGetPathSeparationSlash() +
                "Traversed",
            path(""),
            false);
    }

    {
        assertoff aoError("Space at the end of folder");
        CheckPermit(
            virtfs::F_Permit_ReadContent,
            sys::GGetTempFolder().GetUnslashed() + 
                path::GGetPathSeparationSlash() +
                "Space after " +
                path::GGetPathSeparationSlash(),
            path(""),
            false);
    }

    {
        assertoff aoError("Space after slash");
        CheckPermit(
            virtfs::F_Permit_ReadContent,
            sys::GGetTempFolder().GetUnslashed() + 
                path::GGetPathSeparationSlash() +
                " After space",
            path(""),
            false);
    }

    {
        assertoff aoError("Dot at the end of folder");
        //assertoff aoErrorPath("path is defective");
        CheckPermit(
            virtfs::F_Permit_ReadContent,
            sys::GGetTempFolder().GetUnslashed() + 
                path::GGetPathSeparationSlash() +
                "Full Stop." +
                path::GGetPathSeparationSlash() +
                "nice.file",
            path(""),
            false);
    }

    {
        assertoff aoError("Trailing dot in path");
        assertoff aoErrorPath("path is defective");
        CheckPermit(
            virtfs::F_Permit_ReadContent,
            sys::GGetTempFolder().GetUnslashed() + 
                path::GGetPathSeparationSlash() +
                "Full Stop.",
            path(""),
            false);
    }

    {
        assertoff aoError("Binary data in path");
        assertoff aoPathClass1("unallowed char in path");
        assertoff aoPathClass2("unallowed char in ospath");
        CheckPermit(
            virtfs::F_Permit_ReadContent,
            sys::GGetTempFolder().
                GetAppendedPath("Zero" + StrChar(0) + "Inside"),
            path(""),
            false);
    }

    {
        assertoff aoError("Invalid character in path");
        assertoff aoPathClass1("unallowed char in path");
        assertoff aoPathClass2("unallowed char in ospath");
        CheckPermit(
            virtfs::F_Permit_ReadContent,
            sys::GGetTempFolder().
                GetAppendedPath("Control\v" "Char"),
            path(""),
            false);
    }

    // test for ".." with GetAppendedPath()
    {
        assertoff aoTrav("cannot create traversal with GetAppendedPath()");
        path pathDenied = 
            sys::GGetTempFolder().GetAppendedPath("..");
        rASSERT(pathDenied == "");

        assertoff aoBadPath1("path :: GGetGoodPathChars()");
        assertoff aoBadPath2("this -> GetAt(0) != ' '");
        assertoff aoBadPath3("No absolute drive in path");
        assertoff aoBadPath4("unallowed char in path << \" >>");

        CheckPermit(
            virtfs::F_Permit_ReadContent,
            path("  \"c:\\invalid\" "),
            path(""),
            false);
    }

    // test bool parameter for virtfsbypass
    {
        TestBypassActivation(true);
        TestBypassActivation(false);
    }

    // non-C: for tests
    {
        path pathQ = 
            virtfs::GVirtualizeLocalWindowsDriveLetterOnlyForTests(
                'q');

        rASSERT(pathQ == "q" C_szVirtualPathMagic);
    }
}

