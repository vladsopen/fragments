// CSpeedometerImpl.cpp

#include "CUsmainProject.h"
#include "CSpeedometerImpl.h"
#include "CDocIfaceGp.h"
#include "CDocMetricsIfaceGp.h"
#include "CAudioSliceGp.h"

CSpeedometerImpl::CSpeedometerImpl()
{
    _init_AttachToSpeedometerImpl();
}

void CSpeedometerImpl::_init_AttachToSpeedometerImpl()
{
    //x_r->_x_p = this;
}

void CSpeedometerImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToSpeedometerImpl();

    rFAIL("make sure it is OK to expose CSpeedometerImpl!");
}

void CSpeedometerImpl::MeasureNextAudioReception(
        ref<CAudioSliceGp> rAudioSlice,
        ref<CDocIfaceGp> rAnyDocForConverters)
{
    utc utcNow = sys::GGetUtc();
    utcNow;

    num numReceptionSeconds = 
        rAnyDocForConverters->
            GetDocMetrics()->
                ConvertDocAudioLengthFromCompositesToSeconds(
                    rAudioSlice->GetCount());

    ASSERT_SOFT(numReceptionSeconds > 0);

    numReceptionSeconds;

    // 1st time
    if (m_utcInitialStart == utc())
        m_utcInitialStart = utcNow;

    // where are we?
    if (utcNow - m_utcLastReception > 1000) // 1s too slow?
    {
        // init or reset
        m_utcSecondStart = utcNow;
        m_numLastSecPerSec = 0;
        m_numAverageSecPerSec = 0;
        m_nResetCount++;
    }
    else
    if (utcNow - m_utcSecondStart > 1000)
    {
        // recalc every second
        m_numLastSecPerSec = m_numSecondsReceived;
        m_numSecondsReceived = 0;

        if (m_numAverageSecPerSec > 0)
        {
            m_numAverageSecPerSec = 
                (m_numAverageSecPerSec + m_numLastSecPerSec)
                    /
                    2;
        }
        else
        {
            m_numAverageSecPerSec = 
                m_numLastSecPerSec;
        }

        if (m_numAverageSecPerSec > 0)
        {
            m_numLastValidSecPerSec = 0;
        }

        m_utcSecondStart = utcNow;
    }
    else
    {
        // append
        m_numSecondsReceived += 
            numReceptionSeconds;
    }

    m_utcLastReception = utcNow;
}

num CSpeedometerImpl::OnGetRoundedSpeedRatio()
{
    num numRatio = m_numAverageSecPerSec;

    // make it just look nice
    if (numRatio < 0.1)
    {
        numRatio = math::GEnsurePrecision(numRatio, 3);
    }
    else
    if (numRatio < 2)
    {
        numRatio = math::GEnsurePrecision(numRatio, 2);
    }
    else
    {
        numRatio = math::GRound(numRatio);
    }

    return numRatio;
}

str CSpeedometerImpl::OnGetFriendlySpeedRatio()
{
    num numRatio = 
        GetRoundedSpeedRatio();

    // dont show absurd slowness at the start
    if (sys::GGetUtc() - m_utcInitialStart < 5000 &&
        numRatio < 1)
    {
        numRatio = 0;
    }

    // maintain 2 digits always
    if (numRatio == 1.0)
        return "1.0";
    else
        return Str(numRatio);
}

str CSpeedometerImpl::OnGetLongSpeedReport()
{
    return 
        "" +
            Str(m_numAverageSecPerSec) + "x average\n" +
            Str(m_numLastSecPerSec) + "x instant\n" +
            Str(m_numLastValidSecPerSec) + "x valid\n" +
            Str(m_nResetCount) + " resets\n" +
            "";
}

