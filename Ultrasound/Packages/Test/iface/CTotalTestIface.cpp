// CTotalTestIface.cpp

#include "CUsmainProject.h"
#include "CTotalTestIface.h"
#include "CTotalTestImpl.h"

CTotalTestIface::CTotalTestIface()
{
}

void CTotalTestIface::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CTotalTestIface!");
}
