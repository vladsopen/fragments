// CTotalTest.cpp

#include "CProject.h"
#include "CTotalTest.h"
#include "CDownloaderImpl.h"
#include "CDeviceManagerIface.h"
#include "CTomographEmulator.h"
#include "CFakeTomograph.h"
#include "VirtOs_ask.h"
#include "CSessionIface.h"
#include "CPatientLinkImpl.h"
#include "CLoaderImpl.h"
#include "CPatientBaseConnectionIfaceGp.h"
#include "CWizardStepType.h"
#include "CBodySchemeImpl.h"
#include "CBodyPointImpl.h"
#include "CBodyProfileImpl.h"
#include "CGlobalConfigIface.h"
#include "CFakeTomographMemory.h"
#include "CTomogramIface.h"
#include "CTomogramImpl.h"
#include "CPatInfoFieldInputButtonIfaceGp.h"
#include "CPatInfoSessionIfaceGp.h"
#include "CPatInfoBrowserIfaceGp.h"
#include "CPiPatientIfaceGp.h"
#include "CPatInfoStoreIfaceGp.h"

CTotalTest::CTotalTest()
{
}

void CTotalTest::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CTotalTest!");
}

void CTotalTest::_test_RunTotalTestVariation()
{
    ref<CDownloaderImpl> rDownloader;

    _test_RunTotalTestVariationForDownloader(rDownloader);

    rDownloader->ShutdownDownloaderUi();
}

void CTotalTest::_test_RunTotalTestVariationForDownloader(
        ref<CDownloaderImpl> rDownloader)
{
    // Log control
    if (m_bDetailedLog)
        debug::GEnableTestLog("DownloadDetails");
    else
        debug::GDisableTestLog("DownloadDetails");

    // Session
    ref<CSessionIface> rSession;

    // (Patients init moved out)

    // App: Create a Downloader, linked to the session
    rDownloader->_x_pSessionTrusted = rSession;
        // ptr must be non-null always, not separate logic for null to test here

    ref<CPatientLinkImpl> rPatientLink = 
        CProject::GGetIt().
            x_rLoader->
                x_rPatientLink;

    // App: Init session
    rSession->PrepareToNewSession(rPatientLink);
        // must be called otherwise mutiple asserts will call attention

    // Test: Initial status
    rich richStatus = 
        rDownloader->
            x_typeWizardStepType->
                GetShortDownloadStatusForUi(
                    rDownloader);
    rASSERT(richStatus != ""); // this is it for UI testing for now... :(

    // Test: Initial state
    rASSERT(!rDownloader->IsDownloadInProgress());

    // Test: Fake Device
    ref<CDeviceManagerIface> rDeviceManager = 
        rSession->x_rDeviceManager;
    rASSERT(!rDeviceManager->IsTomographOpen());

    // Test: Set device to emulator mode
    //rDeviceManager->EnableTomographEmulatorWuu(true);
    rSession->EnableTomographEmulator(true);
    rASSERT(!rDeviceManager->IsTomographOpen());

    // Test: Access current private device object
    //   We expect to have the Emulator as the active device
    ref<CTomographIface> rTomographIface = 
        rDeviceManager->GetInternalTomographIface();
    ref<CTomographEmulator> rTomographEmulator = 
        Derive<CTomographEmulator>(rTomographIface).Valid();

    // Preset sample length
    if (m_nForceTomographSamplePointCount > 0)
    {
        rTomographEmulator->x_nTomographSamplePointCount = 
            m_nForceTomographSamplePointCount;
    }

    // Preset frame length
    if (m_nForceTomographFrameDataBytes > 0)
    {
        rTomographEmulator->x_nTomographFrameDataBytes = 
            m_nForceTomographFrameDataBytes;
    }

    // Test: Emulator-Device has a link to private Fake Device
    ref<CFakeTomograph> rFakeTomograph = rTomographEmulator->x_rFakeTomograph;
    rFakeTomograph->x_bObsoleteHeaderSignature = m_bObsoleteHeaderSignature;

    // Test: Switch Fake defice on
    //rFakeTomograph->x_bTomographPowered = !x_bTestDeviceOffline;
    rFakeTomograph->x_bUsbPlugedIn = !x_bTestDeviceOffline;

    // Fake memory data
    ref<CFakeTomographMemory> rFakeTomographMemory = 
        rFakeTomograph->x_rFakeTomographMemory;

    // Fill amplitudes directly?
    ref<CTomogramIface> rPredefinedTomogram;
    if (m_anPresetAmplitudes.GetCount())
    {
        rASSERT(
            m_anPresetAmplitudes.GetCount() == 
                m_nForceTomographSamplePointCount);

        // Preload and clear
        rFakeTomographMemory->EnsureFakeTomographMemoryLoaded(rTomographEmulator);
        rFakeTomographMemory->SetTomogramCount(0);

        repeat(iAmplitude, m_anPresetAmplitudes.GetCount())
        {
            int nAmplitude = m_anPresetAmplitudes.GetAt(iAmplitude);

            rPredefinedTomogram->
                AppendNextAmplitudePairFromTomograph(
                    nAmplitude,
                    nAmplitude);
        }

        rASSERT(
            rPredefinedTomogram->GetTomogramAmplitudeCount() == 
                m_anPresetAmplitudes.GetCount());

        rPredefinedTomogram->x_sTomogramUniqueName = "Test";

        // Insert the predefined tomogram
        rFakeTomographMemory->AddTomogram(rPredefinedTomogram);

        rASSERT(rFakeTomographMemory->GetTomogramCount() == 1);
    }

    // Create minimal UI for some indication
    ref<UiContainer> rUi = 
        rDownloader->
            NewUiDownloaderMain(
                rDeviceManager);
    rUi->CreateExclusiveModalTopmost();

    // App: Start download
    if (rDownloader->
            GotoNextStep(
                type<CWizardStepTypeDownloading>(),
                rDeviceManager))
    {
        // Success, did we expect it?

        if (x_bTestDeviceOffline)
        {
            rFAIL("UnitTest started download from disconnected device");

            rSession->TerminateSession();
            return;
        }
    }
    else
    {
        // Failure, is it a surprise?

        if (!x_bTestDeviceOffline)
        {
            rFAIL("UnitTest failed to start download");
        }

        rSession->TerminateSession();
        return;
    }

    rDownloader->x_bDoEraseAllWhenDone = m_bEraseAllUponCompletion;

    // App: Emulate timer until all downloaded
    int nContinuePolling = 100; // extra "timer" cycles to stress the logic
    rASSERT(rDownloader->IsDownloadInProgress());
    const int C_nMaxPolling = 100 * 1000;
    int nPollCount = 0;
    while (nContinuePolling > 0)
    {
        // poll until ready
        rDeviceManager->HandleIdleTomographPolling(rDownloader);

        // and poll some more!
        if (!rDownloader->IsDownloadInProgress())
            nContinuePolling--;

        // device failed?
        if (rDeviceManager->GetFullDeviceErrorInfoOr("") != "")
            nContinuePolling--;

        // inifinite loop breaker
        nPollCount++;
        if failed(nPollCount < C_nMaxPolling)
            break;
    }

    // we could check download sample count and other stats here,
    //   but why bother - everything is sent to Patients and will be checked with TESTLOG()

    // Test: Just check stuff we don't see through Patients contact
    //   and since we are lazy lets just append stuff to the log
    //str sLog = rPatientBaseConnection->x_sPatientLog;

    // expose final object states:
    {
        assertoff ao("make sure it is OK to expose");

        // skip a line
        TESTLOG(
            "",
            "\n");

        // test the tester with a complex expression
        TESTLOG(
            "FinalDownloader",
            "rDownloader: " + 
                rDownloader->ExposeOut().ExportAsText() + "\n\n");

        // the first test for TESTVAR
        TESTVAR("", rDeviceManager->ExposeOut().ExportAsText());
    }

    if (m_anPresetAmplitudes.GetCount())
    {
        rASSERT(rFakeTomographMemory->GetTomogramCount() == 1);

        rASSERT(
            rPredefinedTomogram->GetTomogramAmplitudeCount() == 
                m_anPresetAmplitudes.GetCount());

        ASSERT(rDownloader->GetStoredTomogramCount() == 1);
        ref<CTomogramIface> rStoredTomogramIface = 
            rDownloader->GetStoredTomogramAt(0).Valid();
        ref<CTomogramImpl> rStoredTomogram = 
            rStoredTomogramIface;

        repeat(iAmplitude, m_anPresetAmplitudes.GetCount())
        {
            int nIdealAmplitude = m_anPresetAmplitudes.GetAt(iAmplitude);

            // Check source tomogram consistency first
            {
                int nReceivedAmplitude0 = off;
                int nReceivedAmplitude1 = off;
                VERIFY(
                    rPredefinedTomogram->
                        TryReadAmplitudePairAt(
                            iAmplitude,
                            out nReceivedAmplitude0,
                            out nReceivedAmplitude1));
                rASSERT(nReceivedAmplitude0 == nIdealAmplitude);
                rASSERT(nReceivedAmplitude1 == nIdealAmplitude);
            }

            // Check downloaded tomogram 
            {
                int nReceivedAmplitude0 = off;
                int nReceivedAmplitude1 = off;
                VERIFY(
                    rStoredTomogram->
                        TryReadAmplitudePairAt(
                            iAmplitude,
                            out nReceivedAmplitude0,
                            out nReceivedAmplitude1));
                rASSERT(nReceivedAmplitude0 == nIdealAmplitude);
                rASSERT(nReceivedAmplitude1 == nIdealAmplitude);
            }
        }
    }

    // App: Close session
    rSession->TerminateSession();

    // Test: Check the report
    //debug::GCompareSavedReportFileToCurrentText(
    //    "TotalDownloadLog",
    //    sLog);
    // this should be called automatically now...
}

void CTotalTest::TestWithStore(
        rp<CPatInfoStoreIfaceGp> rpPatInfoStore)
{
    assertoff aoExo1("make sure it is OK to expose CPatInfoRunSendSampleInfo");

    debug::GDisableTestLog("+Udb");
    debug::GDisableTestLog("+NetSync");
    debug::GDisableTestLog("+FileMultipart");
    debug::GDisableTestLog("+Time");
    debug::GDisableTestLog("+Sync");

    ref<CPatientLinkImpl> rPatientLink = 
        CProject::GGetIt().
            x_rLoader->
                x_rPatientLink;

    ref<CPatientBaseConnectionIfaceGp> rPatientBaseConnection = 
        rPatientLink->x_rPatientBaseConnection;

    //
    // Init Pireg test backend
    //
    {
        TESTLOG(
            "",
            "\n"
                "----- Tomo inits DB ----" + 
                "\n\n");

        // Test: Lets log Patients into the global log
        //rPatientBaseConnection->x_sPatientLogName = "";

        // Test: Work as if we are successfully connected to Patients
        rPatientBaseConnection->_x_bEmulatePatientConnectionOkForClassTest = 
            false; // use Pireg
            // no separate standalone logic in app yet, otherwise it got to be
            //   tested for unconnected mode

        rPatientLink->x_rpForceSuggestPatInfoStore = 
            rpPatInfoStore;

        // App: Init Patients Connecton
        rPatientLink->InitPatientIsMainFrameHidden();
            // has simple internal once-init check, linear otherwise

        // update connection object!
        rPatientBaseConnection = 
            rPatientLink->x_rPatientBaseConnection;

        rPatientBaseConnection->_x_bEmulatePatientConnectionOkForClassTest = 
            true; // still use Pireg

        // Test: Handle connection mode
        ASSERT(rPatientLink->IsConnectedToPatient());

        // create a patient
        rPatientBaseConnection->
            CreateAndSelectNewPatient();
    }

    //
    // Prepare dummy config
    //
    {
        TESTLOG(
            "",
            "\n"
                "----- Tomo prepares config ----" + 
                "\n\n");

        // Global ref
        ref<CBodySchemeImpl> rBodyScheme = 
            CProject::GGetIt().
                x_rGlobalConfig->
                    x_rBodyScheme;

        // Stub defs
        ref<CBodyProfileImpl> rBodyProfile = 
            rBodyScheme->AppendNewProfile();

        // we fill second profile added
        rBodyProfile = rBodyScheme->AppendNewProfile();

        // temp point
        ref<CBodyPointImpl> rBodyPoint; 
        
        rASSERT(rBodyProfile->GetBodyPointCount() == 1);
        /*
        // point 0
        rBodyPoint = rBodyProfile->AppendNewPoint();
        rBodyPoint->x_sBodyPointName = "Aaaa";
        */

        // point 1
        rBodyPoint = rBodyProfile->AppendNewPoint();
        rBodyPoint->x_sBodyPointName = "*\\/ B b b b /\\*";

        rBodyProfile->x_iSelectedBodyPoint = 1;

        rBodyScheme->x_linkSelectedBodyProfile = rBodyProfile;
    }

    // Smaller sample
    {
        TESTLOG(
            "",
            "\n"
                "----- Tomo smaller sample ----" + 
                "\n\n");

        ref<CTotalTest> rTotalTest;

        rTotalTest->m_bDetailedLog = true;

        rTotalTest->m_nForceTomographSamplePointCount = 5;
        rTotalTest->m_nForceTomographFrameDataBytes = 2 * 4;

        rTotalTest->m_anPresetAmplitudes.Add(1);
        rTotalTest->m_anPresetAmplitudes.Add(1000);
        rTotalTest->m_anPresetAmplitudes.Add(0);
        rTotalTest->m_anPresetAmplitudes.Add(10);
        rTotalTest->m_anPresetAmplitudes.Add(77);

        rTotalTest->_test_RunTotalTestVariation();
    }

    // Normal execution
    {
        TESTLOG(
            "",
            "\n"
                "----- Tomo run normal ----" + 
                "\n\n");

        ref<CTotalTest> rTotalTest;

        rTotalTest->_test_RunTotalTestVariation();
    }

    // complicate adding new samples

    TESTLOG(
        "",
        "\n" "   ---> Tomo Forcing another Checkup <---- " 
            "\n");

    rPatientBaseConnection->
        ForceCreateNewCheckup();

    // Obsolete frame
    {
        TESTLOG(
            "",
            "\n"
                "----- Tomo run with obsolete frame ----" + 
                "\n\n");

        ref<CTotalTest> rTotalTest;

        rTotalTest->m_bObsoleteHeaderSignature = true;

        rTotalTest->_test_RunTotalTestVariation();
    }

    // Device not ready
    {
        TESTLOG(
            "",
            "\n"
                "----- Tomo run with device not ready ----" + 
                "\n\n");

        ref<CTotalTest> rTotalTest;

        rTotalTest->x_bTestDeviceOffline = true;

        // Start will fail when not connected
        debug::GHideAskByQuoteAndReplyOk(
            "Make sure it is connected");

        rTotalTest->_test_RunTotalTestVariation();
    }

    //
    // Test Card Button Clicks
    //
    {
        TESTLOG(
            "",
            "\n"
                "----- Tomo Card Button Clicks ----" + 
                "\n\n");

        ref<CPatInfoSessionIfaceGp> rPatInfoSession = 
            rPatientBaseConnection->
                GetPatInfoSessionIfEmbeddedMode().
                Valid();
        ref<CPatInfoBrowserIfaceGp> rPatInfoBrowser = 
            rPatInfoSession->GetPatInfoBrowser();

        ref<CPiPatientIfaceGp> rPiPatient = 
            rPatInfoBrowser->
                GetMainSelectedPatient().
                    Valid();

        ref<CPatInfoFieldInputButtonIfaceGp>()->
            JustTestCallAllButtonsInCard(
                rPiPatient);
    }

    //
    // Close PatientLink
    //
    {
        str sDbDump =
            rPatientBaseConnection->
                DumpEmbeddedPatientDatabaseTestStorageToInMemoryUnidef().
                    ExportAsText();

        // db dump
        TESTLOG(
            "",
            "\n"
                "======== Final Tomo Patient DB ===========" + 
                sDbDump + 
                "\n\n");

        rPatientLink->
            ClosePatientLink();
    }
}