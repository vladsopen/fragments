// CLoaderIface.cpp

#include "CProject.h"
#include "CLoaderImpl.h"
#include "CSessionIface.h"
#include "CPatientLinkIface.h"

CLoaderIface::CLoaderIface()
{
}

void CLoaderIface::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("why would you want to expose a Loader object itself?!");
}
