// CCoverageTracerIfaceGp.cpp

#include <VirtOs.h>
#include "CCoverageTracerIfaceGp.h"

CCoverageTracerIfaceGp::CCoverageTracerIfaceGp()
{
}

void CCoverageTracerIfaceGp::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CCoverageTracerIfaceGp!");
}
