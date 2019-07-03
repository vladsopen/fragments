// CProxyImpl.cpp

#include "CProject.h"
#include "CProxyImpl.h"
#include "CLoaderIface.h"
#include "CPatientLinkIface.h"
#include "VirtOs_file.h"
#include "CPatientIndicatorIfaceGp.h"
#include "CPatientSampleIfaceGp.h"
#include "CPatientIndicatorListIfaceGp.h"
#include "CPatientLinkSampleIface.h"
#include "CParaLoaderIface.h"
#include "VirtOs_CAppGp.h"
#include "CInterprocParaMapIfaceGp.h"
#include "CSequence.h"
#include "CInterprocRunProcessIfaceGp.h"
#include "CProtocolIface.h"
#include "CLogTypeIface.h"

CProxyImpl::CProxyImpl()
{
    _init_AttachToProxyImpl();
}

CProxyImpl::~CProxyImpl()
{
    rASSERT(!m_bProxyOpen);
}

void CProxyImpl::_init_AttachToProxyImpl()
{
    x_rSequence->_x_pProxy = this;
}

void CProxyImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToProxyImpl();

    rFAIL("make sure it is OK to expose CProxyImpl!");
}

bool CProxyImpl::OnIsProxyOpened()
{
    return m_bProxyOpen;
}

void CProxyImpl::OnInitProxy(
        path pathOpenExistingAttachment,
        str sAttachmentRevision,
        str sAttachmentExtra)
{
    // do not reuse!
    rASSERT(!_m_bOneTimeInitOk);
    _m_bOneTimeInitOk = true;

    // reinit? (we may have parameters)
    if failed(!m_bProxyOpen)
    {
        return;
    }

    rASSERTM(
        !CProject::GGetIt().
            x_rParaLoader->IsStartedAsParaService(),
        "Do not use Proxy in a ParaService process!");

    // Init parallel process controller
    x_rParaMap->
        InitParaMap();

    rASSERTM(
        !CAppGp::GGet()->x_rInterprocParaMap->IsParaServiceProcess(),
        "Never use Proxy in a ParaService process!");

    m_bProxyOpen = true;

    //
    // Init storage file path
    //

    ref<CPatientLinkIface> rPatientLink = 
        GetPatientLink();

    // in case PatientLink did not initialized the attachment yet
    _x_rPatientLinkSample->_x_pPatientLink = rPatientLink;
    _x_rPatientLinkSample->_x_pProxy = this;

    // Request a db-attached path if needed
    if (pathOpenExistingAttachment != "")
    {
        _m_pathSampleFolder = 
            pathOpenExistingAttachment;
        _m_pathSampleFolder = 
            _m_pathSampleFolder.GetFolder();

        _m_sSampleExternalFilePrefix = 
            pathOpenExistingAttachment.GetFileNameWithExt();
        if failed(_m_sSampleExternalFilePrefix != "")
        {
            ReportProxyError(
                "Missing file prefix in attachment path. "
                    "Internal PatientIo failure.",
                "/ERR:EPZ/");
        }

        _m_sSampleAttachmentRevision = 
            sAttachmentRevision;

        _m_sSampleExternalFileExtension = 
            path(sAttachmentExtra).GetFileExt();

    }
    else
    {
        _m_pathSampleFolder = 
            rPatientLink->
                GetNewFullAttachmentFilePath();
        rASSERT(_m_pathSampleFolder.GetFileNameWithExt() != "");
        _m_pathSampleFolder = 
            _m_pathSampleFolder.GetFolder();

        _m_sSampleExternalFilePrefix = 
            path();
        _m_sSampleAttachmentRevision = 
            "";
        _m_sSampleExternalFileExtension =
            "";
    }

    x_rSequence->
        InitSequence();
}

void CProxyImpl::OnCloseProxy()
{
    _x_rPatientLinkSample->
        HandleCloseProxy();

    if (m_bProxyOpen)
    {
        x_rSequence->
            CloseSequence();

        // Terminate parallel processes we have for this session
        x_rParaMap->
            CloseParaMap();
    }

    m_bProxyOpen = false;
}

void CProxyImpl::FinalizeStudy(
        str sExternalProtocolFilePrefix,
        str sAttachmentRevision,
        str sAttachmentExtension,
        str sIndicatorRecordingStart,
        str sIndicatorRecordingDuration,
        str sIndicatorEcgFragmentCount)
{
    rASSERT(_m_bOneTimeInitOk);

    ref<CPatientLinkIface> rPatientLink = 
        GetPatientLink();

    ref<CPatientSampleIfaceGp> rPatientSample;

    //TODO: Replace with your index set. Save ALL session/signal properties,
    //      device settings and all useful context info.
    //      Any indicator can be hidden by default and the user can customize 
    //      the table in Patients.

    //
    // Start
    //
    {
        ref<CPatientIndicatorIfaceGp> rPatientIndicator;

        rPatientIndicator->x_sIndicatorKey = 
            "RecordingStart"; // internal ID

        rPatientIndicator->x_richIndicatorShortName = 
            TRANSLATE(
                "Recording [... UI-string-removed]");

        rPatientIndicator->x_richIndicatorDescription = 
            TRANSLATE(
                "Monitoring[... UI-string-removed]");

        rPatientIndicator->x_bIndicatorIsPartOfSampleTitle = 
            true; // only for important desciptive indicators

        rPatientIndicator->x_bIndicatorNameOptional = 
            true; // only if value meaning is obvious

        rPatientIndicator->x_bIndicatorHiddenByFactoryDefault = 
            false; // hide in factory setup?

        rPatientIndicator->x_richAssignTextIndicator = 
            sIndicatorRecordingStart;

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
            "RecordingDuration"; // internal ID

        rPatientIndicator->x_richIndicatorShortName = 
            TRANSLATE(
                "Duration[... UI-string-removed]");

        rPatientIndicator->x_richIndicatorDescription = 
            TRANSLATE(
                "Monitoring[... UI-string-removed]");

        rPatientIndicator->x_bIndicatorIsPartOfSampleTitle = 
            false; // only for important desciptive indicators

        rPatientIndicator->x_bIndicatorNameOptional = 
            false; // only if value meaning is obvious
[...]