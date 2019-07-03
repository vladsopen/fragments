// CGameRegistryImpl.cpp

#include "CProject.h"
#include "CGameRegistryImpl.h"
#include "CGlobalConfigIface.h"
#include "CGameFactoryIfaceGp.h"
#include "CLoaderIface.h"
#include "CCallGameIfaceGp.h"
#include "CGameTypeIface.h"
#include "CSessionIface.h"
#include "CSessionIface.h"
#include "CDeviceIface.h"
#include "CGamesIface.h"
#include "VirtOs_file.h"
#include "CAppTypeIface.h"

CGameRegistryImpl::CGameRegistryImpl()
{
    _init_AttachToGameRegistryImpl();
}

void CGameRegistryImpl::_init_AttachToGameRegistryImpl()
{
    //x_r->_x_p = this;
}

void CGameRegistryImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToGameRegistryImpl();

    rFAIL("make sure it is OK to expose CGameRegistryImpl!");
}

void CGameRegistryImpl::OnAddGlobalGameFactory(
        ref<CGameFactoryIfaceGp> rGameFactory)
{
    //
    // Redirect call to the global object
    //
    ref<CGlobalConfigIface> rGlobalConfig = 
        CProject::GGetIt().x_rGlobalConfig;
    ref<CGameRegistryImpl> rGlobalGameRegistry = 
        (ref<CGameRegistryIface>) rGlobalConfig->_x_rGameRegistry;
    if (this != rGlobalGameRegistry)
    {
        rGlobalGameRegistry->
            AddGlobalGameFactory(
                rGameFactory);

        return;
    }

    rASSERT(m_aGameFactory.Find(rGameFactory) == -1);

    Add(rGameFactory);
}

void CGameRegistryImpl::OnSetToGameMode(
        bool bGame,
        type<CGameTypeIfaceGp> typeForceSingleGameType)
{
    //
    // Redirect call to the global object
    //
    ref<CGlobalConfigIface> rGlobalConfig = 
        CProject::GGetIt().x_rGlobalConfig;
    ref<CGameRegistryImpl> rGlobalGameRegistry = 
        (ref<CGameRegistryIface>) rGlobalConfig->_x_rGameRegistry;
    if (this != rGlobalGameRegistry)
    {
        rGlobalGameRegistry->
            SetToGameMode(
                bGame,
                typeForceSingleGameType);

        return;
    }

    if (debug::GIsTest())
    {
        return;
    }

    rASSERT(bGame);

    CProject::GGetIt().
        x_rLoader->
            // conditional init depends on PatientLink status
            //InitLoader();
            StartApplication();
}

void CGameRegistryImpl::OnHandleAppMainWindowInitComplete()
{
    //
    // Redirect call to the global object
    //
    ref<CGlobalConfigIface> rGlobalConfig = 
        CProject::GGetIt().x_rGlobalConfig;
    ref<CGameRegistryImpl> rGlobalGameRegistry = 
        (ref<CGameRegistryIface>) rGlobalConfig->_x_rGameRegistry;
    if (this != rGlobalGameRegistry)
    {
        rGlobalGameRegistry->
            HandleAppMainWindowInitComplete();

        return;
    }

    // in gaming?
    if (!CProject::GGetIt().
            x_rGlobalConfig->
                _x_bGameModeApp)
    {
        return;
    }

    ref<CGamesIface> rGames =
        CProject::GGetIt().
            x_rLoader->
                x_rSession->
                    x_rGames;

    // already open?
    if (rGames->IsGameInMainWindow())
    {
        return;
    }

    str sError;
    str sGameStart;
    file(
        sys::GGetTempFolderUntestedRoot().
            GetAppendedPath("GameStart.txt")).
        Load(out sGameStart, out sError);

    // go to a specific game?
    type<CGameTypeIface> typeIterGameType;
    for (
        iter i;
        type<CGameTypeIface>()->GetDerivedListIface().
            Iterate(out i, out typeIterGameType);)
    {
        str sKey = "--game:" + typeIterGameType->GetGameKey();
        if (sys::GIsCommandLineKeyPresent(sKey)
            ||
            sGameStart.FindPos(sKey) >= 0)
        {
            rGames->
                OpenSelectedGame(
                    typeIterGameType);

            break;
        }
    }
}

ref<UiContainer> CGameRegistryImpl::OnNewUiMainForGames()
{
    //
    // Redirect call to the global object
    //
    ref<CGlobalConfigIface> rGlobalConfig = 
        CProject::GGetIt().x_rGlobalConfig;
    ref<CGameRegistryImpl> rGlobalGameRegistry = 
        (ref<CGameRegistryIface>) rGlobalConfig->_x_rGameRegistry;
    if (this != rGlobalGameRegistry)
    {
        return
            rGlobalGameRegistry->
                NewUiMainForGames();
    }

    return 
        CProject::GGetIt().
            x_rLoader->
                NewUiMainWindow();
}

void CGameRegistryImpl::OnEnableKeyboardSimulation(
        bool bSimulate)
{
    //
    // Redirect call to the global object
    //
    ref<CGlobalConfigIface> rGlobalConfig = 
        CProject::GGetIt().x_rGlobalConfig;
    ref<CGameRegistryImpl> rGlobalGameRegistry = 
        (ref<CGameRegistryIface>) rGlobalConfig->_x_rGameRegistry;
    if (this != rGlobalGameRegistry)
    {
        return
            rGlobalGameRegistry->
                EnableKeyboardSimulation(
                    bSimulate);
    }

    rGlobalConfig->_x_bKeyboardSimulation = bSimulate;

    (Derive<CSessionIface>(CProject::GGetIt().
        x_rLoader->
            x_rSession)).Valid()->
                x_rDevice->
                    HandleKeyboardEmulationConfigChange();
}

ref<CCallGameIfaceGp> CGameRegistryImpl::OnNewCallGameFromFactories(
        type<CGameTypeIface> typeGameType)
{
    // find a factory that instantiates a valid impl for the game

    ref<CGameFactoryIfaceGp> rIterGameFactory;
    bool bGameImplAlreadyFoundInAnotherFactory = false;
    ref<CCallGameIfaceGp> rFoundNewCallGame;
    for (
        iter i;
        this->
            Iterate(
                out i, 
                out rIterGameFactory);)
    {
        ref<CCallGameIfaceGp> rNewCallGame = 
            rIterGameFactory->
                NewGameOrNull(
                    typeGameType);

        if (rNewCallGame->IsGameImplemented())
        {
            rASSERT(!bGameImplAlreadyFoundInAnotherFactory);
            bGameImplAlreadyFoundInAnotherFactory = true;

            rFoundNewCallGame = rNewCallGame;
        }
    }

    rASSERT(bGameImplAlreadyFoundInAnotherFactory);

    return rFoundNewCallGame;
}

