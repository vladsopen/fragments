// CPlayEnergy.h
#pragma once

#include "CPlayBase.h"

class CWaveOutQueueIfaceGp;
class CReadingIface;
class CRangesIface;

//
// CPlayEnergy - 
//
//   Audible Energy Feedback Effect
//
//ASSUME_IMPL_FOR_IFACE(CPlayEnergyImpl, CPlayEnergy)

class CPlayEnergy : public CPlayBase
{
public:
    CPlayEnergy();
    //NEW_LOCAL(CPlayEnergyImpl, CPlayEnergy)
    //NEW_GP(Balance, CPlayEnergyImpl, CPlayEnergy)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

    // Data receiving
    void ReceiveReadingForPlayEnergy(
            ref<CReadingIface> rReading,
            ref<CRangesIface> rRanges);

// UI

protected:
private:
    //bool _m_bOneTimeInitPlayEnergyOk = false;

    num m_numPrevDeltaMm = OO;
    ref<CReadingIface> m_rPrevReading;

    void _init_AttachToPlayEnergy();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
