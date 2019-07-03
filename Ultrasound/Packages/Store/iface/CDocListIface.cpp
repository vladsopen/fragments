// CDocListIface.cpp

#include "CUsmainProject.h"
#include "CDocListIface.h"
#include "CDocListImpl.h"
#include "CDocImpl.h"
#include "CCalcupAsymmetryIfaceGp.h"
//#include "CDocImpl.h"

CDocListIface::CDocListIface()
{
}

void CDocListIface::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CDocListIface!");
}
