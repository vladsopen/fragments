// CAcousticsImpl.cpp

#include "CProject.h"
#include "CAcousticsImpl.h"
#include "CProcedureConfigIface.h"
#include "CSessionIface.h"
#include "CProcedureIface.h"
#include "CPlayEnergy.h"
#include "CPlayOffset.h"
#include "CGlobalConfigIface.h"

CAcousticsImpl::CAcousticsImpl()
{
    _init_AttachToAcousticsImpl();
}

void CAcousticsImpl::_init_AttachToAcousticsImpl()
{
    //x_r->_x_p = this;
}

void CAcousticsImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToAcousticsImpl();

    rFAIL("make sure it is OK to expose CAcousticsImpl!");
}

void CAcousticsImpl::OnReceiveReadingForAcoustics(
        ref<CReadingIface> rReading,
        ref<CRangesIface> rRanges)
{
    ref<CSessionIface> rSession = 
        _x_pSession.Get().Valid();
    ref<CProcedureIface> rProcedure = 
        rSession->x_rProcedure;
    ref<CProcedureConfigIface> rProcedureConfig = 
        rProcedure->GetProcedureConfig();

    bool bPlayEnergy = 
        rProcedureConfig->x_bPlayEnergySound
            &&
            IsAcousticsEnabledAtThisProcedureStage(
                m_rPlayEnergy);

    if (bPlayEnergy != m_bPlayEnergyIsOn)
    {
        m_bPlayEnergyIsOn = bPlayEnergy;

        // reset the player
        m_rPlayEnergy = null();
    }

    if (bPlayEnergy)
    {
        m_rPlayEnergy->
            ReceiveReadingForPlayEnergy(
                rReading,
                rRanges);
    }
}

void CAcousticsImpl::OnPlayPointerPlayOffset(
        num numOffset)
{
    ref<CSessionIface> rSession = 
        _x_pSession.Get().Valid();
    ref<CProcedureIface> rProcedure = 
        rSession->x_rProcedure;
    ref<CProcedureConfigIface> rProcedureConfig = 
        rProcedure->GetProcedureConfig();

    bool bPlayOffset = 
        rProcedureConfig->x_bPlayOffsetSound
            &&
            IsAcousticsEnabledAtThisProcedureStage(
                m_rPlayOffset);

    if (bPlayOffset != m_bPlayOffsetIsOn)
    {
        m_bPlayOffsetIsOn = bPlayOffset;

        // reset the player
        m_rPlayOffset = null();
    }

    if (bPlayOffset)
    {
        m_rPlayOffset->
            ReceiveDeltaForPlayOffset(
                numOffset);
    }
}

bool CAcousticsImpl::IsAcousticsEnabledAtThisProcedureStage(
        ref<CPlayBase> rPlayBaseUpdateVolume)
{
    rPlayBaseUpdateVolume;

    ref<CSessionIface> rSession = 
        _x_pSession.Get().Valid();
    ref<CProcedureIface> rProcedure = 
        rSession->x_rProcedure;
    ref<CProcedureConfigIface> rProcedureConfig = 
        rProcedure->GetProcedureConfig();

    if (CProject::GGetIt().x_rGlobalConfig->_x_bGameModeApp)
    {
        return false;
    }

    if (!rProcedure->IsRecordingOnline())
    {
        return false;
    }

    // interferes with audio prompts

    //// on pause we still need to prepare buffers but stay silent
    //
    //rPlayBaseUpdateVolume->
    //    SetPlaybackAudible(
    //        !rProcedure->IsPausedBeforeNextRecording());

    // predict the end of the pause
    if (rProcedure->
            WillWeBeOutOfPauseInAfter(
                CPlayBase::C_tickPerBlock
                    *
                    (CPlayBase::C_nBuffersAhead + 1)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

