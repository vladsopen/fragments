// CDizzyAnimator.h
#pragma once

#include "VirtOs_ontimer.h"

class CDizzy3d;
class CDbStepConfigIface;

//
// CDizzyAnimator - 
//
//   Animates an image according to Dizzy config.
//
//ASSUME_IMPL_FOR_IFACE(CDizzyAnimatorImpl, CDizzyAnimator)

class CDizzyAnimator : public object
{
public:
    CDizzyAnimator();
    //NEW_LOCAL(CDizzyAnimatorImpl, CDizzyAnimator)
    //NEW_GP(Balance, CDizzyAnimatorImpl, CDizzyAnimator)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Current rendering
    ref<CDbStepConfigIface> x_rAnimateDbStepConfig
            xauto(Get);
    void x_rAnimateDbStepConfig_Set(
            ref<CDbStepConfigIface> rDbStepConfig);

    // 3D
    ref<CDizzy3d> x_rDizzy3d
            xauto(Get);

// Operations

    // see  
    //void UpdateAnimation();

    // Painter
    void PaintAnimation(
            draw dc,
            ref<CUiPanelGp> rUi);

// UI

    // DizzyAnimatorImage
    ref<UiContainer> NewUiDizzyAnimatorImage();
    ptrmap<CUiPanelGp> _m_ptrmapUiDizzyAnimatorImage;

protected:
private:
    //bool _m_bOneTimeInitOk = false;

    // Timer handler 
    void HandleDizzyAnimatorTimer();
    ontimer<HandleDizzyAnimatorTimer> _m_ontimerDizzyAnimator;

    void _init_AttachToDizzyAnimator();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiDizzyAnimatorImage.cpp)
class UiDizzyAnimatorImage : public UiContainer
{
public:
    virtual void Ui();

    ref<CDizzyAnimator> m_rDizzyAnimator;
};
