// CForGameIfaceGp.h
#pragma once

class CFeedStateIfaceGp;
class CGameFactoryIfaceGp;

//
// CForGameIfaceGp -
//
//   SP Game calls Balance via this object.
//
ASSUME_IMPL_FOR_IFACE(CForGameImpl, CForGameIfaceGp)

class CForGameIfaceGp : public object
{
public:
    CForGameIfaceGp();
    //NEW_LOCAL(CForGameImpl, CForGameIfaceGp)
    NEW_GP(Balance, CForGameImpl, CForGameIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // Sync with games!
    static const int C_nNormalizedGameDurationSeconds = 90;

    // Create a temp oject to access this:
    //    ref<CForGameIfaceGp>()->GetOfflineGamesRoot()
    path GetOfflineGamesRoot()
            vhook;

    // For state transition notifications
    ref<CFeedStateIfaceGp> GetGameFeedState()
            vhook;

// Attributes

// Operations

    // Game result must be set terminating the game.
    //   See your impl for CCallGameIfaceGp::EndGame()
    void SaveGameScoreNum(
            num numScore)
            vhook;

    // when time runs out, I want game to be closed
    // rather than implement various modes
    void CloseCurrentGame()
            vhook;


    // Start internal Balance calculations
    void HandleGameStarted()
            vhook;

    // End Balance calculations
    void HandleGameStopped()
            vhook;

    // If trainings called from Patients they must limit the
    //   duration to normalize calculation time
    bool IsTrainingLimitedForPatient()
            vhook;

// UI

    // Common game controls
    ref<UiContainer> NewUiForGameControls()
            vhook;

    // Common game calculation results
    ref<UiContainer> NewUiForGameIndicators()
            vhook;

protected:
    virtual path OnGetOfflineGamesRoot()
            v1pure;
    virtual ref<CFeedStateIfaceGp> OnGetGameFeedState()
            v1pure;
    virtual void OnAddGlobalGameFactory(
            ref<CGameFactoryIfaceGp> rGameFactory)
            v1pure;
    virtual void OnSaveGameScoreNum(
            num numScore)
            v1pure;
    virtual void OnCloseCurrentGame()
            v1pure;
    virtual void OnHandleGameStarted()
            v1pure;
    virtual void OnHandleGameStopped()
            v1pure;
    virtual bool OnIsTrainingLimitedForPatient()
            v1pure;
    virtual ref<UiContainer> OnNewUiForGameControls()
            v1pure;
    virtual ref<UiContainer> OnNewUiForGameIndicators()
            v1pure;
private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToForGameIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
