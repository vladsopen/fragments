// CSequence.cpp

#include "CProject.h"
#include "CSequence.h"
#include "CStepExe.h"
#include "CStepMainWait.h"
#include "CStepAbstract.h"
#include "CProxyImpl.h"
#include "CStepCommandPrep.h"
#include "CStepBeforeDbLock.h"
#include "CStepDbInfo.h"
#include "CStepTerminateDbUnlock.h"
#include "CStepSaveResult.h"
#include "CLoaderIface.h"
#include "VirtOs_CAppGp.h"
#include "VirtOs_CGlobalUi.h"

CSequence::CSequence()
{
    m_arStep.Add(ref<CStepBeforeDbLock>());
    m_arStep.Add(ref<CStepCommandPrep>());
    m_arStep.Add(ref<CStepDbInfo>());
    m_arStep.Add(ref<CStepExe>());
    m_arStep.Add(ref<CStepMainWait>());
    m_arStep.Add(ref<CStepSaveResult>());
    m_arStep.Add(ref<CStepTerminateDbUnlock>());

    _init_AttachToSequence();
}

CSequence::~CSequence()
{
    rASSERT(!_m_bSequenceOpened);
}

void CSequence::_init_AttachToSequence()
{
    //x_r->_x_p = this;

    ref<CStepAbstract> rStep;
    for (
        iter i;
        IterateSteps(
            out i,
            out rStep);)
    {
        rStep->_x_pSequence = this;
    }
}

void CSequence::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToSequence();

    rFAIL("make sure it is OK to expose CSequence!");
}

void CSequence::InitSequence()
{
    rASSERT(!_m_bOneTimeInitSequenceOk);
    _m_bOneTimeInitSequenceOk = true;
    _m_bSequenceOpened = true;

    _m_ontimerSequence.
        SetTimeoutInTicks(
            100,
            this);
}

void CSequence::CloseSequence()
{
    if (_m_bSequenceOpened)
    {
        _m_bSequenceOpened = false;
        // ...
    }
}

void CSequence::HandleSequenceTimer()
{
    rASSERT(_m_bSequenceOpened);

    _m_ptrmapUiSequenceList.InvalidateTreeState();

    x_sDebugLastPollStatus = "Polling crashed!";

    if (GetProxy()->x_bAutoExit)
    {
        if not_null(
            ref<CUiPanelGp> rUiRealMainFrame = 
                CAppGp::GGet()->x_rUi->GetAppMainFrame())
        {
            rUiRealMainFrame->SetHidden(true);
        }
    }

    // search next step to poll
    ref<CStepAbstract> rStep;
    int nSteps = 0;
    for (
        iter i;
        IterateSteps(
            out i,
            out rStep);)
    {
        nSteps++;

        if (!rStep->x_bStepComplete)
        {
            x_sDebugLastPollStatus = 
                "Active step is " + 
                    RichBold(rStep->GetFixedStepTitle()) + 
                    "";

            _x_pLastPolledStep = rStep;

            rStep->
                HandleActiveStepPolling();

            return;
        }
    }

    x_sDebugLastPollStatus = "All " + RichBold(Str(nSteps)) + " steps complete.";
    _x_pLastPolledStep = null();

    if (GetProxy()->x_bAutoExit)
    {
        CProject::GGetIt().
[...]