// CTestSendReceive.cpp

#include "CProject.h"
#include "CTestSendReceive.h"
#include "CPatInfoInitSessionIfaceGp.h"
#include "CPatientBaseConnectionIfaceGp.h"
#include "CPatientPatientCallbackIfaceGp.h"
#include "CPiPatientIfaceGp.h"
#include "CPiOrganizerIfaceGp.h"
#include "CPiCardConfigIfaceGp.h"
#include "CPiFieldValueIfaceGp.h"
#include "CPiCardFillingIfaceGp.h"
#include "CPiFieldConfigIfaceGp.h"
#include "CPatientPatientInfoIfaceGp.h"
#include "CPatientSexTypeIfaceGp.h"
#include "CPatientCheckupIfaceGp.h"
#include "VirtOs_file.h"
#include "CPatientIndicatorIfaceGp.h"
#include "CPatientSampleIfaceGp.h"
#include "CPatientIndicatorListIfaceGp.h"
#include "CPiCheckupIfaceGp.h"
#include "CPiRecordsIfaceGp.h"
#include "CPiIndicatorValueIfaceGp.h"
#include "CPiSampleIfaceGp.h"
#include "VirtOs_dir.h"
#include "VirtOs_CFileList.h"
#include "CPatDbStoreIfaceGp.cpp"
#include "CSessionImpl.h"
#include "CPatLiteFactoryIfaceGp.h"
#include "CPatInfoStoreCommonIfaceGp.h"
#include "CPatInfoStoreDebugIfaceGp.h"
#include "CSenderSessionIface.h"
#include "CTestStubNetSyncOut.h"
#include "CSenderSessionImpl.h"
#include "CReceiverSessionIface.h"
#include "CReceiverSessionImpl.h"
#include "CImportCheckups.h"
#include "CPatInfoSessionIfaceGp.h"
#include "CTestStubSent.h"
#include "CIoCommonIface.h"
#include "CJobListImpl.h"
#include "CJobStateImpl.h"
#include "CSendAlarm.h"
#include "CTaskIface.h"
#include "CTestFakePatientLink.h"
#include "CPatInfoAddedImpl.h"
#include "CNetPumpInIfaceGp.h"
#include "CNetDriverSendatumIfaceGp.h"
#include "CTimeVirtualizerIfaceGp.h"
#include "VirtOs_CAppGp.h"
#include "CNetDirectionIfaceGp.h"
#include "CNetLocatorIfaceGp.h"
#include "CNetPumpOutIfaceGp.h"
#include "CReceiverIoProtocol.h"
#include "CJobQuest.h"
#include "CQuestHandler.h"
#include "CJobSyncOutImpl.h"

CTestSendReceive::CTestSendReceive()
{
    _init_AttachToTestSendReceive();
}

void CTestSendReceive::_init_AttachToTestSendReceive()
{
    m_rAlarmReceiverFakePatientLink->_x_pTestSendReceive = this;
}

void CTestSendReceive::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToTestSendReceive();

    rFAIL("make sure it is OK to expose CTestSendReceive!");
}

void CTestSendReceive::InitStorage(
        out CTestSendReceive::DOneEnd& out_dataEnd,
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

    rSession->
        x_rSenderSession->
            x_bUseSenderAsAlarmEmulator = true;

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

    TESTLOG(
        "",
        "\n"
            "ready (" + sDebugStorageName + ") ========================"
            "\n");
}

void CTestSendReceive::CloseStorage(
        out CTestSendReceive::DOneEnd& out_dataEnd)
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

ref<CPiPatientIfaceGp> CTestSendReceive::AddPatient(
        out CTestSendReceive::DOneEnd& out_dataEnd,
        str sKey,
        str sName,
        type<CPatientSexTypeIfaceGp> typePatientSexType)
{
    TESTLOG(
        "",
        "\n"
            "Adding \"" + sName + "\"..."
            "\n");

    m_iSeedAddingPatient++;

    bool bNewPatientCreated = false;

    ref<CPiPatientIfaceGp> rPiPatient =
        out_dataEnd.
            _m_rPatInfoStore->
                GetAllPatientsPiOrganizer()->
                    CreateAndSelectNewPiPatient(
                        sKey,
                        out bNewPatientCreated);

    rASSERT(bNewPatientCreated);

    ref<CPiCardConfigIfaceGp> rPiCardConfig =
        out_dataEnd.
            _m_rPatInfoStore->
                GetGlobalPiCardConfig();

    ref<CPiFieldConfigIfaceGp> rNameConfig;
    if (!rPiCardConfig->
            LookupCardFieldConfigByKey(
                CPatientPatientInfoIfaceGp::C_sBasicPatientFieldKeyForName,
                out rNameConfig,
                E_AddIfMissing_No))
    {
        rFAIL("no _name?!");
    }

    // Lookup field value for this patient
    ref<CPiFieldValueIfaceGp> rNameValue =
        rPiPatient->
            x_rPiCardFilling->
                LookupFieldValueByConfig(
                    rNameConfig);

    rNameValue->
        SetPiFieldValueAsStr(
            sName);

    // set some other values
    ref<CPiFieldConfigIfaceGp> rIterPiFieldConfig;
    int nBuiltInSet = 0;
    ref<CPiFieldValueIfaceGp> rChangeValue;
    str sBeforeChangeValue;
    for (
        iter iField;
        rPiCardConfig->
            IterateCardFieldConfig(
                out iField,
                out rIterPiFieldConfig);)
    {
        int iSeed = m_iSeedAddingPatient;

        // leave some unset
        if (iField == iSeed)
        {
            continue;
        }

        str sFieldKey = rIterPiFieldConfig->GetPiFieldConfigStrKey();

        // Lookup field value for this patient
        ref<CPiFieldValueIfaceGp> rPiFieldValue =
            rPiPatient->
                x_rPiCardFilling->
                    LookupFieldValueByConfig(
                        rIterPiFieldConfig);

        if (sFieldKey == CPatientPatientInfoIfaceGp::C_sBasicPatientFieldKeyForBirthDate)
        {
            moment momentSet;
            str sWarnings;
            momentSet.
                FromStr(
                    "yyyy-mm-dd",
                    Str(2000 + iSeed) + "-" +
                        Str00(iSeed, 2) + "-" +
                        Str00(iSeed, 2),
                    out sWarnings);
            rASSTR(sWarnings);

            rPiFieldValue->
                SetPiFieldValueAsMoment(
                    momentSet);
        }
        else
        if (sFieldKey == CPatientPatientInfoIfaceGp::C_sBasicPatientFieldKeyForSex)
        {
            rPiFieldValue->
                SetPiPatientSex(
                    typePatientSexType);
        }
        else
        if (sFieldKey == CPatientPatientInfoIfaceGp::C_sBasicPatientFieldKeyForName)
        {
            // already set
            continue;
        }
        else
        {
            str sSetValue =
                Str(iSeed) + "/" + sFieldKey;

            if (iField >= iSeed + 2 &&
                sBeforeChangeValue == "")
            {
                rChangeValue = rPiFieldValue;
                sBeforeChangeValue = sSetValue;
                sSetValue = " *** WILL BE CHANGED! ***";
            }

            if (ref<CPatientPatientInfoIfaceGp>()->
                    JustIsPatientFieldKeyBuiltIn(sFieldKey))
            {
                nBuiltInSet++;
                if (nBuiltInSet > 3)
                {
                    continue;
                }
            }

            rPiFieldValue->
                SetPiFieldValueAsStr(
                    sSetValue);
        }
    }

    // change already set value
    rChangeValue->
        SetPiFieldValueAsStr(
            "Upd! (" + sBeforeChangeValue.GetLeftAbbrev(5, "...") + ")");

    /*
    TESTLOG(
        "",
        "\n"
            "added \"" + sName + "\""
            "\n");
            */

    return rPiPatient;
}

ref<CPiFieldConfigIfaceGp> CTestSendReceive::AddField(
        out CTestSendReceive::DOneEnd& out_dataEnd,
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

void CTestSendReceive::SendSampleInfoTo(
        ptr<CPatientSampleIfaceGp> pPatientSample,
        out CTestSendReceive::DOneEnd& out_dataEnd,
        ref<CPiPatientIfaceGp> rPiPatient)
{
    str sDebugStorageName = out_dataEnd._m_sDebugStorageName;

    TESTLOG(
        "",
        "\n"
        "Sampling " + 
            rPiPatient->GetPiPatientKey() + 
            " in " + 
            sDebugStorageName + " db "
            "_-^-__-^-__-^-__-^-__-^-_"
            "\n");

    ref<CPatientCheckupIfaceGp> rPatientCheckup;
    VERIFY(
        out_dataEnd._m_rPatientBaseConnection->
            GetCurrentCheckup(
                out rPatientCheckup));

    ref<CPatientSampleIfaceGp> rPatientSample;
    if not_null(
        ref<CPatientSampleIfaceGp> rForcedPatientSample = 
            pPatientSample)
    {
        rPatientSample = rForcedPatientSample;
    }
    else
    {
        //
        // Demo Index (title)
        //
        {
            ref<CPatientIndicatorIfaceGp> rPatientIndicator;

            rPatientIndicator->x_sIndicatorKey = 
                "Organ"; // internal ID

            rPatientIndicator->x_richIndicatorShortName = 
                TRANSLATE(
                    "Orga[... UI-string-removed]");

            rPatientIndicator->x_richIndicatorDescription = 
                TRANSLATE(
                    "Descriptio[... UI-string-removed]");

            rPatientIndicator->x_bIndicatorIsPartOfSampleTitle = 
                true; // only for important desciptive indicators

            rPatientIndicator->x_bIndicatorNameOptional = 
                true; // only if value meaning is obvious

            rPatientIndicator->x_bIndicatorHiddenByFactoryDefault = 
                false; // hide in factory setup?

            rPatientIndicator->x_richAssignTextIndicator = 
                TRANSLATE(
                    "Brai[... UI-string-removed]");

            rPatientSample->
                x_rIndicatorList->
                    AddIndicator(
                        rPatientIndicator);
        }

        //
        // Demo Index 
        //
        {
            ref<CPatientIndicatorIfaceGp> rPatientIndicator;

            rPatientIndicator->x_sIndicatorKey = 
                "HR"; // internal ID

            rPatientIndicator->x_richIndicatorShortName = 
                TRANSLATE(
                    "Heart Rate[... UI-string-removed]");

            rPatientIndicator->x_richIndicatorDescription = 
                TRANSLATE(
                    "Index valu[... UI-string-removed]");

            rPatientIndicator->x_bIndicatorIsPartOfSampleTitle = 
                false; // only for important desciptive indicators

            rPatientIndicator->x_bIndicatorNameOptional = 
                false; // only if value meaning is obvious

            rPatientIndicator->x_bIndicatorHiddenByFactoryDefault = 
                false; // hide in factory setup?

            rPatientIndicator->x_richAssignTextIndicator = 
                Str(123.456); // veriable: text, number, etc

            rPatientSample->
                x_rIndicatorList->
                    AddIndicator(
                        rPatientIndicator);
        }

        //
        // Save/flush attached data storage
        //

        path pathAttachPrefix = 
            out_dataEnd._m_rPatientBaseConnection->GetNewAttachmentPrefix();
        rASSERT(pathAttachPrefix != "");

        {
            path pathSave = 
                pathAttachPrefix + "_TestSaved.txt";

            str sSaveError;
            file(pathSave).
                Save(
                    "Demo saved test attachment can be very large.", 
                    out sSaveError);
            rASSERTM(sSaveError == "", sSaveError);
        }
        {
            path pathSave = 
                pathAttachPrefix + "_InitialSend.small";

            str sSaveError;
            file(pathSave).
                Save(
                    "Smaller to be sent with initial packet.", 
                    out sSaveError);
            rASSERTM(sSaveError == "", sSaveError);
        }

        // Attach the file to the sample
        rPatientSample->x_pathAttachmentPrefix = 
            pathAttachPrefix;
    }

    //
    // Send to Patients
    //

    out_dataEnd._m_rPatientBaseConnection->
        ForceCreateNewCheckup();

    // Send info to Patients
    str sError = "";
    if (!rPatientSample->
            SendSampleInfo(
                rPatientCheckup,
                out sError))
    {
        rFAIL("SSI failed! " + sError);
    }
    rASSTR(sError);

    /*
    TESTLOG(
        "",
        //"\n"
            "end."
            "\n");
            */
}

void CTestSendReceive::TestJobList(
        out CTestSendReceive::DOneEnd& out_dataEnd,
        int nMaxEntries)
{
    ref<CSessionImpl> rSession = 
        out_dataEnd._m_rSession;

    ref<CReceiverSessionImpl> rReceiverSession =
        rSession->
            x_rReceiverSession;

    ref<CJobListImpl> rJobList = 
        rSession->x_rJobList;

    ref<CJobStateIface> rIterJobState;
    int nListed = 0;
    for (
        iter i;
        rJobList->
            IterateJobs(
                out i,
                out rIterJobState);)
    {
        if (nListed >= nMaxEntries)
        {
            break;
        }
        nListed++;

        ref<CJobStateImpl> rIterJob = 
            rIterJobState;

        TESTLOG(
            "",
            "\n"
                "Job: " + 
                rIterJob->GetReportInList("\n") + 
                "\n\n");

        rReceiverSession->
            TryImportJob(
                rIterJob);

        rIterJob->
            HandleJobClick();
    }

}

void CTestSendReceive::PollFakeSync(
        ref<CTestStubNetSyncOut> rFakeSyncOut,
        ref<CReceiverSessionImpl> rReceiverSession,
        path pathScanInbox)
{
    ref<CTestStubSent> rIterStubSent;
    for (
        iter i;
        rFakeSyncOut->m_arSent.Iterate(out i, out rIterStubSent);)
    {
        str sError;
        rReceiverSession->
            x_rReceiverNetProtocol->
                ReceiveDownloadedMeta(
                    rIterStubSent->x_unidefExtraHeaderSent,
                    rIterStubSent->x_pathOriginalSent.Get().GetFileNameWithExt(),
                    out sError);

        rASSTR(sError);

        str sCopyError;
        file(rIterStubSent->x_pathOriginalSent).
            CopyFileTo(
                pathScanInbox.
                    GetAppendedPath(
                        rIterStubSent->x_pathOriginalSent.Get().GetFileNameWithExt()),
                true,
                out sCopyError);
        rASSTR(sCopyError);

        //rImporterJobList->
        //    ImportExistingMetas();
    }

    // randomly delete some files, some will be duplicated
    rFakeSyncOut->m_arSent.RemoveAt(0);
}

void CTestSendReceive::ListPatients(
        out CTestSendReceive::DOneEnd& out_dataEnd)
{
    str sDebugStorageName = out_dataEnd._m_sDebugStorageName;

    TESTLOG(
        "",
        "\n"
            "Listing " + sDebugStorageName + " db /////////////////////"
            "\n");

    ref<CPiOrganizerIfaceGp> rPiOrganizer =
        out_dataEnd.
            _m_rPatInfoStore->
                GetAllPatientsPiOrganizer();

    str sDump = 
        out_dataEnd._m_rPatInfoStore->
            x_rPatInfoStoreCommon->
                x_rPatInfoStoreDebug->
                    DumpPiPatients(
                        rPiOrganizer);

    TESTLOG("", sDump);
}

void CTestSendReceive::OnTestClass()
{
    TESTLOG(
        "",
        "\n"
            "Ralarm Checkup Send/Receive"
            "\n");

    debug::GDisableTestLog("+Udb");
    debug::GDisableTestLog("+NetSync");
    debug::GDisableTestLog("+FileMultipart");
    debug::GDisableTestLog("+Time");
    debug::GDisableTestLog("+Sync");
    debug::GDisableTestLog("+Multicall");
    debug::GDisableTestLog("+CacheAction");
    debug::GDisableTestLog("+Udb");

    TESTLOG("", "\n" "WARNING: reenable +NextTick to trace GGetUtc()\n\n");
    debug::GDisableTestLog("+NextTick");

    PRELOAD_PROJECT(PatLite);
    PRELOAD_PROJECT(PatInfo);
    PRELOAD_PROJECT(PatDb);

    DOneEnd dataExporter = DOneEnd(); 
    DOneEnd dataImporter = DOneEnd(); 

    InitStorage(
        out dataExporter,
        "ExporterEnd");
    InitStorage(
        out dataImporter,
        "ImporterEnd");

    ref<CPiPatientIfaceGp> rPiPatientExport1 =
        AddPatient(
            out dataExporter,
            "1PATjoinEXPORT",
            "James One McExport",
            type<CPatientSexTypeIfaceGp>()->
                AsPatientSexTypeAsMale());

    ref<CPiFieldConfigIfaceGp> rCustomField1 = 
        AddField(
            out dataExporter,
            "000FAVCLR1",
            "Fav. color");

    str sTestAuthDevice = "AD000TEST123456";
    ref<CPiFieldConfigIfaceGp> rCustomField2 = 
        AddField(
            out dataExporter,
            CPatientLinkIface::C_sFieldAuthDevice,
            sTestAuthDevice);

    /*
    ref<CPiPatientIfaceGp> rPiPatientNoExport =
        AddPatient(
            out dataExporter,
            "MID-PAT-no-EXP",
            "Midly Skip Stayer",
            type<CPatientSexTypeIfaceGp>()->
                AsPatientSexTypeAsFemale());

    ref<CPiPatientIfaceGp> rPiPatientExport2 =
        AddPatient(
            out dataExporter,
            "2JANEA-PAT-EXP",
            "Jane Two Addler",
            type<CPatientSexTypeIfaceGp>()->
                AsPatientSexTypeAsFemale());
    */

    const int C_nAddCheckups = 3;
    repeat(i, C_nAddCheckups)
    {
        SendSampleInfoTo(
            ptr<CPatientSampleIfaceGp>(), // create a new
            out dataExporter,
            rPiPatientExport1);
    }

    ListPatients(
        out dataExporter);

    ref<CJobListImpl> rImporterJobList = 
        dataImporter._m_rSession->x_rJobList;
    rASSERT(rImporterJobList != dataExporter._m_rSession->x_rJobList);

    ///////////////////////////////////////////////////////////////////////////
    //
    // Export
    //
    ///////////////////////////////////////////////////////////////////////////

    // Export Jobs
    TestJobList(
        out dataExporter);

    //
    // Find checkup
    //

    ref<CPiCheckupIfaceGp> rSendPiCheckup;
    {
        int nCheckups = 0;
        ref<CPiCheckupIfaceGp> rIterPiCheckup;
        for (
            iter iCheckup;
            rPiPatientExport1->
                x_rPiRecords->
                    IteratePiCheckups(
                        out iCheckup,
                        out rIterPiCheckup);)
        {
            nCheckups++;
            if (iCheckup == 1)
            {
                rSendPiCheckup = rIterPiCheckup;
            }
        }
        rASSERT(nCheckups == C_nAddCheckups);
    }

    //
    // Fake Net
    //

    ref<CSenderSessionImpl> rSenderSession =
        dataExporter._m_rSession->
            x_rSenderSession;

    ref<CNetSyncOutIfaceGp> rSaveNetSyncOut =
        rSenderSession->x_rSenderNetSyncOut;

    ref<CTestStubNetSyncOut> rFakeSyncOut;

    rSenderSession->x_rSenderNetSyncOut =
        rFakeSyncOut;
    CJobSyncOutImpl::s_pForceNetSyncOutForTests =
        rFakeSyncOut;

    //
    // Go send!...
    //

    TESTLOG(
        "",
        "\n"
            "------------- Export: --------------"
            "\n\n");

    rSenderSession->
        PromptToSendExpertMailCheckup(
            rSendPiCheckup);

    assertoff aoNet("OK to expose");
    TESTLOG(
        "",
        "\n"
            "Exported Fake Net: " + 
            rFakeSyncOut->ExposeOut().ExportAsTextWithoutHeader() +
            "\n\n");

    // Export Jobs
    TestJobList(
        out dataExporter);

    ///////////////////////////////////////////////////////////////////////////
    //
    // Import
    //
    ///////////////////////////////////////////////////////////////////////////

    TESTLOG(
        "",
        "\n"
            "------------- Import: --------------"
            "\n\n");

    ref<CReceiverSessionImpl> rReceiverSession =
        dataImporter._m_rSession->
            x_rReceiverSession;

    // null Jobs
    TestJobList(
        out dataImporter);

    path pathScanInbox = 
        rReceiverSession->
            GetTempInbox();

    // supply "downloaded" meta
    PollFakeSync(
        rFakeSyncOut,
        rReceiverSession,
        pathScanInbox);

    // Scan for (NO!) new files, (NOT)update the list
    //rReceiverSession->
    //    PollImportCheckups();
    rImporterJobList->
        HandleJobListTimer();
        
    // Import Jobs
    TestJobList(
        out dataImporter);

    // Scan for new files, update the list
    //rReceiverSession->
    //    PollImportCheckups();
    rImporterJobList->
        HandleJobListTimer();

    // Import Jobs
    TestJobList(
        out dataImporter);

    // Scan for new files, update the list
    //rReceiverSession->
    //    PollImportCheckups();
    rImporterJobList->
        HandleJobListTimer();

    // Import Jobs
    TestJobList(
        out dataImporter);

    // Import Jobs
    TestJobList(
        out dataImporter);

    TESTLOG(
        "",
        "\n"
            "-------------------------------------------------------\n"
            "---- IMPORTED PATIENT MUST MATCH THE PATIENT ABOVE ----\n"
            "----      with only selected checkup received      ----\n"
            "-------------------------------------------------------\n"
            "\n");

    ListPatients(
        out dataImporter);

    //
    // Alarm
    //

    TESTLOG(
        "",
        "\n"
            "---------------\n"
            "---- alarm ---->>> Start <<<\n"
            "---------------\n"
            "\n");

    dataImporter._m_rSession->_x_pForcePatientLink =
        m_rAlarmReceiverFakePatientLink;
    m_rAlarmReceiverFakePatientLink->_m_pdataReceivingEnd = 
        &dataImporter;

    ref<CSendAlarm> rSendAlarm = 
        rSenderSession->x_rSendAlarmDemoEmulator;

    rSendAlarm->x_rCurrentTask->x_sAuthDevice = sTestAuthDevice;

    rSendAlarm->x_rCurrentTask->x_sAlarm = "Test_Xxx12345";

    rSendAlarm->x_rCurrentTask->x_sAlarmClass = "Test_ClassPause";

    rSendAlarm->x_rCurrentTask->x_nBatteryPercent = 13;

    rSendAlarm->x_sFlashData = "--__|_.__|_.__|_._|||!";

    str sTaskId = ""; // auto RND
    rSendAlarm->
        GoSendAlarm(
            sTaskId,
            "", // sForceAlarmId
            "Test",
            res(),
            0); // nExtraFlash

    // supply "downloaded" meta
    PollFakeSync(
        rFakeSyncOut,
        rReceiverSession,
        pathScanInbox);

    // Import Jobs
    TestJobList(
        out dataImporter);

    TESTLOG(
        "",
        "\n"
            "---------------\n"
            "---- alarm ---->>> Import <<<\n"
            "---------------\n"
            "\n");

    //rReceiverSession->
    //    PollImportJobAttachments();
    rImporterJobList->
        HandleJobListTimer();


    // Import Jobs
    TestJobList(
        out dataImporter);

    ListPatients(
        out dataImporter);

    TESTLOG(
        "",
        "\n"
            "---------------\n"
            "---- alarm ---->>> Import BLOB <<<\n"
            "---------------\n"
            "\n");

    //rReceiverSession->
    //    PollImportJobAttachments();
    rImporterJobList->
        HandleJobListTimer();

    ListPatients(
        out dataImporter);

    TESTLOG(
        "",
        "\n"
            "-------------------------------------------------------\n"
            "----                    NEW QUEST                  ----\n"
            "----             remote patient monitor            ----\n"
            "-------------------------------------------------------\n"
            "\n");

    {
        str sError;

        ref<CJobStateImpl> rSendQuestJob =
            dataImporter.
                _m_rSession->
                    x_rJobList->
                        TriggerQuestSnapshot(
                            true,
                            "For testing",
                            123,
                            456,
                            "debug = cfg{12.3}!",
                            "", // sExistingSnapshotTaskId,
                            sTestAuthDevice,
                            "John Q. Quest",
                            out sError);
        //FAIL("OFF test");

        rASSTR(sError);

        // List Jobs
        TestJobList(
            out dataImporter,
            1);

        rImporterJobList->
            HandleJobListTimer();

        // Init upload driver
        Derive<CNetDriverSendatumIfaceGp>(
            rSendQuestJob->
                _x_rJobQuestStatus->
                    _m_rCacheQuestUploadDriver).Valid()->
                        PrepareSendatumToClassTest();
        /*
        Derive<CNetDriverSendatumIfaceGp>(
            rSendQuestJob->
                _x_rJobQuestPairing->
                    _m_rCacheQuestUploadDriver).Valid()->
                        PrepareSendatumToClassTest();
                        */

        // Init download driver
        /* once enough!
        Derive<CNetDriverSendatumIfaceGp>(
            rSendAlarm->
                _m_rCacheQuestDownloadDriver).Valid()->
                PrepareSendatumToClassTest();
                */

        // pump upload
        rSendQuestJob->
            _x_rJobQuestStatus->
                x_rpNetPumpOutQuest.Get().Valid()->
                    PollNetPump();
        /*
        rSendQuestJob->
            _x_rJobQuestPairing->
                x_rpNetPumpOutQuest.Get().Valid()->
                    PollNetPump();
                    */

        // pump without headers defined (useless!)
        rSendAlarm->
            x_rQuestHandler->
                x_rNetPumpInQuest->
                    PollNetPump();

        // force all timeouts to expire 
        CAppGp::GGet()->
            x_rTimeVirtualizer->
                AdvanceManualUtc(
                    100LL * 60);

        // define headers
        rSendAlarm->
            x_rQuestHandler->
                UpdateQuestPolling();

        // force debug return
        rSendAlarm->
            x_rQuestHandler->
                _m_rCacheQuestDownloadLocator->
                    _m_mapTestResponseAddUnidef[
                        type<CNetDirectionIfaceGp>()->AsNetDirectionIn()] = 
                            " Qqqqqqqq=\"!\"; ";

        // pump *WITH* headers defined
        rSendAlarm->
            x_rQuestHandler->
                x_rNetPumpInQuest->
                    PollNetPump();
        rSendAlarm->
            x_rQuestHandler->
                x_rNetPumpInQuest->
                    PollNetPump();
    }

    //
    // Cleanup
    //

    // restore original Net so we dont have to implement Close()
    rSenderSession->x_rSenderNetSyncOut = 
        rSaveNetSyncOut;

    CloseStorage(
        out dataExporter);
    CloseStorage(
        out dataImporter);

    TESTLOG(
        "",
        "\n"
            "<<<<<<< test complete! >>>>>>"
            "\n");
}

