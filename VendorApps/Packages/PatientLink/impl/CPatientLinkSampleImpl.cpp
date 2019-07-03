// CPatientLinkSampleImpl.cpp

#include "CProject.h"
#include "CPatientLinkSampleImpl.h"
#include "CPatientLinkImpl.h"
#include "CPatientEmbedLinkImpl.h"
#include "CPatientSampleMap.h"
#include "CRandomIfaceGp.h"

CPatientLinkSampleImpl::CPatientLinkSampleImpl()
{
    _init_AttachToPatientLinkSampleImpl();
}

void CPatientLinkSampleImpl::_init_AttachToPatientLinkSampleImpl()
{
    //x_r->_x_p = this;
}

void CPatientLinkSampleImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToPatientLinkSampleImpl();

    rFAIL("make sure it is OK to expose CPatientLinkSampleImpl!");
}

bool CPatientLinkSampleImpl::OnIsProxyAttachedToPatientSample()
{
    return x_sOpenedPatientSampleKey != "";
}

bool CPatientLinkSampleImpl::OnIsProxyCreatedByPatientLink()
{
    return
        IsPatientSampleOpenedByPatientIo()
        ||
        IsPatientSamplePaintedByPatientIo()
        ||
        IsPatientSampleViewedByPatientEmbed();
}

bool CPatientLinkSampleImpl::OnIsPatientSampleOpenedByPatientIo()
{
    return x_bProxyOpenedFromPatient;
}

bool CPatientLinkSampleImpl::OnIsPatientSamplePaintedByPatientIo()
{
    return x_bTempProxyForPatientPaint;
}

bool CPatientLinkSampleImpl::OnIsPatientSampleViewedByPatientEmbed()
{
    return x_bProxyForPatientEmbed;
}

void CPatientLinkSampleImpl::OpenUiPatientLinkSampleView()
{
    ref<UiContainer> rUiFrame = 
        NewUiPatientLinkSampleView();

    rUiFrame->CreateFloater();

    //DEMO: make sure multiple floaters dont overlay each other
    rUiFrame->thispanel()->UpdateContentAndLayoutNow();
    point pointPos = rUiFrame->thispanel()->GetInstantPos();
    pointPos.x += ref<CRandomIfaceGp>()->GenerateNextRandomInt(-50, 50);
    pointPos.y += ref<CRandomIfaceGp>()->GenerateNextRandomInt(-50, 50);
    rUiFrame->thispanel()->SetInstantPos(pointPos.x, pointPos.y);
}

void CPatientLinkSampleImpl::OnHandleCloseProxy()
{
    _m_ptrmapUiPatientLinkSampleView.CloseAllFrames();
}

ref<UiContainer> CPatientLinkSampleImpl::NewUiPatientLinkSampleView()
{
    ref<UiPatientLinkSampleView> rUi;

    rUi->m_rPatientLinkSample = this;

    return rUi;
}
