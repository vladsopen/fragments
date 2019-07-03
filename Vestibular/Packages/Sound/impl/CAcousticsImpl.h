// CAcousticsImpl.h
#pragma once

#include "CAcousticsIface.h"

class CPlayBase;
class CPlayOffset;
class CPlayEnergy;

//
// CAcousticsImpl - see Iface
//

class CAcousticsImpl : public CAcousticsIface
{
public:
    CAcousticsImpl();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

// Operations

// UI

protected:
    virtual void OnReceiveReadingForAcoustics(
            ref<CReadingIface> rReading,
            ref<CRangesIface> rRanges)
            ;
    virtual void OnPlayPointerPlayOffset(
            num numOffset)
            ;
private:
    //bool _m_bOneTimeInitAcousticsImplOk = false;

    // energy sound
    ref<CPlayEnergy> m_rPlayEnergy;
    bool m_bPlayEnergyIsOn = false;

    // energy sound
    ref<CPlayOffset> m_rPlayOffset;
    bool m_bPlayOffsetIsOn = false;

    // Global state check
    bool IsAcousticsEnabledAtThisProcedureStage(
            ref<CPlayBase> rPlayBaseUpdateVolume);

    void _init_AttachToAcousticsImpl();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
