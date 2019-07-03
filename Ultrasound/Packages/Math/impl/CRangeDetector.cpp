// CRangeDetector.cpp
// ("CRangeDetector.h")

#include "CDucalcProject.h"
#include "CRangeDetector.h"

CRangeDetector::CRangeDetector()
{
}

void CRangeDetector::UpdateMinMax(
        int nAmplitude,
        out int& out_nMin,
        out int& out_nMax)
{
    // First time init min/max value
    if (out_nMin < 0)
        out_nMin = nAmplitude;
    if (out_nMax < 0)
        out_nMax = nAmplitude;

    if (nAmplitude < out_nMin)
        out_nMin = nAmplitude; 

    if (nAmplitude > out_nMax)
        out_nMax = nAmplitude; 
}

void CRangeDetector::FeedNextPoint(
        int nAmplitude,
        int nHalfTimePeriod)
{
    ASSERT(nAmplitude >= 0);
    ASSERT(nHalfTimePeriod > 1);

    // Independently accumulate interval stats
    UpdateMinMax(nAmplitude, out m_nNewMin1stHalf, out m_nNewMax1stHalf);
    UpdateMinMax(nAmplitude, out m_nNewMin2ndHalf, out m_nNewMax2ndHalf);

    // Ready to update output values?
    m_nTimeCounter++;
    if (m_nTimeCounter > nHalfTimePeriod)
    {
        m_nTimeCounter = 0;

        // First time init
        if (this->x_nCurrentMin < 0)
            this->x_nCurrentMin = nAmplitude;
        if (this->x_nCurrentMax < 0)
            this->x_nCurrentMax = nAmplitude;

[...]
    }

    m_bOutputValid = true;
}

bool CRangeDetector::IsOutputValid()
{
    if (m_bOutputValid)
    {
        ASSERT(
            this->x_nCurrentMin >= 0 &&
            this->x_nCurrentMax >= 0);
        return true;
    }
    else
    {
        ASSERT(
            this->x_nCurrentMin < 0 &&
            this->x_nCurrentMax < 0);
        return false;
    }
}
