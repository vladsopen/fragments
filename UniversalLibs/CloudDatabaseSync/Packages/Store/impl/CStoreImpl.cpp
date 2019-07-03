// CStoreImpl.cpp

#include "CProject.h"
#include "CStoreImpl.h"
#include "CLogTypeIface.h"
#include "CUdbStorageTypeIfaceGp.h"
#include "CDbRoot.h"
#include "CRootUdbCallback.h"
//#include "CQueryIfaceGp.h"
#include "CDbAction.h"
#include "CActionIface.h"
#include "VirtOs_CInterMem.h"
#include "CCacheAction.h"
#include "CSyncOutIface.h"
#include "CSessionIface.h"
#include "CNetIface.h"
#include "CSyncInIface.h"
#include "CAddonMapIface.h"
#include "CActionImpl.h"
#include "CSyncTypeImpl.h"
#include "CUdbInitConnectIfaceGp.h"
#include "CRecentValueIfaceGp.h"
#include "CBasicClassIface.h"

bool CStoreImpl::OnIterateActionsForNode(
        key keyNode,
        out iter& out_i,
        out ref<CActionIfaceGp>& out_rAction)
{
    rASSERT(m_bStoreOpened);

    ref<CDbRoot> rDbRoot = QueryDbRoot();

    ref<CQueryIfaceGp> rQuery = 
        out_i.AdvanceQuery();

    from<CDbAction> fromAction(rQuery);

    if (rQuery->Init())
    {
        fromAction.
            BeginFrom(
                rDbRoot->
                    _x_dblistAction,
                key());

        rQuery->x_qxboolWhere = 
            Qx
                fromAction->x_keyOnNode 
                    == 
                    keyNode;

        rQuery->x_qxsortOrderBy = 
            Qx
                fromAction->x_utcUpdate
                    ORDER(E_Order_Ascending),
                fromAction->x_nnLocalUpdateCounter
                    ORDER(E_Order_Ascending);
    }

    bool bOk = rQuery->Next();
    if (bOk)
    {
        ref<CActionImpl> rActionImpl;
        rActionImpl->x_rDbAction = fromAction.FetchRef();
        out_rAction = rActionImpl;
    }

    return bOk;
}

bool CStoreImpl::OnQueryLastTraitForNode(
        key keyNode,
        key keyTrait,
        key keyOnlyForTie,
        out ref<CActionIfaceGp>& out_rAction)
{
    rASSERT(m_bStoreOpened);

    ref<CDbRoot> rDbRoot = QueryDbRoot();

    ref<CQueryIfaceGp> rQuery; 

    from<CDbAction> fromAction(rQuery);

    if (rQuery->Init())
    {
        fromAction.
            BeginFrom(
                rDbRoot->
                    _x_dblistAction,
                key());

        qxbool qxboolWhere = 
            Qx
                fromAction->x_keyOnNode == keyNode
                &&
                fromAction->x_keyTrait == keyTrait;

        if (keyOnlyForTie != key())
        {
            qxboolWhere = 
                Qx
                    (qxboolWhere)
                    &&
                    fromAction->x_keyTie == keyOnlyForTie;
        }

        rQuery->x_qxboolWhere = 
            qxboolWhere;

        rQuery->x_qxsortOrderBy = 
            Qx
                fromAction->x_utcUpdate
                    ORDER(E_Order_Descending),
                fromAction->x_nnLocalUpdateCounter
                    ORDER(E_Order_Descending);
    }

    if (rQuery->Next())
    {
        // init encapsulator
        ref<CActionImpl> rActionImpl;
        rActionImpl->x_rDbAction = fromAction.FetchRef();
        out_rAction = rActionImpl;
        return true;
    }
    else
    {
        return false;
    }
}

void CStoreImpl::OnDumpJournal()
{
    str sDump;

    ref<CActionIface> rIterActionIface;
    utc utcPrev;
    for (
        iter i;
        QueryActions(
            out i,
            out rIterActionIface);)
    {
[...]