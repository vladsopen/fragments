// CPatientSampleMap.cpp

#include "CProject.h"
#include "CPatientSampleMap.h"
#include "CPatientLinkSampleImpl.h"
#include "CProxyIface.h"
#include "CPatientSampleIfaceGp.h"

CPatientSampleMap::CPatientSampleMap()
{
    _init_AttachToPatientSampleMap();
}

void CPatientSampleMap::_init_AttachToPatientSampleMap()
{
    //x_r->_x_p = this;
}

void CPatientSampleMap::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToPatientSampleMap();

    rFAIL("make sure it is OK to expose CPatientSampleMap!");
}

bool CPatientSampleMap::AddAndOpenNewProxy(
        ref<CPatientSampleIfaceGp> rPatientSample,
        ref<CProxyIface> rNewProxy)
{
    if (FindProxyByPatientSampleKey(
            rPatientSample->x_sSampleKey) 
            != 
            null())
    {
        return false;
    }

    // Create a new Proxy
    ref<CPatientLinkSampleImpl> rPatientLinkSample =
        rNewProxy->
            _x_rPatientLinkSample;

    // Load Proxy
    rNewProxy->
        InitProxy(
            rPatientSample->x_pathAttachmentPrefix,
            rPatientSample->x_sAttachmentRevision,
            rPatientSample->x_sAttachmentExtra);

    // Save the sample key to identify the sample later
    rPatientLinkSample->x_sOpenedPatientSampleKey = 
        rPatientSample->x_sSampleKey;

    // save into the map
    rASSERT(_m_aViewProxy.Find(rNewProxy) == -1);
    _m_aViewProxy.Add(rNewProxy);

    return true;
}

void CPatientSampleMap::CloseAllViewedProxys()
{
    // close all sessions properly
    ref<CProxyIface> rIterProxy;
    for (
        iter i;
        this->
            IterateViewedSessions(
                out i,
                out rIterProxy);)
    {
        rIterProxy->
            CloseProxy();
    }

    // now it's safe to empty the array
    _m_aViewProxy.RemoveAll();
}

ptr<CProxyIface> CPatientSampleMap::FindProxyByPatientSampleKey(
        str sPatientSampleKey)
{
    rASSERT(sPatientSampleKey != "");
    ptr<CProxyIface> pFoundProxy = null();
    ref<CProxyIface> rIterProxy;
    for (
        iter i;
        this->
            IterateViewedSessions(
                out i,
                out rIterProxy);)
    {
        ref<CPatientLinkSampleImpl> rPatientLinkSample = 
            rIterProxy->_x_rPatientLinkSample;

        if (rPatientLinkSample->x_sOpenedPatientSampleKey == sPatientSampleKey)
        {
            rASSERT(pFoundProxy == null());
            pFoundProxy = rIterProxy;
        }
    }

    return pFoundProxy;
}

bool CPatientSampleMap::IterateViewedSessions(
        out iter& out_i,
        out ref<CProxyIface>& out_rProxy)
{
    if (out_i.IsBeforeFirstIteration())
    {
        CleanupClosedProxys();
    }
    
    return 
        _m_aViewProxy.
            Iterate(
                out out_i,
                out out_rProxy);
}

int CPatientSampleMap::CountOpenedProxys()
{
    CleanupClosedProxys();

    return _m_aViewProxy.GetCount();
}

void CPatientSampleMap::CleanupClosedProxys()
{
    repeat(iRepeat, _m_aViewProxy.GetCount())
    {
        if (!FindAndDeleteFirstClosedProxy())
        {
            break;
        }
    }
}

bool CPatientSampleMap::FindAndDeleteFirstClosedProxy()
{
    ref<CProxyIface> rIterProxy;
    for (
        iter i;
        _m_aViewProxy. // *** direct iter to avoid infinite recursion ***
            Iterate(
                out i,
                out rIterProxy);)
[...]