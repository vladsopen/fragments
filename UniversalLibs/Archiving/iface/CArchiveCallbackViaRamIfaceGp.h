// CArchiveCallbackViaRamIfaceGp.h
#pragma once

#include "CArchiveCallbackIfaceGp.h"

//
// CArchiveCallbackViaRamIfaceGp - 
//
//   Implements archive streams with in-memory buffers.
//
//ASSUME_IMPL_FOR_IFACE(CArchiveCallbackViaRamImpl, CArchiveCallbackViaRamIfaceGp)

class CArchiveCallbackViaRamIfaceGp : public CArchiveCallbackIfaceGp
{
public:
    CArchiveCallbackViaRamIfaceGp();
    //NEW_LOCAL(CArchiveCallbackViaRamImpl, CArchiveCallbackViaRamIfaceGp)
    NEW_GP(SysCommon, CArchiveCallbackViaRamImpl, CArchiveCallbackViaRamIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // In and out stream storage.
    //   Assign input stream before starting and retrieve output
    //   after archive operations are complete.
    sbuf x_sbufStreamInput
            xauto(Get, Set)
            xassert(true);
    sbuf x_sbufStreamOutput
            xauto(Get, Set)
            xassert(true);

// Operations

// UI

protected:
private:
    //bool _m_bOneTimeInitArchiveCallbackViaRamIfaceGpOk = false;

    void _init_AttachToArchiveCallbackViaRamIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
