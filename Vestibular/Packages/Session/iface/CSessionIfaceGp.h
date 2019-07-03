// CSessionIfaceGp.h
#pragma once

class CPatientSampleIfaceGp;
class CModusTypeIfaceGp;
class CModusSessionIfaceGp;
class CFeedRegistryIfaceGp;

//
// CSessionIfaceGp -
//
//   Main Balance application context.
//
ASSUME_IMPL_FOR_IFACE(CSessionIface, CSessionIfaceGp)
ASSUME_IMPL_FOR_IFACE(CSessionImpl, CSessionIface)

class CSessionIfaceGp : public object
{
public:
    CSessionIfaceGp();
    //NEW_LOCAL(CSessionImpl, CSessionIfaceGp)
    NEW_GP(Balance, CSessionImpl, CSessionIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Feeds
    ref<CFeedRegistryIfaceGp> GetFeedRegistry()
            vhook;

    // Modus Selector
    ref<CModusSessionIfaceGp> GetModusSession()
            vhook;

    // Fake signal?
    bool IsSignalEmulatedDemo()
            vhook;

    // AMBIGUOUS! we have games, recording/monitoring,
    //   pauses between stages... Specific status functions maybe
    //   required for the controlling apps according to its needs.
    // On/off?
    //bool IsSignalRecordingInProgress()
    //        vhook;

// Operations

    // Open
    void InitSessionForMonitoring(
            type<CModusTypeIfaceGp> typeModusType)
            vhook;

    // Finalize
    void CloseSession()
            vhook;

    // For multistage mode only
    //    AsMonitoringMultiStage()
    //    configures time span for any stage
    void ConfigureMultiStageStep(
            int nSteps,
            int iStep,
            num numStepSeconds)
            vhook;

    // For non-multistage modes, including games and double-graph Classic
    void ConfigureSingleStageStep(
            num numStepSeconds,
            num numPauseSeconds)
            vhook;

    // Online
    void SetDeviceOnline(
            bool bOnline)
            vhook;

    // Idle
    void HandleExternalPolling(
            out CProject::EIdleNext& out_eIdleNextMayBeUpgraded)
            vhook;

    // MONITORING at Home needs indexes from Balance
    void AddIndexesTo(
            ref<CPatientSampleIfaceGp> rPatientSample)
            vhook;

    // Need to iterate Indexes send from HomeMon through Balance to Patients
    // for Individual norm plugin. Balance adds this postfix for them
    static const str C_sInternalNeutralIndexKeyPostfix = "Neutral";

// UI

    // Default main window content (Devmod)
    ref<UiContainer> NewUiSessionMainFull()
            vhook;

    // Selected online signals with minimal bars around (Devmod)
    ref<UiContainer> NewUiSessionOnlineSignalsSelectedBare()
            vhook;

    // Selected online trends (Devmod)
    ref<UiContainer> NewUiSessionOnlineTrends()
            vhook;

    // Indexes for monitoring mode (Devmod)
    ref<UiContainer> NewUiSessionIndexMonitor()
            vhook;

    // Toolbar insertion (Devmod)
    ref<UiContainer> NewUiSessionMonitoringToolbar()
            vhook;

    // Settings (UNUSED???)
    ref<UiContainer> NewUiSessionOptions()
            vhook;

    // Settings for multistage mode (Devmod)
    ref<UiContainer> NewUiMultiStageOptions()
            vhook;

    // Setup panel for a single step (Devmod)
    ref<UiContainer> NewUiSingleStageOptions(
            int iStage)
            vhook;

    // Filter dialog button
    ref<UiContainer> NewUiFilterOptions()
            vhook;

protected:
    virtual ref<CFeedRegistryIfaceGp> OnGetFeedRegistry()
            v1pure;
    virtual ref<CModusSessionIfaceGp> OnGetModusSession()
            v1pure;
    virtual bool OnIsSignalEmulatedDemo()
            v1pure;
    virtual void OnInitSessionForMonitoring(
            type<CModusTypeIfaceGp> typeModusType)
            v1pure;
    virtual void OnCloseSession()
            v1pure;
    virtual void OnConfigureMultiStageStep(
            int nSteps,
            int iStep,
            num numStepSeconds)
            v1pure;
    virtual void OnConfigureSingleStageStep(
            num numStepSeconds,
            num numPauseSeconds)
            v1pure;
    virtual void OnSetDeviceOnline(
            bool bOnline)
            v1pure;
    virtual void OnHandleExternalPolling(
            out CProject::EIdleNext& out_eIdleNextMayBeUpgraded)
            v1pure;
    virtual ref<UiContainer> OnNewUiSessionMainFull()
            v1pure;
    virtual ref<UiContainer> OnNewUiSessionOnlineSignalsSelectedBare()
            v1pure;
    virtual ref<UiContainer> OnNewUiSessionOnlineTrends()
            v1pure;
    virtual ref<UiContainer> OnNewUiSessionIndexMonitor()
            v1pure;
    virtual ref<UiContainer> OnNewUiSessionMonitoringToolbar()
            v1pure;
    virtual ref<UiContainer> OnNewUiSessionOptions()
            v1pure;
    virtual ref<UiContainer> OnNewUiMultiStageOptions()
            v1pure;
    virtual ref<UiContainer> OnNewUiSingleStageOptions(
            int iStage)
            v1pure;
    virtual ref<UiContainer> OnNewUiFilterOptions()
            v1pure;
    virtual void OnAddIndexesTo(
            ref<CPatientSampleIfaceGp> rPatientSample)
            v1pure;
private:
    //bool _m_bOneTimeInitSessionIfaceGpOk = false;

    void _init_AttachToSessionIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
