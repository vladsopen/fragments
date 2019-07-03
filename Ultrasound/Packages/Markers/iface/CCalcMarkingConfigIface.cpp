// CCalcMarkingConfigIface.cpp

#include "CUsmainProject.h"
#include "CCalcMarkingConfigIface.h"
#include "CCalcMarkingConfigImpl.h"

CCalcMarkingConfigIface::CCalcMarkingConfigIface()
{
}

void CCalcMarkingConfigIface::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    //rFAIL("make sure it is OK to expose CCalcMarkingConfigIface!");
}
