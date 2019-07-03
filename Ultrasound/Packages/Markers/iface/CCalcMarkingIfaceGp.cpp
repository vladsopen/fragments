// CCalcMarkingIfaceGp.cpp

#include <VirtOs.h>
#include "CCalcMarkingIfaceGp.h"

CCalcMarkingIfaceGp::CCalcMarkingIfaceGp()
{
}

void CCalcMarkingIfaceGp::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);
}
