// CFlowSessionIface.h
#pragma once

class CPatientSampleIfaceGp;
class CPatientLinkIface;
class CSessionIface;

//
// CFlowSessionIface -
//
//   Manages checkup procedure by either FlowConfig or by single procedure config.
//
ASSUME_IMPL_FOR_IFACE(CFlowSessionImpl, CFlowSessionIface)

class CFlowSessionIface : public object
{
public:
    CFlowSessionIface();
    NEW_LOCAL(CFlowSessionImpl, CFlowSessionIface)
    //NEW_GP(Balance, CFlowSessionImpl, CFlowSessionIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CSessionIface> _x_pSession
            xauto(Get, Set);

    // For Patients indicator
    int GetCurrentStageNumber()
            vhook;

    // Timing dependent on config mode
    num GetCurrentStageDurationInSeconds()
            vhook;
    num GetCurrentStagePauseInSeconds()
            vhook;

    // Manual start mode
    bool x_bWaitingManualStartNextStep = false
            xauto(Get, Set);

    // Disable normal procedure start notifications
    bool x_bAutoStartingNextStep = false
            xauto(Get, Set);

    // All done
    bool x_bLastStageComplete = false
            xauto(Get, Set);

// Operations

    // Start
    void InitFlowSession()
            vhook;

    // Next step
    bool HandleStageCompleteIsLastDone()
            vhook;

    // Start dizzer
    void HandleOnlineRecordingStarted()
            vhook;

    // Abort dizzer
    void HandleOnlineAbort()
            vhook;

    // We calc the sum for all stages
    void ReceiveStabilityIndex(
            num numStabilityIndex)
            vhook;

    // Sum index to Patients
    void UpdateFlowSessionToPatient(
            ref<CPatientSampleIfaceGp> rPatientSample,
            ref<CPatientLinkIface> rPatientLink)
            vhook;

// UI

    // Current stage indicator
    ref<UiContainer> NewUiFlowSessionProgressStatus()
            vhook;

protected:
    virtual int OnGetCurrentStageNumber()
            v1pure;
    virtual num OnGetCurrentStageDurationInSeconds()
            v1pure;
    virtual num OnGetCurrentStagePauseInSeconds()
            v1pure;
    virtual void OnInitFlowSession()
            v1pure;
    virtual bool OnHandleStageCompleteIsLastDone()
            v1pure;
    virtual void OnHandleOnlineRecordingStarted()
            v1pure;
    virtual void OnHandleOnlineAbort()
            v1pure;
    virtual void OnReceiveStabilityIndex(
            num numStabilityIndex)
            v1pure;
    virtual void OnUpdateFlowSessionToPatient(
            ref<CPatientSampleIfaceGp> rPatientSample,
            ref<CPatientLinkIface> rPatientLink)
            v1pure;
    virtual ref<UiContainer> OnNewUiFlowSessionProgressStatus()
            v1pure;
private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToFlowSessionIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
