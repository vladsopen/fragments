// CDemoIface.cpp

#include "CProject.h"
#include "CDemoImpl.h"

CDemoIface::CDemoIface()
{
    _init_AttachToDemoIface();
}

void CDemoIface::_init_AttachToDemoIface()
{
    //x_r->_x_p = this;
}

void CDemoIface::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToDemoIface();

    rFAIL("make sure it is OK to expose CDemoIface!");
}
