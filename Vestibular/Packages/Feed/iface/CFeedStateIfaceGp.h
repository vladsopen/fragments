// CFeedStateIfaceGp.h
#pragma once

//
// CFeedStateIfaceGp -
//
//    Current recording AND/OR game state for the main app (HomeMon)
//
//ASSUME_IMPL_FOR_IFACE(CFeedStateImpl, CFeedStateIfaceGp)

class CFeedStateIfaceGp : public object
{
public:
    CFeedStateIfaceGp();
    //~CFeedStateIfaceGp();
    //NEW_LOCAL(CFeedStateImpl, CFeedStateIfaceGp)
    //NEW_GP(Balance, CFeedStateImpl, CFeedStateIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Stable minimal platform pressure is detected
    //num x_numCurrentGameScore = 0
    //        xauto(Get, Set),
    //        xassert(value >= 0);
    // see FeedIndex() with AsIndexTypeGameScore()

// Operations

    // Game, math calculation or graph recording is in progress.
    //   Must be called after initial pause if a game/recording is really started.
    void HandleProcessingStarted()
            vhooked;

    // After completed or aborted session
        enum EWhyStop
        {
            // Normal termination for time-limited game or math processing
            E_WhyStop_OutOffTime,

            // Optional [Stop] button or external game window closed
            E_WhyStop_UiAbort,

            // No patient pressure detected
            //   and AskAbortNoValidWeight() returned true
            E_WhyStop_NoValidWeight,
        };
    void HandleProcessingStopped(
            EWhyStop eWhyStop)
            vhooked;

    // No patient weight pressure detected, or device disconnect.
    //    Caller should abort if handler returns true or continue otherwise.
    bool AskAbortNoValidWeight()
            vhooked;

    // Continuous Restart hack
    void HandleProcessingRestarted()
            vhooked;

    // stage changed: opened eyes -> closed eyes (Classic only)
    void HandleNextClassicStageStarted()
            vhook;

// UI

protected:

    virtual void OnHandleProcessingStarted()
            v1null;

    virtual void OnHandleProcessingStopped(
            EWhyStop eWhyStop)
            v1null;

    virtual bool OnAskAbortNoValidWeight()
            v1st
            return
                true;

    virtual void OnHandleProcessingRestarted()
            v1null;

    virtual void OnHandleNextClassicStageStarted()
            v1null;

private:
    //bool _m_bOneTimeInitFeedStateIfaceGpOk = false;
    //bool _m_bFeedStateIfaceGpOpened = false;

    // ASSERT support
    bool m_bStarted = false;
    bool m_bNoWeightAsked = false;

    void _init_AttachToFeedStateIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
