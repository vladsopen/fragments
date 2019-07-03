// CPatientLinkSampleIface.cpp

#include "CProject.h"
#include "CPatientLinkSampleImpl.h"

CPatientLinkSampleIface::CPatientLinkSampleIface()
{
    _init_AttachToPatientLinkSampleIface();
}

void CPatientLinkSampleIface::_init_AttachToPatientLinkSampleIface()
{
    //x_r->_x_p = this;
}

void CPatientLinkSampleIface::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToPatientLinkSampleIface();

    rFAIL("make sure it is OK to expose CPatientLinkSampleIface!");
}
