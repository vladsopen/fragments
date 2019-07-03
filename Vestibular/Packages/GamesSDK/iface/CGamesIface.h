// CGamesIface.h
#pragma once

class CGameTypeIface;
class CGameStateIface;
class CRangesIface;
class CReadingIface;
class CPatientSampleIfaceGp;
class CSessionIface;

//
// CGamesIface -
//
//   Game manager.
//
ASSUME_IMPL_FOR_IFACE(CGamesImpl, CGamesIface)

class CGamesIface : public object
{
public:
    CGamesIface();
    NEW_LOCAL(CGamesImpl, CGamesIface)
    //NEW_GP(Balance, CGamesImpl, CGamesIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CSessionIface> _x_pSession
            xauto(Get, Set);

    // Replace main UI
    bool IsGameInMainWindow()
            vhook;

    // For HomeMon embedded
    bool x_bAllowGameCloseApp = true
            xauto(Get, Set);

// Operations

    // Online stream
    void ReceiveReadingForGames(
            ref<CReadingIface> rReading,
            ref<CRangesIface> rRanges)
            vhook;

    // Optionally can be called for some apps (HomeMon) after game init
    void SetAllGamesDuration(
            num numSeconds)
            vhook;

    // clean up
    void CloseGames()
            vhook;

    // OnClose
    void ExportAllGameScoresToPatient(
            ref<CPatientSampleIfaceGp> rPatientSample)
            vhook;

    // Open and init a game
    void OpenSelectedGame(
            type<CGameTypeIface> typeGameType)
            vhook;

// UI

    ref<UiContainer> NewUiGamesOptions(
            ptr<CGameStateIface> pGameState)
            vhook;

    ref<UiContainer> NewUiGamesMain()
            vhook;

    ref<UiContainer> NewUiGamesMenu()
            vhook;

    ref<UiContainer> NewUiGamesButton()
            vhook;

protected:
    virtual bool OnIsGameInMainWindow()
            v1pure;
    virtual void OnSetAllGamesDuration(
            num numSeconds)
            v1pure;
    virtual void OnReceiveReadingForGames(
            ref<CReadingIface> rReading,
            ref<CRangesIface> rRanges)
            v1pure;
    virtual void OnExportAllGameScoresToPatient(
            ref<CPatientSampleIfaceGp> rPatientSample)
            v1pure;
    virtual void OnOpenSelectedGame(
            type<CGameTypeIface> typeGameType)
            v1pure;
    virtual ref<UiContainer> OnNewUiGamesOptions(
            ptr<CGameStateIface> pGameState)
            v1pure;
    virtual ref<UiContainer> OnNewUiGamesMain()
            v1pure;
    virtual ref<UiContainer> OnNewUiGamesMenu()
            v1pure;
    virtual ref<UiContainer> OnNewUiGamesButton()
            v1pure;
    virtual void OnCloseGames()
            v1pure;
private:
    //bool _m_bOneTimeInitGamesIfaceOk = false;

    void _init_AttachToGamesIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
