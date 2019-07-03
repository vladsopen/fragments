// CLoaderIface.cpp

#include "CProject.h"
#include "CLoaderImpl.h"
#include "CProxyIface.h"
#include "CPatientLinkIface.h"
#include "CDemoIface.h"

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
