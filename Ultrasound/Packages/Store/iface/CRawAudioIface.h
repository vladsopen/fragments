// CRawAudioIface.h
#pragma once

//
// CRawAudioIface - 
//
//   Raw audio storage for Doc.
//   Contains both audio channels in RawWave.
//
ASSUME_IMPL_FOR_IFACE(CRawAudioImpl, CRawAudioIface)

class CRawAudioIface : public object
{
public:
    CRawAudioIface();
    NEW_LOCAL(CRawAudioImpl, CRawAudioIface)
    //NEW_GP(MyProject, CRawAudioImpl, CRawAudioIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // This is *really* fixed, even for tests!
    static const int C_nAudioChannelCount = 2;
    static const int C_nAudioBytesPerPoint = sizeof(short);
    CASSERT(C_nAudioBytesPerPoint == 2);

    // Fixed for now, but may vary in the future
    static const int C_nMaxAudioAmplitude = 32688;

// Attributes

// Operations

// UI

protected:
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
