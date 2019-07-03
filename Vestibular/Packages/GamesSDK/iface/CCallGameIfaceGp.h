// CCallGameIfaceGp.h
#pragma once

class CGameTypeIfaceGp;
class CForGameIfaceGp;

//
// CCallGameIfaceGp -
//
//   Balance calls games via this callback.
//

class CCallGameIfaceGp : public object
{
public:
    CCallGameIfaceGp();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Override and return true
    bool IsGameImplemented()
            vhook;

// Operations

    // Initialization
    //   - Called after the user has selected the game
    //       and before requesting the game UI.
    //   - Normally this is called only once per game object.
    //   - Save a ref to newly created rNewForGame to be able to
    //       call back to Balance to return the game score.
    //   - You should already know the type<> of your game, anyway you
    //       are given typeGameType to be able to verify the validity
    //       of the game selection you are associaed with.
    void OpenGame(
            ref<CForGameIfaceGp> rNewForGame,
            type<CGameTypeIfaceGp> typeGameType)
            vhook;

    // Termination notification. Allows the game to save
    //   final results via calls to CForGameIfaceGp.
    void EndGame()
            vhook;

    // Optionally can be called for some apps (HomeMon) after game init
    void SetGameDuration(
            num numSeconds)
            vhook;

    // All units are theoretical mm. It is not clear if a game should
    //   use them as is or scale them to percents of the platform size.
    //   This may change in the future.
    void ReceivePlatformCoordinateChange(
            num numKilo,
            num numPosX,
            num numPosY,
            num numMinX,
            num numMinY,
            num numMaxX,
            num numMaxY)
            vhook;

    // [Zero] button remembers current coordinate position as (0, 0)
    bool HandleZeroReset()
            vhook;

// UI

    // Main game UI
    ref<UiContainer> NewUiGameMain()
            vhook;

    // Settings dialog insertion
    ref<UiContainer> NewUiGameOptions()
            vhook;

protected:
    virtual bool OnIsGameImplemented()
            v1st
            return
                false;

    virtual void OnOpenGame(
            ref<CForGameIfaceGp> rNewForGame,
            type<CGameTypeIfaceGp> typeGameType)
            v1pure;
    virtual void OnEndGame()
            v1pure;
    virtual void OnSetGameDuration(
            num numSeconds)
            v1pure;
    virtual void OnReceivePlatformCoordinateChange(
            num numKilo,
            num numPosX,
            num numPosY,
            num numMinX,
            num numMinY,
            num numMaxX,
            num numMaxY)
            v1pure;
    virtual bool OnHandleZeroReset()
            v1pure;
    virtual ref<UiContainer> OnNewUiGameMain()
            v1pure;
    virtual ref<UiContainer> OnNewUiGameOptions()
            v1pure;
private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToCallGameIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
