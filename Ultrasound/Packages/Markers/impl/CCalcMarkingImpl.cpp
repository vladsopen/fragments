// CCalcMarkingImpl.cpp

#include "CUsmainProject.h"
#include "CCalcMarkingImpl.h"
#include "CCalcModeIface.h"
#include "CCalcupSystemIface.h"
#include "CUsoundCalcModeIfaceGp.h"
#include "CCalcLineList.h"
#include "CCalcMachineIfaceGp.h"
#include "CCalcMarkingConfigImpl.h"
#include "CGlobalConfigIface.h"
#include "CCalcVariableListIfaceGp.h"
#include "CUsoundIndexTypeIfaceGp.h"
#include "CCalcVariableIfaceGp.h"
#include "CCalcLineState.h"
#include "CCalcLineType.h"
#include "CDocIface.h"
#include "CStoreIface.h"
#include "CCurveTypeIface.h"
#include "CImageIface.h"
#include "CMatrixSliceIface.h"
#include "CCurveSliceIface.h"
#include "CMatrixFormatGp.h"
#include "CMediaDocIfaceGp.h"

//NOTEST; //todo

CCalcMarkingImpl::CCalcMarkingImpl()
{
    _init_AttachProperties();
}

void CCalcMarkingImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachProperties();

    //rFAIL("make sure it is OK to expose CCalcMarkingImpl!");
}

void CCalcMarkingImpl::_init_AttachProperties()
{
    _x_rCalcLineList->_x_pParentCalcMarking = this;
}

void CCalcMarkingImpl::OnPrepareCalcMarkingForCalcModeWuu(
        type<CUsoundCalcModeIfaceGp> typeUsoundCalcMode)
{
    // copy all indexes to markers
    ref<CCalcLineState> rCalcLineState;
    for (
        iter i;
        _x_rCalcLineList->
            IterateLinesInPaintOrder(
                out i, 
                out rCalcLineState);)
    {
        rCalcLineState->
            PrepareCalcLineForCalcModeWuu(
                typeUsoundCalcMode);
    }

    // update UI
    _m_pUiCalcMarkingMain.DefaultIfNull()->InvalidateContent();
}

num CCalcMarkingImpl::OnGetCalcMakerPos(
        ECalcMarkingLine eCalcMarkingLine,
        type<CUsoundAxisIfaceGp> rUsoundAxis)
{
    rUsoundAxis;

    type<CCalcLineType> typeCalcLineType = 
        type<CCalcLineType>()->
            FromCalcLineSharedEnum(eCalcMarkingLine);

    ref<CCalcLineState> rCalcLineState = 
        _x_rCalcLineList->
            LookupByKey(
                typeCalcLineType).Valid();
    
    return //10;
        rCalcLineState->x_numMarkerPosInDib; 
}

num CCalcMarkingImpl::OnGetOriginalCalcMakerIndexValueToRestoreOrOo(
        ECalcMarkingLine eCalcMarkingLine,
        type<CUsoundAxisIfaceGp> rUsoundAxis)
{
    rUsoundAxis;

    type<CCalcLineType> typeCalcLineType = 
        type<CCalcLineType>()->
            FromCalcLineSharedEnum(eCalcMarkingLine);

    ref<CCalcLineState> rCalcLineState = 
        _x_rCalcLineList->
            LookupByKey(
                typeCalcLineType).Valid();

    return
        rCalcLineState->GetRestoreOriginalIndexValueRoundedOrOo(); 
}

ref<UiContainer> CCalcMarkingImpl::OnNewUiCalcMarkingMainOverlay()
{
    ref<UiCalcMarkingMain> rUi;

    rUi->m_rCalcMarking = this;

    return rUi;
}

void CCalcMarkingImpl::OpenFormulaEditor()
{
    // Delayed load
    PRELOAD_PROJECT(Calc);

    // config
    ref<CCalcMarkingConfigImpl> rCalcMarkingConfig = 
        GetGlobalCalcMarkingConfig();
    ref<CCalcMachineIfaceGp> rCalcMachine = 
        rCalcMarkingConfig->GetLoadedCalcMachine();

    ACT(
        TRANSLATE(
            "Preparing [... UI-string-removed]");

    // add indexes in *FIXED* order
    type<CUsoundIndexTypeIfaceGp> typeIterUsoundIndexType;
    for (
        iter i;
        type<CUsoundIndexTypeIfaceGp>()->
            GetUsoundIndexTypeList().
                Iterate(
                    out i,
                    out typeIterUsoundIndexType);)
    {
        // we cant calc asym yet
        if (typeIterUsoundIndexType->IsAsymmetryCompareIndex())
        {
            continue;
        }

        DefineCalcVariableFromUsoundIndex(
            typeIterUsoundIndexType,
            rCalcMachine);
    }

    /*
    // Special values
    DefineCalcVariable(
        "Diameter",
        "0", //todo
        rCalcMachine);
        */

    // Open UI
    rCalcMachine->OpenCalcDialog();
}

void CCalcMarkingImpl::DefineCalcVariableFromUsoundIndex(
        type<CUsoundIndexTypeIfaceGp> typeUsoundIndexType,
        ref<CCalcMachineIfaceGp> rCalcMachine)
{
    str sSetName = typeUsoundIndexType->GetFixedIndexName();
    str sSetFormula = typeUsoundIndexType->GetFixedIndexFormula();

    // Set value instead of an empty formula
    if (sSetFormula == "")
    {
        // parents
        ref<CCalcupSystemIface> rCalcupSystem = 
            _x_pParentCalcupSystem.Get().Valid();
        ref<CDocIface> rDoc = 
            rCalcupSystem->_x_pParentDoc.Get().Valid();

        // index value
        num numIndexValue = 
            rCalcupSystem->
                GetIndexValueRoundedInUiUnits(
                    typeUsoundIndexType);

        sSetFormula = Str(numIndexValue);
    }

    DefineCalcVariable(
        sSetName,
        sSetFormula,
        rCalcMachine);
}

void CCalcMarkingImpl::DefineCalcVariable(
        str sName,
        str sFormula,
        ref<CCalcMachineIfaceGp> rCalcMachine)
{
    ref<CCalcVariableListIfaceGp> rCalcVariableList = 
        rCalcMachine->GetCalcVariableList();
    
    ref<CCalcVariableIfaceGp> rDefineCalcVariable;

    if not_null(
        ref<CCalcVariableIfaceGp> rExistingCalcVariable = 
            rCalcVariableList->LookupUniqueCalcVariableByName(sName))
    {
        // already present
        rDefineCalcVariable = 
            rExistingCalcVariable;
    }
    else
    {
        // add anew
        rDefineCalcVariable = 
            rCalcVariableList->AppendNewCalcVariable();
    }

    rDefineCalcVariable->
        ChangeCalcVariableNameFromApp(sName);
    rDefineCalcVariable->
        ChangeCalcVariableFormulaFromApp(sFormula);
}

ref<CCalcMarkingConfigImpl> CCalcMarkingImpl::GetGlobalCalcMarkingConfig()
{
    // parents
    ref<CCalcupSystemIface> rCalcupSystem = 
        _x_pParentCalcupSystem.Get().Valid();
    ref<CDocIface> rDoc = 
        rCalcupSystem->_x_pParentDoc.Get().Valid();

    return
        rDoc->GetConfigForDoc()->x_rCalcMarkingConfig;
}

res CCalcMarkingImpl::GetFormulaEditorLargeIcon()
{
    return RES_FORMULA_EDITOR_L();
    /* too slow for just a button icon visible always in manual calc

    // Delayed load
    PRELOAD_PROJECT(Calc);

    return ref<CCalcMachineIfaceGp>()->GetCalcLargeIcon();
    */
}

void CCalcMarkingImpl::UpdateMeanVelocity()
{
    // parents
    ref<CCalcLineList> rList = 
        _x_rCalcLineList;
    ref<CCalcMarkingImpl> rCalcMarking = 
        this;
    ref<CCalcupSystemIface> rCalcupSystem = 
        _x_pParentCalcupSystem.Get().Valid();
    ref<CDocIface> rDoc = 
        rCalcupSystem->_x_pParentDoc.Get().Valid();
    ref<CStoreIface> rStore = 
        rDoc->x_rStore;
    ref<CImageIface> rImage = 
        rDoc->x_rImage;
    ref<CMediaDocIfaceGp> rMediaDoc = 
        rDoc->GetMediaDocForCalcIndex();

    int nDibMatrixHeight = 
        rDoc->x_rMatrixFormat->x_nMatrixLength;

    num numDiastole =
        rList->
            LookupByKey(
                type<CCalcLineTypeDiastolicVelocity>()).
                Valid("missing diastole marker!")->
                x_numMarkerPosInDib;

    if (math::GIsNumUnreal(numDiastole))
    {
        return;
    }

    num numMean = OO; //debug("Indexes.MeanIs?").InputNum();

    int nT1 = (int)
        // E_CalcMarkingLine_LeftTime1,
        rList->
            LookupByKey(
                type<CCalcLineTypeTimeAtSystole>()).
                Valid("missing T1 marker!")->
                x_numMarkerPosInDib;

    int nT2 = (int)
        // E_CalcMarkingLine_RightTime2,
        rList->
            LookupByKey(
                type<CCalcLineTypeTimeAtNextSystole>()).
                Valid("missing T2 marker!")->
                x_numMarkerPosInDib;

    int iFlowInStorage = 
        type<CCurveTypeIface>()->
            AsCurveTypeForFlowAbove()->
                GetCurveIndexInStorage();

    //int nStoredSlices = 
    //    rStore->
    //        GetStoreRipeMatrixSliceCount();

    int iFlowScroll = 
        rImage->GetViewScrollInMatrixSlices();

    //int nView = 
    //    rImage->GetViewWidthInMatrixSlices();

    draw dcDebug;
    if (debug::GIsAllowed...())
    {
        dcDebug.MapToScreenDc();
    }

    dcDebug.SelectStockObject(NULL_BRUSH);

    int nDebugX = 0;
    int nDebugY = 0;
    int nDebugX0 = off;
    int nZeroY = 1000;

    bool bFirstPoint = true;

    //sync:_calc_M_
    num numMeanSum = 0;
    int nSummedPoints = 0;

    if (nT2 > nT1)
    for (
        int nTimeAt = nT1;
        nTimeAt < nT2;
        nTimeAt++)
    {
        // Load slice data from storage
        ref<CMatrixSliceIface> rMatrixSlice = 
            rStore->
                GetMatrixSliceAt(
                    iFlowScroll + nTimeAt,
                    ptr<CMatrixSliceIface>());

        if (rMatrixSlice->GetMatrixSlicePointCount() == 0)
        {
            continue;
        }
                
        // get the curve data
        ref<CCurveSliceIface> rCurveSlice = 
            rMatrixSlice->x_rCurveSlice;

        // get the amplitude scaled in our units
        int nFlowInDib = 
            rCurveSlice->
                GetCurvePointAmplitudeAt(
                    iFlowInStorage,
                    nDibMatrixHeight);

        rASSERT(nFlowInDib >= 0 && nFlowInDib <= 255);

        //if (iFlowAt >= 0 && 
        //    iFlowAt < rPainter->GetArhWidth())
        {
            num numFlow =
                rMediaDoc->[...]

            //sync: M += (y[i]-D);
            numMeanSum += 
                numFlow - numDiastole;
            nSummedPoints++;

            const bool bDrawBack = false;

            dcDebug.SetPen(
                PS_SOLID, 
                bDrawBack? 30 : 1, 
                bDrawBack? rgb(255, 255, 255) : rgb(255, 50, 255));

            nDebugX = nTimeAt;
            nDebugY = nZeroY - (int) numFlow;

            if (bFirstPoint)
            {
                bFirstPoint = false;
                nDebugX0 = nDebugX;

                dcDebug.
                    MoveTo(
                        nDebugX - nDebugX0,
                        nDebugY);
            }

            dcDebug.
                LineTo(
                    nDebugX - nDebugX0,
                    nDebugY);
        }
    }

    // M = math::GDivNot0(M, (num) (ts2 - ts1), or_return 1.);
    // M += D;
    // if (M == 0) M = 0.01;

    if (nSummedPoints > 0)
    {
        numMean = 
            math::GDiv0(
                numMeanSum,
                (num) nSummedPoints, 
                or_return 0.001);

        numMean += numDiastole;
    }

    // end marker
    dcDebug.SetPen(
        PS_SOLID, 
        10, 
        rgb(255, 50, 255));
    dcDebug.
        LineTo( 
            nDebugX - nDebugX0,
            nDebugY - 100);
    dcDebug.
        LineTo(
            nDebugX - nDebugX0,
            nDebugY + 100);
    dcDebug.
        OutText(
            nDebugX - nDebugX0,
            nDebugY,
            "Curve[T1-T2] M = " + Str(numMean));

    // diastole line
    dcDebug.SetPen(
        PS_SOLID, 
        1, 
        rgb(0, 0, 255));
    dcDebug.
        Line( 
            nDebugX - nDebugX0,
            nZeroY - (int) numDiastole,
            0,
            draw::equal);

    // mean line
    dcDebug.SetPen(
        PS_SOLID, 
        1, 
        rgb(255, 0, 0));
    dcDebug.
        Line( 
            nDebugX - nDebugX0,
            nZeroY - (int) numMean,
            0,
            draw::equal);

    //
    // Force calculated position
    //

    if (//m_calc_current != E_CalcMarkingLine_MeanVelocity &&
            // checked by the caller
        numMean != OO)
    {
        rList->
            LookupByKey(
                type<CCalcLineTypeFlowMeanVelocity>()).
                Valid("missing M marker!")->
                x_numMarkerPosInDib 
            =
            rMediaDoc->
                [...]
    }
}

