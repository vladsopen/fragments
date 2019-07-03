// CStepAbstract.cpp

#include "CProject.h"
#include "CStepAbstract.h"
#include "CSequence.h"

CStepAbstract::CStepAbstract()
{
    _init_AttachToStepAbstract();
}

void CStepAbstract::_init_AttachToStepAbstract()
{
    //x_r->_x_p = this;
}

void CStepAbstract::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToStepAbstract();

    rFAIL("make sure it is OK to expose CStepAbstract!");
}

/*
CStepAbstract::~CStepAbstract()
{
    rASSERT(!_m_bStepAbstractOpened);
}

void CStepAbstract::InitStepAbstract()
{
    rASSERT(!_m_bOneTimeInitStepAbstractOk);
    _m_bOneTimeInitStepAbstractOk = true;
    _m_bStepAbstractOpened = true;
}

void CStepAbstract::CloseStepAbstract()
{
    if (_m_bStepAbstractOpened)
    {
        _m_bStepAbstractOpened = false;
        // ...
    }
}
*/

str CStepAbstract::GetInternalStepName()
{
    str sName = 
        GetObjectClassInfo().GetClassNameInProcess();

    sName =
        sName.
            GetTruncateLeft(sName.FindPos("Step") + 4);

    rASSERT(sName.Len() >= 3);

    return sName;
}

ref<UiContainer> CStepAbstract::NewUiStepAbstractRow()
{
    ref<UiStepAbstractRow> rUi;

    rUi->m_rStep = this;

    return rUi;
}
