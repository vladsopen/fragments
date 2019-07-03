// CPatientCallbackImpl.cpp

#include "CProject.h"
#include "CPatientCallbackImpl.h"
#include "CLoaderIface.h"
#include "CPatientSampleIfaceGp.h"
#include "CPatientPaintContextIfaceGp.h"
#include "VirtOs_file.h"
#include "CProxyIface.h"
#include "CPatientLinkSampleIface.h"
#include "CPatientLinkSampleImpl.h"
#include "CPatientLinkImpl.h"
#include "CPatientSampleMap.h"
#include "CDemoIface.h"

NOTEST; //todo

CPatientCallbackImpl::CPatientCallbackImpl()
{
}

void CPatientCallbackImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);
}

void CPatientCallbackImpl::OnActivateSamplerApp()
{
    TESTLOG(
        "",
        "Callback hit: ActivateSamplerApp.\n");

    // Make sure window is visible and the checkup session started
    CProject::GGetIt().
        x_rLoader->
            EnsureProxyOpenAndActivateMainWindow();
}

void CPatientCallbackImpl::OnHandlePatientExit()
{
    TESTLOG(
        "",
        "Callback hit: HandlePatientExit.\n");

    // nothing else we can do.
    //   eveything must have been saved before, or it is lost until recovery

    sys::GPostQuitMessage();
}

void CPatientCallbackImpl::OnOpenSample(
        ref<CPatientSampleIfaceGp> rPatientSample)
{
    ref<CPatientLinkImpl> rPatientLink = 
        _x_pPatientLink.Get().Valid();

    //FAIL_SOFT("OpenSample");

    TESTLOG(
        "",
        "Callback-OpenSample is about to create a new Proxy...\n");

    // Terminate previous proxy session
    CProject::GGetIt().
        x_rLoader->
            CloseProxyAndMainWindow();

    // Create a new Proxy
    ref<CProxyIface> rNewProxy =
        CProject::GGetIt().
            x_rLoader->
                x_rProxy;

    ref<CPatientLinkSampleImpl> rPatientLinkSample =
        rNewProxy->
            _x_rPatientLinkSample;
    rPatientLinkSample->
        x_bProxyOpenedFromPatient = 
            true;

    rPatientLinkSample->x_sOpenedPatientSampleKey =
        rPatientSample->x_sSampleKey;

    TESTLOG(
        "",
        "Callback hit: OpenSample. Making sure there is Main Frame and session:\n");

    rNewProxy->
        InitProxy(
            rPatientSample->x_pathAttachmentPrefix,
            rPatientSample->x_sAttachmentRevision,
            rPatientSample->x_sAttachmentExtra);

    // Make sure window is visible and the checkup session started
    CProject::GGetIt().
        x_rLoader->
            EnsureProxyOpenAndActivateMainWindow();

    TESTLOG(
        "",
        "Callback-OpenSample is about to load the new Proxy...\n");

    /*
    // Add to the map
    if (rPatientLink->
            x_rOpenedPatientSampleMap->
                AddAndOpenNewProxy(
                    rPatientSample,
                    rNewProxy))
    {
        TESTLOG(
            "",
            "Callback-OpenSample opened new Proxy...\n");

        //DEMO: Create a separate floater for opened sample
        rPatientLinkSample->
            OpenUiPatientLinkSampleView();
    }
    else
    {
        TESTLOG(
            "",
            "Callback-OpenSample resusing existing Proxy...\n");
    }
    */
}

void CPatientCallbackImpl::OnPaintSample(
        ref<CPatientSampleIfaceGp> rPatientSample,
        ref<CPatientPaintContextIfaceGp> rPatientPaintContext)
{
    rPatientSample;
    rPatientPaintContext;

    rFAIL("unsupported");
}

void CPatientCallbackImpl::OnHandleNewDbSelectionReport()
{
    static int s_nDebugCountSelections = 0;
    s_nDebugCountSelections++;

    debug("PatientLink./show.SelectionReports:").
        FloatNote(
            "Db selection changed " + Str(s_nDebugCountSelections) + " times");

    // see GetCurrentPatientNameAgeSexAndId()
}

void CPatientCallbackImpl::OnHandlePatientSearchResult(
        str sUniqueSearchOperationId,
        int nMatchingPatientsFound,
        bool bPatientSelectionChanged,
        str sSearchError)
{
    bPatientSelectionChanged;

    debug("PatientLink./show.SearchReports:").
        FloatNote(
            "Found " + Str(nMatchingPatientsFound) + " patients " + 
                sSearchError);
}
[...]