// CAddonMapImpl.cpp

#include "CProject.h"
#include "CAddonMapImpl.h"
#include "CAddonSessionIfaceGp.h"
#include "CSessionIface.h"
#include "CActionIfaceGp.h"
#include "CTableClassIface.h"

CAddonMapImpl::CAddonMapImpl()
{
    _init_AttachToAddonMapImpl();
}

CAddonMapImpl::~CAddonMapImpl()
{
    rASSERT(!_m_bAddonMapOpenedForSession);
}

void CAddonMapImpl::_init_AttachToAddonMapImpl()
{
    //x_r->_x_p = this;
}

void CAddonMapImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToAddonMapImpl();

    rFAIL("make sure it is OK to expose CAddonMapImpl!");
}

void CAddonMapImpl::OnInitAddonMapForSession()
{
    rASSERT(!_m_bAddonMapUsedForSession);
    rASSERT(!_m_bAddonMapOpenedForSession);
    _m_bAddonMapUsedForSession = true;
    _m_bAddonMapOpenedForSession = true;

    ref<CSessionIface> rSession = 
        _x_pSession.Get().Valid();

    // **** do NOT assume Session is completelly initialized here! ****

    // make the map visible
    m_aAddonSession = m_aAddonSessionInitial;
    m_aAddonSessionInitial.RemoveAll();

    // notify all addons so they init their interlinks
    HandleMapRegistrationsComplete();
}

void CAddonMapImpl::OnCloseAddonMapForSession()
{
    if (_m_bAddonMapOpenedForSession)
    {
        // Close all contexts
        ref<CAddonSessionIfaceGp> rIterAddonSession;
        for (
            iter i;
            this->
                IterateAddonSessions(
                    out i,
                    out rIterAddonSession);)
        {
            rIterAddonSession->
                CloseAddonSession();
        }

        m_aAddonSession.RemoveAll();

        _m_bAddonMapOpenedForSession = false;
    }
}

void CAddonMapImpl::OnRegisterAddonSession(
        ref<CAddonSessionIfaceGp> rAddonSession)
{
    rASSERT(!_m_bAddonMapOpenedForSession);

    rASSERTM(
        m_aAddonSession.GetCount() == 0, 
        "cannot register addons after init is complete");

    if ok(m_aAddonSessionInitial.Find(rAddonSession) == -1)
    {
        // remember it
        m_aAddonSessionInitial.Add(rAddonSession);

        // init parent link
        rAddonSession->_x_pCoreSession = 
            _x_pSession.Get().Valid();

        // call init hook
        rAddonSession->
            InitAddonSessionBeforeCloud();
    }
}

void CAddonMapImpl::OnRegisterTableClassSession(
        classinfo classinfoDb,
        str sClassIdCore,
        ref<CTableClassIface> rTableClass)
{
    rASSERT(!_m_mapTableClassInfo.IsIn(classinfoDb));
    rASSERT(!_m_mapTableClassCore.IsIn(sClassIdCore));
    rASSERT(classinfoDb != null());

    _m_mapTableClassInfo[classinfoDb] = rTableClass;
    _m_mapTableClassCore[sClassIdCore] = rTableClass;

    RegisterAddonSession(
        rTableClass);
}

queryorcount CAddonMapImpl::OnQueryTableClasses(
        out iter& out_i,
        out ref<CTableClassIfaceGp>& out_rTableClass,
        str sClassIdCore,
        classinfo classinfoAppDb)
{
    if (out_i.IsGetCount())
    {
        // we are called just to count items
        int nCount = _m_mapTableClassInfo.GetCount();
        rASSERT(nCount == _m_mapTableClassCore.GetCount());

        return
            queryorcount(
                nCount);
    }
    else
    if (sClassIdCore != "")
    {
        rASSERT(classinfoAppDb == null());

        // we are called to lookup a single record
        ref<CTableClassIface> rTableClassIface;
        if (_m_mapTableClassCore.
                LookupExisting(
                    sClassIdCore,
                    out rTableClassIface))
        {
            out_rTableClass = rTableClassIface;
            return true;
        }
    }
    else
    if (classinfoAppDb != null())
    {
        rASSERT(sClassIdCore == "");

        // we are called to lookup a single record
        ref<CTableClassIface> rTableClassIface;
        if (_m_mapTableClassInfo.
                LookupExisting(
                    classinfoAppDb,
                    out rTableClassIface))
        {
            out_rTableClass = rTableClassIface;
            return true;
        }
    }
    else
    {
        // we are called to iterate over all items
        ref<CTableClassIface> rTableClassIface;
        if (_m_mapTableClassInfo.
                Iterate(
                    out out_i,
                    out rTableClassIface))
        {
            out_rTableClass = rTableClassIface;
            return true;
        }
    }

    return false;
}

bool CAddonMapImpl::OnIterateAddonSessions(
        out iter& out_i,
        out ref<CAddonSessionIfaceGp>& out_rAddonSession)
{
    rASSERT(_m_bAddonMapOpenedForSession);
    rASSERTM(
        m_aAddonSessionInitial.GetCount() == 0, 
        "cannot call until HandleMapRegistrationsComplete()");

    return 
        m_aAddonSession.Iterate(out out_i, out out_rAddonSession);
}

ptr<CAddonSessionIfaceGp> CAddonMapImpl::OnFindSingletonAddonSession(
        classinfo classinfoAddonSession)
{
    rASSERT(
        classinfoAddonSession._internal_IsKindOf(
            CAddonSessionIfaceGp::GGetStaticClassInfo()));
    rASSERTM(
        m_aAddonSessionInitial.GetCount() == 0, 
        "cannot call until HandleMapRegistrationsComplete()");

    ref<CAddonSessionIfaceGp> rIterAddonSession;
    ptr<CAddonSessionIfaceGp> pFoundSession;
    for (
        iter i;
        this->
            IterateAddonSessions(
                out i,
                out rIterAddonSession);)
    {
        if (rIterAddonSession->GetObjectClassInfo().
                _internal_IsKindOf(classinfoAddonSession))
        {
            rASSERTM(
                pFoundSession == null(),
                "only one " + 
                    classinfoAddonSession.GetClassNameInProcess() + 
                    " is expected per Session");

            pFoundSession = 
                rIterAddonSession;
        }
    }

    /* optional, check in caller
    rASSERTM(
        pFoundSession != null(),
        classinfoAddonSession.GetClassNameInProcess() + 
            " addon is not found in Session");
            */

    return pFoundSession;
}

void CAddonMapImpl::OnHandleDbClassInfoListUpdate(
        ref<CUdbConnectionIfaceGp> rUdbConnection)
{
    rASSERT(_m_bAddonMapOpenedForSession);

    // broadcast to all addons
    ref<CAddonSessionIfaceGp> rIterAddonSession;
    for (
        iter i;
        this->
            IterateAddonSessions(
                out i,
                out rIterAddonSession);)
    {
        rIterAddonSession->
            HandleDbClassInfoListUpdate(
                rUdbConnection);
    }
}

void CAddonMapImpl::OnHandleStoreReady(
        ref<CUdbConnectionIfaceGp> rUdbConnection)
{
    rASSERT(_m_bAddonMapOpenedForSession);

    // broadcast to all addons
    ref<CAddonSessionIfaceGp> rIterAddonSession;
    for (
        iter i;
        this->
            IterateAddonSessions(
                out i,
                out rIterAddonSession);)
    {
        rIterAddonSession->
            HandleStoreReady(
                rUdbConnection);
    }
}

void CAddonMapImpl::OnHandleMapRegistrationsComplete()
{
    rASSERT(_m_bAddonMapOpenedForSession);

    // broadcast to all addons
    ref<CAddonSessionIfaceGp> rIterAddonSession;
    for (
        iter i;
        this->
            IterateAddonSessions(
                out i,
                out rIterAddonSession);)
    {
        rIterAddonSession->
            HandleMapRegistrationsComplete();
    }
}

void CAddonMapImpl::OnHandleAddonInitAfterCloud()
{
    rASSERT(_m_bAddonMapOpenedForSession);

    // broadcast to all addons
    ref<CAddonSessionIfaceGp> rIterAddonSession;
    for (
        iter i;
        this->
            IterateAddonSessions(
                out i,
                out rIterAddonSession);)
    {
        rIterAddonSession->
            InitAddonSessionAfterCloud();
    }
}

void CAddonMapImpl::OnHandleActionInterpreter(
        ref<CActionIfaceGp> rAction)
{
    rASSERT(_m_bAddonMapOpenedForSession);

    // broadcast to all addons
    ref<CAddonSessionIfaceGp> rIterAddonSession;
    for (
        iter i;
        this->
            IterateAddonSessions(
                out i,
                out rIterAddonSession);)
    {
        rIterAddonSession->
            HandleActionInterpreter(
                rAction);
            // see CBasicClassImpl::OnHandleActionInterpreter()
    }
}

