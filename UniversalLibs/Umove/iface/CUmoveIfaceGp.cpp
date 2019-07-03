// CUmoveIfaceGp.cpp

#include <VirtOs.h>
#include "CUmoveIfaceGp.h"

CUmoveIfaceGp::CUmoveIfaceGp()
{
}

void CUmoveIfaceGp::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("it may not be safe to expose this GuiPlus class!");
}

