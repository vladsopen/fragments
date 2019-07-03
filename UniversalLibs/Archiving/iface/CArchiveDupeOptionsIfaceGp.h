// CArchiveDupeOptionsIfaceGp.h
#pragma once

#include "CArchiveBasicOptionsIfaceGp.h"

//
// CArchiveDupeOptionsIfaceGp - 
//
//   Options/factory for CArchiveDupeSessionIfaceGp (NOOP impl)
//
//ASSUME_IMPL_FOR_IFACE(CArchiveDupeOptionsImpl, CArchiveDupeOptionsIfaceGp)

class CArchiveDupeOptionsIfaceGp : public CArchiveBasicOptionsIfaceGp
{
public:
    CArchiveDupeOptionsIfaceGp();
    //~CArchiveDupeOptionsIfaceGp();
    //NEW_LOCAL(CArchiveDupeOptionsImpl, CArchiveDupeOptionsIfaceGp)
    //NEW_GP(SysCommon, CArchiveDupeOptionsImpl, CArchiveDupeOptionsIfaceGp)
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

    // NOP archiver. Does nothing. No options.

// Operations

// UI

protected:
    virtual ref<CArchiveBasicSessionIfaceGp> OnNewArchiveBasicSession()
            return
                ref<CArchiveDupeSessionIfaceGp>();
private:
    //bool _m_bOneTimeInitArchiveDupeOptionsIfaceGpOk = false;
    //bool _m_bArchiveDupeOptionsIfaceGpOpened = false;

    void _init_AttachToArchiveDupeOptionsIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
