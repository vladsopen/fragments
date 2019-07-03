// CClassTestingTempIface.cpp

#include "CDebuxProject.h"
#include "CClassTestingTempImpl.h"

CClassTestingTempIface::CClassTestingTempIface()
{
    _init_AttachToClassTestingTempIface();
}

void CClassTestingTempIface::_init_AttachToClassTestingTempIface()
{
    //x_r->_x_p = this;
}

void CClassTestingTempIface::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToClassTestingTempIface();

    rFAIL("make sure it is OK to expose CClassTestingTempIface!");
}
