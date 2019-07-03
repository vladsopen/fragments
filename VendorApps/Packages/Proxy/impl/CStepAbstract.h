// CStepAbstract.h
#pragma once

class CProxyImpl;
class CSequence;

//
// CStepAbstract -
//
//   Sequentical checkup stage and its execution context and debug reports
//

class CStepAbstract : public object
{
public:
    CStepAbstract();
    //~CStepAbstract();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CSequence> _x_pSequence
            xauto(Get, Set);
    ref<CSequence> GetSequence()
            return
                _x_pSequence.Get().Valid();
    ref_ret<CProxyImpl> GetProxy()
            return
                GetSequence()->GetProxy();

    // Status
    bool x_bStepComplete = false
            xauto(Get, Set);

    // Main step text
    str x_sReplaceStepTitle = ""
            xauto(Get, Set)
            xassert(true);

    // Multiline optional info
    str x_sStepDetails = ""
            xauto(Get, Set)
            xassert(true);

    // For debug UI
    str GetInternalStepName();

    // Initial test
    str GetFixedStepTitle()
            vhook;

// Operations

    // One-time opener
    //void InitStepAbstract();

    // Cleanup
    //void CloseStepAbstract();

    // Called when the step is next and is not complete
    void HandleActiveStepPolling()
            vhook;

// UI

    // StepAbstractRow
    ref<UiContainer> NewUiStepAbstractRow();
    ptrmap<CUiPanelGp> _m_ptrmapUiStepAbstractRow;

    // Virtual add-ons
    ref<UiContainer> NewUiStepAdvanced()
            vhook;

protected:

    virtual str OnGetFixedStepTitle()
            v1pure;
    virtual void OnHandleActiveStepPolling()
            v1pure;
    virtual ref<UiContainer> OnNewUiStepAdvanced()
            v1null;

private:
    //bool _m_bOneTimeInitStepAbstractOk = false;
    //bool _m_bStepAbstractOpened = false;

    void _init_AttachToStepAbstract();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiStepAbstractRow.cpp)
class UiStepAbstractRow : public UiContainer
{
public:
    virtual void Ui();

    ref<CStepAbstract> m_rStep;

private:
};
