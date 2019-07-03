// CImageIface.cpp

#include "CUsmainProject.h"
#include "CImageIface.h"
#include "CImageImpl.h"

CImageIface::CImageIface()
{
}

void CImageIface::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    //rFAIL("make sure it is OK to expose CImageIface!");
}
