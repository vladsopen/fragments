// CStepSaveResult.h
#pragma once

#include "CStepAbstract.h"

//
// CStepSaveResult - 
//
//   'SendSampleInfo' step
//
//ASSUME_IMPL_FOR_IFACE(CStepSaveResultImpl, CStepSaveResult)

class CStepSaveResult : public CStepAbstract
{
public:
    CStepSaveResult();
    //~CStepSaveResult();
    //NEW_LOCAL(CStepSaveResultImpl, CStepSaveResult)
    //NEW_GP(CustomApps, CStepSaveResultImpl, CStepSaveResult)
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
    //void InitStepSaveResult();

    // Cleanup
    //void CloseStepSaveResult();

// UI

protected:

    virtual str OnGetFixedStepTitle()
            ;
    virtual void OnHandleActiveStepPolling()
            ;

private:
    //bool _m_bOneTimeInitStepSaveResultOk = false;
    //bool _m_bStepSaveResultOpened = false;

    void _init_AttachToStepSaveResult();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
