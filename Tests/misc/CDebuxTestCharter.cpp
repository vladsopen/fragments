// CDebuxTestCharter.cpp

#include "CDebuxProject.h"
#include "CDebuxTestCharter.h"
#include "CDebuxLoaderIface.h"
#include "CPatientBaseConnectionIfaceGp.h"
#include "CPatInfoSessionIfaceGp.h"
#include "CPatInfoStoreIfaceGp.h"
#include "CPiOrganizerIfaceGp.h"
#include "CPiPatientIfaceGp.h"
#include "CPiCardFillingIfaceGp.h"
#include "CPiFieldValueIfaceGp.h"
#include "CPatientPatientInfoIfaceGp.h"
#include "CPiCardConfigIfaceGp.h"
#include "CPatientSexTypeIfaceGp.h"
#include "CPiFieldConfigIfaceGp.h"
#include "CCharterSessionIfaceGp.h"
#include "CCallDealIfaceGp.h"
#include "CAccelIfaceGp.h"
#include "VirtOs_dir.h"
#include "CPatInfoBrowserIfaceGp.h"
#include "CDebuxPatientLinkIface.h"
#include "CDebuxPatientLinkImpl.h"
#include "VirtOs_unicode.h"
#include "CDebuxGlobalConfigIface.h"
#include "CDebuxPatientEmbedLinkIface.h"

CDebuxTestCharter::CDebuxTestCharter()
{
    _init_AttachToDebuxTestCharter();
}

void CDebuxTestCharter::_init_AttachToDebuxTestCharter()
{
    //x_r->_x_p = this;
}

void CDebuxTestCharter::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToDebuxTestCharter();

    rFAIL("make sure it is OK to expose CDebuxTestCharter!");
}

void CDebuxTestCharter::TestCharterWithStore(
        str sStoreName,
        ref<CPatInfoStoreIfaceGp> rPatInfoStore)
{
    debug::GRedirectTestLog("+Udb", "DbCrap");

    debug::GRedirectTestLog("+NetSync", "DbCrap");
    debug::GRedirectTestLog("+FileMultipart", "DbCrap");
    debug::GRedirectTestLog("+Time", "DbCrap");
    debug::GRedirectTestLog("+Sync", "DbCrap");

    assertoff ao("no rowid in PatLite");

    // no longer needed?
    /*
    // clear all old test files
    {
        str sDeleteError;
        dir(ref<CPatInfoStoreIfaceGp>()->GetPatientDatabaseFolder()).
            EraseDirContent(out sDeleteError);
        rASSERTM(sDeleteError == "", "Failed to clean old db " + sDeleteError);
    }
    */

    ref<CDebuxLoaderIface> rDebuxLoader = 
        CDebuxProject::GGetIt().x_rDebuxLoader;

    ref<CDebuxGlobalConfigIface> rDebuxGlobalConfig =
        CDebuxProject::GGetIt().
            x_rDebuxGlobalConfig;

    ref<CDebuxPatientEmbedLinkIface> rDebuxPatientEmbedLink = 
        CDebuxProject::GGetIt().
            x_rDebuxPatientEmbedLink;

    ref<CDebuxPatientLinkImpl> rDebuxPatientLink = 
        rDebuxLoader->x_rDebuxPatientLink;

    ref<CPatientBaseConnectionIfaceGp> rPatientBaseConnection =
        rDebuxPatientLink->x_rPatientBaseConnection;

    rDebuxLoader->
        x_rDebuxPatientLink->
            _x_pForcePatInfoStore =
                rPatInfoStore;

    rPatientBaseConnection->_x_bEmulatePatientConnectionOkForClassTest = 
        false; // force resort to PatInfo

    /*
    ref<CPatLiteGlobalConfigIface> rPatLiteGlobalConfig =
        CDebuxProject::GGetIt().
            x_rPatLiteGlobalConfig;
            */

    //
    // Start Application 
    //

    TESTLOG(
        "",
        "\n"
            "=== Initializing PatLite Session ===\n\n");

    rDebuxLoader->
        StartApplication();
    rDebuxLoader->
        EnsureDebuxSessionOpenAndActivateMainWindow(
            true); // bReallyOpenSession

    //ref<CDebuxSessionImpl> rDebuxSession = 
    //    rDebuxLoader->x_rDebuxSession;
    ref<CPatInfoSessionIfaceGp> rPatInfoSession =
        rPatInfoStore->GetPatInfoSession();

    /*
    ref<CPatInfoStoreIfaceGp> rPatInfoStore2 = 
        rPatInfoSession->GetPatInfoStore();
    rASSERT(rPatInfoStore2 == rPatInfoStore);
    */

    m_rPatInfoBrowser = 
        rPatInfoSession->GetPatInfoBrowser();
    m_rPiOrganizer =
        rPatInfoStore->GetAllPatientsPiOrganizer();

    //bool bPatDb = 
    //    m_rPiOrganizer->GetObjectClassInfo().GetClassNameInProcess().
    //        FindPos("PatDb") >= 0;

    m_rPiCardConfig =
        rPatInfoStore->GetGlobalPiCardConfig();

    ref<CPiFieldConfigIfaceGp> rNamePiFieldConfig;
    VERIFY(
        m_rPiCardConfig->
            LookupCardFieldConfigByKey(
                CPatientPatientInfoIfaceGp::C_sBasicPatientFieldKeyForName,
                out rNamePiFieldConfig,
                E_AddIfMissing_No));

    ref<CPiFieldConfigIfaceGp> rSexPiFieldConfig;
    VERIFY(
        m_rPiCardConfig->
            LookupCardFieldConfigByKey(
                CPatientPatientInfoIfaceGp::C_sBasicPatientFieldKeyForSex,
                out rSexPiFieldConfig,
                E_AddIfMissing_No));

    ref<CPiFieldConfigIfaceGp> rBirthPiFieldConfig;
    VERIFY(
        m_rPiCardConfig->
            LookupCardFieldConfigByKey(
                CPatientPatientInfoIfaceGp::C_sBasicPatientFieldKeyForBirthDate,
                out rBirthPiFieldConfig,
                E_AddIfMissing_No));

    TESTLOG(
        "",
        "\n"
            "=== Creating Patients ===\n\n");

    // Patient 1
    ref<CPiPatientIfaceGp> rFirstPiPatient;
    {
        bool bNewPatientCreated = false;

        ref<CPiPatientIfaceGp> rPiPatient =
            m_rPiOrganizer->
                CreateAndSelectNewPiPatient(
                    "", //sForcePatientKey
                    out bNewPatientCreated);

        rASSERT(bNewPatientCreated);

        rFirstPiPatient = rPiPatient;

        ref<CPiCardFillingIfaceGp> rPiCardFilling =
            rPiPatient->x_rPiCardFilling;

        // Name
        {
            ref<CPiFieldValueIfaceGp> rPiFieldValue = 
                rPiCardFilling->
                    LookupFieldValueByConfig(
                        rNamePiFieldConfig);

            rPiFieldValue->
                SetPiFieldValueAsStr(   
                    unicode::GConvertSourceToSloc(
                        "Youngster Jimmy"));
        }

        // Sex
        {
            ref<CPiFieldValueIfaceGp> rPiFieldValue = 
                rPiCardFilling->
                    LookupFieldValueByConfig(
                        rSexPiFieldConfig);

            rPiFieldValue->
                SetPiFieldValueAsStr(
                    type<CPatientSexTypeIfaceGp>()->
                        AsPatientSexTypeAsMale()->
                            GetDicomSexId());
        }

        // Age
        {
            ref<CPiFieldValueIfaceGp> rPiFieldValue = 
                rPiCardFilling->
                    LookupFieldValueByConfig(
                        rBirthPiFieldConfig);

            moment momentBirth;
            momentBirth.SetYear(1990);
            momentBirth.SetMonth(1);
            momentBirth.SetDay(1);

            rPiFieldValue->
                SetPiFieldValueAsMoment(
                    momentBirth);
        }
    }

    // Patient 2
    ref<CPiPatientIfaceGp> rSecondPiPatient;
    {
        bool bNewPatientCreated = false;

        ref<CPiPatientIfaceGp> rPiPatient =
            m_rPiOrganizer->
                CreateAndSelectNewPiPatient(
                    "", //sForcePatientKey
                    out bNewPatientCreated);

        rASSERT(bNewPatientCreated);

        rSecondPiPatient = rPiPatient;

        ref<CPiCardFillingIfaceGp> rPiCardFilling =
            rPiPatient->x_rPiCardFilling;

        // Name
        {
            ref<CPiFieldValueIfaceGp> rPiFieldValue = 
                rPiCardFilling->
                    LookupFieldValueByConfig(
                        rNamePiFieldConfig);

            rPiFieldValue->
                SetPiFieldValueAsStr(   
                    unicode::GConvertSourceToSloc(
                        "Xyz Abc"));
        }

        // Sex
        {
            ref<CPiFieldValueIfaceGp> rPiFieldValue = 
                rPiCardFilling->
                    LookupFieldValueByConfig(
                        rSexPiFieldConfig);

            rPiFieldValue->
                SetPiFieldValueAsStr(
                    type<CPatientSexTypeIfaceGp>()->
                        AsPatientSexTypeAsMale()->
                            GetDicomSexId());
        }

        // Age
        {
            ref<CPiFieldValueIfaceGp> rPiFieldValue = 
                rPiCardFilling->
                    LookupFieldValueByConfig(
                        rBirthPiFieldConfig);

            moment momentBirth;
            momentBirth.SetYear(1986);
            momentBirth.SetMonth(4);
            momentBirth.SetDay(1);

            rPiFieldValue->
                SetPiFieldValueAsMoment(
                    momentBirth);
        }
    }

    // Patient 3
    ref<CPiPatientIfaceGp> rPiPatient3;
    {
        bool bNewPatientCreated = false;

        ref<CPiPatientIfaceGp> rPiPatient =
            m_rPiOrganizer->
                CreateAndSelectNewPiPatient(
                    "", //sForcePatientKey
                    out bNewPatientCreated);

        rASSERT(bNewPatientCreated);

        rPiPatient3 = rPiPatient;

        ref<CPiCardFillingIfaceGp> rPiCardFilling =
            rPiPatient->x_rPiCardFilling;

        // Name
        {
            ref<CPiFieldValueIfaceGp> rPiFieldValue = 
                rPiCardFilling->
                    LookupFieldValueByConfig(
                        rNamePiFieldConfig);

            rPiFieldValue->
                SetPiFieldValueAsStr(   
                    unicode::GConvertSourceToSloc(
                        "Skjhsdfjsjhlkjhdsflkjhtelrthj S.AS.Dlkjlkewrh D."));
        }

        // Sex
        {
            ref<CPiFieldValueIfaceGp> rPiFieldValue = 
                rPiCardFilling->
                    LookupFieldValueByConfig(
                        rSexPiFieldConfig);

            rPiFieldValue->
                SetPiFieldValueAsStr(
                    type<CPatientSexTypeIfaceGp>()->
                        AsPatientSexTypeAsFemale()->
                            GetDicomSexId());
        }

        // Age
        {
            ref<CPiFieldValueIfaceGp> rPiFieldValue = 
                rPiCardFilling->
                    LookupFieldValueByConfig(
                        rBirthPiFieldConfig);

            moment momentBirth;
            momentBirth.SetYear(1988);
            momentBirth.SetMonth(1);
            momentBirth.SetDay(1);

            rPiFieldValue->
                SetPiFieldValueAsMoment(
                    momentBirth);
        }
    }

    // Patient 4
    ref<CPiPatientIfaceGp> rPiPatient4;
    {
        bool bNewPatientCreated = false;

        ref<CPiPatientIfaceGp> rPiPatient =
            m_rPiOrganizer->
                CreateAndSelectNewPiPatient(
                    "", //sForcePatientKey
                    out bNewPatientCreated);

        rASSERT(bNewPatientCreated);

        rPiPatient4 = rPiPatient;

        ref<CPiCardFillingIfaceGp> rPiCardFilling =
            rPiPatient->x_rPiCardFilling;

        // Name
        {
            ref<CPiFieldValueIfaceGp> rPiFieldValue = 
                rPiCardFilling->
                    LookupFieldValueByConfig(
                        rNamePiFieldConfig);

            rPiFieldValue->
                SetPiFieldValueAsStr(   
                    unicode::GConvertSourceToSloc(
                        "X"));
        }

        // Sex
        {
            ref<CPiFieldValueIfaceGp> rPiFieldValue = 
                rPiCardFilling->
                    LookupFieldValueByConfig(
                        rSexPiFieldConfig);

            rPiFieldValue->
                SetPiFieldValueAsStr(
                    type<CPatientSexTypeIfaceGp>()->
                        AsPatientSexTypeAsFemale()->
                            GetDicomSexId());
        }

        // Age
        {
            ref<CPiFieldValueIfaceGp> rPiFieldValue = 
                rPiCardFilling->
                    LookupFieldValueByConfig(
                        rBirthPiFieldConfig);

            moment momentBirth;
            momentBirth.SetYear(1976);
            momentBirth.SetMonth(1);
            momentBirth.SetDay(1);

            rPiFieldValue->
                SetPiFieldValueAsMoment(
                    momentBirth);
        }
    }

    //
    // Select some patients
    //

    m_rPatInfoBrowser->
        SetMainSelectedPatientKey(
            rPiPatient3->GetPiPatientKey());
    m_rPatInfoBrowser->
        SetMainSelectedPatientKey(
            rPiPatient4->GetPiPatientKey());
    m_rPatInfoBrowser->
        SetMainSelectedPatientKey(
            rSecondPiPatient->GetPiPatientKey());

    DumpPatientList(
        "Initial");

    // Save
    //rPatLiteSession->
    //    FinalizeStudy();

    //
    // Generate Stats
    //

    TESTLOG(
        "",
        "\n"
            "======= Init Statistics =======\n");

    PRELOAD_PROJECT(Normer);
    ref<CCharterSessionIfaceGp> rCharterSession;

    /*
    // too much db dumps in our main log
    debug::GRedirectTestLog(
        "+Udb", 
        "UdbStats" + 
            //(bPatDb? "PatDb" : ""));
            sStoreName);
    */

    debug::GDisableTestLog("+AccelUi");

    // Init charter
    ref<CCallDealIfaceGp> rCallDealExecute;
    rCallDealExecute->RequestCommandExecution();
    rCharterSession->
        DealCharterOpen(
            rCallDealExecute,
            m_rPiOrganizer);

    // Configure source fields
    rCharterSession->
        ConfigureFieldAsCharterSource(
            CPatientPatientInfoIfaceGp::C_sBasicPatientFieldKeyForSex,
            true, // use as stats source
            1.0);
    rCharterSession->
        ConfigureFieldAsCharterSource(
            CPatientPatientInfoIfaceGp::C_sBasicPatientFieldKeyForBirthDate,
            true, // use as stats source
            10.0);

    TESTLOG(
        "",
        "\n"
            "======= Recalc Statistics =======\n");

    // Dump results and close
    rCharterSession->
        CloseCharterSession();

    //debug::GRedirectTestLog("+Udb", "UdbPatOp");

    //
    // Delete a patient
    //

    TESTLOG(
        "",
        "\n"
            "======= Deleting second patient =======\n");

    rSecondPiPatient->
        DeletePiPatient();

    DumpPatientList(
        "After delete second");

    //
    // Add a patient after delete
    //

    TESTLOG(
        "",
        "\n"
            "======= Adding patient after delete =======\n");

    // Patient 5(4)
    {
        bool bNewPatientCreated = false;

        ref<CPiPatientIfaceGp> rPiPatient =
            m_rPiOrganizer->
                CreateAndSelectNewPiPatient(
                    "", //sForcePatientKey
                    out bNewPatientCreated);

        rASSERT(bNewPatientCreated);

        ref<CPiCardFillingIfaceGp> rPiCardFilling =
            rPiPatient->x_rPiCardFilling;

        // Name
        {
            ref<CPiFieldValueIfaceGp> rPiFieldValue = 
                rPiCardFilling->
                    LookupFieldValueByConfig(
                        rNamePiFieldConfig);

            rPiFieldValue->
                SetPiFieldValueAsStr(   
                    unicode::GConvertSourceToSloc(
                        "D.D."));
        }
    }

    DumpPatientList(
        "Add after delete");

    //
    // Finish
    //

    {
        TESTLOG(
            "",
            "\n\n"
                "====== Final Charter Patient DB Dump =======\n");

        str sDump = 
            rPatInfoStore->
                DumpSmallPatInfoDatabaseTestStorageToInMemoryUnidef().
                    ExportAsTextWithoutHeader();

        TESTLOG(
            "",
            sDump + "\n");
    }

    TESTLOG(
        "",
        "\n"
            "======= Closing Test... =======\n");

    rDebuxLoader->
        CloseDebuxSessionAndMainWindow(
            false); // bAllowExitApp

    TESTLOG(
        "",
        "\n"
            "======= Test is complete. Dumping states... =======\n");

    // expose final object states:
    {
        assertoff ao1("make sure it is OK to expose");
        assertoff ao2("expose a Loader object itself");
        assertoff aoGuiPlus("it may not be safe to expose this GuiPlus class!");

        // skip a line
        TESTLOG(
            "",
            "\n");

        /*
        // session dump
        TESTLOG(
            "FinalLoader" + 
                //(bPatDb? "PatDb" : ""),
                sStoreName,
            "rDebuxLoader: + " + 
                rDebuxLoader->ExposeOut().ExportAsText().
                    GetReplaced(sStoreName, "PatLite") + 
                "\n\n");

        // config dump
        TESTLOG(
            "FinalConfig",
            "GlobalConfig: " + 
                rPatLiteGlobalConfig->ExposeOut().ExportAsText().
                    GetReplaced(sStoreName, "PatLite") + 
                "\n\n");
                */
    }

    //
    // Close Application 
    //

    TESTLOG(
        "",
        ""
            "======= Closing ======= + \n\n");

    // This will use fake PatientLink to make important decisions
    //   (will not really exit in TEST mode I hope...)
    rDebuxLoader->
        CloseDebuxSessionAndMainWindow(
            false); // bAllowExitApp

    rDebuxPatientLink->
        ClosePatientLink();

    TESTLOG(
        "",
        "\n"
            "=== The End ==="
            "\n\n");
}

void CDebuxTestCharter::DumpPatientList(
        str sLog)
{
    TESTLOG(
        "",
        "\n"
            "======= Dumping Patient List (" + sLog + ")... =======\n");

    TESTLOG(
        "PatientList",
        "\n\n"
            "======= Patient List for " + sLog + "... =======\n");
    
    // Iterate all patients in an organizer
    {
        ref<CPiPatientIfaceGp> rIterPiPatient;
        for (
            iter iPatient;
            m_rPiOrganizer->
                IteratePiOrganizerPatients(
                    out iPatient,
                    out rIterPiPatient);)
        {
            str sSelected;
            bool bMultiSelected = 
                rIterPiPatient->
                    IsPiPatientSelected();
            if (m_rPatInfoBrowser->GetMainSelectedPatientKey() == 
                    rIterPiPatient->GetPiPatientKey())
            {
                rASSERT(bMultiSelected);
                sSelected = "[>]";
            }
            else
            if (bMultiSelected)
            {
                sSelected = "[+]";
            }
            else
            {
                sSelected = "[-]";
            }

            // one patient per line
            TESTLOG(
                "",
                "\n"
                    "    Patient: " + sSelected + " "
                    FORCE_TESTLOG_SLOC);
            TESTLOG(
                "PatientList",
                "\n " + sSelected + " " +
                    FORCE_TESTLOG_SLOC);

            // iterate all patient field values corresponding to global card config
            ref<CPiFieldConfigIfaceGp> rIterPiFieldConfig;
            for (
                iter iField;
                m_rPiCardConfig->
                    IterateCardFieldConfig(
                        out iField,
                        out rIterPiFieldConfig);)
            {
                // Lookup field value for this patient
                ref<CPiFieldValueIfaceGp> rPiFieldValue =
                    rIterPiPatient->
                        x_rPiCardFilling->
                            LookupFieldValueByConfig(
                                rIterPiFieldConfig);

                // We can convert any value to string
                str sPatientFieldValue =
                    rPiFieldValue->
                        GetPiFieldValueAsStr();

                // dump it
                TESTLOG(
                    "",
                    "" +
                        sPatientFieldValue + ", "
                        FORCE_TESTLOG_SLOC);
                TESTLOG(
                    "PatientList",
                    "" +
                        sPatientFieldValue + ", "
                        FORCE_TESTLOG_SLOC);
            }
        }
    }

    str sSelection = "\n  (selected:";

    // Iterate selected patients in an organizer
    {
        ref<CPiPatientIfaceGp> rIterPiPatient;
        for (
            iter iPatient;
            m_rPiOrganizer->
                IteratePiOrganizerSelectedPatients(
                    out iPatient,
                    out rIterPiPatient);)
        {
            sSelection += 
                " <" +
                    rIterPiPatient->GetPiPatientNameOrStub("NotNamed") +
                    ">"
                    FORCE_TESTLOG_SLOC;
        }
    }

    sSelection += ")\n";

    // after last patient
    TESTLOG(
        "",
        sSelection);
    TESTLOG(
        "PatientList",
        sSelection);

}