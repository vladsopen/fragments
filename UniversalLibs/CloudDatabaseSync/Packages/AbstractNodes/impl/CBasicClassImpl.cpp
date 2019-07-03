// CBasicClassImpl.cpp

#include "CProject.h"
#include "CBasicClassImpl.h"
#include "CDbNode.h"
#include "CDbGlobalNodes.h"
#include "CActionIfaceGp.h"
//#include "DeclareKeys.h"
#include "CBasicNodeImpl.h"
#include "CStoreIfaceGp.h"
#include "CNodeSubclasses.h"
#include "CNodeBasicCacheAction.h"
#include "CUbroStoreIface.h"
#include "CUbroSessionIfaceGp.h"

#include "CDbRecentTraits.h"
#include "CFilterIface.h"
#include "CRecentValueImpl.h"
#include "CActionIface.h"
#include "CUbroRefineIfaceGp.h"
#include "CUbroRefineFilterIfaceGp.h"

queryorcount CBasicClassImpl::OnQueryBasicNodesInFilter(
        out iter& out_i,
        out ref<CBasicNodeIfaceGp>& out_rBasicNode,
        key keyOptionalPrimary,
        ref<CFilterIfaceGp> rFilter,
        ref<CUbroRefineIfaceGp> rUbroRefine)
{
    ref<CFilterIface> rFilterIface = rFilter;

    ref<CStoreIfaceGp> rStore = 
        GetCoreStore();
    ref<CDbGlobalNodes> rDbGlobal = 
        QueryGlobalNodes();
    ref<CQueryIfaceGp> rQuery = 
        out_i.AdvanceQuery();
    ptr<CUdbConnectionIfaceGp> pUdbConnection = 
        //rDbGlobal->GetUdbConnectionPtr_AMBIGUOUS_().Valid(); 
            // LL SQLite!!!
        rStore->GetTablesHighLevelUdbConnection();
            // confuses CQuery already running with LL connection

    from<CDbNode> fromNode(rQuery);
    tryfrom<CDbRecentTraits> tryfromRecentTraitsFilter(rQuery);
    tryfrom<CDbRecentTraits> tryfromRecentTraitsRefine(rQuery);
    tryfrom<CDbRecentTraits> tryfromRecentTraitsSort(rQuery);
    from<dbobject> fromFilter(rQuery, rFilterIface->NewDbTableResult());

    if (rQuery->Init())
    {
        fromNode.
            BeginFrom(
                rDbGlobal->
                    _x_dblistNode,
                key());

        // kill duplicate nodes which have many trait updates
        //rQuery->x_bSelectDistinct = true;

        // we need recent trait records for each node key
        if (!rFilter->IsNullFilter())
        {
            tryfromRecentTraitsFilter.
                JoinLeft(
                    rDbGlobal->
                        _x_dblistRecentTraits,
                    key(),
                    (
                        Qx
                            tryfromRecentTraitsFilter->x_keyNodeUpdated 
                                == 
                                fromNode->GetKey()
                    ),
                    // make DISTINCT apply only to nodes
                    E_UdbSelectResult_Exclude
                    );
        }
        else
        {
            tryfromRecentTraitsFilter.
                SkipTable();
        }

        // separate attribute selection for the refiner
        if not_null(
            ref<CUbroRefineFilterIfaceGp> rUbroRefineFilter = 
                rUbroRefine->x_rUbroRefineFilter)
        {
            tryfromRecentTraitsRefine.
                JoinLeft(
                    rDbGlobal->
                        _x_dblistRecentTraits,
                    key(),
                    (
                        Qx
                            tryfromRecentTraitsRefine->x_keyNodeUpdated 
                                == 
                                fromNode->GetKey()
                    ),
                    E_UdbSelectResult_Exclude
                    );
        }
        else
        {
            tryfromRecentTraitsRefine.
                SkipTable();
        }

        // all properties for sorting
        tryfromRecentTraitsSort.
            // we want nodes without any updates included also
            JoinLeft(
                rDbGlobal->
                    _x_dblistRecentTraits,
                key(),
                (
                    Qx
                        tryfromRecentTraitsSort->x_keyNodeUpdated 
                            == 
                            fromNode->GetKey()
                ),
                // make DISTINCT apply only to nodes
                E_UdbSelectResult_Exclude
                );

        rFilterIface->
            AddFilterJoins(
                out fromFilter,
                rQuery,
                pUdbConnection,
                Qx
                    fromFilter->GetKey()
                        == 
                        fromNode->GetKey());

        qxbool qxboolMatchFilter;
        if (!rFilter->IsNullFilter())
        {
            qxboolMatchFilter =
                rFilterIface->
                    PrepareFilterMatchingQx(
                        //TODO: Tie filter?
                        (Qx
                            tryfromRecentTraitsFilter->x_keyTraitUpdated),
                        (Qx
                            tryfromRecentTraitsFilter->x_sUpdatedScalar),
                        (Qx
                            tryfromRecentTraitsFilter->x_keyUpdatedNonscalar));
        }
        else
        {
            qxboolMatchFilter =
                Qx
                    true;
        }

        qxbool qxboolMatchRefine;
        if not_null(
            ref<CUbroRefineFilterIfaceGp> rUbroRefineFilter = 
                rUbroRefine->x_rUbroRefineFilter)
        {
            qxboolMatchRefine =
                rUbroRefineFilter->
                    PrepareFilterRecentTraitsQx(
                        (Qx
                            tryfromRecentTraitsRefine->x_keyTraitUpdated),
                        (Qx
                            tryfromRecentTraitsRefine->x_sUpdatedScalar),
                        (Qx
                            tryfromRecentTraitsRefine->x_keyUpdatedNonscalar));
        }
        else
        {
            qxboolMatchRefine =
                Qx
                    true;
        }

        //str sOnlyTableClass = "PatDbPatient";

        rQuery->x_qxboolWhere = 
            //qxboolMatchFilter;
            Qx
                (qxboolMatchFilter)
                &&
                (qxboolMatchRefine);
                /*
                    &&
                    STRING(tryfromRecentTraits->x_keyTraitUpdated) == 
                        sOnlyTableClass;
                */

        qxsort qxsortFromFilter;
        qxsortFromFilter.
            _udb_SetSql(
                rFilterIface->GetValidFilterSortSql());
                //" fromFilter.sBupWritFlashReport DESC ");

        // replace DISTINCT 
        //   and make aggregates MIN/MAX available in ORDER BY
        rQuery->x_qxmanyGroupBy = 
            Qx
                fromNode->GetKey(), true;

        //TODO: get from filter
        rQuery->x_qxsortOrderBy = 
            Qx
                qxsortFromFilter,
                fromNode->x_utcNodeLastChange 
                    ORDER(E_Order_Descending);
    }

    if (rQuery->Next())
    {
        // init encapsulator
        out_rBasicNode = 
            ref<CBasicNodeImpl>()->
                NewBasicNodeFromQuery(
                    fromNode.FetchRef(),
                    this);
    }

    return rQuery;
}

queryorcount CBasicClassImpl::QueryRecentTraitUpdateRecord(
        out iter& out_i,
        out ref<CDbRecentTraits>& out_rDbRecentTraits,
        key keyNode,
        key keyTrait,
        key keyOnlyForTie)
{
    ref<CDbGlobalNodes> rDbGlobal = 
        QueryGlobalNodes();
    //ref<CUdbConnectionIfaceGp> rUdbConnection = 
    //    rDbGlobal->GetUdbConnectionPtr().Valid();

    ref<CQueryIfaceGp> rQuery = out_i.AdvanceQuery();

    from<CDbRecentTraits> fromRecentTraits(rQuery);

    if (rQuery->Init())
    {
        fromRecentTraits.
            BeginFrom(
                rDbGlobal->
                    _x_dblistRecentTraits,
                key());

        //qxbool qxboolWhere = 
        //    Qx
        //        fromRecentTraits->x_keyNodeUpdated == keyNode
        //        &&
        //        fromRecentTraits->x_keyTraitUpdated == keyTrait;

        qxbool qxboolWhere = 
            Qx
                fromRecentTraits->x_keyNodeUpdated == keyNode;

        // allow unfiltered ALL traits for a node
        if (keyTrait != key())
        {
            qxboolWhere = 
                Qx
                    (qxboolWhere)
                    &&
                    fromRecentTraits->x_keyTraitUpdated == keyTrait;
        }

        if (keyOnlyForTie != key())
        {
            qxboolWhere = 
                Qx
                    (qxboolWhere)
                    &&
                    fromRecentTraits->x_keyTieUpdated == keyOnlyForTie;
        }

        rQuery->x_qxboolWhere = 
            qxboolWhere;
    }

    if (rQuery->Next())
    {
        out_rDbRecentTraits = fromRecentTraits.FetchRef();
        return true;
    }
    else
    {
        return false;
    }
}

queryorcount CBasicClassImpl::QueryBasicNodesOrTraits(
        ptr<CAbstractClassIfaceGp> pDerivedAbstractClass,
        out iter& out_i,
        out ref<CBasicNodeIfaceGp>& out_rBasicNode,
        key keyAt)
{
    ref<CDbGlobalNodes> rDbGlobal = 
        QueryGlobalNodes();
    ref<CQueryIfaceGp> rQuery = 
        out_i.AdvanceQuery();
    //ref<CUdbConnectionIfaceGp> rUdbConnection = 
    //    rDbGlobal->GetUdbConnectionPtr().Valid();

    from<CDbNode> fromNode(rQuery);

    if (rQuery->Init())
    {
        fromNode.
            BeginFrom(
                rDbGlobal->
                    _x_dblistNode,
                keyAt);

        // Create (class || class || class ....) query condition
        qxbool qxboolClass = Qx(false);

        ptr<CAbstractClassIfaceGp> pIterSession = 
            pDerivedAbstractClass;
        bool bClassConditionsApply = false;
        repeat(i, C_nMaxNodeClassDerivedDepth)
        {
            rASSERT(i < C_nMaxNodeClassDerivedDepth - 1);

            if not_null(
                ref<CAbstractClassIfaceGp> rSession = 
                    pIterSession)
            {
                str sClass = rSession->GetNodeClassId();
                rASSERT(sClass != "");

                bClassConditionsApply = true;
[...]