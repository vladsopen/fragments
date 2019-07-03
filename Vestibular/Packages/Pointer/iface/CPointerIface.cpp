// CPointerIface.cpp

#include "CProject.h"
#include "CPointerIface.h"
#include "CPointerImpl.h"

CPointerIface::CPointerIface()
{
    _init_AttachToPointerIface();
}

void CPointerIface::_init_AttachToPointerIface()
{
    //x_r->_x_p = this;
}

void CPointerIface::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToPointerIface();

    rFAIL("make sure it is OK to expose CPointerIface!");
}
