// CStepExe.h
#pragma once

#include "CStepAbstract.h"

//
// CStepExe - 
//
//   Run Ecg/Press exe
//
//ASSUME_IMPL_FOR_IFACE(CStepExeImpl, CStepExe)

class CStepExe : public CStepAbstract
{
public:
    CStepExe();
    //~CStepExe();
    //NEW_LOCAL(CStepExeImpl, CStepExe)
    //NEW_GP(CustomApps, CStepExeImpl, CStepExe)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);
    //ref<> Get ()
    //        return
    //            _x_p .Get().Valid();

// Operations

    // One-time opener
    //void InitStepExe();

    // Cleanup
    //void CloseStepExe();

    // Can we start?
    path CheckExeReady(
            out rich& out_richReport);

// UI

    // StepExeAdvanced
    ref<UiContainer> NewUiStepExeAdvanced();
    ptrmap<CUiPanelGp> _m_ptrmapUiStepExeAdvanced;

protected:

    virtual str OnGetFixedStepTitle()
            ;
    virtual void OnHandleActiveStepPolling()
            ;
    virtual ref<UiContainer> OnNewUiStepAdvanced()
            ;

private:
    //bool _m_bOneTimeInitStepExeOk = false;
    //bool _m_bStepExeOpened = false;

    void _init_AttachToStepExe();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiStepExeAdvanced.cpp)
class UiStepExeAdvanced : public UiContainer
{
public:
    virtual void Ui();

    ref<CStepExe> m_rStepExe;

private:
};
