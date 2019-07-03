// CFilterSystemImpl.cpp

#include "CProject.h"
#include "CFilterSystemImpl.h"
#include "CDbAllFilters.h"
#include "CDbFilter.h"
#include "CFilterImpl.h"
#include "CStoreIfaceGp.h"

CFilterSystemImpl::CFilterSystemImpl()
{
    _init_AttachToFilterSystemImpl();
}

CFilterSystemImpl::~CFilterSystemImpl()
{
    rASSERT(!_m_bFilterSystemImplOpened);
}

void CFilterSystemImpl::_init_AttachToFilterSystemImpl()
{
    m_rCacheAllNodeFilter->_x_pFilterSystem = this;
}

void CFilterSystemImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToFilterSystemImpl();

    rFAIL("make sure it is OK to expose CFilterSystemImpl!");
}

void CFilterSystemImpl::OnInitFilterSystemAfterAllModulesReady()
{
    rASSERT(!_m_bOneTimeInitFilterSystemImplOk);
    _m_bOneTimeInitFilterSystemImplOk = true;
    _m_bFilterSystemImplOpened = true;

    //
    // Init "All-Node" filter
    //

    // if any app NodeTables created we establish all-node filter
    if (_m_pUdbConnectionUsedToCreateTableNodes != null())
    {
        rASSERT(!x_bDisableFilterSystemForTest);
        rASSERT(!m_bCachedAllNodeFilter);
        m_bCachedAllNodeFilter = true;

        ref<CDbAllFilters> rDbAllFilters = 
            GetAllFilters();
        ref<CDbFilter> rDbFilter =
            rDbAllFilters->
                _x_dblistFilter.Get().
                    EnsureRowAdded(
                        C_keyFilterForAllNodes);

        m_rCacheAllNodeFilter->_x_pFilterSystem = this;
        m_rCacheAllNodeFilter->x_rDbFilter = 
            rDbFilter;
    }
    else
    {
        rASSERTM(
            x_bDisableFilterSystemForTest,
            "OnHandleApplicationLevelDbClassInfoListUpdate must be called "
                "prior to " __FUNCTION__);
    }
}

void CFilterSystemImpl::OnCloseFilterSystem()
{
    if (_m_bFilterSystemImplOpened)
    {
        _m_bFilterSystemImplOpened = false;
        // ...
    }
}

void CFilterSystemImpl::OnHandleApplicationLevelDbClassInfoListUpdate(
        ref<CUdbConnectionIfaceGp> rUdbConnection)
{
    _m_pUdbConnectionUsedToCreateTableNodes = rUdbConnection;

    // Called when it's time to declare our tables.
    //   Call only DeclareUdbClassFromCallback() 
    //   doing anything else is not supported and forbidden!

    rUdbConnection->DeclareUdbClassFromCallback(ref<CDbFilter>(), "Filter");
    rUdbConnection->DeclareUdbClassFromCallback(ref<CDbAllFilters>(), "AllFilters");
}

ref<CDbAllFilters> CFilterSystemImpl::GetAllFilters()
{
    //if (!_m_bCacheAllFiltersInitOk)
    if (!_m_rpCacheAllFilters)
    {
        //_m_bCacheAllFiltersInitOk = true;

        dbref<CDbAllFilters> dbrefAllFilters;
        dbrefAllFilters.
            _deprecated_EnsureRootInit(
                _m_pUdbConnectionUsedToCreateTableNodes.Valid());
        _m_rpCacheAllFilters = dbrefAllFilters.QueryRef();
    }

    return _m_rpCacheAllFilters.Valid();
}

ref<CFilterIfaceGp> CFilterSystemImpl::OnAddNewFilter(
        key keyOptional)
{
    ref<CDbAllFilters> rDbAllFilters = 
        GetAllFilters();

    ref<CDbFilter> rDbFilter;

    if (keyOptional != key())
    {
        rDbFilter->InitKey(keyOptional);
    }

    rDbFilter->x_keyFilterProperty = key::GFromStr("?");
    rDbFilter->x_sFilterMatch = "?";

    rDbAllFilters->
        _x_dblistFilter.Get().
            AddNewRow(
                rDbFilter);

    ref<CFilterImpl> rFilter;
    rFilter->_x_pFilterSystem = this;
    rFilter->x_rDbFilter = rDbFilter;

    return rFilter;
}

ref<CFilterIfaceGp> CFilterSystemImpl::OnGetAllNodeFilter()
{
    if (!x_bDisableFilterSystemForTest)
    {
        rASSERTM(
            m_bCachedAllNodeFilter,
            "all-node filter init is done last "
                "and it is not available during CloudSync module init");
    }

    return m_rCacheAllNodeFilter;
}

int CFilterSystemImpl::OnGetFilterCount()
{
    ref<CQueryIfaceGp> rQuery;

    // SELECT COUNT(*)
    return
        GetAllFilters()->
            _x_dblistFilter.Get().
                _deprecated_QueryCount(
                    rQuery);
}

bool CFilterSystemImpl::OnIterateFilters(
        out iter& out_i,
        out ref<CFilterIfaceGp>& out_rFilter)
{
    ref<CQueryIfaceGp> rQuery = 
        out_i.AdvanceQuery();

    from<CDbFilter> fromFilter(rQuery);

    if (rQuery->Init())
    {
        fromFilter.
            BeginFrom(
                GetAllFilters()->
                    _x_dblistFilter,
                key());

        //rQuery->x_qxboolWhere = 
        //    condWhere;
    }

    if (rQuery->Next())
    {
        ref<CFilterImpl> rFilter;
        rFilter->_x_pFilterSystem = this;
        rFilter->x_rDbFilter = fromFilter.FetchRef();

        out_rFilter = rFilter;

        return true;
    }
    else
    {
        return false;
    }
}

bool CFilterSystemImpl::OnLookupFilter(
        str sOrganizerKey,
        out ref<CFilterIfaceGp>& out_rFilter)
{
    ref<CDbFilter> rDbFilter;

    if (GetAllFilters()->
            _x_dblistFilter.Get().
                LookupAtKey(
                    key::GFromStr(sOrganizerKey),
                    out rDbFilter))
    {
        ref<CFilterImpl> rFilter;
        rFilter->_x_pFilterSystem = this;
        rFilter->x_rDbFilter = rDbFilter;

        out_rFilter = rFilter;

        return true;
    }
    else
    {
        return false;
    }
}

ref<UiContainer> CFilterSystemImpl::OnNewUiFilterSystemBar()
{
    ref<UiFilterSystemBar> rUi;

    rUi->m_rFilterSystem = this;

    return rUi;
}
