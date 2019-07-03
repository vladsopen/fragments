// CDemoVehicle.h
#pragma once

class CUmoveTest;
class CUmoveIfaceGp;

//
// CDemoVehicle - 
//
//   An item Umove moves. This could be a marker, thumb, or any other window.
//     In your app you will encapsulate the only object Umove exports: 
//       CUmoveIfaceGp
//
//ASSUME_IMPL_FOR_IFACE(CDemoVehicleImpl, CDemoVehicle)

class CDemoVehicle : public object
{
public:
    CDemoVehicle();
    //NEW_LOCAL(CDemoVehicleImpl, CDemoVehicle)
    //NEW_GP(MyProject, CDemoVehicleImpl, CDemoVehicle)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // The Vehicle we move within its parent panel using logical coordinates
    ref<CUmoveIfaceGp> x_rUmove
            xauto(Get);

    // Marker size depends on parent panel
    bool x_bStretchWidthAsForHorizontalMarker = false
            xauto(Get, Set);
    bool x_bStretchHeightAsForVerticalMarker = false
            xauto(Get, Set);

    // We disallow movement into some places
    point x_pointForbiddenPlace = point(off, off)
            xauto(Get, Set)
            xassert(true);

    // demo
    bool x_bUnmovable = false
            xauto(Get, Set);

// Operations

// UI

    // DemoVehicle
    ref<UiContainer> NewUiUmoveTestVehicle(
            ref<CUmoveTest> rUmoveTest);
    ptrmap<CUiPanelGp> _m_ptrmapUiUmoveTestVehicle;

protected:
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiUmoveTestVehicle.cpp)
class UiUmoveTestVehicle : public UiContainer
{
public:
    virtual void Ui();

    ref<CDemoVehicle> m_rDemoVehicle;
    ref<CUmoveTest> m_rUmoveTest;
};
