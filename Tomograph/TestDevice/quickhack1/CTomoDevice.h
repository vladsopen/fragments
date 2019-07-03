//***********************************************
// WARNING: UNSAFE PROTOTYPE CODE TO THROW AWAY!
// Intentionally Cta incompatible! You can not 
// copypaste it into production! Tied to MFC.
// Run as debug so you have buffer checks.
//***********************************************
// CTomoDevice.h
#pragma once

class CTomographicsView;

//
// CTomoDevice - all device functions
//
//ASSUME_IMPL_FOR_IFACE(CTomoDeviceImpl, CTomoDevice)

class CTomoDevice : public object
{
public:
    CTomoDevice();
    //NEW_LOCAL(CTomoDeviceImpl, CTomoDevice)
    //NEW_GP(MyProject, CTomoDeviceImpl, CTomoDevice)
    //virtual void OnExposeContent(ref<CExpose> rExpose);

// Constants

// Attributes

    bool xx_bEnablePolling;
    bool xx_bEmulateSignal;
    int xx_nMaxRows;

// Operations

    void Init();
    void ReInitLibrary();
    void FlushBuffers();

    void PollNextBlock(
            CTomographicsView* pView);
    void SkipSomeTimers();

    void ClearAll();

    str RunGetSampleCount(
            int nBytesToRead = 2);

    str RunGetVersions();

    bool SendCommand(
            int nCode,
            str sId);

    static sloc GStringizeSiUsbErrorCode(
            int nSiError);

// UI

protected:
private:
    bool m_bInitialized;
    HANDLE m_hDevice;
    int m_nBlockCount;
    bool m_bTerse;
    int m_iReadingSample;
    int m_nReadSamplePoints;
    bool m_bWasEndOfSample;

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
