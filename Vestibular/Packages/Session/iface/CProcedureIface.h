// CProcedureIface.h
#pragma once

class CReadingIface;
class CProcedureConfigIface;
class CRecordTypeIface;

//
// CProcedureIface -
//
//   Controls checkup sequence.
//
ASSUME_IMPL_FOR_IFACE(CProcedureImpl, CProcedureIface)

class CProcedureIface : public object
{
public:
    CProcedureIface();
    NEW_LOCAL(CProcedureImpl, CProcedureIface)
    //NEW_GP(Balance, CProcedureImpl, CProcedureIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // We only start after receiving this weight indication
    static const num C_numMinKiloToStart =
            //20.;
            10.;

    // Max time to wait until they are on the platform
    static const tick C_tickAbortStartIfNotReady = 
            30 * 1000;
    static const tick C_tickAbortIfNullWeight =
            1000;
            //3000;

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Procedure started?
    bool IsRecordingOnline()
            vhook;

    // Waiting patient ready?
    bool IsPausedBeforeNextRecording()
            vhook;

    // A check for preemptive audio activation
    bool WillWeBeOutOfPauseInAfter(
            tick tickAfter)
            vhook;

    // Online/offline mode
    type<CRecordTypeIface> GetActiveRecordTypeOrNull()
            vhook;
    type<CRecordTypeIface> GetRecordTypeForCurrentStageOrNull()
            vhook;

    // Local or global config
    ref<CProcedureConfigIface> GetProcedureConfig()
            vhook;

// Operations

    // Procedure stages
    void ToggleRecordingOnline()
            vhook;
    void StartRecordingOnline()
            vhook;
    void CompleteRecordingOnline()
            vhook;
    void CompleteLastStage()
            vhook;
    void AbortRecordingOnline()
            vhook;
    void SaveCompleteRecording()
            vhook;
    void AbandonAndCloseProcedure()
            vhook;

    // High frequency timing from the device
    void HandleProcedureControlAtPollingEvent(
            ref<CReadingIface> rReading)
            vhook;

// UI

protected:
    virtual bool OnIsRecordingOnline()
            v1pure;
    virtual bool OnIsPausedBeforeNextRecording()
            v1pure;
    virtual bool OnWillWeBeOutOfPauseInAfter(
            tick tickAfter)
            v1pure;
    virtual type<CRecordTypeIface> OnGetActiveRecordTypeOrNull()
            v1pure;
    virtual type<CRecordTypeIface> OnGetRecordTypeForCurrentStageOrNull()
            v1pure;
    virtual ref<CProcedureConfigIface> OnGetProcedureConfig()
            v1pure;
    virtual void OnToggleRecordingOnline()
            v1pure;
    virtual void OnStartRecordingOnline()
            v1pure;
    virtual void OnCompleteRecordingOnline()
            v1pure;
    virtual void OnCompleteLastStage()
            v1pure;
    virtual void OnAbortRecordingOnline()
            v1pure;
    virtual void OnSaveCompleteRecording()
            v1pure;
    virtual void OnAbandonAndCloseProcedure()
            v1pure;
    virtual void OnHandleProcedureControlAtPollingEvent(
            ref<CReadingIface> rReading)
            v1pure;
private:
    //bool _m_bOneTimeInitProcedureIfaceOk = false;

    void _init_AttachToProcedureIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
