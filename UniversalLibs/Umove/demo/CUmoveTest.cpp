// CUmoveTest.cpp

#include "CProject.h"
#include "CUmoveTest.h"
#include "CDemoVehicleList.h"
#include "CDemoVehicle.h"
#include "CUmoveIfaceGp.h"

CUmoveTest::CUmoveTest()
{
}

void CUmoveTest::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CUmoveTest!");
}

ref<UiContainer> CUmoveTest::NewUiUmoveTestDialog()
{
    ref<UiUmoveTestDialog> rUi;

    rUi->m_rUmoveTest = this;

    return rUi;
}

void CUmoveTest::OpenUiUmoveTestDialog()
{
    if (x_rDemoVehicleList->GetVehicleCount() == 0)
    {
        x_rDemoVehicleList->RenderDemoVehicles();
    }

    ref<UiContainer> rUiFrame = NewUiUmoveTestDialog();
    rUiFrame->CreateFloater();
}

ref<UiContainer> CUmoveTest::NewUiUmoveTestSliderWrapper()
{
    ref<UiUmoveTestSliderWrapper> rUi;

    rUi->m_rUmoveTest = this;

    return rUi;
}

ref<UiContainer> CUmoveTest::NewUiUmoveTestMainArena()
{
    ref<UiUmoveTestMainArena> rUi;

    rUi->m_rUmoveTest = this;

    return rUi;
}

ref<UiContainer> CUmoveTest::NewUiUmoveTestDemoSetup()
{
    ref<UiUmoveTestDemoSetup> rUi;

    rUi->m_rUmoveTest = this;

    return rUi;
}

void CUmoveTest::UpdateAllTestUi()
{
    _m_ptrmapUiUmoveTestDialog.InvalidateContent();
}

void CUmoveTest::UpdateForNewVehiclePositions()
{
    _m_ptrmapUiUmoveTestSliderWrapper.InvalidateTreeState();
    _m_ptrmapUiUmoveTestMainArena.InvalidateTreeLayout();
}

void CUmoveTest::UpdateForNewSimepleDemoPositions()
{
    _m_ptrmapUiUmoveSimpleDemoSetup.InvalidateTreeState();
    _m_ptrmapUiUmoveSimpleDemo.InvalidateLayout();
}

ref<UiContainer> CUmoveTest::NewUiUmoveSimpleDemo()
{
    ref<UiUmoveSimpleDemo> rUi;

    rUi->m_rUmoveTest = this;

    return rUi;
}

ref<UiContainer> CUmoveTest::NewUiUmoveSimpleDemoSetup()
{
    ref<UiUmoveSimpleDemoSetup> rUi;

    rUi->m_rUmoveTest = this;

    return rUi;
}
