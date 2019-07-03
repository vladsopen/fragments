// CStepCommandPrep.cpp

#include "CProject.h"
#include "CStepCommandPrep.h"
#include "CProxyImpl.h"
#include "VirtOs_dir.h"
#include "CProtocolIface.h"

CStepCommandPrep::CStepCommandPrep()
{
    _init_AttachToStepCommandPrep();
}

void CStepCommandPrep::_init_AttachToStepCommandPrep()
{
    //x_r->_x_p = this;
}

void CStepCommandPrep::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToStepCommandPrep();

    rFAIL("make sure it is OK to expose CStepCommandPrep!");
}

str CStepCommandPrep::OnGetFixedStepTitle()
{
    return "Prepare .ini commands.";
}

void CStepCommandPrep::OnHandleActiveStepPolling()
{
    rASSERT(!x_bStepComplete);
    x_bStepComplete = true;

    ref<CProxyImpl> rProxy = 
        GetProxy();
    ref<CProtocolIface> rProtocol = 
        rProxy->x_rProtocol;

    path pathIniFolder = 
        sys::GGetTempFolder().
[...]

    x_sStepDetails += 
        RichBold("Read only:") + 
            " " +
            (rProtocol->x_bIniReadOnly?
                "yes"
                :
                "no"
            ) +
            "\n";

    rProtocol->
        WriteIniContextSection(
            pathIni);

    x_sReplaceStepTitle = 
        "Commands written.";
}

