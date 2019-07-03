// CStepTerminateDbUnlock.h
#pragma once

#include "CStepAbstract.h"

//
// CStepTerminateDbUnlock - 
//
//   'CheckupFinished' step
//
//ASSUME_IMPL_FOR_IFACE(CStepTerminateDbUnlockImpl, CStepTerminateDbUnlock)

class CStepTerminateDbUnlock : public CStepAbstract
{
public:
    CStepTerminateDbUnlock();
    //~CStepTerminateDbUnlock();
    //NEW_LOCAL(CStepTerminateDbUnlockImpl, CStepTerminateDbUnlock)
    //NEW_GP(CustomApps, CStepTerminateDbUnlockImpl, CStepTerminateDbUnlock)
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
    //void InitStepTerminateDbUnlock();

    // Cleanup
    //void CloseStepTerminateDbUnlock();

// UI

protected:

    virtual str OnGetFixedStepTitle()
            ;
    virtual void OnHandleActiveStepPolling()
            ;

private:
    //bool _m_bOneTimeInitStepTerminateDbUnlockOk = false;
    //bool _m_bStepTerminateDbUnlockOpened = false;

    void _init_AttachToStepTerminateDbUnlock();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
