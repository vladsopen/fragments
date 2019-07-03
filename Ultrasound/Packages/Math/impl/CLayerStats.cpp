// CLayerStats.cpp

#include "CDucalcProject.h"
#include "CLayerStats.h"

CLayerStats::CLayerStats()
{
}

void CLayerStats::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);
}
