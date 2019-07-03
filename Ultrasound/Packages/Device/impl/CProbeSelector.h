// CProbeSelector.h
#pragma once

class CChannel;

//
// CProbeSelector - 
//
//   Links low level device channel to probe UI
//
//ASSUME_IMPL_FOR_IFACE(CProbeSelectorImpl, CProbeSelector)

class CProbeSelector : public object
{
public:
    CProbeSelector();
    //NEW_LOCAL(CProbeSelectorImpl, CProbeSelector)
    //NEW_GP(MyProject, CProbeSelectorImpl, CProbeSelector)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Parent device backref
    ptr<CChannel> _x_pParentChannel
            xauto(Get, Set);

// Operations

// UI

    // ProbeSelectorList
    ref<UiContainer> NewUiProbeSelectorList();
    ptrmap<CUiPanelGp> _m_ptrmapUiProbeSelectorList;

    // ProbeSelectorDropdownButton
    ref<UiContainer> NewUiProbeSelectorDropdownButton();
    ptrmap<CUiPanelGp> _m_ptrmapUiProbeSelectorDropdownButton;

    // AgcOnOff
    ref<UiContainer> NewUiAgcOnOff();
    ptrmap<CUiPanelGp> _m_ptrmapUiAgcOnOff;

protected:
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiProbeSelectorList.cpp)
class UiProbeSelectorList : public UiContainer
{
public:
    virtual void Ui();

    ref<CProbeSelector> m_rProbeSelector;
};

// (UiProbeSelectorDropdownButton.cpp)
class UiProbeSelectorDropdownButton : public UiContainer
{
public:
    virtual void Ui();

    ref<CProbeSelector> m_rProbeSelector;
};

// (UiAgcOnOff.cpp)
class UiAgcOnOff : public UiContainer
{
public:
    virtual void Ui();

    ref<CProbeSelector> m_rProbeSelector;
};
