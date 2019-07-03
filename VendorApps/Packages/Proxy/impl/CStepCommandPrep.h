// CStepCommandPrep.h
#pragma once

#include "CStepAbstract.h"

//
// CStepCommandPrep - 
//
//   .Ini creation
//
//ASSUME_IMPL_FOR_IFACE(CStepCommandPrepImpl, CStepCommandPrep)

class CStepCommandPrep : public CStepAbstract
{
public:
    CStepCommandPrep();
    //~CStepCommandPrep();
    //NEW_LOCAL(CStepCommandPrepImpl, CStepCommandPrep)
    //NEW_GP(CustomApps, CStepCommandPrepImpl, CStepCommandPrep)
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
    //void InitStepCommandPrep();

    // Cleanup
    //void CloseStepCommandPrep();

// UI

protected:

    virtual str OnGetFixedStepTitle()
            ;
    virtual void OnHandleActiveStepPolling()
            ;


private:
    //bool _m_bOneTimeInitStepCommandPrepOk = false;
    //bool _m_bStepCommandPrepOpened = false;

    void _init_AttachToStepCommandPrep();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
