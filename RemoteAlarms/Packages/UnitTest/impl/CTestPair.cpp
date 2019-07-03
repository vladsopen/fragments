// CTestPair.cpp

#include "CProject.h"
#include "CTestPair.h"
#include "CTaskIface.h"
#include "CAuthDeviceFieldInput.h"
#include "CPatInfoAddedImpl.h"
#include "CSessionImpl.h"
#include "CPatDbStoreIfaceGp.h"
#include "CPatInfoInitSessionIfaceGp.h"
#include "CPatientModalityFeaturesIfaceGp.h"
#include "CPatInfoPimpSessionIfaceGp.h"
#include "CIoCommonIface.h"
#include "CPatLiteFactoryIfaceGp.h"
#include "CPatientBaseConnectionIfaceGp.h"
#include "CReceiverSessionImpl.h"
#include "CPiOrganizerIfaceGp.h"
#include "CPatientLinkIface.h"
#include "CPiCardConfigIfaceGp.h"
#include "CPiFieldConfigIfaceGp.h"
#include "CPatInfoSessionIfaceGp.h"
#include "CPatInfoViewIfaceGp.h"
#include "CJobListImpl.h"
#include "CJobSyncOutImpl.h"
#include "CTestStubNetSyncOut.h"
#include "CPiFieldValueIfaceGp.h"
#include "CPatientPatientCallbackIfaceGp.h"
#include "CPiPatientIfaceGp.h"
#include "CPatientPatientInfoIfaceGp.h"
#include "VirtOs_file.h"
#include "CJobStateIface.h"
#include "CPatientEmbedLinkIface.h"
#include "CPatientEmbedConnectionIfaceGp.h"
#include "VirtOs_unicode.h"
#include "CUbroRefineIfaceGp.h"

CTestPair::CTestPair()
{
    _init_AttachToTestPair();
}

void CTestPair::_init_AttachToTestPair()
{
    //x_r->_x_p = this;
}

void CTestPair::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToTestPair();

    rFAIL("make sure it is OK to expose CTestPair!");
}

void CTestPair::InitStorage(
        out CTestPair::DOneEnd& out_dataEnd,
        str sDebugStorageName)
{
    TESTLOG(
        "",
        "\n"
            "Init " + sDebugStorageName + 
            " db ========================"
            "\n");

    out_dataEnd._m_sDebugStorageName = sDebugStorageName;

    CPatInfoAddedImpl::s_pUseOnceTestSession =
        out_dataEnd._m_rSession;

    ref<CSessionImpl> rSession = 
        out_dataEnd._m_rSession;

    // separate task folders!
    rSession->x_rIoCommon->x_pathForceTestDataRoot = 
        sDebugStorageName;

    // Test-storage driver
    PRELOAD_PROJECT(PatDb);
    ref<CPatInfoStoreIfaceGp> rPatInfoStore = 
        ref<CPatDbStoreIfaceGp>();
    out_dataEnd._m_rPatInfoStore = rPatInfoStore;

    ref<CPatInfoInitSessionIfaceGp> rPatInfoInitSession;
    ref<CPatientModalityFeaturesIfaceGp> rPatientModalityFeatures;

    rPatientModalityFeatures->x_sDeclareModalityKey = 
        "TestSampler";

    rPatInfoInitSession->
        SuggestPatInfoStoreImpl(
            rPatInfoStore);

    rPatInfoInitSession->
        x_sAppendToClassTestDbName =
            sDebugStorageName;

    // special dependency-breaking factory (#included .cpp!)
    PRELOAD_PROJECT(PatLite);
    ref<CPatInfoPimpSessionIfaceGp> rNewPatInfoPimpSession;
    ref<CPatLiteFactoryIfaceGp>()->
        NewPatLiteSessionImpl(
            out rNewPatInfoPimpSession);

    // someth we usually call...
    ref<CPatientBaseConnectionIfaceGp> rPatientBaseConnection =
        CPatientBaseConnectionIfaceGp::GNewPimpedConnection(
            rNewPatInfoPimpSession,
            rPatInfoInitSession,
            rPatientModalityFeatures);

    //rPatientBaseConnection->x_sPatientLogName = "PatientLog";
    //debug::GDisableTestLog(rPatientBaseConnection->x_sPatientLogName);
    debug::GDisableTestLog("+PatientLog");

    //debug::GRedirectTestLog("+PatientLog", "PatientLog");

    // receiver ususlly gets the session from PatientEmbed - missing in tests

    ref<CReceiverSessionImpl> rReceiverSession =
        out_dataEnd._m_rSession->
            x_rReceiverSession;

    rReceiverSession->_x_pAttachedPatInfoSessionForImport = 
        rPatInfoStore->GetPatInfoSession();

    // save until Close()
    out_dataEnd._m_rPatientBaseConnection = 
        rPatientBaseConnection;

    // init session
    rPatientBaseConnection->
        InitPatientBaseConnection(
            ref<CPatientPatientCallbackIfaceGp>(),
            rPatientModalityFeatures);

    // Ralarm init
    //rSession->
    //    InitSession(
    //        path());
    rASSERT(rSession->IsSessionOpened());
    // must be cleared:
    rASSERT(CPatInfoAddedImpl::s_pUseOnceTestSession == null());

    // init plugins
    CProject::GGetIt().
        x_rPatientEmbedLink->
            InitPatientEmbedLink(
                rNewPatInfoPimpSession->
                    NewPatientEmbedConnection());

    TESTLOG(
        "",
        "\n"
            "ready (" + sDebugStorageName + ") ========================"
            "\n");
}

void CTestPair::CloseStorage(
        out CTestPair::DOneEnd& out_dataEnd)
{
    str sDebugStorageName = out_dataEnd._m_sDebugStorageName;

    TESTLOG(
        "",
        "\n"
            "Closing " + sDebugStorageName + " db ========================"
            "\n");

    out_dataEnd.
        _m_rSession->
            CloseSession();

    out_dataEnd.
        _m_rPatientBaseConnection->
            ClosePatientBaseConnection();

    TESTLOG(
        "",
        "\n"
            "closed (" + sDebugStorageName + ") ========================"
            "\n");
}

ref<CPiPatientIfaceGp> CTestPair::AddPatient(
        out CTestPair::DOneEnd& out_dataEnd,
        str sKey)
{
    TESTLOG(
        "",
        "\n"
            "Adding patient \"" + sKey + "\"..."
            "\n");

    bool bNewPatientCreated = false;

    ref<CPiPatientIfaceGp> rPiPatient =
        out_dataEnd.
            _m_rPatInfoStore->
                GetAllPatientsPiOrganizer()->
                    CreateAndSelectNewPiPatient(
                        sKey,
                        out bNewPatientCreated);

    rASSERT(bNewPatientCreated);

    return rPiPatient;
}

ref<CPiFieldConfigIfaceGp> CTestPair::AddField(
        out CTestPair::DOneEnd& out_dataEnd,
        str sFieldKey,
        str sFieldName)
{
    TESTLOG(
        "",
        "\n"
            "Adding field [" + sFieldKey + "]: \"" + sFieldName + "\"..."
            "\n");

    ref<CPiCardConfigIfaceGp> rPiCardConfig =
        out_dataEnd.
            _m_rPatInfoStore->
                GetGlobalPiCardConfig();

    ref<CPiFieldConfigIfaceGp> rPiFieldConfig;
    if not_null(
        ref<CPiFieldConfigIfaceGp> rAddedPiFieldConfig = 
            rPiCardConfig->
                DefinePatientFieldByPlugin(
                    sFieldKey))
    {
        rPiFieldConfig = rAddedPiFieldConfig;
    }
    else
    {
        rFAIL("cannot add a field?");
        return null();
    }

    rPiFieldConfig->
        SetPiFieldLabelForUi(
            "Initial field label WILL BE OVERWRITTEN!");

    rPiFieldConfig->
        SetPiFieldLabelForUi(
            sFieldName);

    /*
    TESTLOG(
        "",
        "\n"
            "added \"" + sName + "\""
            "\n");
            */

    return rPiFieldConfig;
}

void CTestPair::DumpPatient(
        ref<CPiPatientIfaceGp> rPiPatient,
        ref<UiContainer> rUiPatientCard)
{
    const array<str> asFields = 
        CPatientLinkIface::GGetAllCardFields();

    str sDump = "";

    repeat(iField, asFields.GetCount())
    {
        str sKey = 
            asFields[iField];
        str sValue =
            rPiPatient->
                GetPiPatientFieldValueOrStub(
                    sKey,
                    "");

        sDump += 
            "" + sKey + ": <" + sValue + ">\n"
    }

    TESTLOG(
        "",
        "\n"
            "Parient Card:\n" +
            sDump +
            "\n");

    rUiPatientCard->thispanel()->UpdateContentAndLayoutNow();
    rUiPatientCard->thispanel()->UpdatePaint();
    //sys::GSleepInTicksForTests(100);
}

void CTestPair::OnTestClass()
{
    TESTLOG(
        "",
        "\n"
            "Ralarm Pairing"
            "\n");

    debug::GDisableTestLog("+Udb");
    debug::GDisableTestLog("+NetSync");
    debug::GDisableTestLog("+FileMultipart");
    debug::GDisableTestLog("+Time");
    debug::GDisableTestLog("+Sync");
    debug::GDisableTestLog("+Multicall");
    debug::GDisableTestLog("+CacheAction");
    debug::GDisableTestLog("+Udb");
    debug::GDisableTestLog("+PatLiteUiDump");

    TESTLOG("", "\n" "WARNING: reenable +NextTick to trace GGetUtc()\n\n");
    debug::GDisableTestLog("+NextTick");

    assertoff aoNet("OK to expose");

    PRELOAD_PROJECT(PatLite);
    PRELOAD_PROJECT(PatInfo);
    PRELOAD_PROJECT(PatDb);

    CAuthDeviceFieldInput::s_pLastAuthDeviceFieldInputTest = null();
    CAuthDeviceFieldInput::s_pLastPiFieldValueTest = null();

    DOneEnd dataBackend = DOneEnd(); 

    InitStorage(
        out dataBackend,
        "Backend");

    const array<str> asFields = 
        CPatientLinkIface::GGetAllCardFields();
    repeat(iField, asFields.GetCount())
    {
        //ref<CPiFieldConfigIfaceGp> rField = 
            AddField(
                out dataBackend,
                asFields[iField],
                ""); //sTestAuthDevice);
    }

    ref<CPiPatientIfaceGp> rPiPatient1 =
        AddPatient(
            out dataBackend,
            "TestPair1");

    //
    // Create patient UI
    //

    ref<UiContainer> rUiPatientCard =
        dataBackend._m_rPatInfoStore->GetPatInfoSession()->GetPatInfoView()->
            NewUiPimpViewSelection(
                ref<CUbroRefineIfaceGp>()); // legacy test

    rUiPatientCard->CreateFloater();
    rUiPatientCard->thispanel()->SetInstantPos(10, 10);
    rUiPatientCard->thispanel()->SetInstantSize(650, 800);
    DumpPatient(
        rPiPatient1,
        rUiPatientCard);
    //sys::GSleepInTicksForTests(500);

    ref<CJobListImpl> rImporterJobList = 
        dataBackend._m_rSession->x_rJobList;

    //
    // Fake Net
    //

    ref<CTestStubNetSyncOut> rFakeSyncOut;

    CJobSyncOutImpl::s_pForceNetSyncOutForTests =
        rFakeSyncOut;

    //
    // Go send!...
    //

    TESTLOG(
        "",
        "\n"
            "------------- PairMe request: --------------"
            "\n\n");

    ref<CTaskIface> rTask;

    rTask->x_sTaskType = 
        CTaskIface::C_sTaskTypeIsAlarm;

    rTask->x_sDeviceSerial = 
        "SN1000HOLT";

    rTask->x_sTaskId = 
        ""; //"Ts-Pair-TASK1";

    rTask->x_nPairMe = 
        123456;

    path pathDownloadedFile = 
        sys::GGetTempFolder().
            GetAppendedPath("Pairing-Download-1.justtest");
    file(pathDownloadedFile).Save("", out_IGNORE_sError);

    // create a received task
    ref<CJobStateIface> rPairJob1;
    {
        str sRaiseNetError;
        rPairJob1 =
            rImporterJobList->
                RegisterDownloadedTask(
                    rTask,
                    pathDownloadedFile, // path pathDownloadedFile,
                    out sRaiseNetError).
                    Valid(
                        "Task list rejected out test pairing task!");
        rASSTR(sRaiseNetError);
    }

    TESTLOG(
        "",
        "\n"
            "Pair Job1 received: " + 
            rPairJob1->ExposeOut().ExportAsTextWithoutHeader() +
            "\n\n");

    TESTLOG(
        "",
        "\n"
            "Exported Fake Net: " + 
            rFakeSyncOut->ExposeOut().ExportAsTextWithoutHeader() +
            "\n\n");

    repeat(iRepeatPin, 3)
    {
        ref<CAuthDeviceFieldInput> rAuthDeviceInput = 
            CAuthDeviceFieldInput::s_pLastAuthDeviceFieldInputTest.
                Valid("AuthDevice input test ptr?");

        ref<CPiFieldValueIfaceGp> rAuthDevicePiFieldValue = 
            CAuthDeviceFieldInput::s_pLastPiFieldValueTest.
                Valid("AuthDevice Pi-input ptr?");

        // pin bad, good
        rAuthDeviceInput->x_nInputPinCode = 
            iRepeatPin == 0? 
                666999 
                : 
                123456;

        if (iRepeatPin <= 1)
        {
            TESTLOG(
                "",
                "\n\n"
                    "------------- User Enters PIN: " +
                    Str(rAuthDeviceInput->x_nInputPinCode) +
                    " --------------"
                    "\n\n");

            rAuthDeviceInput->
                CheckPinAndConnect(
                    rAuthDevicePiFieldValue);
        }
        else
        {
            TESTLOG(
                "",
                "\n\n"
                    "------------- User DETACH! "
                    " --------------"
                    "\n\n");

            rAuthDeviceInput->
                DestroyPairing(
                    rAuthDevicePiFieldValue);
        }

        DumpPatient(
            rPiPatient1,
            rUiPatientCard);

    }

    //
    // Cleanup
    //

    TESTLOG(
        "",
        "\n\n"
            "------------- DONE! "
            " --------------"
            "\n\n");

    // restore original Net so we dont have to implement Close()
    //rSenderSession->x_rSenderNetSyncOut = 
    //    rSaveNetSyncOut;

    rUiPatientCard->thispanel()->CloseFrame();

    CloseStorage(
        out dataBackend);
    CloseStorage(
        out dataBackend);

    TESTLOG(
        "",
        "\n"
            "<<<<<<< test complete! >>>>>>"
            "\n");
}

