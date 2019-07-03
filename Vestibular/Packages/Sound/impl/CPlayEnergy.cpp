// CPlayEnergy.cpp

#include "CProject.h"
#include "CPlayEnergy.h"
#include "CReadingIface.h"

CPlayEnergy::CPlayEnergy()
{
    _init_AttachToPlayEnergy();
}

void CPlayEnergy::_init_AttachToPlayEnergy()
{
    //x_r->_x_p = this;
}

void CPlayEnergy::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToPlayEnergy();

    rFAIL("make sure it is OK to expose CPlayEnergy!");
}

void CPlayEnergy::ReceiveReadingForPlayEnergy(
        ref<CReadingIface> rReading,
        ref<CRangesIface> rRanges)
{
    InitPlayBaseOnce();

    num numDeltaMm = 
        math::GSqrt(
            math::GPow(
                rReading->GetVirtualXmm(rRanges)
[...]                    - 
        m_numPrevDeltaMm = numDeltaMm0;
        numDeltaMm = 0;
    }
    else
    {
        m_numPrevDeltaMm = numDeltaMm0;
    }

    num numEnergy = 
        debug("Sound.Energy.Add?/4e").InputIntOrSet(300) +
            numDeltaMm * numDeltaMm *
                debug("Sound.Energy.Multiply?/4e").InputIntOrSet(300); 
            //sys::GGetMousePosAsynchronously().x / 10;
            //rReading->GetVirtualXmm(rRanges) * 100;

    PlayNextFreq(
        numEnergy,
        debug("Sound.Energy.LimitMin?/4e").InputNumOrSet(303),
        debug("Sound.Energy.LimitMax?/4e").InputNumOrSet(5000),
        debug("Sound.Energy.VolumeMaxAt?/4e").InputNumOrSet(100),
        debug("Sound.Energy.Alpha?").InputNumOrSet(0));
}

