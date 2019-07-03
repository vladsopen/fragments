// CPlayOffset.h
#pragma once

#include "CPlayBase.h"

class CWaveOutQueueIfaceGp;
class CReadingIface;
class CRangesIface;

//
// CPlayOffset - 
//
//   Audible Offset Feedback Effect
//
//ASSUME_IMPL_FOR_IFACE(CPlayOffsetImpl, CPlayOffset)

class CPlayOffset : public CPlayBase
{
public:
    CPlayOffset();
    //NEW_LOCAL(CPlayOffsetImpl, CPlayOffset)
    //NEW_GP(Balance, CPlayOffsetImpl, CPlayOffset)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

    // Data receiving
    void ReceiveDeltaForPlayOffset(
            num numDelta);

// UI

protected:
private:
    //bool _m_bOneTimeInitPlayOffsetOk = false;

    void _init_AttachToPlayOffset();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
