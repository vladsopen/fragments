// CQuestHandler.h
#pragma once

class CSenderSessionImpl;
class CFeedIoProtocol;
class CNetLocatorIfaceGp;
class CNetDriverIfaceGp;
class CNetPumpInIfaceGp;
class CSendAlarm;

//
// CQuestHandler - 
//
//   Receives and responds to commands sent to the device
//
//ASSUME_IMPL_FOR_IFACE(CQuestHandlerImpl, CQuestHandler)

class CQuestHandler : public object
{
public:
    CQuestHandler();
    ~CQuestHandler();
    //NEW_LOCAL(CQuestHandlerImpl, CQuestHandler)
    //NEW_GP(Ralarm, CQuestHandlerImpl, CQuestHandler)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CSendAlarm> _x_pSendAlarm
            xauto(Get, Set);
    ref<CSendAlarm> GetSendAlarm()
            return
                _x_pSendAlarm.Get().Valid();
    ref<CSenderSessionImpl> GetSenderSession()
            return
                GetSendAlarm()->GetSenderSession();

    // Quest uploader
    ref<CNetPumpInIfaceGp> x_rNetPumpInQuest
            xauto(Get);

    // Online mode STOP flag
    str x_sLastReceivedSnapshotTaskId = ""
            xauto(Get, Set)
            xassert(true);

    // Online mode STOP flag
    str x_sSnapshotStoppedForTaskId = ""
            xauto(Get, Set)
            xassert(true);

    // A debug counter
    int x_nSentSnapshots = 0
            xauto(Get, Set)
            xassert(value >= 0);

    // Error status
    bool x_bWeRespondedToLastQuery = false
            xauto(Get, Set);

    // Reports
    str x_sSnapshotServerStatus = "No Response"
            xauto(Get, Set)
            xassert(true);
    str x_sSnapshotDeviceStatus = "Undefined"
            xauto(Get, Set)
            xassert(true);

    // View received config
    str x_sReceivedConfigReport = "(waiting config)"
            xauto(Get, Set)
            xassert(true);

// Operations

    // One-time opener
    void InitQuestHandler();

    // Cleanup
    void CloseQuestHandler();

    // Poll
    void UpdateQuestPolling();

    // Callback
    void ReceiveDownloadedQuest(
            unidef unidefFeed,
            out str& out_sRaiseNetError);

// UI

    // QuestHandlerStatus
    ref<UiContainer> NewUiQuestHandlerStatus();
    ptrmap<CUiPanelGp> _m_ptrmapUiQuestHandlerStatus;
    void InvalidateStatusState();

protected:
private:
    bool _m_bOneTimeInitQuestHandlerOk = false;
    bool _m_bQuestHandlerOpened = false;

    ref<CNetDriverIfaceGp> _m_rCacheQuestDownloadDriver;
    ref<CNetLocatorIfaceGp> _m_rCacheQuestDownloadLocator;
    ref<CFeedIoProtocol> m_rFeedIoProtocol;

    friend class CTestSendReceive;

    void _init_AttachToQuestHandler();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiQuestHandlerStatus.cpp)
class UiQuestHandlerStatus : public UiContainer
{
public:
    virtual void Ui();

    ref<CQuestHandler> m_rQuestHandler;

private:
};
