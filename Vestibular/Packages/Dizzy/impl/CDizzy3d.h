// CDizzy3d.h
#pragma once

class CDbStepConfigIface;

//
// CDizzy3d - 
//
//   OpenGL implemenation for stripe animation.
//
//ASSUME_IMPL_FOR_IFACE(CDizzy3dImpl, CDizzy3d)

class CDizzy3d : public object
{
public:
    CDizzy3d();
    //NEW_LOCAL(CDizzy3dImpl, CDizzy3d)
    //NEW_GP(MyProject, CDizzy3dImpl, CDizzy3d)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

    // Init
    void PrepareToAnimate();

    // Hide
    void StopAnimation();

    // Update
    void AnimateNextFrame(
            ref<CDbStepConfigIface> rDbStepConfig);

    // Position window
    void SetWindowRect(
            rect rectWindow);

// UI

protected:
private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToDizzy3d();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
