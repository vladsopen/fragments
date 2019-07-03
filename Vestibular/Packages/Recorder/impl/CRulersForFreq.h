// CRulersForFreq.h
#pragma once

class CDrawFreqAxis;
class CRulerIfaceGp;
class CRulerbarIfaceGp;

//
// CRulersForFreq - 
//
//   Ruler bars around FFT graphs.
//
//ASSUME_IMPL_FOR_IFACE(CRulersForFreqImpl, CRulersForFreq)

class CRulersForFreq : public object
{
public:
    CRulersForFreq();
    //NEW_LOCAL(CRulersForFreqImpl, CRulersForFreq)
    //NEW_GP(Balance, CRulersForFreqImpl, CRulersForFreq)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CDrawFreqAxis> _x_pDrawFreqAxis
            xauto(Get, Set);

    // External Ruler Coordinator
    ref<CRulerIfaceGp> _x_rRuler
            xauto(Get);

    // External RulerBar Controller
    ref<CRulerbarIfaceGp> x_rRulerbarVertical
            xauto(Get, Set);
    ref<CRulerbarIfaceGp> x_rRulerbarHorizontal
            xauto(Get, Set);

// Operations

    // Rulebar init
    void ResetRulers();

// UI

    // RulersForFreqHorizontal
    ref<UiContainer> NewUiRulersForFreqHorizontal();
    ptrmap<CUiPanelGp> _m_ptrmapUiRulersForFreqHorizontal;

    // RulersForFreqVertical
    ref<UiContainer> NewUiRulersForFreqVertical();
    ptrmap<CUiPanelGp> _m_ptrmapUiRulersForFreqVertical;

protected:
private:
    bool m_bRulerInitOk = false;

    void InitCommonRulerBarAttributes(
            ref<CRulerbarIfaceGp> rRulerbar);

    void _init_AttachToRulersForFreq();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiRulersForFreqVertical.cpp)
class UiRulersForFreqVertical : public UiContainer
{
public:
    virtual void Ui();

    ref<CRulersForFreq> m_rRulersForFreq;
};

// (UiRulersForFreqHorizontal.cpp)
class UiRulersForFreqHorizontal : public UiContainer
{
public:
    virtual void Ui();

    ref<CRulersForFreq> m_rRulersForFreq;
};
