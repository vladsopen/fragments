// CAppEventFloaterIfaceGp.cpp

#include <VirtOs.h>
#include "CAppEventIfaceGp.h"
#include "CAppEventFloaterIfaceGp.h"

CAppEventFloaterIfaceGp::CAppEventFloaterIfaceGp()
{
}

void CAppEventFloaterIfaceGp::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);
}

ref<CAppEventFloaterIfaceGp> CAppEventFloaterIfaceGp::_init_GNewAppEventFloater(
        sloc slocCaption,
        res resMainLargeIcon)
{
    ref<CAppEventFloaterIfaceGp> rFloater;

    rFloater->x_slocFloaterWindowCaption = slocCaption;
    rFloater->_x_resMainLargeFloaterIcon = resMainLargeIcon;

    return rFloater;
}

