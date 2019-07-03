// CFilterImpl.cpp

#include "CProject.h"
#include "CFilterImpl.h"
#include "CBandPassFilterIfaceGp.h"
#include "CRealSignal.h"
#include "CRecordTypeIface.h"
#include "CDbReadingIface.h"
#include "CDbConfigIface.h"
#include "CCalcupIface.h"
#include "CSessionIface.h"
#include "COptionsIface.h"
#include "CRecorderIface.h"
#include "CIncrementalFilter.h"
#include "CReadingIface.h"

CFilterImpl::CFilterImpl()
{
    _init_AttachToFilterImpl();
}

void CFilterImpl::OnFilterOfflineReadingCache(
        rowset<CDbReadingIface> rowsetRawReading,
        out rowset<CDbReadingIface>& out_rowsetFilteredReading)
{
    //if (!debug("Calc.Filter.FullOffline?").InputBool())
    if (debug("Calc.Filter.FullDisable?").InputBool())
    {
        return;
    }

    if (out_rowsetFilteredReading.GetRowCount() > 0
        &&
        !debug("Calc.Filter.Force?").InputBool()
        &&
        !m_bFilterCacheInvalidated)
    {
        return;
    }

    out_rowsetFilteredReading = null();

    // have input?
    if (rowsetRawReading.GetRowCount() < 2)
    {
        return;
    }

    m_bFilterCacheInvalidated = false;

    //
    // Get config
    //

    ref<CDbConfigIface> rDbConfig = 
        GetDbConfigForFilter();
    if (!rDbConfig->x_bBandFilterEnabled)
    {
        // filter is off
        out_rowsetFilteredReading = rowsetRawReading;
        return;
    }

    num numLowHz = OO;
    num numHighHz = OO;
    GetSafeNormalizedHighAndLowHz(
        out numLowHz,
        out numHighHz);

    //
    // Filter all signals
    //

    // Make sure all type<>s represented as our items
    type<CRecordTypeIface> typeIterRecordType;
    for (
        iter i;
        type<CRecordTypeIface>()->GetDerivedList().
            Iterate(out i, out typeIterRecordType);)
    {
        //
        // Convert filter input
        //

        ref<CRealSignal> rRealInputX;
        ref<CRealSignal> rRealInputY;
        GConvertRowsetToRealSignal(
            rowsetRawReading,
            typeIterRecordType,
            rRealInputX,
            rRealInputY);

        int nDuplicateAtEdges = 
            debug("Calc.Filter.Duplicate?").InputIntOrSet(1);
        if (nDuplicateAtEdges == 1)
        {
            // "1" means FULL signal double copy
            nDuplicateAtEdges = rRealInputX->GetCount();
        }

        nDuplicateAtEdges = 
            math::GMin(nDuplicateAtEdges, rRealInputX->GetCount());

        rRealInputX = 
            GDuplicateAtEdges(
                nDuplicateAtEdges,
                rRealInputX);
        rRealInputY = 
            GDuplicateAtEdges(
                nDuplicateAtEdges,
                rRealInputY);

        // we may not have data of a certain type
        if (rRealInputX->GetCount() == 0)
        {
            rASSERT(rRealInputY->GetCount() == 0);
            continue;
        }
        rASSERT(rRealInputY->GetCount() != 0);

        //
        // Init filter
        //

        ref<CBandPassFilterIfaceGp> rBandPassFilter;

        rBandPassFilter->
            InitFilter(
                C_nSamplesPerSecond,
                numLowHz, //debug("Calc.Filter.HzLowCut?").InputNumOrSet(1.0),
                numHighHz, //debug("Calc.Filter.HzHighCut?").InputNumOrSet(10.0),
                debug("Calc.Filter.AmpOne?").
                    InputNumOrSet(CFilterImpl::C_numFilterOne),
                debug("Calc.Filter.AmpZero?").
                    InputNumOrSet(CFilterImpl::C_numFilterZero),
                debug("Calc.Filter.Order?").
                    InputIntOrSet(CFilterImpl::C_nFilterOrder));

        //
        // Run filter
        //

        str sError;
        ref<CRealSignal> rRealOutputX =
            rBandPassFilter->
                FilterSignalBatch(
                    rRealInputX,
                    out sError);
        ref<CRealSignal> rRealOutputY;
        if (sError == "")
        {
            rRealOutputY =
                rBandPassFilter->
                    FilterSignalBatch(
                        rRealInputY,
                        out sError);
        }
        rASSERTM(sError == "", sError);
        if (sError != "")
        {
            return;
        }

        //
        // Convert filter output
        //

        GConvertRealSignalToRowset(
            nDuplicateAtEdges,
            rowsetRawReading,
            rRealOutputX,
            rRealOutputY,
            typeIterRecordType,
            out out_rowsetFilteredReading);
    }
}

bool CFilterImpl::OnFilterOnlineIncremental(
        ref<CReadingIface> rReading,
        out ref<CReadingIface>& out_rFilteredReading)
{
    if (debug("Calc.Filter.DisableIncremental?").InputBool())
    {
        return false;
    }

    if (!m_rIncrementalFilterX->
            IsFilterSettingsMatchContext(
                this))
    {
        ResetFilterForNewRecording();
    }

    // access db objects
    ref<CDbReadingIface> rInDbReading = 
        rReading->x_rDbReading;
    ref<CDbReadingIface> rOutDbReading = 
        out_rFilteredReading->x_rDbReading;

    // duplicate unfiltered data
    rInDbReading->
        DuplicateDbReadingTo(
            rOutDbReading);

    // save filtered X
    rOutDbReading->x_numXmm = 
        m_rIncrementalFilterX->
            FilterIncrementally(
                rInDbReading->x_numXmm,
                this);

    // save filtered Y
    rOutDbReading->x_numYmm = 
        m_rIncrementalFilterY->
            FilterIncrementally(
                rInDbReading->x_numYmm,
                this);

    return true;
}

void CFilterImpl::HandleFilterSetupChange(
        ref<CDbConfigIface> rDbConfig)
{
    ref<CSessionIface> rSession = 
        _x_pSession.Get().Valid();
    ref<CCalcupIface> rCalcup = 
        rSession->x_rCalcup;
    ref<CRecorderIface> rRecorder = 
        rSession->x_rRecorder;

    // write config to db
    rDbConfig->
        CommitRef();
    
    // invalidate signal cache
    InvalidateFilterCache();

    // update math/graphs
    rRecorder->
        ResetAllScaling();
    rCalcup->
        UpdateCalcup();

    // update UI 
    _m_ptrmapUiFilterExperimenter.InvalidateTreeState();
    _m_ptrmapUiFilterSetup.InvalidateTreeState();
}

[...]