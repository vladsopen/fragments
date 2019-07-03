// CSpeedometerImpl.h
#pragma once

#include "CSpeedometerIface.h"

class CDocIfaceGp;

//
// CSpeedometerImpl - see Iface
//

class CSpeedometerImpl : public CSpeedometerIface
{
public:
    CSpeedometerImpl();
    //~CSpeedometerImpl();
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
    //void InitSpeedometer();

    // Cleanup
    //void CloseSpeedometer();

    void MeasureNextAudioReception(
            ref<CAudioSliceGp> rAudioSlice,
            ref<CDocIfaceGp> rAnyDocForConverters);

// UI

protected:
    SEE_ALSO(CSpeedometerIfaceGp)

    virtual num OnGetRoundedSpeedRatio()
            ;
    virtual str OnGetFriendlySpeedRatio()
            ;
    virtual str OnGetLongSpeedReport()
            ;

    SEE_ALSO(CSpeedometerIface)

    // ... your virtuals from Iface here

private:
    //bool _m_bInited = false;
    //bool _m_bOpened = false;

    int m_nResetCount = 0;
    utc m_utcLastReception;
    utc m_utcInitialStart;
    utc m_utcSecondStart;
    num m_numSecondsReceived = 0.0;
    num m_numLastSecPerSec = 0.0;
    num m_numAverageSecPerSec = 0.0;
    num m_numLastValidSecPerSec = 0.0;

    void _init_AttachToSpeedometerImpl();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
