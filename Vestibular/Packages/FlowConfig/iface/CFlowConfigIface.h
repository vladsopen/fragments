// CFlowConfigIface.h
#pragma once

class CDbFlowConfigIface;
class CStepConfigIface;
class CSessionIface;

//
// CFlowConfigIface -
//
//   Checkup step sequence setup.
//
ASSUME_IMPL_FOR_IFACE(CFlowConfigImpl, CFlowConfigIface)

class CFlowConfigIface : public object
{
public:
    CFlowConfigIface();
    NEW_LOCAL(CFlowConfigImpl, CFlowConfigIface)
    //NEW_GP(Balance, CFlowConfigImpl, CFlowConfigIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    static const sloc C_slowMultiStageMethodName =
            TRANSLATE(
                "Multistage[... UI-string-removed]");

// Attributes

    // parent
    ptr<CSessionIface> _x_pSession
            xauto(Get, Set);

    // ext monitor
    bool x_bUseFlowSteps
            xabstract;
    bool x_bUseFlowSteps_Get()
            vhook;
    //void x_bUseFlowSteps_Set(
    //        bool value)
    //        vhook;

    // Db
    ref<CDbFlowConfigIface> GetDbFlowConfig()
            vhook;

    // FlowSession needs config for specific steps
    ref<CStepConfigIface> FindStepConfig(
            int iStep)
            vhook;
    int GetStepConfigCount()
            vhook;

    // Home monitoring turns this off
    bool _x_bAllowStageTimingConfig = true
            xauto(Get, Set);

// Operations

    // Precheck config
    void InitFlowConfig()
            vhook;

    // MultiStage monitoring support
    void ConfigureFlowSteps(
            int nSteps,
            int iStep,
            num numStepSeconds)
            vhook;

// UI

    // Main config
    ref<UiContainer> NewUiFlowConfigGeneral()
            vhook;

    // Single step config for external app use
    ref<UiContainer> NewUiFlowConfigForStep(
            int iStep)
            vhook;

protected:
    virtual bool Onx_bUseFlowSteps_Get()
            v1pure;
    //virtual void Onx_bUseFlowSteps_Set(
    //        bool value)
    //        v1pure;
    virtual ref<CDbFlowConfigIface> OnGetDbFlowConfig()
            v1pure;
    virtual ref<CStepConfigIface> OnFindStepConfig(
            int iStep)
            v1pure;
    virtual int OnGetStepConfigCount()
            v1pure;
    virtual void OnInitFlowConfig()
            v1pure;
    virtual void OnConfigureFlowSteps(
            int nSteps,
            int iStep,
            num numStepSeconds)
            v1pure;
    virtual ref<UiContainer> OnNewUiFlowConfigGeneral()
            v1pure;
    virtual ref<UiContainer> OnNewUiFlowConfigForStep(
            int iStep)
            v1pure;
private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToFlowConfigIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
