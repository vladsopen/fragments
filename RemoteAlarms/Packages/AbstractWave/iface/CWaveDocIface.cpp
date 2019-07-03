// CWaveDocIface.cpp

#include "CProject.h"
#include "CWaveDocImpl.h"
#include "CSessionIface.h"
#include "CPatientLinkSampleIface.h"

CWaveDocIface::CWaveDocIface()
{
    _init_AttachToWaveDocIface();
}

void CWaveDocIface::_init_AttachToWaveDocIface()
{
    //x_r->_x_p = this;
}

void CWaveDocIface::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToWaveDocIface();

    rFAIL("make sure it is OK to expose CWaveDocIface!");
}

/*
CWaveDocIface::~CWaveDocIface()
{
    rASSERT(!_m_bWaveDocIfaceOpened);
}

void CWaveDocIface::InitWaveDoc()
{
    rASSERT(!_m_bOneTimeInitWaveDocIfaceOk);
    _m_bOneTimeInitWaveDocIfaceOk = true;
    _m_bWaveDocIfaceOpened = true;
}

void CWaveDocIface::CloseWaveDoc()
{
    if (_m_bWaveDocIfaceOpened)
    {
        _m_bWaveDocIfaceOpened = false;
        // ...
    }
}
*/

