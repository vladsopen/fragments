// CTotalTestImpl.cpp

#include "CUsmainProject.h"
#include "CTotalTestImpl.h"
#include "CPatientLinkImpl.h"
#include "CPatientBaseConnectionIfaceGp.h"
#include "CPatientSampleIfaceGp.h"
#include "CLoaderImpl.h"
#include "CSessionImpl.h"
#include "CDocListImpl.h"
#include "CDocImpl.h"
#include "CDocFormatGp.h"
#include "CMatrixFormatGp.h"
#include "CHardwareImpl.h"
#include "CDeviceCommon.h"
#include "CRawCapture.h"
#include "CImageImpl.h"
#include "CViewCache.h"
#include "CFftImpl.h"
#include "CGlobalConfigIface.h"
#include "CColoring.h"
#include "CLocalPaletteConfigIface.h"
#include "CAmplitudeToColorIndexGp.h"
#include "CGlobalPaletteConfigIface.h"
#include "CColorListGp.h"
#include "CGenerateFftTest.h"
#include "CGeneratorListIface.h"
#include "CGeneratorIface.h"
#include "CViewState.h"
#include "CStoreImpl.h"
#include "CMatrixListImpl.h"
#include "CRawAudioImpl.h"
#include "CRawWaveImpl.h"
#include "CTestDoc.h"
#include "CCalcupSystemImpl.h"
#include "CUsoundCalcupSystemIfaceGp.h"
#include "CAccelIfaceGp.h"
#include "CMapDealsIfaceGp.h"
#include "CCallDealIfaceGp.h"
#include "CRawCaptureList.h"
#include "CCustomizeDocIfaceGp.h"
#include "VirtOs_CAppGp.h"
#include "VirtOs_CAppGp.h"
#include "VirtOs_CGlobalUi.h"

CTotalTestImpl::CTotalTestImpl()
{
}

void CTotalTestImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CTotalTestImpl!");
}

void CTotalTestImpl::_test_RunTotalTestVariation()
{
    debug::GMirrorAssertFailuresToLog("");

    TESTLOG(
        "",
        "\n"
            "----------- TotalTest started... ------------\n\n");

    // searching for rogue key calls
    debug::GRedirectTestLog("_NextKey", "");

    /*
    // Log control
    if (m_bDetailedLog)
        debug::GEnableTestLog("DownloadDetails");
    else
        debug::GDisableTestLog("DownloadDetails");
    */

    // what should not normally be exposed
    assertoff aoNoExposeFlow("to expose CFlowContextIfaceGp!");

    //
    // Loader (we test the Loader itself here!)
    //

    ref<CLoaderImpl> rLoader = 
        CUsmainProject::GGetIt().x_rLoader;

    //
    // PatientLink - prepare to emulate Patients
    //

    TESTLOG(
        "",
        "\n"
            "=== Init fake Patients connection parameters ===\n");

    // Access to Patients Link (for result log)
    ref<CPatientLinkImpl> rPatientLink = rLoader->x_rPatientLink;

    // Access to lower level PatientIo API (for result log)
    this->x_rPatientBaseConnection = 
        rPatientLink->x_rPatientBaseConnection;

    // Test: Lets log Patients into the global log
    //x_rPatientBaseConnection->x_sPatientLogName = "";

    // Test: Work as if we are successfully connected to Patients
    x_rPatientBaseConnection->_x_bEmulatePatientConnectionOkForClassTest = true;
        // no separate standalone logic in app yet, otherwise it got to be
        //   tested for unconnected mode

    // App: Init Patients Connecton
    //rPatientLink->InitPatientConnection();
        // has simple internal once-init check, linear otherwise

    //
    // Start Application 
    //

    TESTLOG(
        "",
        "\n"
            "=== Actual application starts here ===\n\n");

    // This will use fake PatientLink to make important decisions
    rLoader->StartApplication();

    //
    // For Patients-connected mode we emulate activation call as if it came from Patients itself
    //

    TESTLOG(
        "",
        "\n"
            "=== Now real app would stay hidden and wait for a Patients call ===\n"
            "=== But we won't wait: lets emulate an activation call from Patients ==="
            "\n\n");

    // Test: Handle connection mode
    ASSERT(rPatientLink->IsUsoundConnectedToPatient());

    // This should create a session and show the Main Frame
    x_rPatientBaseConnection->EmulateCallbackActivateSamplerApp();

    TESTLOG(
        "",
        "\n"
            "=== by now the app is visible and ready to receive user input ==="
            "\n");

    //
    // First Test Doc Context
    //

    array< ref<CTestDoc> > aTestDoc;
    aTestDoc.SetCount(2);

    //
    // Session
    //

    ref<CSessionImpl> rSession = rLoader->x_rSession;
    ref<CDocListImpl> rDocList = rSession->x_rDocList;
    ref<CHardwareImpl> rHardware = 
        Derive<CHardwareImpl>(rSession->x_rHardware).Valid();
    ref<CDeviceCommon> rDeviceCommon = rHardware->GetDeviceForClassTests();

    rDocList->x_bOwnDocs = true;

    repeat(iDoc, aTestDoc.GetCount())
    {
        aTestDoc.GetAt(iDoc)->x_rRawCapture = 
            rDeviceCommon->
                x_rRawCaptureList->
                    GetRawCaptureForClassTests(
                        iDoc);
    }

    ref<CGeneratorListIface> rGeneratorList = 
        rHardware->x_rGeneratorList;
    ref<CGeneratorIface> rGenerator = 
        rGeneratorList->GetActiveGenerator();

    // global config (temporary for the test of course!)
    ref<CGlobalConfigIface> rGlobalConfig = 
        CUsmainProject::GGetIt().
            x_rUsmainGlobalConfig;
    ref<CGlobalPaletteConfigIface> rGlobalPaletteConfig = 
        rGlobalConfig->_x_rGlobalPaletteConfig;

    InitTestPalette(rGlobalPaletteConfig);

    //
    // Add a Media to the List
    //

    TESTLOG(
        "",
        "\n"
            "=== we need a device attached doc to work with ==="
            "\n");

    // Fill in init parameters
    ref<CDocFormatGp> rDocFormat;
    m_rMatrixFormat = 
        rDocFormat->x_rMatrixFormat;

    // Unusual media metrics for faster test
    m_rMatrixFormat->x_nMatrixSliceAmplitudeRange = 5;
    m_rMatrixFormat->x_nMatrixLength = 7;
    rDocFormat->x_nDefaultViewWidthSlices = 3;

    //
    // Init for online mode
    //

    repeat(iDoc, aTestDoc.GetCount())
    {
        aTestDoc.GetAt(iDoc)->x_rRawCapture->
            AddTargetDoc(
                rDocFormat);
    }

    // find the doc created by the device
    int nCountDocs = 0;
    ref<CDocIface> rIterDoc;
    for (
        iter i;
        rDocList->
            IterateRegisteredDocs(
                out i,
                out rIterDoc);)
    {
        NOTEST;
 
        if (rIterDoc->IsDocInitializedForRecording() &&
            !rIterDoc->_x_rCustomizeDoc->x_bHideAboveTimeBar)
        {
            ref<CTestDoc> rIterTestDoc = 
                aTestDoc.GetAt(nCountDocs);

            rIterTestDoc->x_iTestDocInList = i;
            rIterTestDoc->x_rNewDoc = rIterDoc;

            nCountDocs++;
        }
    }

    rASSERT(aTestDoc.GetCount() == 2);

    //
    // Init UI
    //

    TESTLOG(
        "",
        "\n"
            "=== Create Media Recorder Windows  ==="
            "\n\n");

    repeat(iDoc, aTestDoc.GetCount())
    {
        ref<CTestDoc> rIterTestDoc = aTestDoc.GetAt(iDoc);

        // Doc attribute variations
        str sDocLabel = "";
        if (iDoc == 0)
        {
            sDocLabel = "First (Online)";
            rIterTestDoc->x_pointVisualFramePos = point(100, 50);
            rIterTestDoc->x_nOnlineIterations = 8;
            rIterTestDoc->x_nPostOnlineIterations = 4;
        }
        else
        if (iDoc == 1)
        {
            sDocLabel = "Second (Online)";
            rIterTestDoc->x_pointVisualFramePos = point(100, 400);
            rIterTestDoc->x_nOnlineIterations = 1;
            rIterTestDoc->x_nPostOnlineIterations = 0;
        }
        else
        {
            rFAIL("add init for more docs!");
        }

        // Set label as vessel
        rIterTestDoc->
            x_rRawCapture->
                x_sEmulateVesselName = sDocLabel;

        // Prepare online state
        rIterTestDoc->
            x_rRawCapture->
                SendClusterStateToAllDocs();

        rIterTestDoc->PrepareRecorderUi(this);
    }

    //
    // Online Mode
    //

    TESTLOG(
        "",
        "\n"
            "=== ON! ON! ON! ON! ON! ON! ON! ON! ON! ON! ON! ON!  ==="
            "\n");

    // ON!
    rDeviceCommon->StartRecordingInAllCaptures();
    repeat(iDoc, aTestDoc.GetCount())
    {
        ref<CTestDoc> rIterTestDoc = aTestDoc.GetAt(iDoc);

        assertoff aoNoPictureToUpdateYet("dcScaledPicture.IsReadyToDraw()");

        //rIterTestDoc->
        //    x_rRawCapture->
        //        TriggerOnlineModeEmulation();

        // dig calcup system
        ref<CDocImpl> rDoc = rIterTestDoc->x_rNewDoc;
        ref<CCalcupSystemImpl> rCalcupSystem = rDoc->x_rCalcupSystem;
        ref<CUsoundCalcupSystemIfaceGp> rUsoundCalcupSystem = 
            rCalcupSystem->_x_rCalcupSystem;

        // recalc indexes often enough
        rUsoundCalcupSystem->x_numMaxScreenWidthToSnapCalcToInSeconds = 0.001;
    }

    TESTLOG(
        "",
        "\n"
            "=== Recording  ==="
            "\n");

    // Record!
    repeat(iDoc, aTestDoc.GetCount())
    {
        ref<CTestDoc> rIterTestDoc = aTestDoc.GetAt(iDoc);

        // Go!
        rIterTestDoc->
            RunOnlineTest(this);
    }

    TESTLOG(
        "",
        "\n"
            "=== OFF! OFF! OFF! OFF! OFF! OFF! OFF! OFF! OFF! OFF! OFF! OFF!  ==="
            "\n");

    // OFF!
    rDeviceCommon->StopRecordingInAllCaptures();
    repeat(iDoc, aTestDoc.GetCount())
    {
        ref<CTestDoc> rIterTestDoc = aTestDoc.GetAt(iDoc);

        //rIterTestDoc->
        //    x_rRawCapture->
        //        TriggerOnlineModeEmulation();
    }

    TESTLOG(
        "",
        "\n"
            "=== Review  ==="
            "\n");

    // Offline Review
    repeat(iDoc, aTestDoc.GetCount())
    {
        ref<CTestDoc> rIterTestDoc = aTestDoc.GetAt(iDoc);

        rIterTestDoc->
            RunOfflineTest(this);
    }

    TESTLOG(
        "",
        "\n"
            "=== Plaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaayback  ==="
            "\n");

    debug::GHideAskByQuoteAndReplyOk(
        "No data for playback");

    // Playback
    repeat(iDoc, aTestDoc.GetCount())
    {
        TESTLOG("", "\n" "Testing Playback for Doc " + Str(iDoc) + ":\n");

        ref<CRawCapture> rRawCapture = 
            aTestDoc.GetAt(iDoc)->x_rRawCapture;

        rRawCapture->TriggerAudioPlayback();

        CProject::EIdleNext eIdleNext = CProject::E_IdleNext_StandBy;
        repeat(i, 10)
        {
            rHardware->HandleIdleIsOnlineMode(out eIdleNext);
        }

        // turn off if on
        if (rRawCapture->x_iPlayingAtComposite != -1)
        {
            rRawCapture->TriggerAudioPlayback();
        }

        TESTLOG("", "\n" "Playback test " + Str(iDoc) + " complete.\n");
    }

    // Asymmetry calc
    //aTestDoc.GetAt(0)->
    //    x_rNewDoc->x_rCalcupSystem->CalcAsymetryIndexesComparingToDoc(
    //        aTestDoc.GetAt(1)->x_rNewDoc);

    TESTLOG(
        "",
        "\n"
            "=== Calc  ==="
            "\n");

    TESTLOG("", "\n" "Asymetry for our 2 docs:\n");

    rSession->
        CalcAsymetryIndexesForDocPair(
            aTestDoc.GetAt(0)->x_rNewDoc,
            aTestDoc.GetAt(1)->x_rNewDoc);

    TESTLOG("", "\n" "Asymetry via session own test:\n");
    rSession->TestCalcAsymmetryFor2Docs();

    // Test index setup dialog updates
    TESTLOG("", "\n" "Test Index Setup updates...\n");
    aTestDoc.GetAt(1)->
        x_rNewDoc->x_rCalcupSystem->
            OpenIndexSetupDialogForDoc();

    TESTLOG(
        "",
        "\n"
            "=== Commands  ==="
            "\n");

    // Call all accels for all docs
    repeat(iRepeat, 2)
    {
        repeat(iDoc, aTestDoc.GetCount())
        {
            ref<CTestDoc> rIterTestDoc = aTestDoc.GetAt(iDoc);
            ref<CDocImpl> rDoc = rIterTestDoc->x_rNewDoc;

            type<CAccelIfaceGp> typeIterAccel;
            int nAccels = 0;
            for (
                iter iAccel;
                type<CAccelIfaceGp>()->
                    GetDerivedListUsound().
                        Iterate(
                            out iAccel,
                            out typeIterAccel);)
            {
                nAccels++;
                /*
                if (iAccel > 118) // OK: 15 fail 18
                {
                    continue;
                }
                */

                //// by default filter only menu items
                //if (!typeIterAccel->IsUsoundOptionsMenuItem() &&
                //    !sys::GIsAccess(...))
                //{
                //    continue;
                //}

                //
                // Emulate Next Menu Item (slow in loop, but this is a menu - OK)
                //
                // Only query every command once per doc
                if (iRepeat == 0)
                {
                    //ref<CUiMenuItemGp> rUi;

                    // Attribute query 
                    ref<CCallDealIfaceGp> rCallDeal;

                    // Generic call is limited, but we dont need a return value.
                    //   Normally a client should call Doc through rDealDocCommands,
                    //   directly, but I want to test a trickier way here.
                    //   Anyway I would not be able to make all of them in a loop 
                    //   with rDealDocCommands.
                    typeIterAccel->
                        TryDealWithSession(
                            rCallDeal,
                            rSession);
                    typeIterAccel->
                        TryDealWithDoc(
                            rCallDeal,
                            rDoc);

                    // show everything for testing
                    sloc slocCommandTextForUi = typeIterAccel->GetCommandTextForUi();

                    // dont forget this!
                    //rUi->SetTip(typeIterAccel->GetCommandDescriptionTipForUi());

                    // (icon return is optional for the type<>!)
                    //res resIcon = typeIterAccel->GetCommandIcon(size());
                    //rUi->SetIcon(
                    //    resIcon == null()? 
                    //        RES__OK_ICON() // temp stub
                    //        : 
                    //        resIcon);

                    ////rUi->SetIconLocationAtTop();

                    //// the deal-handler may disable the command
                    //if (rCallDeal->x_slocCommandDisableInfo != "")
                    //{
                    //    rUi->SetDisableInfo(rCallDeal->x_slocCommandDisableInfo);
                    //}

                    TESTLOG(
                        "",
                        typeIterAccel->GetCommandSymbol() + ": " 
                            "(" + 
                            CAppGp::GGet()->x_rUi->ConvertToPlainText(slocCommandTextForUi) + 
                            ") " + 
                            (typeIterAccel->GetCommandIcon(size()) == null()? 
                                "null icon" 
                                : 
                                "icon OK") + 
                            ", " +
                            "X:[" + rCallDeal->GetCommandDisableInfo() + "]"
                            "\n");
                }

                //rUi->OnClick = [=]
                {
                    // Actual command execution
                    //   Ececule all commands once interleaving both interfaces
                    //   for both docs
                    
                    if (iRepeat != 0)
                    {
                        // via Doc

                        // Attribute query 
                        ref<CCallDealIfaceGp> rCallDealUsoundGo;

                        // (the same notes for the direct call)

                        assertoff aoScaleOutOfRange(
                            "nConvertToRange >= 0 && nConvertToRange <= oo");

                        rCallDealUsoundGo->RequestCommandExecution();
                        typeIterAccel->
                            TryDealWithSession(
                                rCallDealUsoundGo,
                                rSession);
                        typeIterAccel->
                            TryDealWithDoc(
                                rCallDealUsoundGo,
                                rDoc);
                    }
                    else
                    {
                        // Execute via accel map

                        ref<CMapDealsIfaceGp> rMapDeals = 
                            CAppGp::GGet()->
                                x_rMapDeals;

                        assertoff aoNoPictureToUpdateYet(
                            "pure CDeviceCommon::OnSetSlotAdjuster called");
                        assertoff aoScaleOutOfRange(
                            "nConvertToRange >= 0 && nConvertToRange <= oo");

                        ref<CCallDealIfaceGp> rCallDealUsoundGlobal;
                        rCallDealUsoundGlobal->RequestCommandExecution();
                        rMapDeals->
                            DealGlobalCommandByAccel(
                                typeIterAccel->GetGlobalAccel(),
                                rCallDealUsoundGlobal);
                    }
                }

                //NL();
            }

            TRACE("\n--------------------- nAccels " + Str(nAccels) + "\n");
        }
    }

    // Save
    repeat(iDoc, aTestDoc.GetCount())
    {
        ref<CTestDoc> rIterTestDoc = aTestDoc.GetAt(iDoc);

        //
        // Save media to Patients
        //

        assertoff ao1("no table exist in db for Usmain-CDbPaletteIface");
        assertoff ao2("no root object in db for Usmain-CDbPaletteIface");
        assertoff ao3("member object does not exist in DB yet: Usmain-CDbLo");

        TESTLOG(
            "",
            "\n"
                "=== Lets emulate the user clicking Save button ==="
                "\n\n");

        rIterTestDoc->x_rNewDoc->SendMediaToDbNow(rPatientLink);

        //
        // Load Test
        //

        TestLoadFromPatient(rIterTestDoc->x_rNewDoc->_x_pathLastSavedDoc);
    }

    TESTLOG(
        "",
        "\n"
            "=== Closing second doc ==="
            "\n\n");

    // Test normal doc close
    aTestDoc.GetAt(1)->x_rNewDoc->CloseDoc();
    //
    // Finish
    //

    TESTLOG(
        "",
        "\n"
            "======= TotalTest is complete. Dumping states... =======\n");

    // expose final object states:
    {
        assertoff ao1("make sure it is OK to expose");
        assertoff ao1SendCluster("it is not OK to expose CSendClusterIfaceGp");
        assertoff ao2("why CMatrixListImpl is exposed without");
        assertoff ao3("no table exist in db for Usmain-CDbPaletteIface");
        assertoff ao4("it may not be safe to expose this GuiPlus class!");
        //assertoff ao4("no root object in db for Usmain-CDbPaletteIface");

        // skip a line
        TESTLOG(
            "",
            "\n");

        // session dump
        TESTLOG(
            "FinalSession",
            "rSession: " + 
                rSession->ExposeOut().ExportAsText() + "\n\n");

        // config dump
        TESTLOG(
            "FinalConfig",
            "GlobalConfig: " + 
                rGlobalConfig->ExposeOut().ExportAsText() + "\n\n");
    }

    // App: Close session
    //rSession->TerminateSession();

    //
    // Close Application 
    //

    TESTLOG(
        "",
        ""
            "======= Closing UI, session close may reenter " + 
                "because OnClose calls it again =======\n\n");

    repeat(iDoc, aTestDoc.GetCount())
    {
        ref<CTestDoc> rIterTestDoc = aTestDoc.GetAt(iDoc);

        rIterTestDoc->_m_rUiOnlineMedia->thispanel()->CloseFrame();
    }

    // This will use fake PatientLink to make important decisions
    //   (will not really exit in TEST mode I hope...)
    rLoader->CloseUsoundSessionAndMainWindow();

    TESTLOG(
        "",
        "\n"
            "--------------- final dumps complete for this run ------------------\n\n");
}

void CTotalTestImpl::TestLoadFromPatient(
        path pathFrom)
{
    //
    // Load media from Patients
    //

    TESTLOG(
        "",
        "\n"
            "=== Lets emulate a request to open a media from Patients ==="
            "\n\n");

    ref<CPatientSampleIfaceGp> rPatientSample;
    rPatientSample->x_pathAttachmentPrefix = 
        pathFrom;

    TESTLOG(
        "",
        "(reusing saved temp file " + rPatientSample->x_pathAttachmentPrefix + 
            " to load from it now)\n");

    // Open
    {
        // stored x_nDelayAheadSlices scrambles thing
        assertoff aoViewFailure(
            "m_iScrollSlice <= "
                "nStoreSlices - math::GMin(m_nWidthSlices, nStoreSlices) || "
                "m_iScrollSlice == 0");
        // after I added audio save/load
        assertoff aoViewFailure2(
            "nIndent <= GetAudioSliceShortCount() / 2");
        assertoff aoViewFailure3(
            "out_iFirstVisibleVirtualOffset >= 0");
        assertoff aoViewFailure4(
            "numSecondsMax > 0 && numSecondsMax < 10 * 60");
        assertoff aoViewFailure5(
            "nComposites > 0");
        assertoff aoViewFailure6(
            "CalcMaxStoredAudioComposites() >= rAudioSlice->GetCount() / 2");
        assertoff aoViewFailure7(
            "value > 0 || (debug::GIsTest() && value == -111555)");

        // Go!
        x_rPatientBaseConnection->
            EmulateCallbackOpenSample(
                rPatientSample);
    }
}

void CTotalTestImpl::OnTestClass()
{
    assertoff aoNoInProcPreparation1(
        "there is no Synchronous Parallelizer emulation setup for "
            "this ClassTest");
    assertoff aoNoInProcPreparation2(
        "no parallel process has executed our I/O request");

    //debug::GRedirectTestLog("_NextTick", "");

    //
    // Prepare dummy config
    //
    {
        /*
        // Global ref
        ref<CBodySchemeImpl> rBodyScheme = 
            CProject::GGetIt().
                x_rGlobalConfig->
                    x_rBodyScheme;

        // Stub defs
        ref<CBodyProfileImpl> rBodyProfile = 
            rBodyScheme->AppendNewProfile();
        */
    }

    /*
    // Smaller sample
    {
        ref<CTotalTestImpl> rTotalTest;

        rTotalTest->m_bDetailedLog = true;

        rTotalTest->m_nForceEmigraphSamplePointCount = 5;
        rTotalTest->m_nForceEmigraphFrameDataBytes = 2 * 4;

        rTotalTest->m_anPresetAmplitudes.Add(1);
        rTotalTest->m_anPresetAmplitudes.Add(1000);
        rTotalTest->m_anPresetAmplitudes.Add(0);
        rTotalTest->m_anPresetAmplitudes.Add(10);
        rTotalTest->m_anPresetAmplitudes.Add(77);

        rTotalTest->_test_RunTotalTestVariation();
    }
    */

    // Normal execution
    {
        ref<CTotalTestImpl> rTotalTest;

        rTotalTest->_test_RunTotalTestVariation();
    }

    /*
    // Obsolete frame
    {
        ref<CTotalTestImpl> rTotalTest;

        rTotalTest->m_bObsoleteHeaderSignature = true;

        rTotalTest->_test_RunTotalTestVariation();
    }

    // Device not ready
    {
        ref<CTotalTestImpl> rTotalTest;

        rTotalTest->x_bTestDeviceOffline = true;

        // Start will fail when not connected
        debug::GHideAskByQuoteAndReplyOk(
            "Make sure it is connected");

        rTotalTest->_test_RunTotalTestVariation();
    }
    */
}

void CTotalTestImpl::InitTestPalette(
        ref<CGlobalPaletteConfigIface> rGlobalPaletteConfig)
{
    //
    // Change to color palette midway through the test
    //

    ref<CAmplitudeToColorIndexGp> rAmplitudeToColorIndex = 
        rGlobalPaletteConfig->x_rAmplitudeToColorIndex;
    ref<CColorListGp> rGammaColorList = 
        rGlobalPaletteConfig->x_rGammaColorList;

    // a recognizable palette 
    //  (some colors are mapped out when we init non-linear map)
    rGammaColorList->Add(rgb(255, 100, 100)); // [0] -- unused -- 
[...]