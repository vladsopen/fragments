// CSendAlarm.h
#pragma once

#include "VirtOs_ontimer.h"

class CQuestHandler;
class CSenderSessionImpl;
class CTaskIface;

//
// CSendAlarm - 
//
//   GSM sender emulator
//
//ASSUME_IMPL_FOR_IFACE(CSendAlarmImpl, CSendAlarm)

class CSendAlarm : public object
{
public:
    CSendAlarm();
    ~CSendAlarm();
    //NEW_LOCAL(CSendAlarmImpl, CSendAlarm)
    //NEW_GP(Ralarm, CSendAlarmImpl, CSendAlarm)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CSenderSessionImpl> _x_pSenderSession
            xauto(Get, Set);
    ref<CSenderSessionImpl> GetSenderSession()
            return
                _x_pSenderSession.Get().Valid();

    // Emulator file
    big x_nnFlashFileBytes = 0 //10 * 1024 * 1024
            xauto(Get, Set)
            xassert(value >= 0);
    path x_pathFlashFile = path()
            xauto(Get)
            xassert(true);
    str x_sFlashData = ""
            xauto(Get, Set)
            xassert(true);
    void RenderFlashEcg();

    // Nice debug crap
    bool x_bAsciiArt = debug::GIsTest()
            xauto(Get, Set);
    path x_pathSelectedEmulator = "emu.dat"
            xauto(Get, Set);
    path GetFullEmulatorFilePath();
    static path GGetDistroAlarmEmulatorPath();
    big x_iiNextBlockInEmulator = 0
            xauto(Get, Set)
            xassert(value >= 0);

    // Emulate incremental parts repetitively
    int x_nRepeatSend = 0
            xauto(Get, Set)
            xassert(value >= 0);
    int x_nTodoRepeatSend = 0
            xauto(Get, Set)
            xassert(value >= 0);
    big x_iiNextContentOffset = 0
            xauto(Get, Set)
            xassert(value >= 0);
    int x_nRepeatBlockBytes = 4096 * 4
            xauto(Get, Set)
            xassert(value >= 0);
    num x_numRepeatPeriod = 2
            xauto(Get, Set)
            xassert(value >= 0);
    utc x_utcLastSend = utc()
            xauto(Get, Set);

    // Depends on the package size
    int x_nMultipartSplitBytes = -1
            xauto(Get, Set)
            xassert(value > 0);

    // Current state
    ref<CTaskIface> x_rCurrentTask
            xauto(Get);

    // Last alarm info
    str _x_sLastSentTaskId = ""
            xauto(Get, Set);
    res _x_resLastSentIcon = res()
            xauto(Get, Set)
            xassert(true);

    // Quest uploader
    ref<CQuestHandler> x_rQuestHandler
            xauto(Get);

// Operations

    // One-time opener
    void InitSendAlarm();

    // Cleanup
    void CloseSendAlarm();

    // Prepare data
    str CreateAlarmUploadFile(
            str sTaskId);

    // Upload as alarm
    void UploadAlarmFileOrAppend(
            bool bAppend);

    // UI test button
    bool GoSendAlarm(
            str sTaskId,
            str sForceAlarmId,
            sloc slocWhat,
            res resWhatIcon,
            int nExtraFlash);

// UI

    // SendAlarmSetup
    ref<UiContainer> NewUiSendAlarmSetup();
    ptrmap<CUiPanelGp> _m_ptrmapUiSendAlarmSetup;
    ptrmap<CUiPanelGp> _m_ptrmapUiSenderSessionMain;
    ptrmap<CUiPanelGp> _m_ptrmapUiEcgBox;
    ptrmap<CUiEditGp> _m_ptrmapUiEcgEdit;
    ptrmap<CUiActionGp> _m_ptrmapUiGoSendButton;
    ptrmap<CUiActionGp> _m_ptrmapUiLastSent;
    void InvalidateSendAlarmSetup();

protected:
private:
    bool _m_bOneTimeInitSendAlarmOk = false;
    bool _m_bSendAlarmOpened = false;

    str m_sLastSeenAuthDevice = "";

    void HandleAlarmUploadTimer();
    ontimer<HandleAlarmUploadTimer> _m_ontimerAlarmUpload;

    void UpdateAuthDevice();

    // Quest alert
    void FlashGo(
            out ptrmap<CUiPanelGp>& out_ptrmapUiSendAlarmSetup,
            out ptrmap<CUiActionGp>& out_ptrmapUiGoSendButton,
            sloc slocWhat,
            res resWhatIcon,
            int nExtraFlash);

    friend class CTestSendReceive;

    void _init_AttachToSendAlarm();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiSendAlarmSetup.cpp)
class UiSendAlarmSetup : public UiContainer
{
public:
    virtual void Ui();

    ref<CSendAlarm> m_rSendAlarm;

private:
};
