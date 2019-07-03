// COverlayImageIfaceGp.cpp

#include <VirtOs.h>
#include "COverlayImageIfaceGp.h"

COverlayImageIfaceGp::COverlayImageIfaceGp()
{
    _init_AttachToOverlayImageIfaceGp();
}

void COverlayImageIfaceGp::_init_AttachToOverlayImageIfaceGp()
{
    //x_r->_x_p = this;
}

void COverlayImageIfaceGp::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToOverlayImageIfaceGp();

    rFAIL("make sure it is OK to expose COverlayImageIfaceGp!");
}

/*
COverlayImageIfaceGp::~COverlayImageIfaceGp()
{
    rASSERT(!_m_bOpened);
}

void COverlayImageIfaceGp::InitOverlayImage()
{
    rASSERT(!_m_bInited);
    _m_bInited = true;
    _m_bOpened = true;
}

void COverlayImageIfaceGp::CloseOverlayImage()
{
    if (_m_bOpened)
    {
        _m_bOpened = false;
        // ...
    }
}
*/

