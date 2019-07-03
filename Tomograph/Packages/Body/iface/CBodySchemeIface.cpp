// CBodySchemeIface.cpp

#include "CProject.h"
#include "CBodySchemeImpl.h"
#include "CBodyProfileImpl.h"
#include "CBodyPointImpl.h"

CBodySchemeIface::CBodySchemeIface()
{
}

void CBodySchemeIface::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);
}
