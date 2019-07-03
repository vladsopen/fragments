// CPatientLinkIface.cpp

#include "CProject.h"
#include "CPatientLinkImpl.h"

CPatientLinkIface::CPatientLinkIface()
{
}

void CPatientLinkIface::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);
}
