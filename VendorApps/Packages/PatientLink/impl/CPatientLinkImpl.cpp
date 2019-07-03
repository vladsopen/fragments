// CPatientLinkImpl.cpp

#include "CProject.h"
#include "CPatientLinkImpl.h"
#include "CLoaderIface.h"
#include "CPatientBaseConnectionIfaceGp.h"
#include "CPatientCallbackImpl.h"
#include "CPatientSampleIfaceGp.h"
#include "CPatientIndicatorListIfaceGp.h"
#include "CPatientIndicatorIfaceGp.h"
#include "CPatientCheckupIfaceGp.h"
#include "CAppEventFloaterIfaceGp.h"
#include "VirtOs_file.h"
#include "VirtOs_LegacyResource.h"
#include "CPatientEmbedConnectionIfaceGp.h"
#include "CProxyIface.h"
#include "CPatientEmbedLinkImpl.h"
#include "CPatientLinkSampleImpl.h"
//#include "CPatientSampleMap.h"
#include "CLogTypeIface.h"
#include "CPatLiteFactoryIfaceGp.cpp"
#include "CPatInfoPimpSessionIfaceGp.h"
#include "CPatientDebugIfaceGp.h"
#include "CPatientPatientInfoIfaceGp.h"
#include "CPatientSexTypeIfaceGp.h"
#include "CPatientModalityFeaturesIfaceGp.h"
#include "CDemoIface.h"
#include "CUbroRefineIfaceGp.h"

NOTEST; //todo

CPatientLinkImpl::CPatientLinkImpl()
{
    _init_AttachToPatientLinkImpl();
}

void CPatientLinkImpl::_init_AttachToPatientLinkImpl()
{
    m_rPatientCallback->_x_pPatientLink = this;
}

void CPatientLinkImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    _init_AttachToPatientLinkImpl();
}

bool CPatientLinkImpl::OnInitPatientIsMainFrameHidden()
{
    ASSERT(!_m_bPatientLinkInitOk);
    _m_bPatientLinkInitOk = true;

    // no Patients, no PatLite for Demo mode
    if (CProject::GGetIt().
            x_rLoader->
                x_rDemo->
                    IsExeStartedForDemo())
    {
        return false;
    }

    //
    // Select DB features required for this modality application
    //

    ref<CPatientModalityFeaturesIfaceGp> rPatientModalityFeatures;

    rPatientModalityFeatures->x_sDeclareModalityKey = 
        "CustomApps";

    // Allow user to add new fields to the card?
    rPatientModalityFeatures->
        x_slocDisableAddNewCardFields =
            CPatientModalityFeaturesIfaceGp::C_sHideUi;

    // Show graphs and stats module button for patient list?
    rPatientModalityFeatures->
        x_slocDisableCharterStatistics = 
            CPatientModalityFeaturesIfaceGp::C_sHideUi;

    // Shows check boxes for multi-patient operations
    rPatientModalityFeatures->
        x_slocDisableMultiplePatientSelection = 
            CPatientModalityFeaturesIfaceGp::C_sHideUi;

    // Init PatientIo
    x_rPatientBaseConnection->
        InitPatientBaseConnection(
            m_rPatientCallback,
            rPatientModalityFeatures);

    // Switch to PatLite if no Patients detected
    if (!x_rPatientBaseConnection->IsConnectedToPatient()
        &&
        !debug::GIsTest())
    {
        //
        // Instantiate PatLite session without creating extra link-dependency on 
        //   its classes or on PatInfo
        //

        PRELOAD_PROJECT(PatLite);

        // special dependency-breaking factory (#included .cpp!)
        ref<CPatInfoPimpSessionIfaceGp> rNewPatInfoPimpSession;
        ref<CPatLiteFactoryIfaceGp>()->
            NewPatLiteSessionImpl(
                out rNewPatInfoPimpSession);

        // extra init parameters
        ref<CPatInfoInitSessionIfaceGp> rPatInfoInitSession;

        // link to selected impl
        this->x_rPatientBaseConnection = 
            CPatientBaseConnectionIfaceGp::GNewPimpedConnection(
                rNewPatInfoPimpSession,
                rPatInfoInitSession,
                rPatientModalityFeatures);

        // second attempt to init using another Patients impl
        x_rPatientBaseConnection->
            InitPatientBaseConnection(
                m_rPatientCallback,
                rPatientModalityFeatures);

        // retry PatientEmbed init which must have failed 
        //   as we definitely aren't running as a DLL within Patients
        CProject::GGetIt().
            x_rPatientEmbedLink->
                InitPatientEmbedLink(
                    rNewPatInfoPimpSession->
                        NewPatientEmbedConnection());

    }

    // Prepare checkup object
    if (!x_rPatientBaseConnection->
            GetCurrentCheckup(
                out m_rPatientCheckup))
    {
        rFAIL("cannot get current checkup");
    }

    return IsConnectedToPatient();
}

void CPatientLinkImpl::OnClosePatientLink()
{
    // Added for advanced PatInfo Pi-DB implementations.
    // This is mainly for ASSERTs because DB cannot depend
    //   on close-handlers to save patient data!

    x_rPatientBaseConnection->
        ClosePatientBaseConnection();
}

bool CPatientLinkImpl::OnIsConnectedToPatient()
{
    return x_rPatientBaseConnection->IsConnectedToPatient();
}

/*
str CPatientLinkImpl::OnGetCurrentPatientNameAgeSexAndId()
{
    ref<CPatientPatientInfoIfaceGp> rPatientPatientInfo = 
        x_rPatientBaseConnection->GetCurrentPatientInfo();

    str sReport;
    if (rPatientPatientInfo->x_sPatientKey == "")
    {
        sReport += 
            TRANSLATE(
                "No patient[... UI-string-removed]");
    }
    else
    {
        // Name
        sReport += 
            "\"" + rPatientPatientInfo->x_sPatientName + "\"";

        sReport += 
            ", ";

        // Age
        sReport += 
            Str(rPatientPatientInfo->x_nPatientAgeInYears) + 
                " years (" + 
                rPatientPatientInfo->x_momentPatientBirthDate.Get().
                    ToStr("yyyy-mm-dd HH:MM:SS.TTT") +
                ")";

        sReport += 
            ", ";

        // Sex
        sReport += 
            Str(rPatientPatientInfo->x_typePatientSex->GetShortSexForUi());

        // Public ID (optional!)
        sReport += 
            " [";
        sReport += 
            rPatientPatientInfo->
                LookupPatientFieldValueOr(
                    CPatientPatientInfoIfaceGp::
                        C_sBasicPatientFieldKeyForIdNumber,
                    "ID?");
        sReport += 
            "]";
    }

    return sReport;
}
*/

str CPatientLinkImpl::OnGetCurrentPatientFieldValue(
        str sFieldKey,
        str sOrReturn)
{
    ref<CPatientPatientInfoIfaceGp> rPatientPatientInfo = 
        x_rPatientBaseConnection->GetCurrentPatientInfo();

    rASSERT(sFieldKey != "");
    return 
        rPatientPatientInfo->
            LookupPatientFieldValueOr(
                sFieldKey,
                sOrReturn);
}

int CPatientLinkImpl::OnGetCurrentPatientAge()
{
    ref<CPatientPatientInfoIfaceGp> rPatientPatientInfo = 
        x_rPatientBaseConnection->GetCurrentPatientInfo();

    return 
        rPatientPatientInfo->
            x_nPatientAgeInYears;
}

void CPatientLinkImpl::OnHandleStartNewCheckup()
{
    rASSERT(_m_bPatientLinkInitOk);

    // Start *new* checkup (do not continue exisiting)
    x_rPatientBaseConnection->ForceCreateNewCheckup();
}

void CPatientLinkImpl::OnSendSampleProxyToPatient(
        ref<CPatientSampleIfaceGp> rPatientSample,
        ref<CProxyIface> rProxy)
{
    rASSERT(_m_bPatientLinkInitOk);

    // leave x_unidefSaveAsBlob empty for now - everything is in the external file
    //   but dont send an empty str, comply to the ritual - NON-EMPTY unidef!
    unidef unidefEmpty;
    unidefEmpty.DefineStr("Empty", "Proxy"); // meaningless! just not empty.
    rPatientSample->x_unidefSaveAsBlob = unidefEmpty;

    TESTLOG(
        "",
        "PatientLink is sending " + 
            Str(rPatientSample->x_rIndicatorList->GetIndicatorCount()) + 
            " indicators.\n");

    // Attach to the existing key if already defined
    ref<CPatientLinkSampleImpl> rPatientLinkSample =
        rProxy->
            _x_rPatientLinkSample;

    // update for existing
    rPatientSample->x_sSampleKey = 
        rPatientLinkSample->x_sOpenedPatientSampleKey;

    // Send info to Patients
    str sError = "";
    if (!rPatientSample->
            SendSampleInfo(
                m_rPatientCheckup,
                out sError))
    {
        TESTLOG(
            "",
            "PatientLink got an error from SendSampleInfo: " +
                sError +
                "\n\n");

        ReportPatientCommunicationError(
            "Send Proxy failed: " + sError);
    }
    else
    {
        TESTLOG(
            "",
            "PatientLink has sent the sample data.\n\n");

        // save the sample key
        rPatientLinkSample->
            x_sOpenedPatientSampleKey = 
                rPatientSample->x_sSampleKey;
    }
}

path CPatientLinkImpl::OnGetNewFullAttachmentFilePath()
{
    // Get external base file name
    path pathProxy = 
        x_rPatientBaseConnection->
            GetNewAttachmentPrefix();

    // can Patients save?
    if (pathProxy == "")
    {
        TESTLOG(
            "",
            "PatientLink could not get a file path from Patients.\n");

        if (x_rPatientBaseConnection->IsConnectedToPatient())
        {
            ReportPatientCommunicationError(
                TRANSLATE(
                    "Cannot get[... UI-string-removed]");
        }

        //todo: provide a save-to-file stub for standalone mode?

        return path("");
    }

    // We get a path WITHOUT an ext. Be careful!
    rASSERT(pathProxy.GetFileExt() == "");
    rASSERT(C_pathMainProxyExternalFileExt.GetLeft(1) == ".");
    pathProxy = 
        pathProxy.
            GetAppendedPath(
                C_pathMainProxyExternalFileExt);

    TESTLOG(
        "",
        "PatientLink got a file path from Patients and added .ext: \"" + 
            pathProxy + "\"\n");

    return pathProxy;
}

void CPatientLinkImpl::OnShowPatientCard()
{
    x_rPatientBaseConnection->
        GotoPatientCard();
}

void CPatientLinkImpl::OnLockCurrentCheckup(
        bool bLockOn)
{
    if (bLockOn)
    {
        m_rPatientCheckup->
            CheckupStarted();
    }
    else
    {
        m_rPatientCheckup->
            CheckupFinished();
    }
}

void CPatientLinkImpl::ReportPatientCommunicationError(
        str sReport)
{
    rASSERT(_m_bPatientLinkInitOk);

    if (x_rPatientBaseConnection->IsConnectedToPatient())
    {
        type<CLogTypeIface>()->
            AsLogTypeForDatabaseErrors()->
                LogEvent(
                    TRANSLATE(
                        "Database c[... UI-string-removed]");
    }
}

ref<UiContainer> CPatientLinkImpl::OnNewUiPatInfoBrowser()
{
    return 
        x_rPatientBaseConnection->
            NewUiPatInfoBrowserForMainWindow(
                ptr<CUbroRefineIfaceGp>()); // default: all patients
}

ref<UiContainer> CPatientLinkImpl::OnNewUiPatientDebug()
{
    return 
        x_rPatientBaseConnection->
            x_rPatientDebug->
                NewUiPatientDebugMenuButton();
}

