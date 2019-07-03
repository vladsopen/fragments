// CCurveSliceIface.cpp

#include "CUsmainProject.h"
#include "CCurveSliceIface.h"
#include "CCurveSliceImpl.h"

CCurveSliceIface::CCurveSliceIface()
{
}

void CCurveSliceIface::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    //rFAIL("make sure it is OK to expose CCurveSliceIface!");
}
