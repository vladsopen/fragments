// CProtocolIface.cpp

#include "CProject.h"
#include "CProtocolImpl.h"

CProtocolIface::CProtocolIface()
{
    _init_AttachToProtocolIface();
}

void CProtocolIface::_init_AttachToProtocolIface()
{
    //x_r->_x_p = this;
}

void CProtocolIface::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToProtocolIface();

    //rFAIL("make sure it is OK to expose CProtocolIface!");
}

/*
CProtocolIface::~CProtocolIface()
{
    rASSERT(!_m_bProtocolIfaceOpened);
}

void CProtocolIface::InitProtocol()
{
    rASSERT(!_m_bOneTimeInitProtocolIfaceOk);
    _m_bOneTimeInitProtocolIfaceOk = true;
    _m_bProtocolIfaceOpened = true;
}

void CProtocolIface::CloseProtocol()
{
    if (_m_bProtocolIfaceOpened)
    {
        _m_bProtocolIfaceOpened = false;
        // ...
    }
}
*/

