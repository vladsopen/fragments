// CClassTestGlobalsIface.cpp

#include "CDebuxProject.h"
#include "CClassTestGlobalsImpl.h"

CClassTestGlobalsIface::CClassTestGlobalsIface()
{
    _init_AttachToClassTestGlobalsIface();
}

void CClassTestGlobalsIface::_init_AttachToClassTestGlobalsIface()
{
    //x_r->_x_p = this;
}

void CClassTestGlobalsIface::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToClassTestGlobalsIface();

    rFAIL("make sure it is OK to expose CClassTestGlobalsIface!");
}

/*
CClassTestGlobalsIface::~CClassTestGlobalsIface()
{
    rASSERT(!_m_bClassTestGlobalsIfaceOpened);
}

void CClassTestGlobalsIface::InitClassTestGlobals()
{
    rASSERT(!_m_bOneTimeInitClassTestGlobalsIfaceOk);
    _m_bOneTimeInitClassTestGlobalsIfaceOk = true;
    _m_bClassTestGlobalsIfaceOpened = true;
}

void CClassTestGlobalsIface::CloseClassTestGlobals()
{
    if (_m_bClassTestGlobalsIfaceOpened)
    {
        _m_bClassTestGlobalsIfaceOpened = false;
        // ...
    }
}
*/

