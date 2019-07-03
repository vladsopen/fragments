// CCalcMarkingIface.cpp

#include "CUsmainProject.h"
#include "CCalcMarkingIface.h"
#include "CCalcMarkingImpl.h"

CCalcMarkingIface::CCalcMarkingIface()
{
    _init_AttachToCalcMarkingIface();
}

void CCalcMarkingIface::_init_AttachToCalcMarkingIface()
{
    //x_r->_x_p = this;
}

void CCalcMarkingIface::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToCalcMarkingIface();

    rFAIL("make sure it is OK to expose CCalcMarkingIface!");
}
