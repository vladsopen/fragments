// CDownloaderIface.cpp

#include "CProject.h"
#include "CDownloaderImpl.h"
// (uninclude) #include "CDeviceManagerIface.h"
#include "CSessionIface.h"

CDownloaderIface::CDownloaderIface()
{
}

void CDownloaderIface::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);
}
