// CUmoveTest.h
#pragma once

class CUmoveIfaceGp;
class CDemoVehicle;
class CDemoVehicleList;

//
// CUmoveTest - 
//
//   CUmoveIfaceGp demo.
//
//     see "//TODO:" markers in UiUmoveTest*.cpp files
//
//ASSUME_IMPL_FOR_IFACE(CUmoveTestImpl, CUmoveTest)

class CUmoveTest : public object
{
public:
    CUmoveTest();
    //NEW_LOCAL(CUmoveTestImpl, CUmoveTest)
    //NEW_GP(MyProject, CUmoveTestImpl, CUmoveTest)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // For simple demo
    ref<CUmoveIfaceGp> x_rUmoveForSimpleDemo
            xauto(Get);
    int x_iSimpleDemoPos = 0
            xauto(Get, Set)
            xassert(value >= 0);
    int x_nSimpleDemoRange = 10
            xauto(Get, Set)
            xassert(value >= 0);
    void UpdateForNewSimepleDemoPositions();

    // Whether you need a list of markers to move or an encapsulator 
    //   for a single movable item (see CDemoVehicle) depends on your app
    ref<CDemoVehicleList> x_rDemoVehicleList
            xauto(Get);

    // Updated from OnSize and requred by Umove
    size x_sizeUmoveVehicleParent = size(0, 0)
            xauto(Get, Set)
            xassert(true);

    // Scrollbar-like buttons for horizontal/vertical Slider demo
    bool x_bShowSliderWrapper = false
            xauto(Get, Set);
    bool x_bGenerateHorizontalSlider = false
            xauto(Get, Set);
    bool x_bGenerateVerticalSlider = false
            xauto(Get, Set);

// Operations

// UI

    // UmoveTest
    void OpenUiUmoveTestDialog();
    ref<UiContainer> NewUiUmoveTestDialog();
    ptrmap<CUiPanelGp> _m_ptrmapUiUmoveTestDialog;
    void UpdateAllTestUi();
    void UpdateForNewVehiclePositions();

    // UmoveTestSliderWrapper
    ref<UiContainer> NewUiUmoveTestSliderWrapper();
    ptrmap<CUiPanelGp> _m_ptrmapUiUmoveTestSliderWrapper;

    // UmoveTestMainArena
    ref<UiContainer> NewUiUmoveTestMainArena();
    ptrmap<CUiPanelGp> _m_ptrmapUiUmoveTestMainArena;

    // UmoveTestDemoSetup
    ref<UiContainer> NewUiUmoveTestDemoSetup();
    ptrmap<CUiPanelGp> _m_ptrmapUiUmoveTestDemoSetup;

    // UmoveSimpleDemo
    ref<UiContainer> NewUiUmoveSimpleDemo();
    ptrmap<CUiPanelGp> _m_ptrmapUiUmoveSimpleDemo;

    // UmoveSimpleDemoSetup
    ref<UiContainer> NewUiUmoveSimpleDemoSetup();
    ptrmap<CUiPanelGp> _m_ptrmapUiUmoveSimpleDemoSetup;

protected:
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiUmoveTestDialog.cpp)
class UiUmoveTestDialog : public UiContainer
{
public:
    virtual void Ui();

    ref<CUmoveTest> m_rUmoveTest;
};

// (UiUmoveTestSliderWrapper.cpp)
class UiUmoveTestSliderWrapper : public UiContainer
{
public:
    virtual void Ui();

    ref<CUmoveTest> m_rUmoveTest;

private:
    void EqualizeSliderPlusMinus(
            ref<CUiButtonGp> rUi);
};

// (UiUmoveTestMainArena.cpp)
class UiUmoveTestMainArena : public UiContainer
{
public:
    virtual void Ui();

    ref<CUmoveTest> m_rUmoveTest;
};

// (UiUmoveTestDemoSetup.cpp)
class UiUmoveTestDemoSetup : public UiContainer
{
public:
    virtual void Ui();

    ref<CUmoveTest> m_rUmoveTest;
};

// (UiUmoveSimpleDemo.cpp)
class UiUmoveSimpleDemo : public UiContainer
{
public:
    virtual void Ui();

    ref<CUmoveTest> m_rUmoveTest;

private:
    size m_sizeUmoveVehicleParent = size(0, 0),
};

// (UiUmoveSimpleDemoSetup.cpp)
class UiUmoveSimpleDemoSetup : public UiContainer
{
public:
    virtual void Ui();

    ref<CUmoveTest> m_rUmoveTest;
};
