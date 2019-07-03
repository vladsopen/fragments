// CDemoImpl.cpp

#include "CProject.h"
#include "CDemoImpl.h"
#include "CProtocolIface.h"
#include "VirtOs_text.h"
#include "VirtOs_virtfs.h"
#include "CDemoExitType.h"
#include "VirtOs_ask.h"
#include "VirtOs_file.h"
#include "CLogTypeIface.h"

CDemoImpl::CDemoImpl()
{
    _init_AttachToDemoImpl();
}

CDemoImpl::~CDemoImpl()
{
    rASSERT(!_m_bDemoImplOpened);
}

void CDemoImpl::_init_AttachToDemoImpl()
{
    //x_r->_x_p = this;
}

void CDemoImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToDemoImpl();

    rFAIL("make sure it is OK to expose CDemoImpl!");
}

void CDemoImpl::OnInitDemo()
{
    rASSERT(!_m_bOneTimeInitDemoImplOk);
    _m_bOneTimeInitDemoImplOk = true;
    _m_bDemoImplOpened = true;

    //FAIL_SOFT("Starting demo");

    str sError;

    path pathIni = GetCommandLineIni();
    if ok(pathIni != "")
    {
        x_rExternalProtocol->
            ReadIniContextSectionAsExternalApp(
                pathIni);
    }
    else
    {
        sError = "No .ini in command line. /ERR:NDE/";
    }

    // Create?
    if (x_rExternalProtocol->x_sIniOpenPrefix == "")
    {
        x_bCreatingNewSample = true;

        x_rExternalProtocol->x_sIniNewFilePrefix = 
            "DemoPrefix-" + moment::GGetLocal().ToStr("yymmdd-HHMMSS");

        x_typeSelectedExitType = 
            type<CDemoExitTypeIsCreateNew>();

        x_sDemoData = 
            "This is demo data (stored in a file).";
    }
    else
    {
        x_typeSelectedExitType = 
            type<CDemoExitTypeIsUpdatedVersion>();

        LoadDemoDataFile(out sError);
        rASSTR(sError); 
    }

    if (sError != "")
    {
        ReportDemoError(
            "Failed to initialize demo mode. /ERR:EMI/ ",
            sError);
    }
}

void CDemoImpl::OnCloseDemo()
{
    if (_m_bDemoImplOpened)
    {
        _m_bDemoImplOpened = false;
        // ...
    }
}

void CDemoImpl::PrepareDemoResultsAndExit()
{
    rASSERT(_m_bDemoImplOpened);

    //
    // Save results
    //

    str sError;
    x_typeSelectedExitType->
        MakeDemoExit(
            this,
            out sError);

    if (sError == "")
    {
        CloseDemo();

        sys::GPostQuitMessage();
    }
    else
    {
        ask("Cannot exit with the current setup.\n\n" + sError);
    }
}

path CDemoImpl::GetCommandLineIni()
{
    str sIni =
        sys::GGetCommandLineParameterList("").
            GetWord(
                CProtocolIface::C_iCommandLineIniPathAt,
                "");

    if (sIni != "")
    {
        path pathIni =
            virtfs::GVirtualizeLocalOrNetworkOsPath(
                (ospath) sIni);

        return pathIni;
    }

    return path();
}

bool CDemoImpl::OnIsExeStartedForDemo()
{
    return 
        sys::GIsCommandLineKeyPresent(
            CProtocolIface::C_sCommandLineProtocolKey);
}

bool CDemoImpl::OnIsDemoInitOk()
{
    return _m_bDemoImplOpened;
}

path CDemoImpl::GetDemoDataFilePath(
        str sPrefix,
        str sAttachmentRevision,
        out str& out_sError)
{
    path pathFolder = 
        x_rExternalProtocol->x_pathIniSampleFolder;

    if (pathFolder == "")
    {
        out_sError = "Sample directory is unidefined. /ERR:EDI/";
        return path();
    }
    
    if (sPrefix == "")
    {
        out_sError = "File prefix is unidefined. /ERR:EFP/";
        return path();
    }
    
    if (sAttachmentRevision == "")
    {
        sAttachmentRevision = "r0001";
    }

    path pathDataFile = 
        pathFolder.
            GetAppendedPath(
                sPrefix + 
                "-" + 
                sAttachmentRevision).
            GetAppendedPath(".txt");

    return pathDataFile;
}

void CDemoImpl::LoadDemoDataFile(
        out str& out_sError)
{
    path pathData = 
        GetDemoDataFilePath(
            x_rExternalProtocol->x_sIniOpenPrefix,
            x_rExternalProtocol->x_sIniUseFileRevision,
            out out_sError);

    if (out_sError != "")
    {
        return;
    }

    if ok(pathData != "")
    {
        str sLoaded;
        file(pathData).Load(out sLoaded, out out_sError);
        rASSTR(out_sError); 

        x_sDemoData = 
            sLoaded;
    }
    else
    {
        out_sError = "Internal error getting demo data path. /ERR:EPL/";
    }
}

void CDemoImpl::SaveDemoDataFile(
        out str& out_sError)
{
    path pathData = 
        GetDemoDataFilePath(
            x_rExternalProtocol->x_sIniNewFilePrefix.Get().
                GetStub(
                    x_rExternalProtocol->x_sIniOpenPrefix),
            x_rExternalProtocol->x_sIniUpdatedFileRevision,
            out out_sError);

    if (out_sError != "")
    {
        return;
    }

    if ok(pathData != "")
    {
        str sLoaded;
        file(pathData).Save(x_sDemoData, out out_sError);
        rASSTR(out_sError); 
    }
    else
    {
        out_sError = "Internal error getting demo data path. /ERR:EPS/";
    }
}

void CDemoImpl::ReportDemoError(
        sloc slocMain,
        str sDetails)
{
    type<CLogTypeIface>()->
        AsLogTypeForDemoError()->
            LogEvent(
                slocMain,
                sDetails);
}

ref<UiContainer> CDemoImpl::OnNewUiDemoMain()
{
    ref<UiDemoMain> rUi;

    rUi->m_rDemo = this;

    return rUi;
}

ref<UiContainer> CDemoImpl::NewUiDemoControl()
{
    ref<UiDemoControl> rUi;

    rUi->m_rDemo = this;

    return rUi;
}

ref<UiContainer> CDemoImpl::NewUiDemoExitRadio(
        type<CDemoExitType> typeExitType)
{
    ref<UiDemoExitRadio> rUi;

    rUi->m_rDemo = this;
    rUi->m_typeExitType = typeExitType;

    return rUi;
}

ref<UiContainer> CDemoImpl::NewUiDemoExitSetupCreate()
{
    ref<UiDemoExitSetupCreate> rUi;

    rUi->m_rDemo = this;

    return rUi;
}

ref<UiContainer> CDemoImpl::NewUiDemoExitSetupUpdate()
{
    ref<UiDemoExitSetupUpdate> rUi;

    rUi->m_rDemo = this;

    return rUi;
}

ref<UiContainer> CDemoImpl::NewUiDemoExitSetupNop()
{
    ref<UiDemoExitSetupNop> rUi;

    rUi->m_rDemo = this;

    return rUi;
}

ref<UiContainer> CDemoImpl::NewUiDemoExitSetupFail()
{
    ref<UiDemoExitSetupFail> rUi;

    rUi->m_rDemo = this;

    return rUi;
}

ref<UiContainer> CDemoImpl::NewUiDemoData()
{
    ref<UiDemoData> rUi;

    rUi->m_rDemo = this;

    return rUi;
}
