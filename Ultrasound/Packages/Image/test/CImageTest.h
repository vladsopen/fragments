// CImageTest.h
#pragma once

//
// CImageTest - 
//
//   UT for media image redering
//
//ASSUME_IMPL_FOR_IFACE(CImageTestImpl, CImageTest)

class CImageTest : public object
{
public:
    CImageTest();
    //NEW_LOCAL(CImageTestImpl, CImageTest)
    //NEW_GP(MyProject, CImageTestImpl, CImageTest)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

// Operations

// UI

protected:
private:

    // UT Animation :)
    void PauseAfterPaletteUiUpdate();

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    virtual void OnTestClass();
};
