// CNetStatsIfaceGp.cpp

#include <VirtOs.h>
#include "CNetStatsIfaceGp.h"

CNetStatsIfaceGp::CNetStatsIfaceGp()
{
    _init_AttachToNetStatsIfaceGp();
}

void CNetStatsIfaceGp::_init_AttachToNetStatsIfaceGp()
{
    //x_r->_x_p = this;
}

void CNetStatsIfaceGp::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToNetStatsIfaceGp();

    //rFAIL("make sure it is OK to expose CNetStatsIfaceGp!");
}

num CNetStatsIfaceGp::GCalcNextAverage(
        num numPrevAverage,
        num numNextPoint,
        int nWindow)
{
    if (numPrevAverage == 0
        ||
        math::GIsNumUnreal(numPrevAverage))
    {
        return 
            numNextPoint;
    }
    else
    {
        return
            math::GDivNot0(
                numPrevAverage * (nWindow - 1) + numNextPoint,
                (num) nWindow,
                or_return numNextPoint);    
    }
}

