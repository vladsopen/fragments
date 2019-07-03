// CClassTestingImpl.cpp

#include "CDebuxProject.h"
#include "CClassTestingImpl.h"
#include "CTestingOptionsIfaceGp.h"
#include "VirtOs_CInterMem.h"
#include "VirtOs_CInterLock.h"
#include "VirtOs_CPathList.h"
#include "lib__classinfo_DClassAttr.h"
#include "CGlobalUvBotStateIfaceGp.h"
#include "VirtOs_CGlobalUi.h"
#include "CClassDelay.h"

CClassTestingImpl::CClassTestingImpl()
{
}

void CClassTestingImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CClassTestingImpl!");
}

void CClassTestingImpl::OnRunClassTests(
        ref<CTestingOptionsIfaceGp> rTestingOptions)
{
    ref<CClassTestGlobalsImpl> rClassTestGlobals =
        CDebuxProject::GGetIt().
            x_rDebuxGlobalConfig->
                _x_rClassTestGlobals;

    // for consolidated running later
    if (rClassTestGlobals->x_bSkipRunningClassTests
        ||
        rClassTestGlobals->IsInsideDebuxStartersTest())
    {
        return;
    }

    // save options
    m_rTestingOptions = rTestingOptions;

    // use once
    rASSERT(!_m_ClassTestingUsed);
    _m_ClassTestingUsed = true;

    // no unique tests in ParaService process
    if (CAppGp::GGet()->
            x_rInterprocParaMap->
                IsParaServiceProcess())
    {
        return;
    }

    if (sys::GIsCommandLineKeyPresent(C_sNoTestSwitch))
    {
        return;
    }

    // a hack because Parallelizer was not a ParaService
    if (CProject::GGetInternalAppLogo() == "Parallelizer" &&
        !sys::GIsDebugBreakAvailable())
    {
        return;
    }

    // repeated tests are prevented
    /*
    // we predelete all test files
    CInterLock lockClassTests(
        CInterLock::SYNC,
        "AllClassTests",
        oo);
    */

    // debug display
    draw dcDisplay;
    dcDisplay.MapToScreenDc();

    // Profiling option
    bool bProfile = 
        debug("#lib.Debug.Tests.Profile?").InputBool();

    m_sProfileClass = 
        debug("#lib.Debug.Tests.Profile?").InputText();
    if (m_sProfileClass.Len() > 1)
    {
        bProfile = false;
    }
    else
    {
        m_sProfileClass = "";
    }

    // Set debug flag
    bool bWasInsideClassTests = 
        debug::GChangeInsideClassTestsGetPrevState(
            true);
    ASSERT_SOFT(!bWasInsideClassTests); // loading a DLL from a test?!!

    // temp dir isn't created autmatically anymore
    str sReCreateError;
    dir(sys::GGetTempFolder()).CreateDir(out sReCreateError);
    rASSTR(sReCreateError);

    // preject ref
    ref<CProject> rThisProject = 
        m_rTestingOptions->_x_pTestInitiatingProject.Get().Valid();

    ref<CTimeVirtualizerIfaceGp> rTimeVirtualizer = 
        CAppGp::GGet()->
            x_rTimeVirtualizer;

    {
        // Load stop-list
        path pathStopList = 
            loci::GGetMyProjectsUniPersonalSettings().
                GetAppendedPath("ClassTestSkip.txt");
        str sStopListLoadError;
        file(pathStopList).Load(out m_sSkipTests, out sStopListLoadError);
        m_sSkipTests = " " + m_sSkipTests.GetReplaced(CHARMAP(charCTRL), " ") + " ";
    }

    {
        // Load only-list
        path pathOnlyList = 
            loci::GGetMyProjectsUniPersonalSettings().
                GetAppendedPath("ClassTestOnly.txt");
        str sStopListLoadError;
        file(pathOnlyList).Load(out m_sOnlyTest, out sStopListLoadError);
        //m_sOnlyTest = " " + m_sOnlyTest.GetReplaced(CHARMAP(charCTRL), " ") + " ";
        m_sOnlyTest = m_sOnlyTest.GetStripBoth();
    }

    if (CProject::GGetInternalAppLogo() == "Debux")
    {
        m_sSkipTests = "";
        m_sOnlyTest = "";
    }

    TRACE(
        "\n" + 
            "---------- ClassTests for " + 
                rThisProject->GetProjectFixedPrivateLogo() + " ----------\n\n");

    globalprofiler* pglobalprofilerController = 
        globalprofiler::GGetProcessGlobalHairyProfilerPointer();
    if (bProfile)
    {
        pglobalprofilerController->StartProcessGlobalHairyProfiler();
    }

    tick tickStart = sys::GGetUtcPhysical();

    //
    // Main loop
    //

    TestAllRegisteredClasses(
        dcDisplay);

    // stop virtual time
    rTimeVirtualizer->
        RestorePhysicalTime();

    // Run other local tests
    RunProjectLocalTestWithinClassTests(
        m_rTestingOptions);

    // Clear debug flag
    debug::GChangeInsideClassTestsGetPrevState(
        bWasInsideClassTests);

    // Dump profiling info
    if (bProfile)
    {
        pglobalprofilerController->StopProcessGlobalHairyProfiler();
    }

    tick tickElapsed = sys::GGetUtcPhysical() - tickStart;
    if (tickElapsed > 3000)
    {
        debug("#lib.Debug.Tests./show.Time:").
            FloatNote("Tested in " + Str(GTickToBig(tickElapsed) / 1000.) + " s. ");
    }

    // done.
    str sTiming = 
        rThisProject->GetProjectFixedPrivateLogo() + 
            " ClassTest run done in " + 
                Str(GTickToBig(tickElapsed) / 1000.) + 
                " s. ";
    DisplayTestProgress(
        dcDisplay,
        sTiming);

    TRACE("\n---------- " + sTiming + " ----------\n\n");

    // restore global services
    CAppGp::GGet()->
        FinalizeAfterClassTestsComplete();

    // Dump profiling info
    if (bProfile)
    {
        pglobalprofilerController->OpenSessionReportAsDocument();
    }

    if (!m_rTestingOptions->x_bFakeVisualTests &&
        !m_rTestingOptions->x_bOnlyRunTestsForOneProject &&
        m_rTestingOptions->x_bDumpConverageForMultipleProjects)
    {
        DDebugGlobals::GGetProcessDebugGlobals()->_m_rCoverageTracer->
            DumpCoverageResults();
    }

    // save log to view in CCES
    rClassTestGlobals->x_sClassTestTimingLog =
        rClassTestGlobals->x_sClassTestTimingLog +
            m_sTimingLog;
    m_sTimingLog = "";

    TRACE(
        "\n"
            "Slow tests:\n" + 
            rClassTestGlobals->x_sClassTestTimingLog +
            "\n");

    //
    // Call delayed tests
    //

    CallDelayedTests();
}

void CClassTestingImpl::RunProjectLocalTestWithinClassTests(
        ref<CTestingOptionsIfaceGp> rTestingOptions)
{
    if (rTestingOptions->x_bOnlyRunTestsForOneProject)
    {
        // preject ref
        ref<CProject> rProject = 
            rTestingOptions->_x_pTestInitiatingProject.Get().Valid();

        RunSingleProjectLocalTestWithinClassTests(rProject);
    }
    else
    {
        // Run for all
        ref<CProjectsInProcess> rProjectsInProcess;
        ref<CProject> rIterProject;
        for (
            iter i; 
            rProjectsInProcess->IterateProjects(out i, out rIterProject);)
        {
            RunSingleProjectLocalTestWithinClassTests(rIterProject);
        }
    }
}

void CClassTestingImpl::On_internal_RestartVirtualTime()
{
    ref<CTimeVirtualizerIfaceGp> rTimeVirtualizer = 
        CAppGp::GGet()->
            x_rTimeVirtualizer;

    // start at fixed virtual time point
    rTimeVirtualizer->
        ActivateVirtualTimeManualModeAtUtc(
            utc::GUtcFromInternalMillisecondCounter(
                CTimeConstants::C_nnTestVirtualTimeInitialUtc));

    // auto-increment at every GetUtc()
    rTimeVirtualizer->
        SetVirtualTimeManualUtcDelta(
            CTimeConstants::C_nnTestVirtualTimeIncrement);
}

void CClassTestingImpl::DeleteTestFiles()
{
    // delete temp test folder
    {
        path pathTestTemp0 = sys::GGetTempFolder(); //.GetParentFolder();
        path pathTestTempRoot = 
            pathTestTemp0.
                GetParentFolder().
                GetUnslashed();
        if failed(pathTestTempRoot.GetFileNameWithExt().GetTruncateRight(1) == C_szTestDir)
        {
            return;
        }

        nontest nontestReallyDelete;

        // move all to unique name to force tests to create 
        //   new dir at new handles even if deleting old files failed
        path pathRenamed = 
            pathTestTemp0.GetUnslashed() 
                + 
                key::GNewUniqueStrKey(DEBUG_ARG "TstCln").
                    GetRight(20);

        bool bRenameOk = false;

        repeat(iRepeatDelete, 3)
        {
            //
            // rename __temp__.0 to __temp__.0XXXXXXX
            //

            // repeat rename until successful
            str sRenameError;
            repeat(iRepeatRename, 1)
            {
                if (!dir(pathTestTemp0).IsExist())
                {
                    bRenameOk = true;
                    break;
                }

                dir(pathTestTemp0).MoveDirByRenamingTo(pathRenamed, out sRenameError);
                sys::GSleepInTicksForTests(100);
            }

            // failed to rename?
            if (dir(pathTestTemp0).IsExist())
            {
                if (iRepeatDelete > 0)
                {
                    rFAIL(
                        "Tmp test dir rename for delete failed, attempt " + 
                            Str(iRepeatDelete + 1) +
                            "\n" +
                            sRenameError + "\n");
                }

                continue;
            }

            /*
            ref<CPathList> rPathList = 
                dir(pathTestTemp).
                    GetFilesAndDirectoriesIncludingSubfolders();

            str sNotDeleted = 
                rPathList->ToText().CatenateTextSeparatingWith("\n");
            if (sNotDeleted == "")
            {
                break;
            }

            if (iRepeatDelete > 0)
            {
                rFAIL(
                    "Tmp test dir delete failed, attempt " + 
                        Str(iRepeatDelete + 1) +
                        "\n" +
                        sDeleteError + "\n" +
                        "Not deleted:\n" +
                        sNotDeleted)
            }
            */
        }

        // finally delete all previous renamed dirs
        str sDeleteError;
        dir(pathTestTempRoot).EraseDirContent(out sDeleteError);

        if (!bRenameOk)
        {
            // leave this for a pause hopping CreateDir won't fail 
            //   on a freshly deleted path
            rFAIL("WARNING: continuing without proper temp file delete");
        }

        // sys::GGetTempFolder() won't restore it
        str sReCreateError;
        dir(pathTestTemp0).CreateDir(out sReCreateError);
        rASSTR(sReCreateError);
    }
}

[...]