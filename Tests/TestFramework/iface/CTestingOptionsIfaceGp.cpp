// CTestingOptionsIfaceGp.cpp

#include <VirtOs.h>
#include "CTestingOptionsIfaceGp.h"

CTestingOptionsIfaceGp::CTestingOptionsIfaceGp()
{
}

void CTestingOptionsIfaceGp::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CTestingOptionsIfaceGp!");
}
