// CStepMainWait.h
#pragma once

#include "CStepAbstract.h"

//
// CStepMainWait - 
//
//   While we wait for external exe completion
//
//ASSUME_IMPL_FOR_IFACE(CStepMainWaitImpl, CStepMainWait)

class CStepMainWait : public CStepAbstract
{
public:
    CStepMainWait();
    //~CStepMainWait();
    //NEW_LOCAL(CStepMainWaitImpl, CStepMainWait)
    //NEW_GP(CustomApps, CStepMainWaitImpl, CStepMainWait)
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
    //void InitStepMainWait();

    // Cleanup
    //void CloseStepMainWait();

// UI

protected:

    virtual str OnGetFixedStepTitle()
            ;
    virtual void OnHandleActiveStepPolling()
            ;

private:
    //bool _m_bOneTimeInitStepMainWaitOk = false;
    //bool _m_bStepMainWaitOpened = false;

    int m_nWaitCount = 0;

    void _init_AttachToStepMainWait();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
