// CRulersForRoutes.h
#pragma once

class CDrawAnythingRouted;
class CRulerIfaceGp;
class CRulerbarIfaceGp;

//
// CRulersForRoutes - 
//
//   Ruler bars around route graphs.
//
//ASSUME_IMPL_FOR_IFACE(CRulersForRoutesImpl, CRulersForRoutes)

class CRulersForRoutes : public object
{
public:
    CRulersForRoutes();
    //NEW_LOCAL(CRulersForRoutesImpl, CRulersForRoutes)
    //NEW_GP(Balance, CRulersForRoutesImpl, CRulersForRoutes)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CDrawAnythingRouted> _x_pDrawAnythingRouted
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

    // RulersForRoutesHorizontal
    ref<UiContainer> NewUiRulersForRoutesHorizontal();
    ptrmap<CUiPanelGp> _m_ptrmapUiRulersForRoutesHorizontal;

    // RulersForRoutesVertical
    ref<UiContainer> NewUiRulersForRoutesVertical();
    ptrmap<CUiPanelGp> _m_ptrmapUiRulersForRoutesVertical;

protected:
private:
    bool m_bRulerInitOk = false;

    void InitCommonRulerBarAttributes(
            ref<CRulerbarIfaceGp> rRulerbar);

    void _init_AttachToRulersForRoutes();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiRulersForRoutesVertical.cpp)
class UiRulersForRoutesVertical : public UiContainer
{
public:
    virtual void Ui();

    ref<CRulersForRoutes> m_rRulersForRoutes;
};

// (UiRulersForRoutesHorizontal.cpp)
class UiRulersForRoutesHorizontal : public UiContainer
{
public:
    virtual void Ui();

    ref<CRulersForRoutes> m_rRulersForRoutes;
};
