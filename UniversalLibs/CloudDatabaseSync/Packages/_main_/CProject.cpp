// CProject.cpp

#define IMPLEMENT_PROJECT
#define VERIFY_WHOLE_PROGRAM_OPTIMIZATION VERIFY_PROJECT(CloudSync)
#include "CProject.h"
//(after-main-include-in-main-cpp-CloudSync)

#include "CLoaderIface.h"
#include "UniversalTouch.h"
#include "Shared_res.hxx"
#include "VirtOs_CAppGp.h"
#include "CConfigHookMapIfaceGp.h"
#include "CGlobalConfigIface.h"
#include "VirtOs_ask.h"
#include "CUvOptionsIfaceGp.h"
#include "VirtOs_CGlobalUi.h"
#include "CLogMapIface.h"
#include "CParaLoaderIface.h"

// implement test replacement for global properties
IMPLEMENT_REPLACE_IN_TESTS(CProject, REPLACE_IN_TESTS);

void TheProjectCallback(out project_definition& out_oDefine)
{
    //
    // Project Attribute Definition
    // (be careful: nothing is initialized here:
    //  advanced core services may NOT be initialized)
    //

    out_oDefine.m_szProjectLogo = "CloudSync";
    DEFINE_PROJECT_ATTRIBUTES();

    out_oDefine.m_fAppOption |= 0 |
        //project_definition::F_AppOption_UniViewFromRegistryOnly : 0)

        // all apps using UniConfig should at least enable the warning:
        project_definition::F_AppOption_ConfigMissingWarning | 

        0;

    if (out_oDefine.m_bAllocateProject)
    {
        CProject* pProject = new CProject;
        out_oDefine.m_pProject = pProject;
    }
}

CProject::CProject()
{
    // NOTE: Be careful - provide only the simplest variable initialization here,
}

DEFINE_GGetIt(CloudSync);

void CProject::OnInitProject()
{
    //
    // Common Initialization
    //  (after all system libraries are ready)
    //

    x_rParaLoader->TryInitAsParaServiceProcess();
    if (!x_rParaLoader->IsStartedAsParaService())
    {
        // Dependencies only for main non-ParaService app
        //PRELOAD_PROJECT(Xxxx);
    }

    // Common Dependencies
    PRELOAD_PROJECT(Net);
    PRELOAD_PROJECT(Ubro);

    //
    // BEFORE you create any application objects depending on x_rGlobalConfig:
    //
    // This is the only sure and safe place to register our config into
    //   the global config registry.
    MapProjectConfigIntoProcessConfig();

    //
    // Now you can init app objects here
    //

    // ...
}

void CProject::OnInitStandalone()
{
    //
    // Standalone "AutoloadDll" Mode Initialization
    //  (after OnInitProject())
    //

    // Common init as main app
    InitStandaloneAppOrInstaller();

    // Register our UniConfig as the app skin list source
    CAppGp::GGet()->
        x_rUvOptions->
            RegisterMainAppConfigForUvOptions(
                x_rGlobalConfig);

    // Don't start ParaService UI until requested
    //   and don't start the Session ever!
    if (!x_rParaLoader->IsStartedAsParaService())
    {
        // Create main UI and start running as an application
        x_rLoader->StartApplication();
    }
}

void CProject::OnInitStandaloneFromInstaller()
{
    // Common init as main app
    InitStandaloneAppOrInstaller();
}

void CProject::InitStandaloneAppOrInstaller()
{
    // App logo
    //CAppGp::GGet()->x_rUi->
    //    x_sAppLogoForUi = 
    //        "Xxxx";
}

void CProject::OnShutdownProject()
{
    //
    // Clean up code
    //  (while system libraries are still alive)
    //

    x_rParaLoader->ShutdownParaServiceLoader();
}

ref<CConfigHookIfaceGp> CProject::OnNewProjectUniConfig()
{
    return ref<CGlobalConfigIface>();
}

ref<CGlobalConfigIface> CProject::x_rGlobalConfig_Get()
{
    return 
        Derive<CGlobalConfigIface>(GetProjectUniConfig()).
            Valid();
}

ref<UiContainer> CProject::OnNewUiCces(
        ECcesSection eCcesSection)
{
    //
    // Project-Global CCES Support
    //

    if (eCcesSection == E_CcesSection_MainPublic)
    {
        return x_rGlobalConfig->NewUiCces()
    }
    else
    {
        return __super::OnNewUiCces(eCcesSection);
    }
}

CProject::EIdleNext CProject::OnHandleProjectIdle(
        CProject::EIdleStage eIdleStage)
{
    NOTEST;

    if (eIdleStage == CProject::E_IdleStage_BeforeWindowUpdates)
    {
        // too high priority handler, we may slow down or suppress UI updates!
    }
    else
    if (eIdleStage == CProject::E_IdleStage_AfterSysUpdatesComplete)
    {
        // only one client now (see notes at declaration)

        // note: _x_pRealHardwareToHandleRealIdlesInStandaloneUscan pointer
        //       is only initialized in Uscan standalone mode by the AppLoader.
        //       Otherwise we rely on the client Idle calls.

        CProject::EIdleNext eIdleNext = CProject::E_IdleNext_StandBy;

        x_rLoader->
            HandleLoaderIdle(
                out eIdleNext);

        return eIdleNext;
    }
    else
    {
        rFAIL("unknown new eIdleStage, please fix this project too");
    }

    // (note: return E_IdleNext_NeverCallAgain and you will never have 
    //        this function called again)

    return CProject::E_IdleNext_StandBy;
}
