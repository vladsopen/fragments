// CStepTerminateDbUnlock.cpp

#include "CProject.h"
#include "CStepTerminateDbUnlock.h"
#include "CProxyImpl.h"
#include "CPatientLinkIface.h"

CStepTerminateDbUnlock::CStepTerminateDbUnlock()
{
    _init_AttachToStepTerminateDbUnlock();
}

void CStepTerminateDbUnlock::_init_AttachToStepTerminateDbUnlock()
{
    //x_r->_x_p = this;
}

void CStepTerminateDbUnlock::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToStepTerminateDbUnlock();

    rFAIL("make sure it is OK to expose CStepTerminateDbUnlock!");
}

str CStepTerminateDbUnlock::OnGetFixedStepTitle()
{
    return "Unlock database.";
}

void CStepTerminateDbUnlock::OnHandleActiveStepPolling()
{
    rASSERT(!x_bStepComplete);
    x_bStepComplete = true;

    GetProxy()->
        GetPatientLink()->
            LockCurrentCheckup(
                false);

    x_sReplaceStepTitle = "Database unlocked.";
}
