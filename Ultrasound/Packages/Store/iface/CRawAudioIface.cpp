// CRawAudioIface.cpp

#include "CUsmainProject.h"
#include "CRawAudioIface.h"
#include "CRawAudioImpl.h"
#include "CRawWaveImpl.h"

CRawAudioIface::CRawAudioIface()
{
}

void CRawAudioIface::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    //rFAIL("make sure it is OK to expose CRawAudioIface!");
}
