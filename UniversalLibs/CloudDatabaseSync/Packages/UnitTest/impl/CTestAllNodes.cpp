// CTestAllNodes.cpp

#include "CProject.h"
#include "CTestAllNodes.h"
#include "CLoaderIface.h"
#include "CPatientBaseConnectionIfaceGp.h"
#include "CGlobalConfigIface.h"
#include "CTestSessionIfaceGp.h"
#include "CActionIfaceGp.h"
#include "CDbNode.h"
#include "CSessionIfaceGp.h"
#include "CNetIfaceGp.h"
#include "CStoreIfaceGp.h"
//#include "DeclareKeys.h"
#include "CBasicClassImpl.h"
#include "CBasicNodeImpl.h"
#include "CAddonMapIfaceGp.h"
#include "CFileClassImpl.h"
#include "CFilemanLocal.h"
#include "VirtOs_file.h"
#include "CFileNodeIfaceGp.h"
#include "CFileNodeImpl.h"
#include "CDbFile.h"
#include "CNetDriverPingPongIfaceGp.h"
#include "CFilemanNetCommon.h"
#include "CFilemanNetIn.h"
#include "CFilemanNetOut.h"
#include "VirtOs_dir.h"
#include "VirtOs_CPathList.h"
#include "CRecentValueIfaceGp.h"
#include "CClassTestingIfaceGp.h"
#include "VirtOs_CAppGp.h"
#include "CClassTestingTempIfaceGp.h"

CTestAllNodes::CTestAllNodes()
{
    _init_AttachToTestAllNodes();
}

void CTestAllNodes::_init_AttachToTestAllNodes()
{
    //x_r->_x_p = this;
}

void CTestAllNodes::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToTestAllNodes();

    rFAIL("make sure it is OK to expose CTestAllNodes!");
}

void CTestAllNodes::OnTestClass()
{
    /*
    ref<CLoaderIface> rLoader = 
        CProject::GGetIt().x_rLoader;
        */

    //debug::GRedirectTestLog("_NextTick", "");

    if not_null(
        ref<CClassTestingIfaceGp> rClassTesting = 
            CAppGp::GGet()->GetRunningClassTestingInClassTests())
    {
        rClassTesting->
            x_rClassTestingTemp->
                x_bPleaseDontReallyCompress = 
                    true;
    }
    else
    {
        rFAIL("cant be outside tests");
    }

    TESTLOG("", "\n" "WARNING: reenable +NextTick to trace GGetUtc()\n\n");
    debug::GDisableTestLog("+NextTick");

    TESTLOG(
        "",
        "   (utc101 = " + Str(sys::GGetUtc()) + ")\n");

    ref<CGlobalConfigIface> rGlobalConfig =
        CProject::GGetIt().
            x_rGlobalConfig;

    rGlobalConfig->x_bAllowLzmaInTests = true;

    // fileman has its own Net channel, needs emulator setup
    ref<CNetDriverPingPongIfaceGp> rNetDriverForFileman;
    CFilemanNetCommon::s_pForceNetDriverForTests = 
        rNetDriverForFileman;

    // we have 2 test sessions using the same driver in+out
    rNetDriverForFileman->x_nTestHackForPingPongDontShutdown += 1;
    rNetDriverForFileman->x_nTestHackForPingPongDontShutdown += 1;
    rNetDriverForFileman->x_nTestHackForPingPongDontShutdown += 1;

    // run CloudSync in test mode
    ref<CTestSessionIfaceGp> rTestSession;
    rTestSession->
        InitTestSession();

    ref<CSessionIfaceGp> rSessionUpload = 
        rTestSession->GetSessionUpload();
    ref<CNetIfaceGp> rNetUpload = 
        rSessionUpload->x_rNetGp;
    //ref<CSyncOutIfaceGp> rSyncOut = 
    //    rNetUpload->x_rSyncOut;
    ref<CStoreIfaceGp> rStoreUpload = 
        rSessionUpload->x_rStoreGp;

    ref<CSessionIfaceGp> rSessionDownload = 
        rTestSession->GetSessionDownload();
    ref<CNetIfaceGp> rNetDownload = 
        rSessionDownload->x_rNetGp;
    //ref<CSyncInIfaceGp> rSyncIn = 
    //    rNetDownload->x_rSyncIn;
    ref<CStoreIfaceGp> rStoreDownload = 
        rSessionDownload->x_rStoreGp;

    ref<CBasicClassImpl> rBasicClassDownload =
        (ref<CBasicClassIface>)
        Derive<CBasicClassIfaceGp>(
            rSessionDownload->
                x_rAddonMapGp->
                    FindSingletonAddonSession(
                        CBasicClassIfaceGp::GGetStaticClassInfo())).
                        Valid();

    ref<CFileClassImpl> rFileClassDownload =
        (ref<CFileClassIface>)
        Derive<CFileClassIfaceGp>(
            rSessionDownload->
                x_rAddonMapGp->
                    FindSingletonAddonSession(
                        CFileClassIfaceGp::GGetStaticClassInfo())).
                        Valid();
    ref<CFilemanLocal> rFilemanLocalDownload =
        rFileClassDownload->
            x_rFilemanLocal;

    ref<CFileClassImpl> rFileClassUpload =
        (ref<CFileClassIface>)
        Derive<CFileClassIfaceGp>(
            rSessionUpload->
                x_rAddonMapGp->
                    FindSingletonAddonSession(
                        CFileClassIfaceGp::GGetStaticClassInfo())).
                        Valid();
    ref<CFilemanLocal> rFilemanLocalUpload =
        rFileClassUpload->
            x_rFilemanLocal;

    // both netcommon
    ref<CFilemanNetCommon> rFilemanNetCommonDownload = 
        rFileClassDownload->x_rFilemanNetCommon;
    ref<CFilemanNetCommon> rFilemanNetCommonUpload = 
        rFileClassUpload->x_rFilemanNetCommon;

    ref<CFilemanNetIn> rFilemanNetInDownload = 
        rFilemanNetCommonDownload->x_rFilemanNetIn;
    ref<CFilemanNetOut> rFilemanNetOutUpload = 
        rFilemanNetCommonUpload->x_rFilemanNetOut;

    // close unused but interfering endpoints
    {
        ref<CFilemanNetIn> rFilemanNetInUpload = 
            rFilemanNetCommonUpload->x_rFilemanNetIn;
        ref<CFilemanNetOut> rFilemanNetOutDownload = 
            rFilemanNetCommonDownload->x_rFilemanNetOut;

        rFilemanNetInUpload->CloseFilemanNetIn();
        rFilemanNetOutDownload->CloseFilemanNetOut();
    }

    TESTLOG(
        "",
        "\n"
            "=== Application started... ===\n\n");

    //
    // Upload changes from the original session
    //
    {
        // see TESTLOG(... rBox->ExposeOut().ExportAsText() + ...
        assertoff aoExpose("make sure it is OK to expose");

        TESTLOG(
            "",
            "\n"
                "=== Create new node ===\n\n");

        //
        // Create patient name
        //
        key keyPatientNameField = 
            //key::GNewUniqueKey();
            key::GConstKeyStr("test_PatientName");

        {
            /*
            // Name field class
            {
                ref<CActionIfaceGp> rNewAction;

                rNewAction->
                    DefineAction(
                        keyPatientNameField,
                        C_keyNodeClass,
                        "Trait");

                // Record and cache
                rStoreUpload->
                    RecordNewLocalAction(
                        rNewAction);
            }
            */

            // Name field property
            // Record and cache
            rStoreUpload->
                RecordNewLocalAction(
                    keyPatientNameField,
                    //C_keyPropertyName,
                    //GET_PROPERTY_NAME(CDbNode, x_sNodeName));
                    C_keyPropertyClass,
                    C_keyTieProp,
                    "test_PatientClass",
                    key(),
                    ptr<CActionIfaceGp>());
        }

        //
        // Create a patient
        //

        // Record and cache
        key keyPatient1 = key::GNewUniqueKey(DEBUG_ARG "Pat1");
        rStoreUpload->
            RecordNewLocalAction(
                // Node
                keyPatient1,
                // Trait
                keyPatientNameField,
                // Tie
                C_keyTieProp,
                // Scalar
                "Doe John",
                key(),
                ptr<CActionIfaceGp>());
        /*
        rStoreUpload->
            RecordNewLocalAction(
                // Node
                keyPatient1,
                // Trait
                C_keyNodeClass,
                // Scalar
                "test_PatientClass");
                */

        TESTLOG(
            "",
            "\n"
                "=== Poll upload ===\n\n");

        map<path, str> mapCreateFiles;
        mapCreateFiles["test.bat"] = "@echo off";
        mapCreateFiles["winres/cursors/wait.cur"] = "[O><O]";

        path pathUploadFilesRoot = 
            rFilemanLocalUpload->GetFilemanLocalRoot();

        bool bOutHaveJobs = false;
        bool bInHaveJobs = false;
        bool bOutOfJobs = false;
        int nLoopsOutOfJobs = 0;
        repeat(iPoll, 1000)
        {
            //
            // Upload Next
            //

            TESTLOG(
                "",
                "\n"
                    "--- Poll out ........\n\n");

            bOutHaveJobs = 
                rTestSession->
                    TestPollSyncOut();

            if (bOutHaveJobs)
            {
                TESTLOG(
                    "",
                    ""
                        "   upload " + Str(iPoll) + " performed.\n");
            }
            else
            {
                TESTLOG(
                    "",
                    ""
                        "   upload " + Str(iPoll) + " is idle.\n");
            }

            //
            // Download Next
            //

            TESTLOG(
                "",
                "\n"
                    "--- Poll in ........\n\n");

            bInHaveJobs = 
                rTestSession->
                    TestPollSyncIn();

            if (bInHaveJobs)
            {
                TESTLOG(
                    "",
                    ""
                        "   download " + Str(iPoll) + " performed.\n");
            }
            else
            {
                TESTLOG(
                    "",
                    ""
                        "   download " + Str(iPoll) + " is idle.\n");
            }

            //
            // Create next file
            //

            if (mapCreateFiles.GetCount() > 0)
            {
                TESTLOG(
                    "",
                    "\n"
                        "--- Create next test file ........\n\n");

                // fname and test content
                path pathFile = 
                    pathUploadFilesRoot.
                        GetAppendedPath(
                            mapCreateFiles.GetHeadKey());
                str sContent = mapCreateFiles.GetHeadValue();
                mapCreateFiles.
                    RemoveHead();
                    //RemoveAtPos(mapCreateFiles.GetHeadPos());

                // create
                str sError;
                file(pathFile).Save(sContent, out sError);
                rASSERTM(sError == "", sError);
            }

            // poll it out
            rFilemanLocalUpload->
                HandleLocalScanTimer();
            rFilemanNetOutUpload->
                HandleFilemanUploadTimer();

            // poll it in
            rFilemanNetInDownload->
                HandleFilemanDownloadTimer();

            if (bOutHaveJobs
                ||
                bInHaveJobs)
            {
                bOutOfJobs = false;
            }

            if (!bOutHaveJobs
                &&
                !bInHaveJobs
                &&
                !bOutOfJobs)
            {
                bOutOfJobs = true;

                str sNoMore = 
                    "\n"
                        "   ((((( no more jobs at loop " + Str(iPoll) + " )))))\n";
                TESTLOG(
                    "LocalFiles",
                    sNoMore);
                TESTLOG(
                    "Downloaded",
                    sNoMore);
                TESTLOG(
                    "",
                    sNoMore);
            }

            if (bOutOfJobs)
            {
                nLoopsOutOfJobs++;

                if (nLoopsOutOfJobs == 2)
                {
                    // !!!
                    // Otherwise debug splits by 3 actions 
                    //   and remainder is left hanging!
                    rStoreUpload->
                        FlushStore();
                }

                if (nLoopsOutOfJobs > 3)
                {
                    break;
                }
            }
            else
            {
                nLoopsOutOfJobs = 0;
            }
        }

        rASSERTM(nLoopsOutOfJobs > 0, "not enough loops to sync all?");

        rASSERT(!bOutHaveJobs);
        rASSERT(!bInHaveJobs);
    }

    //
    // Import downloaded
    //

    TESTLOG(
        "",
        "\n"
            "======= Import downloads =======\n");

    //debug::GRedirectTestLog("+Sync", "Downloaded");
    debug::GRedirectTestLog("+GoImport", "Downloaded");
    debug::GRedirectTestLog("+Download", "Downloaded");

    repeat(iRepeat, 100000)
    {
        bool bHaveJobs = false;
        rTestSession->
            TestImportSyncIn(
                out bHaveJobs); 

        str sLog =
            "\n"
                "  ((import loop " + Str(iRepeat) + 
                (bHaveJobs? " still have jobs..." : " no more jobs.") + 
                "))\n";

        TESTLOG(
            "Downloaded", 
            sLog);
        TESTLOG(
            "",
            sLog);

        if (!bHaveJobs)
        {
            break;
        }

        if (iRepeat > 100)
        {
            rFAIL("import never ends?");
        }
    }

    //
    // Finish
    //

    TESTLOG(
        "",
        "\n"
            "======= Test is complete. Dumping states... =======\n");

    //
    // Dump nodes
    //
    {
        TESTLOG(
            "Downloaded", 
            "\n\n"
                "======= Nodes =======\n\n");

        // iterate nodes
        ref<CBasicNodeIfaceGp> rIterBasicNode;
        for (
            iter i;
            rBasicClassDownload->
                QueryBasicNodes(
                    out i,
                    out rIterBasicNode,
                    key());)
        {
            str sClasses;

            ref<CRecentValueIfaceGp> rClassRecord;
            /*
            for (
                iter iClass;
                rBasicClassDownload->
                    QueryActualTraits(
                        out iClass,
                        out rClassRecord,
                        rIterBasicNode->GetNodePrimaryKey(),
                        key(), // all traits (classes here)
                        ptr<CFilterIfaceGp>(),
                        C_sScalarFieldOffObject, // sSkipValuesEqualTo
                        C_keyTieClass);)
                        */
            str sClassId;
            for (
                iter iClass;
                rBasicClassDownload->
                    QueryTieClassAssignments(
                        out iClass,
                        out sClassId,
                        out rClassRecord,
                        rIterBasicNode->GetNodePrimaryKey());)
            {
                if (sClasses != "")
                {
                    sClasses += "|";
                }

                sClasses += 
                    Str(sClassId) + // rClassRecord->x_keyValueForTrait
                        "[" + Str(rClassRecord->x_keyValueNonscalar) + "]" +
                        rClassRecord->x_sValueScalar;
            }

            TESTLOG(
                "Downloaded", 
                "" +
                    rIterBasicNode->GetNodePrimaryKey().DumpToBareStrKey() +
                    ": " +
                    "C::" + 
                    sClasses +
                    " " +
                    ":" + 
                    rIterBasicNode->GetPropertyClass() +
                    " " +
                    rIterBasicNode->GetNodeNameAsIs() + 
                    "\n");
        }
    }

    //
    // Dump Nodes Exposed
    //
    {
        TESTLOG(
            "DownloadedExposed", 
            "\n\n"
                "======= Nodes Exposed =======\n\n");

        assertoff ao1("make sure it is OK to expose");

        // iterate nodes
        ref<CBasicNodeIfaceGp> rIterBasicNode;
        for (
            iter i;
            rBasicClassDownload->
                QueryBasicNodes(
                    out i,
                    out rIterBasicNode,
                    key());)
        {
            TESTLOG(
                "DownloadedExposed", 
                "" +
                    rIterBasicNode->ExposeOut().ExportAsTextWithoutHeader() +
                    "\n");
        }
    }

    //
    // Dump Files
    //
    {
        TESTLOG(
            "Downloaded", 
            "\n\n"
                "======= Files =======\n\n");

        // iterate Files
        ref<CFileNodeIfaceGp> rIterFileNodeGp;
        for (
            ref<CQueryIfaceGp> rIterQuery;
            rFileClassDownload->
                IterateFileNodes(
                    out rIterQuery,
                    out rIterFileNodeGp);)
        {
            ref<CFileNodeImpl> rIterFileNode = 
                (ref<CFileNodeIface>)
                rIterFileNodeGp;

            ref<CDbFile> rDbFile = 
                rIterFileNode->x_rDbFile;

            TESTLOG(
                "Downloaded", 
                "" +
                    rIterFileNode->GetFilePrimaryKey().DumpToBareStrKey() +
                    ": " +
                    "[ " + 
                    rIterFileNode->GetFileDir() +
                    " ]/< " +
                    rIterFileNode->GetFileName() +
                    " > " +
                    Bytes(rDbFile->x_nnFileBytes) + 
                    " bytes, " +
                    Bytes(rDbFile->x_nnFileUploadedBytes) + 
                    " uploaded." +
                    "\n");
        }

        //
        // Dump received files with content
        //
        path pathFileSyncInRoot = 
            rFilemanLocalDownload->
                GetFilemanLocalRoot();

        ref<CPathList> rPathList =
            dir(pathFileSyncInRoot).GetFilesAndDirectoriesIncludingSubfolders(oo);

        TESTLOG(
            "Downloaded", 
            "\n"
                "---- Actual receive content for " + 
                Str(rPathList->GetCount()) +
                " files in " +
                pathFileSyncInRoot +
                ":\n\n");

        path pathIter;
        for (
            iter i;
            rPathList->
                Iterate(
                    out i,
                    out pathIter);)
        {
            // skip dirs
            if (!file(pathIter).IsExist())
            {
                continue;
            }

            str sError;
            str sContent;
            file(pathIter).Load(out sContent, out sError);
            rASSERTM(sError == "", sError);

            sContent = sContent.GetReplaced("\n", "\\n");

            TESTLOG(
                "Downloaded", 
                "" +
                    "   " + pathIter + " --- \n        \"" +
                    sContent + 
                    "\"\n"
                    "\n");
        }
    }

[...]

    //
    // Dump Files Exposed
    //
    {
        TESTLOG(
            "DownloadedExposed", 
            "\n\n"
                "======= Files Exposed =======\n\n");

        assertoff ao1("make sure it is OK to expose");

        // iterate Files
        ref<CFileNodeIfaceGp> rIterFileNode;
        for (
            ref<CQueryIfaceGp> rIterQuery;
            rFileClassDownload->
                IterateFileNodes(
                    out rIterQuery,
                    out rIterFileNode);)
        {
            TESTLOG(
                "DownloadedExposed", 
                "" +
                    rIterFileNode->ExposeOut().ExportAsTextWithoutHeader() +
                    "\n");
        }
    }

    // expose final object states:
    {
        assertoff ao1("make sure it is OK to expose");
        assertoff ao2("expose a Loader object itself");
        assertoff aoGuiPlus("it may not be safe to expose this GuiPlus class!");

        // skip a line
        TESTLOG(
            "",
            "\n");

        // session dump
        #ifndef ONLYPORT // missing packer changes file sizes in reports
            TESTLOG(
                "FinalTestSession",
                "FinalTestSession: " + 
                    rTestSession->ExposeOut().ExportAsText() + "\n\n");
        #endif

        // config dump
        TESTLOG(
            "FinalConfig",
            "GlobalConfig: " + 
                rGlobalConfig->ExposeOut().ExportAsText() + "\n\n");
    }

    //
    // Close Application 
    //

    TESTLOG(
        "",
        ""
            "======= Closing =======\n\n");

    // This will use fake PatientLink to make important decisions
    //   (will not really exit in TEST mode I hope...)
    rTestSession->
        CloseTestSession();

    CFilemanNetCommon::s_pForceNetDriverForTests = null();

    TESTLOG(
        "",
        "\n"
            "=== The End ==="
            "\n\n");

    TESTLOG(
        "",
        "   (utc999 = " + Str(sys::GGetUtc()) + ")\n");
}

