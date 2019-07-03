// CProxyIfaceGp.cpp

#include <VirtOs.h>
#include "CProxyIfaceGp.h"

CProxyIfaceGp::CProxyIfaceGp()
{
    _init_AttachToProxyIfaceGp();
}

void CProxyIfaceGp::_init_AttachToProxyIfaceGp()
{
    //x_r->_x_p = this;
}

void CProxyIfaceGp::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToProxyIfaceGp();

    rFAIL("make sure it is OK to expose CProxyIfaceGp!");
}
