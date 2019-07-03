//***********************************************
// WARNING: UNSAFE PROTOTYPE CODE TO THROW AWAY!
// Intentionally Cta incompatible! You can not 
// copypaste it into production! Tied to MFC.
// Run as debug so you have buffer checks.
//***********************************************
// CTomoSample.h
#pragma once

class CTomographicsDoc;

//
// CTomoSample - data for one 10 s. sample
//
//ASSUME_IMPL_FOR_IFACE(CTomoSampleImpl, CTomoSample)

class CTomoSample : public object
{
public:
    CTomoSample(
            int iChannel,
            CTomographicsDoc* pDoc);
    //NEW_LOCAL(CTomoSampleImpl, CTomoSample)
    //NEW_GP(MyProject, CTomoSampleImpl, CTomoSample)
    //virtual void OnExposeContent(ref<CExpose> rExpose);

// Constants

    // 2 kHz * 10 s.
    static const int C_nTomoRealAmplitudesPerSample = 2000 * 10; 
    static const int C_nTomoAllocatedAmplitudesPerSample = 
            C_nTomoRealAmplitudesPerSample;

// Attributes

    int xx_iChannel;
    CTomographicsDoc* xx_pDoc;

    array<int> xx_anCurve;

// Operations

    // Online acquisition
    bool TryAppendAmplitudeIsOverflow(
            int nAmplitude);

    // Wave save export
    int ExportToStereoInterleaveWaveCountBytes(
            short* pshortBufferOrNull);

    // Paint
    void DrawCurve(
            CDC* pDc,
            CRect rectClient,
            int iSample,
            int iDrawn,
            int nScrollScaledSignalOffset);

// UI

protected:
private:
    int m_nAdded;
    array<POINT> m_apointCurveCache;

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
