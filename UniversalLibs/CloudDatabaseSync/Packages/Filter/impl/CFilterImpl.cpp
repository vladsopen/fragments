// CFilterImpl.cpp

#include "CProject.h"
#include "CFilterImpl.h"
#include "CDbFilter.h"
#include "CFilterSystemImpl.h"
#include "CSessionIface.h"
#include "CBasicClassIface.h"
#include "CBasicNodeIfaceGp.h"
#include "CStoreIfaceGp.h"
#include "CAddonMapIface.h"
#include "CTableClassIface.h"
#include "CColumnOrder.h"

qxbool CFilterImpl::OnPrepareFilterMatchingQx(
        qxkey qxkeyTrait,
        qxstring qxstringMatchScalar,
        qxkey qxkeyMatchNonscalar)
{
    key keyFilterTrait = x_rDbFilter->x_keyFilterProperty;
    str sMatch = x_rDbFilter->x_sFilterMatch;
    key keyMatch = key::GFromStr(x_rDbFilter->x_sFilterMatch);
    str sLike = x_rDbFilter->x_sFilterLike;

    str sKeyFilterTrait = keyFilterTrait._internal_ConvertToKeyStorage();
        // TEST!

    qxbool qxboolAnd = 
        Qx
            true;

    if (sLike != "")
    {
        qxboolAnd = 
            Qx
                (qxstringMatchScalar) LIKE sLike;
    }

    if (sMatch != "")
    {
        qxboolAnd = 
            Qx
                (qxboolAnd)
                &&
                (
                    (
                        (qxstringMatchScalar)
                            == //TODO: op!
                            sMatch
                    )
                    ||
                    (
                        (qxkeyMatchNonscalar)
                            ==
                            keyMatch
                    )
                );
    }

    if (sKeyFilterTrait == "")
    {
        // All-topics or all-trait-match filter 
        return
            qxboolAnd;
    }

    return 
        Qx
            STRING(qxkeyTrait)
                // TEST!
                ==
                sKeyFilterTrait
            &&
            (qxboolAnd);
}

ref<CBasicNodeIfaceGp> CFilterImpl::GetFilterAsBasicNode()
{
    if (!m_rBasicCached)
    {
        m_rBasicCached = true;

        ref<CFilterSystemImpl> rFilterSystem = 
            GetFilterSystem();
        ref<CSessionIface> rSession = 
            rFilterSystem->GetSession();
        ref<CBasicClassIface> rBasicClass = 
            rSession->x_rSessionBasicClass;

        key keyFilterAndNode = GetFilterKey();
        rASSERT(keyFilterAndNode != null());
        if (rBasicClass->
                QueryBasicNodeByKey(
                    keyFilterAndNode,
                    out m_rCacheFilterAsBasicNode))
        {
            // ok
        }
        else
        {
            rFAIL(
                "failed to query BasicNode representing a Filter "
                    "at key <" + keyFilterAndNode.DumpToBareStrKey() + ">");
        }
    }

    return m_rCacheFilterAsBasicNode;
}

bool CFilterImpl::OnIsNullFilter()
{
    ALL_MEMBERS_OF(CDbFilter);

    x_rDbFilter->x_typeFilterComparator;
    x_rDbFilter->x_sFilterColumns;
    x_rDbFilter->x_sFilterSort;

    return 
        x_rDbFilter->x_keyFilterProperty == key()
        &&
        x_rDbFilter->x_sFilterMatch == ""
        &&
        x_rDbFilter->x_sFilterLike == "";
}

void CFilterImpl::OnMakeFilterMatchByProperty(
        key keyProperty,
        str sMatch,
        str sLike)
{
    x_rDbFilter->x_keyFilterProperty = keyProperty;
    x_rDbFilter->x_sFilterMatch = sMatch;
    x_rDbFilter->x_sFilterLike = sLike;
    x_rDbFilter->CommitRef();
}

void CFilterImpl::OnBuildColumnsFromUnidef(
        unidef unidefSort,
        unidef unidefVisible)
{
    x_rColumnOrder->
        ReinitOrderFromUnidef(
            unidefSort,
            unidefVisible);
}

bool CFilterImpl::OnLookupBasicNodeByKeyUnfiltered(
        key keyNode,
        out ref<CBasicNodeIfaceGp>& out_rBasicNode)
{
    ref<CFilterSystemImpl> rFilterSystem = 
        GetFilterSystem();
    ref<CSessionIface> rSession = 
        rFilterSystem->GetSession();
    ref<CBasicClassIface> rBasicClass = 
        rSession->x_rSessionBasicClass;

    return
        rBasicClass->
            QueryBasicNodeByKey(
                keyNode,
                out out_rBasicNode);
}

ptr<CTableClassIface> CFilterImpl::FindFilterTableClass()
{
    str sMayBelongToClass = Str(x_rDbFilter->x_keyFilterProperty);
    str sMatch = x_rDbFilter->x_sFilterMatch;

    if (sMatch == "" &&
        // or we just get the filter class itself as the filtering target
        sMayBelongToClass != "")
    {
        ref<CFilterSystemImpl> rFilterSystem = 
            GetFilterSystem();
        ref<CSessionIface> rSession = 
            rFilterSystem->GetSession();
        ref<CAddonMapIface> rAddonMap = 
            rSession->x_rAddonMap;

        // is it a known class reference?
        ref<CTableClassIfaceGp> rMatchTableClass;
        if (rAddonMap->
                QueryTableClasses(
                    out_ATKEY(),
                    out rMatchTableClass,
                    sMayBelongToClass,
                    classinfo()))
        {
            return (ref<CTableClassIface>) rMatchTableClass;
        }
    }

    return null();
}

rp<dbobject> CFilterImpl::OnNewDbTableResult()
{
    if not_null(
        ref<CTableClassIface> rTableClass = 
            FindFilterTableClass())
    {
        return
            rTableClass->
                NewTableNodeRef();
    }

    return null();
}

void CFilterImpl::OnAddFilterJoins(
        out from<dbobject>& out_fromResult,
        ref<CQueryIfaceGp> rQuery,
        ptr<CUdbConnectionIfaceGp> pUdbConnection,
        qxbool qxboolOn)
{
    rQuery;

    //str sMayBelongToClass = Str(x_rDbFilter->x_keyFilterProperty);
    //str sMatch = x_rDbFilter->x_sFilterMatch;

    if not_null(
        ref<CTableClassIface> rTableClass = 
            FindFilterTableClass())
    {
        out_fromResult.
            _udb_CustomTableReference(
                pUdbConnection.Valid(),
                key(), // keyDbContainer
                key(), // keyOptional
                qxboolOn);

            /*
            rQuery->
                _udb_MakeJoin(
                    rQuery,
                    m_iResultAttached,
                    //_m_rResult,
                    m_sName,
                    dblistSource.GetUdbConnection(),
                    dblistSource.GetDbContainerKey(),
                    keyOptional,
                    qxboolOn,
                    E_UdbJoin_Inner,
                    E_UdbJoin_Inner,
                    E_UdbFromTable_Join);
                    */
    }
    else
    {
        out_fromResult.
            SkipTable();
    }
}

bool CFilterImpl::OnIterateFilterColumnVisibility(
        out iter& out_i,
        out key& out_keyColumnTrait,
        bool bOnlyVisible,
        out bool& out_bVisible)
{
    return
        x_rColumnOrder->
            IterateColumnVisibility(
                out out_i,
                out out_keyColumnTrait,
                bOnlyVisible,
                out out_bVisible);
}

void CFilterImpl::OnChangeColumnOrderForFilter(
        key keyFieldClicked)
{
    x_rColumnOrder->
        ChangeColumnOrder(
            keyFieldClicked);
}

str CFilterImpl::OnGetValidFilterSortSql()
{
    return
        x_rColumnOrder->
            GetValidSortSql();
}

[...]