// CProject.h

// Core System Libraries
#include <VirtOs.h>
#include "VirtOsStaticLibLink.h"

// Udb Support
#include "UdbBasedProject.h"

// IntellinonSense must sense standard header presence even in include absence
#ifdef ISENSE_PARSER
    #include "Precompiled.h"
#endif // ISENSE_PARSER

class CLogMapIface;
class CPatientEmbedLinkIface;
class CLoaderIface;
class CParaLoaderIface;
class CGlobalConfigIface;

//
// CProject - Low level project life cycle control
//

class CProject : public CGenericProject
{
public:
    // Access to the CProject object with proper initialization
    static CProject& GGetIt();
private:
    CProject(); // the object can only be constructed in TheProjectCallback
             //  this ensures proper CloudSync Project initialization in all cases
    friend void TheProjectCallback(out project_definition& out_oDefine);

public:

// Global Objects

    // This is only used as the root starter when this project is The Main Module
    //   in the application, or if it is running standalone for testing.
    //   In these cases the DLL is directly started with StartCloud.exe
    //   (when the project is linked as a secondary DLL its objects are created 
    //   with NEW_GP() only upon request from other modules)
    ref<CLoaderIface> x_rLoader;
    ref<CLoaderIface> x_rLoader_Get();

    // Alternate loader for ParaService mode
    ref<CParaLoaderIface> x_rParaLoader;
    ref<CParaLoaderIface> x_rParaLoader_Get();

    // Global module config data controller 
    //   (always exists as this single instance)
    ref<CGlobalConfigIface> x_rGlobalConfig;
    ref<CGlobalConfigIface> x_rGlobalConfig_Get();

    // Global event logging
    ref<CLogMapIface> x_rLogMap;
    ref<CLogMapIface> x_rLogMap_Get();

protected:
    virtual void OnInitProject(); // called always
    virtual void OnInitStandalone(); // called for standalone-mode only
    virtual void OnInitStandaloneFromInstaller();
    virtual void OnShutdownProject(); // clean up
    virtual ref<CConfigHookIfaceGp> OnNewProjectUniConfig();
    virtual ref<UiContainer> OnNewUiCces(
            ECcesSection eCcesSection);
    virtual CProject::EIdleNext OnHandleProjectIdle(
            CProject::EIdleStage eIdleStage);

private:

    // global object replacements for ClassTests
    #define REPLACE_IN_TESTS(PROJECT, PROPERTY) \
        PROPERTY(PROJECT, CLoaderIface, x_rLoader) \
        PROPERTY(PROJECT, CParaLoaderIface, x_rParaLoader) \
        PROPERTY(PROJECT, CLogMapIface, x_rLogMap) \

    // declare replacement refs
    DECLARE_REPLACE_IN_TESTS(CProject, REPLACE_IN_TESTS);

    // All main exe modes
    void InitStandaloneAppOrInstaller();
};

