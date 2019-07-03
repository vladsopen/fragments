// CDemoVehicle.cpp

#include "CProject.h"
#include "CDemoVehicle.h"
#include "CUmoveIfaceGp.h"
#include "CUmoveTest.h"

CDemoVehicle::CDemoVehicle()
{
}

void CDemoVehicle::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CDemoVehicle!");
}

ref<UiContainer> CDemoVehicle::NewUiUmoveTestVehicle(
        ref<CUmoveTest> rUmoveTest)
{
    ref<UiUmoveTestVehicle> rUi;

    rUi->m_rDemoVehicle = this;
    rUi->m_rUmoveTest = rUmoveTest;

    return rUi;
}
