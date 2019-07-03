// CClassTestingTempIfaceGp.cpp

#include <VirtOs.h>
#include "CClassTestingTempImpl.h"
#include "VirtOs_text.h"

CClassTestingTempIfaceGp::CClassTestingTempIfaceGp()
{
    _init_AttachToClassTestingTempIfaceGp();
}

void CClassTestingTempIfaceGp::_init_AttachToClassTestingTempIfaceGp()
{
    //x_r->_x_p = this;
}

void CClassTestingTempIfaceGp::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToClassTestingTempIfaceGp();

    rFAIL("make sure it is OK to expose CClassTestingTempIfaceGp!");
}
