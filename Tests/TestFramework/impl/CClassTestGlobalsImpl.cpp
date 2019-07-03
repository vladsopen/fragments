// CClassTestGlobalsImpl.cpp

#include "CDebuxProject.h"
#include "CClassTestGlobalsImpl.h"
#include "VirtOs_CInterMem.h"
#include "CDebuxSessionImpl.h"

CClassTestGlobalsImpl::CClassTestGlobalsImpl()
{
    _init_AttachToClassTestGlobalsImpl();
}

void CClassTestGlobalsImpl::_init_AttachToClassTestGlobalsImpl()
{
    //x_r->_x_p = this;
}

void CClassTestGlobalsImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToClassTestGlobalsImpl();

    rFAIL("make sure it is OK to expose CClassTestGlobalsImpl!");
}

/*
CClassTestGlobalsImpl::~CClassTestGlobalsImpl()
{
    rASSERT(!_m_bClassTestGlobalsImplOpened);
}

void CClassTestGlobalsImpl::InitClassTestGlobals()
{
    rASSERT(!_m_bOneTimeInitClassTestGlobalsImplOk);
    _m_bOneTimeInitClassTestGlobalsImplOk = true;
    _m_bClassTestGlobalsImplOpened = true;
}

void CClassTestGlobalsImpl::CloseClassTestGlobals()
{
    if (_m_bClassTestGlobalsImplOpened)
    {
        _m_bClassTestGlobalsImplOpened = false;
        // ...
    }
}
*/

bool CClassTestGlobalsImpl::OnIsInsideDebuxStartersTest()
{
    return 
        *GetInsideDebuxStartersTestGuardFlag()
        &&
        !CProject::GGetIt().IsStandaloneProject();
}

void CClassTestGlobalsImpl::OnGuardInsideDebuxStartersTest(
        bool bInside)
{
    *GetInsideDebuxStartersTestGuardFlag() = bInside;
}

ref<UiContainer> CClassTestGlobalsImpl::OnNewUiClassTestGlobalsCces()
{
    ref<UiClassTestGlobalsCces> rUi;

    rUi->m_rClassTestGlobals = this;

    return rUi;
}
