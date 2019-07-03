// CDrawAnythingTimed.h
#pragma once

#include "CDrawAnythingRecorded.h"

class CRangesIface;
class CRulersForTimed;

//
// CDrawAnythingTimed -
//
//   Abstract extension for graphs in time domain.
//
//ASSUME_IMPL_FOR_IFACE(CDrawAnythingTimedImpl, CDrawAnythingTimed)

class CDrawAnythingTimed : public CDrawAnythingRecorded
{
public:
    CDrawAnythingTimed();
    //NEW_LOCAL(CDrawAnythingTimedImpl, CDrawAnythingTimed)
    //NEW_GP(Balance, CDrawAnythingTimedImpl, CDrawAnythingTimed)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Rulers
    ref<CRulersForTimed> x_rRulersForTimed
            xauto(Get);

    // Optional scaling
    num GetAmplitudeRange()
            vhook;

    // For X/Y graph having negative values
    num GetAmplitudeRangeNegative()
            vhook;

// Operations

    // For derived classes
    void HandleNewRulerRangesDetected()
            vhook;

// UI

protected:

    virtual void OnHandleRecordingChange()
            vcontinue;

    virtual void OnUpdateGraphNow()
            vcontinue;

    virtual num OnGetAmplitudeRange()
            v1pure;

    virtual void OnHandleNewRulerRangesDetected()
            v1null;

    virtual num OnGetAmplitudeRangeNegative()
            v1st
            return 0;

private:
    //bool _m_bOneTimeInitOk = false;

    //ref<CRangesIface> m_rLastRanges;
    num m_numLastRulerScale = 0;

    // Determines if scale is changed and rulers need be reset
    void CheckRulerUpdate();

    void _init_AttachToDrawAnythingTimed();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
