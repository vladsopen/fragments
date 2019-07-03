// CStepExe.cpp

#include "CProject.h"
#include "CStepExe.h"
#include "CInterprocRunProcessIfaceGp.h"
#include "CProxyImpl.h"
#include "CProtocolIface.h"
#include "VirtOs_virtfs.h"
#include "VirtOs_file.h"
#include "CPatientBaseConnectionIfaceGp.h"
#include "VirtOs_CAppGp.h"
#include "VirtOs_CGlobalUi.h"

CStepExe::CStepExe()
{
    _init_AttachToStepExe();
}

void CStepExe::_init_AttachToStepExe()
{
    //x_r->_x_p = this;
}

void CStepExe::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToStepExe();

    rFAIL("make sure it is OK to expose CStepExe!");
}

str CStepExe::OnGetFixedStepTitle()
{
    return "Launch the modality.";
}

void CStepExe::OnHandleActiveStepPolling()
{
    rASSERT(!x_bStepComplete);

    ref<CProxyImpl> rProxy = 
        GetProxy();
    ref<CInterprocRunProcessIfaceGp> rProcess = 
        GetProxy()->x_rRunExternalApp;
    rASSERT(!rProcess->IsProcessAlive());
    rASSERT(!rProcess->IsProcessComplete());

    rich richReport;
    path pathExe = 
        CheckExeReady(
            out richReport);

    x_sStepDetails = richReport;

    if (pathExe == "")
    {
        return;
    }

    // exe path
    rProcess->
        x_pathRunExe = 
            pathExe;

    path pathIni = 
        GetProxy()->x_pathProtocolIni;

    if (pathIni == "")
    {
        GetProxy()->
            ReportProxyError(
                "Missing .ini, cannot execute the application. /ERR:XIN/",
                "(exe-step stall)");

        return;
    }

    ospath ospathIni;
    str sAccessError; 
    if (!virtfs::GPermitPath(
            virtfs::F_Permit_FullExternalAppAccess,
            virtfs::F_Permit_none,
            virtfs::F_Permit_none,
            pathIni,
            out ospathIni,
            out sAccessError))
    {
        rFAIL(sAccessError);

        GetProxy()->
            ReportProxyError(
                "Exec denied. /ERR:XVF/ ",
                sAccessError);

        return;
    }

    rASSERT(!x_bStepComplete);
    x_bStepComplete = true;

    // args 
    str sProcessArgs = 
        CProtocolIface::C_sCommandLineProtocolKey +
            " " +
            "\"" + ospathIni + "\"";
            
    //rProcess->
    //    x_sAppendParaServiceCommandLine = 
    // irrelevant to non-service

    // for msiexec.exe
    // ?
    //rProcess->
    //    x_bRunStandardExeWithMagicSearch = 
    //        x_bMagicSearch;
            
    // for msiexec.exe
    //rProcess->
        //x_bHideConsoleWindow = false; //true;
            
    //
    // Do start
    //

    str sRunError;
    {
        //**********************************************************
        //sync:_TEMPORARY_UNVIRT_EXE_ until autodetect implemented
        //**********************************************************
        virtfsbypass virtfsbypassRunAnythingFromDebugInput;

        rProcess->
            StartProcess(
                " " + sProcessArgs, // the command line
                out sRunError);
    }

    if (sRunError == "")
    {
        x_sReplaceStepTitle = 
            "Process started.";
    }
    else
    {
        x_sReplaceStepTitle = 
            "Process failed to start.";

        GetProxy()->
            ReportProxyError(
                x_sReplaceStepTitle + " /ERR:XER/ ",
                sRunError);
    }
}

path CStepExe::CheckExeReady(
        out rich& out_richReport)
{
    ref<CProxyImpl> rProxy = 
        GetProxy();

    path pathExe = rProxy->x_pathExternalExe;

    {
        //**********************************************************
        //sync:_TEMPORARY_UNVIRT_EXE_ until autodetect implemented
        //**********************************************************
        virtfsbypass virtfsbypassRunAnythingFromDebugInput;

        if (pathExe == ""
            ||
            !file(pathExe).IsExist())
        {
[...]
        }
    }

    if (pathExe == "")
    {
        out_richReport = 
            RichError("Executable is not selected.");
        return path();
    }

    if (pathExe.GetFileExt().GetLower() != ".exe")
    {
        out_richReport = 
            RichErrorBold("Executable is not an .exe file.");
        return path();
    }

    out_richReport = 
        RichSuccessBold("Executable ready.");

    return pathExe;
}

ref<UiContainer> CStepExe::OnNewUiStepAdvanced()
{
    return NewUiStepExeAdvanced();
}

ref<UiContainer> CStepExe::NewUiStepExeAdvanced()
{
    ref<UiStepExeAdvanced> rUi;

    rUi->m_rStepExe = this;

    return rUi;
}
