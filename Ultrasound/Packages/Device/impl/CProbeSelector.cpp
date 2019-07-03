// CProbeSelector.cpp

#include "CUsmainProject.h"
#include "CProbeSelector.h"

CProbeSelector::CProbeSelector()
{
}

void CProbeSelector::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CProbeSelector!");
}

ref<UiContainer> CProbeSelector::NewUiProbeSelectorList()
{
    ref<UiProbeSelectorList> rUi;

    rUi->m_rProbeSelector = this;

    return rUi;
}

ref<UiContainer> CProbeSelector::NewUiProbeSelectorDropdownButton()
{
    ref<UiProbeSelectorDropdownButton> rUi;

    rUi->m_rProbeSelector = this;

    return rUi;
}

ref<UiContainer> CProbeSelector::NewUiAgcOnOff()
{
    ref<UiAgcOnOff> rUi;

    rUi->m_rProbeSelector = this;

    return rUi;
}
