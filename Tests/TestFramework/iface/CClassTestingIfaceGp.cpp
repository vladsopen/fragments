// CClassTestingIfaceGp.cpp

#include <VirtOs.h>
#include "CClassTestingIfaceGp.h"
#include "CClassTestingTempIfaceGp.h"
#include "CTestingOptionsIfaceGp.h"

CClassTestingIfaceGp::CClassTestingIfaceGp()
{
}

void CClassTestingIfaceGp::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CClassTestingIfaceGp!");
}
