// CProject.h

// Core System Libraries
#include <VirtOs.h>
#include "VirtOsStaticLibLink.h"

// Udb Support
#define UDB_LEGACY
#include "UdbBasedProject.h"

// Application Project Class Naming Rules
DECLARE_PROJECT_CLASS_PREFIX(Balance);
DECLARE_NEXT_APP_PREFIX(PatientEmbededModule); // fixed naming

// IntellinonSense must sense standard header presence even in include absence
#ifdef ISENSE_PARSER
    #include "Precompiled.h"
#endif // ISENSE_PARSER

// Const
#include "SharedConst.h"

class CSessionIface;
class CLoaderIface;
class CGlobalConfigIface;
class CPatientEmbedLinkIface;
class CLogMapIface;
class CPatientEmbedLinkIface;

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
             //  this ensures proper Balance Project initialization in all cases
    friend void TheProjectCallback(out project_definition& out_oDefine);

public:

// Global Objects

    // This is only used as the root starter when this project is The Main Module
    //   in the application, or if it is running standalone for testing.
    //   In these cases the DLL is directly started with Start.exe
    //   (when the project is linked as a secondary DLL its objects are created 
    //   with NEW_GP() only upon request from other modules)
    ref<CLoaderIface> x_rLoader;
    ref<CLoaderIface> x_rLoader_Get();

    // Global module config data controller 
    //   (always exists as this single instance)
    ref<CGlobalConfigIface> x_rGlobalConfig;
    ref<CGlobalConfigIface> x_rGlobalConfig_Get();

    // Global event logging
    ref<CLogMapIface> x_rLogMap;
    ref<CLogMapIface> x_rLogMap_Get();
    
    // PatientEmbed is a global service separated from all other application modes.
    //   It has its own Session inside.
    ref<CPatientEmbedLinkIface> x_rPatientEmbedLink;
    ref<CPatientEmbedLinkIface> x_rPatientEmbedLink_Get();

    // To show active sessions in CCES
    ptrmap<CSessionIface> _m_ptrmapDebugSessionTrace;

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
    virtual void OnHandleAppActivationStateChange(
            EActivationStateChange eActivationStateChange);

private:

    // global object replacements for ClassTests
    #define REPLACE_IN_TESTS(PROJECT, PROPERTY) \
        PROPERTY(PROJECT, CLoaderIface, x_rLoader) \
        /* // ----- required only for PatientLink ----- */ \
        PROPERTY(PROJECT, CPatientEmbedLinkIface, x_rPatientEmbedLink) \
        /* // */ \
        PROPERTY(PROJECT, CLogMapIface, x_rLogMap) \

    // declare replacement refs
    DECLARE_REPLACE_IN_TESTS(CProject, REPLACE_IN_TESTS);

    // Config init
    void MapModuleConfigIntoProcessConfig();

    // For both app and installer
    void InitCommonStandalone();
};

