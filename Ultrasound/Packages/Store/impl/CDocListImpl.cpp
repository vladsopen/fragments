// CDocListImpl.cpp

#include "CUsmainProject.h"
#include "CDocListImpl.h"
#include "CDocImpl.h"
#include "CMatrixFormatGp.h"
#include "CSessionIface.h"
#include "CMediaDocImpl.h"
#include "CCalcupSystemIface.h"
#include "CInitTypeIface.h"
#include "CDocMetricsIfaceGp.h"
#include "CSendClusterIfaceGp.h"
#include "CCustomizeDocIfaceGp.h"
#include "CDocRegistryPtrImpl.h"
#include "CDocRegistryRefImpl.h"
#include "CBotStateIfaceGp.h"

CDocListImpl::CDocListImpl()
{
    x_bOwnDocs = false; // allocate storage controller
}

void CDocListImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CDocListImpl!");
}

int CDocListImpl::GetDocCount()
{
    return m_rpDocRegistryAbstract.Valid()->GetDocCount();
}

void CDocListImpl::AddDoc(
        ref<CDocImpl> value)
{
    m_rpDocRegistryAbstract.Valid()->AddDoc(value);

    CalcRecordedStats();

}

CDocListIface::DRecordingStats CDocListImpl::OnCalcRecordedStats() 
{
    ref<CDocImpl> rIterDoc;
    int nTotal = 0;
    int nExtra = 0;
    int nCluster0 = 0;
    int nCluster0Data = 0;
    num numSecondsMax = 0;
    num numSecondsRecorded = 0;
    for (
        iter i;
        this->IterateDocs(out i, out rIterDoc);)
    {
        if (rIterDoc->_x_rCustomizeDoc->x_bHideAllBars 
            ||
            rIterDoc->x_bDocLimitedPreview
            ||
            rIterDoc->_x_pDupomDoc)
        {
            nExtra++;
            continue;
        }

        nTotal++;

        if (rIterDoc->GetRawSlotCluster() == 0)
        {
            nCluster0++;

            if (rIterDoc->
                    GetDocMetrics()->
                        GetDocRecordedMatrixSliceCount() > 1)
            {
                nCluster0Data++;
            }

            numSecondsMax +=
                rIterDoc->
                    GetDocMetrics()->
                        ConvertDocAudioLengthFromCompositesToSeconds(
                            rIterDoc->
                                GetDocMetrics()->
                                    GetDocRecordableAudioLengthInComposites());

            numSecondsRecorded +=
                rIterDoc->
                    GetDocMetrics()->
                        ConvertDocViewWidthFromSlicesToSeconds(
                            rIterDoc->
                                GetDocMetrics()->
                                    GetDocRecordedMatrixSliceCount());
        }
    }

    str sStats = 
        Str(nCluster0Data) + "/" + Str(nCluster0) + 
            "..." + Str(nTotal) + "+" + Str(nExtra) + 
            ", " + 
            period::GFromBigMillisec(
                big(numSecondsRecorded * 1000))
                .ToStr(
                    "[yyyy years ][mm months ][dd days ]HH:MM:SS") + 
            " (^" + 
            period::GFromBigMillisec(
                big(numSecondsMax * 1000))
                .ToStr(
                    "[yyyy years ][mm months ][dd days ]HH:MM:SS") + 
            ")";

    SETBOT(CBotStateIfaceGp, _m_slotDocsStats, =, sStats); 
    //SETBOT(CBotStateIfaceGp, m_nDocsTotal, =, nTotal); 
    //SETBOT(CBotStateIfaceGp, m_nDocsCluster0, =, nCluster0); 
    //SETBOT(CBotStateIfaceGp, m_nDocsCluster0Data, =, nCluster0Data); 

    DRecordingStats dataStats = DRecordingStats();
    dataStats._m_numTotalSeconds = numSecondsRecorded;
    dataStats._m_nRecordings = nCluster0Data;
    return dataStats;
}

bool CDocListImpl::IterateDocs(out iter& out_i, out ref<CDocImpl>& out_value)
{
    return
        m_rpDocRegistryAbstract.Valid()->IterateDocs(out out_i, out out_value);
}

ref<CCalcupAsymmetryIfaceGp> CDocListImpl::OnGetCalcupAsymmetryRelatedToThisList()
{
    NOTEST;

    return x_rCalcupAsymmetry;
}

void CDocListImpl::OnHandleIndexSetupChanged()
{
    // update all docs
    ref<CDocImpl> rIterDoc;
    for (
        iter i;
        this->
            IterateDocs(
                out i,
                out rIterDoc);)
    {
        if (rIterDoc->_x_typeInitType->IsInSyncWithGlobalConfig())
        {
            rIterDoc->
                x_rCalcupSystem->
                    UpdateCalcupSystemAfterIndexSetupChange();
        }
    }
}

bool CDocListImpl::OnIterateMediaDocs(
        out iter& out_i,
        out ref<CMediaDocIfaceGp>& out_value)
{
    ref<CDocImpl> rIterDoc;
    if (IterateDocs(out out_i, out rIterDoc))
    {
        out_value = rIterDoc->x_rMediaDocCallback;
        return true;
    }
    else
    {
        return false;
    }
}

bool CDocListImpl::OnIterateRegisteredDocs(
        out iter& out_i,
        out ref<CDocIface>& out_rDoc)
{
    ref<CDocImpl> rIterDoc;
    if (IterateDocs(out out_i, out rIterDoc))
    {
        out_rDoc = rIterDoc;
        return true;
    }
    else
    {
        return false;
    }
}

void CDocListImpl::CloseNextDoc()
{
    // close next one
    ref<CDocImpl> rIterDoc;
    for (
        iter i;
        this->
            IterateDocs(
                out i,
                out rIterDoc);)
    {
        // skip internal docs
        if (!rIterDoc->_x_rCustomizeDoc->x_bHideAboveTimeBar &&
            !rIterDoc->_x_pDupomDoc)
        {
            rIterDoc->CloseDoc();

            break; // may have changed the array
        }
    }
}

void CDocListImpl::OnUpdateAllAfterDocListOrRawSlotsChange()
{
    _m_pUiDocListMain.DefaultIfNull()->InvalidateContent();

    // deferred update for extra list after all promary UIs have a chance to update
    _x_bFindAndShowInvisibleExtraDocOnIdle = true;
}

void CDocListImpl::OnUpdateDocListAfterDocHideShow()
{
    _m_pUiDocListMain.DefaultIfNull()->InvalidateContent();
}

void CDocListImpl::OnUpdateAllAfterPaletteChange()
{
    // update all docs
    ref<CDocImpl> rIterDoc;
    for (
        iter i;
        this->
            IterateDocs(
                out i,
                out rIterDoc);)
    {
        if (rIterDoc->_x_typeInitType->IsInSyncWithGlobalConfig())
        {
            rIterDoc->UpdateDocAfterPaletteChange();
        }
    }
}

void CDocListImpl::OnSyncVelocityUnitsToAllRecorderDocs(
        type<CVelocityUnitsIfaceGp> typeNewVelocityUnits)
{
    // update all docs
    ref<CDocImpl> rIterDoc;
    for (
        iter i;
        this->
            IterateDocs(
                out i,
                out rIterDoc);)
    {
        if (rIterDoc->_x_typeInitType->IsInSyncWithGlobalConfig())
        {
            rIterDoc->
                x_rCalcupSystem->
                    RotateCalcupSystemVelocityUnits(
                        typeNewVelocityUnits);
        }
    }
}

ptr<CDocIface> CDocListImpl::OnFindDocByPatientSampleKey(
        str sSampleKey)
{
    rASSERT(sSampleKey != "");
    ptr<CDocIface> pDocFound = null();
    ref<CDocImpl> rIterDoc;
    for (
        iter i;
        this->
            IterateDocs(
                out i,
                out rIterDoc);)
    {
        if (sSampleKey == rIterDoc->_x_sDocOpenAsPatientSampleKey)
        {
            rASSERT(pDocFound == null());
            pDocFound = rIterDoc;
        }
    }

    return pDocFound;
}

void CDocListImpl::StartUninitializedDocRegistration(
        ref<CDocImpl> rDoc)
{
    // Required Init
    rDoc->_x_pParentDocList = this;

    rASSERT(!m_rpDocRegistryAbstract.Valid()->IsDocRegistered(rDoc));

    AddDoc(rDoc);

    TESTLOG(
        "",
        "Doc List phase-1 registration: only link new doc ptr."
            " Total: " + Str(GetDocCount()) + " docs.\n");
}

void CDocListImpl::CompleteInitializedDocRegistration(
        ref<CDocImpl> rDoc)
{
    rASSERT(m_rpDocRegistryAbstract.Valid()->IsDocRegistered(rDoc));

    TESTLOG(
        "",
        "Doc List phase-2 registration: initiate global updates...\n");

    if (!rDoc->_x_pDupomDoc)
    {
        // Update device pointers
        _x_pParentSession.Get().Valid()->
            HandleDocListOrRawSlotsChange();
    }

    TESTLOG(
        "",
        "Doc List phase-2 registration complete.\n");
}

void CDocListImpl::UnregisterDoc(
        ref<CDocImpl> rDoc)
{
    if (m_rpDocRegistryAbstract.Valid()->ForgetDoc(rDoc))
    {
        TESTLOG(
            "",
            "Doc List closed a doc. " + 
                "Total left: " + Str(GetDocCount()) + " docs.\n");

        // Update device pointers
        _x_pParentSession.Get().Valid()->
            HandleDocListOrRawSlotsChange();
    }
    else
    {
        NOTEST;
        rFAIL("doc not found for close");
        TESTLOG(
            "",
            "Doc List FAILED to find a doc to close. " + 
                "Total still left: " + Str(GetDocCount()) + " docs.\n");
    }
}

ref<UiContainer> CDocListImpl::OnNewUiDocListExtra()
{
    ref<UiDocListExtra> rUi;

    rUi->m_rDocList = this;

    return rUi;
}

ref<UiContainer> CDocListImpl::OnOpenUiDocListExtraFloater()
{
    ref<UiContainer> rUiFrame = NewUiDocListExtra();

    rUiFrame->CreateFloater();

    // Default pos (debug!)
    rUiFrame->thispanel()->
        SetInstantPos(
            sys::GGetPrimaryMonitorResolution().w - 200,
            40);

    return rUiFrame;
}

void CDocListImpl::Onx_nPrintMediaColumns_Set(
        int value)
{
    if (this->x_nPrintMediaColumns == value)
    {
        return;
    }

    this->x_nPrintMediaColumns = value;

    _m_pUiDocListMain.DefaultIfNull()->InvalidateContent();
}

void CDocListImpl::Onx_bOwnDocs_Set(
        bool value)
{
    rASSERT(
        !m_rpDocRegistryAbstract 
        ||
        m_rpDocRegistryAbstract.Valid()->GetDocCount() == 0 
        || 
        value == x_bOwnDocs);

    if (value != x_bOwnDocs ||
        !m_rpDocRegistryAbstract)
    {
        //m_bRegistryInitOk = true;

        this->x_bOwnDocs = value;

        if (x_bOwnDocs)
        {
            m_rpDocRegistryAbstract = ref<CDocRegistryRefImpl>();
        }
        else
        {
            m_rpDocRegistryAbstract = ref<CDocRegistryPtrImpl>()
        }
    }
}

ref<UiContainer> CDocListImpl::OnNewUiDocListMain()
{
    ref<UiDocListMain> rUi;

    rUi->m_rDocList = this;

    return rUi;
}

