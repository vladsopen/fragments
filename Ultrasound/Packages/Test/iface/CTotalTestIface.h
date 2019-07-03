// CTotalTestIface.h
#pragma once

//
// CTotalTestIface - auto test for the whole app
//
ASSUME_IMPL_FOR_IFACE(CTotalTestImpl, CTotalTestIface)

class CTotalTestIface : public object
{
public:
    CTotalTestIface();
    NEW_LOCAL(CTotalTestImpl, CTotalTestIface)
    //NEW_GP(MyProject, CTotalTestImpl, CTotalTestIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // mainly for release demo 
    static bool s_bRunSlowFftTests = 
            ifdeb(true) ifrel(sys::GIsRunningInDevelopmentFolder());

// Operations

// UI

protected:
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
