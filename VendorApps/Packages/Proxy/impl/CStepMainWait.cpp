// CStepMainWait.cpp

#include "CProject.h"
#include "CStepMainWait.h"
#include "CInterprocRunProcessIfaceGp.h"
#include "CProxyImpl.h"
#include "CProtocolIface.h"

CStepMainWait::CStepMainWait()
{
    _init_AttachToStepMainWait();
}

void CStepMainWait::_init_AttachToStepMainWait()
{
    //x_r->_x_p = this;
}

void CStepMainWait::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToStepMainWait();

    rFAIL("make sure it is OK to expose CStepMainWait!");
}

str CStepMainWait::OnGetFixedStepTitle()
{
    return "Wait for modality exit.";
}

void CStepMainWait::OnHandleActiveStepPolling()
{
    rASSERT(!x_bStepComplete);

    ref<CInterprocRunProcessIfaceGp> rProcess = 
        GetProxy()->x_rRunExternalApp;
    ref<CProtocolIface> rProtocol = 
        GetProxy()->x_rProtocol;

    rASSERT(
        rProcess->IsProcessAlive()
        ||
        rProcess->IsProcessComplete());

    if (rProcess->IsProcessComplete())
    {
        x_bStepComplete = true;

        x_sReplaceStepTitle = 
            "Modality process exit detected.";
    }
    else
    {
        x_sReplaceStepTitle = 
            "Waiting for modality application process to complete. " +
                RichSuccessBold(
                    RichFontCourier(
                        RichSize(25,
                            str("|/-\\").GetMid(m_nWaitCount % 4, 1))));

        m_nWaitCount++;
    }
}

