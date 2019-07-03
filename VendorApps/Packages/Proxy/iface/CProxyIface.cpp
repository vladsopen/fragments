// CProxyIface.cpp

#include "CProject.h"
#include "CProxyIface.h"
#include "CProxyImpl.h"
#include "CPatientLinkSampleIface.h"

CProxyIface::CProxyIface()
{
    _init_AttachToProxyIface();
}

void CProxyIface::_init_AttachToProxyIface()
{
    //x_r->_x_p = this;
}

void CProxyIface::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToProxyIface();

    rFAIL("make sure it is OK to expose CProxyIface!");
}
