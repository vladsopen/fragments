// CDebuxTestVersions.cpp

#include "CDebuxProject.h"
#include "CDebuxTestVersions.h"
#include "lib__project_version.h"

CDebuxTestVersions::CDebuxTestVersions()
{
    _init_AttachToDebuxTestVersions();
}

void CDebuxTestVersions::_init_AttachToDebuxTestVersions()
{
    //x_r->_x_p = this;
}

void CDebuxTestVersions::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToDebuxTestVersions();

    rFAIL("make sure it is OK to expose CDebuxTestVersions!");
}

void CDebuxTestVersions::OnTestClass()
{
    rASSERT(
        CProject::GCompareVersionCompatibility(
            "1.a0",
            "1.a0")
            ==
            E_Sort_Equal);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "1.a.0",
            "1.a.0")
            ==
            E_Sort_Equal);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "1.a0",
            "1.a2")
            ==
        E_Sort_Less);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "1.1a00",
            "1.1a00")
            ==
        E_Sort_Equal);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "1.1a." C_szVersionEndingUndefined,
            "1.1a." C_szVersionEndingUndefined)
            ==
        E_Sort_Equal);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "1.1aQQ",
            "1.1aQQ")
            ==
        E_Sort_Equal);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "1.1a.01",
            "1.1a.VLfliperr")
            ==
        E_Sort_Equal);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "1.1a.DVfliperr",
            "1.1a.10")
            ==
        E_Sort_Equal);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "1.1a.AB",
            "1.1a.XY")
            ==
        E_Sort_Equal);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "1.1a.ABiii2",
            "1.1a.XY")
            ==
        E_Sort_Equal);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "1.1b." C_szVersionEndingUndefined,
            "1.1a." C_szVersionEndingUndefined)
            ==
        E_Sort_Greater);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "1.1b." C_szVersionEndingUndefined,
            "1.1a." C_szVersionEndingUndefined)
            ==
        E_Sort_Greater);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "1okr." C_szVersionEndingUndefined,
            "1okr.f2g")
            ==
        E_Sort_Equal);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "10.b7",
            "10." C_szVersionEndingUndefined)
            ==
        E_Sort_Equal);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "1a.6",
            "1a.0")
            ==
        E_Sort_Greater);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "7.1234." C_szVersionEndingUndefined,
            "7.1234.r5a")
            ==
        E_Sort_Equal);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "7.1234.sp3",
            "7.1234." C_szVersionEndingUndefined)
            ==
        E_Sort_Equal);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "7.1234.beta." C_szVersionEndingUndefined,
            "7.1234.beta.b3q")
            ==
        E_Sort_Equal);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "7.1234.beta.a1a",
            "7.1234.beta." C_szVersionEndingUndefined)
            ==
        E_Sort_Equal);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "7.1234.beta." C_szVersionEndingUndefined,
            "7.1234.r5h")
            ==
        E_Sort_Less);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "7.1234.sp7",
            "7.1234.beta.a4w")
            ==
        E_Sort_Greater);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "7.1234.sp7",
            "7.1234.beta." C_szVersionEndingUndefined)
            ==
        E_Sort_Greater);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "7.1234.beta.t1win10",
            "7.1234.r." C_szVersionEndingUndefined)
            ==
        E_Sort_Less);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "7.1234.beta." C_szVersionEndingUndefined,
            "7.1234.r." C_szVersionEndingUndefined)
            ==
        E_Sort_Less);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "7.1234.beta." C_szVersionEndingUndefined,
            "7.1234.r2")
            ==
        E_Sort_Less);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "7.1234.r." C_szVersionEndingUndefined,
            "7.1234.beta2")
            ==
        E_Sort_Greater);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "7.1234.r." C_szVersionEndingUndefined,
            "7.1234.r.a1")
            ==
        E_Sort_Equal);

    rASSERT(
        CProject::GCompareVersionCompatibility(
            "7.1234.r." C_szVersionEndingUndefined,
            "7.1234.r.q9f")
            ==
        E_Sort_Equal);

    //
    // For project version check
    //

    rASSERT(
        project_version::GFindVersionVariableEndingPos(
            "1.0")
            ==
            -1);

    rASSERT(
        project_version::GFindVersionVariableEndingPos(
            "A1.0")
            ==
            -1);

    rASSERT(
        project_version::GFindVersionVariableEndingPos(
            "A10.3.01")
            ==
            -1);

    rASSERT(
        project_version::GFindVersionVariableEndingPos(
            "A10.3.0a")
            ==
            -1);

    rASSERT(
        project_version::GFindVersionVariableEndingPos(
            "A10qb.a")
            ==
            -1);

    rASSERT(
        project_version::GFindVersionVariableEndingPos(
            "A10qb.Beta")
            ==
            -1);

    rASSERT(
        project_version::GFindVersionVariableEndingPos(
            "A10qb.B1")
            ==
            -1);

    rASSERT(
        project_version::GFindVersionVariableEndingPos(
            "A10qb.aBX")
            ==
            -1);

    rASSERT(
        project_version::GFindVersionVariableEndingPos(
            "A10qb.b00")
            ==
            -1);

    rASSERT(
        project_version::GFindVersionVariableEndingPos(
            "A10qb.b1")
            ==
            -1);

    rASSERT(
        project_version::GFindVersionVariableEndingPos(
            "A10.1")
            ==
            -1);

    rASSERT(
        project_version::GFindVersionVariableEndingPos(
            "A10.0")
            ==
            -1);

    rASSERT(
        project_version::GFindVersionVariableEndingPos(
            "A10." C_szVersionEndingUndefined)
            ==
            4);

    rASSERT(
        project_version::GFindVersionVariableEndingPos(
            "A10.QZ")
            ==
            4);

    rASSERT(
        project_version::GFindVersionVariableEndingPos(
            "A10.00q")
            ==
            -1);

    rASSERT(
        project_version::GFindVersionVariableEndingPos(
            "A10.ZQ16")
            ==
            4);

    {
        str sError = "";
        bool bIssueAsserts = false;
        project_version::GCompareProjectVersionCompatibilityGetErrors(
            "1.alpha",
            "1.alpha2",
            out sError,
            bIssueAsserts);

        rASSERT(sError.FindPos("differ just in ending") != -1);
    }
    {
        str sError = "";
        bool bIssueAsserts = false;
        project_version::GCompareProjectVersionCompatibilityGetErrors(
            "1.95",
            "1.547",
            out sError,
            bIssueAsserts);

        rASSERT(sError.FindPos("differ in digit segment lengths") != -1);
    }
    {
        str sError = "";
        bool bIssueAsserts = false;
        project_version::GCompareProjectVersionCompatibilityGetErrors(
            "1.195",
            "1.1547",
            out sError,
            bIssueAsserts);

        rASSERT(sError.FindPos("differ in digit segment lengths") != -1);
    }
    {
        str sError = "";
        bool bIssueAsserts = false;
        project_version::GCompareProjectVersionCompatibilityGetErrors(
            "1.a.bb",
            "1.ab.a",
            out sError,
            bIssueAsserts);

        rASSERT(sError.FindPos("between digits, letters or dots") != -1);
    }
    {
        str sError = "";
        bool bIssueAsserts = false;
        project_version::GCompareProjectVersionCompatibilityGetErrors(
            "1.a.1b",
            "1.a.a",
            out sError,
            bIssueAsserts);

        rASSERT(sError.FindPos("between digits, letters or dots") != -1);
    }
    {
        str sError = "";
        bool bIssueAsserts = false;
        project_version::GCompareProjectVersionCompatibilityGetErrors(
            "1.A",
            "1.b",
            out sError,
            bIssueAsserts);

        rASSERT(sError.FindPos("between digits, letters or dots") != -1);
    }
    {
        str sError = "";
        bool bIssueAsserts = false;
        project_version::GCompareProjectVersionCompatibilityGetErrors(
            "1." C_szVersionEndingUndefined,
            "1.a.1b",
            out sError,
            bIssueAsserts);

        rASSERT(sError.FindPos("between digits, letters or dots") == -1);
    }
    {
        str sError = "";
        bool bIssueAsserts = false;
        project_version::GCompareProjectVersionCompatibilityGetErrors(
            "1.a.1b",
            "1." C_szVersionEndingUndefined,
            out sError,
            bIssueAsserts);

        rASSERT(sError.FindPos("between digits, letters or dots") == -1);
    }
    {
        str sError = "";
        bool bIssueAsserts = false;
        project_version::GCompareProjectVersionCompatibilityGetErrors(
            "1.a1",
            "1.a.2",
            out sError,
            bIssueAsserts);

        rASSERT(sError.FindPos("between digits, letters or dots") != -1);
    }
    {
        str sError = "";
        bool bIssueAsserts = false;
        project_version::GCompareProjectVersionCompatibilityGetErrors(
            "1.a05.ggg",
            "1.a6.ggg",
            out sError,
            bIssueAsserts);

        rASSERT(sError.FindPos("differ in digit segment lengths") != -1);
    }
    {
        str sError = "";
        bool bIssueAsserts = false;
        project_version::GCompareProjectVersionCompatibilityGetErrors(
            "1.a05.ggg",
            "1.b6.ggg",
            out sError,
            bIssueAsserts);

        rASSERT(sError == "");
    }
}
