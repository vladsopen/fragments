// CNoteMarkingIface.cpp

#include <VirtOs.h>
#include "CNoteMarkingIface.h"
#include "CNoteMarkingImpl.h"

CNoteMarkingIface::CNoteMarkingIface()
{
}

void CNoteMarkingIface::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);
}
