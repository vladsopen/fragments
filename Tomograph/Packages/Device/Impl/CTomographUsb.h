// CTomographUsb.h
#pragma once

#include "CTomographCommon.h"

class CUsbFrameFromTomograph;

//
// CTomographUsb - impl for CTomographIface wrapping Tomograph USB 
//    USB hardware interface (originally by Mike)
//

class CTomographUsb : public CTomographCommon
{
public:
    CTomographUsb();
    ~CTomographUsb();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // Universal timeout value (see specs)
    static const tick C_tickSiUsbReadWriteTimeouts = 1000;
    static const tick C_tickEraseAllTimeout = 3000;
    static const int C_nTomographExpectedNewFeaturesVersion = 0x01;
    static const int C_nTomographExpectedOldCutOffVersion = 0x02;

// Attributes

// Operations

// UI

protected:
    virtual bool OnIsTomographOpen()
            ;
    virtual void OnHandleDeviceSelected()
            ;
    virtual bool OnOpenTomograph()
            ;
    virtual void OnCloseTomograph()
            ;
    virtual bool OnExecuteTomographCommand(
            ETomographCommand eTomographCommand)
            ;
    virtual ref<CUsbFrameFromTomograph> OnTryReadTomographFrame()
            ;
    virtual int OnGetTomogramCountAndStartDownload()
            ;
    virtual bool OnEraseAllTomograms()
            ;
    virtual sloc OnStringizeSiUsbErrorCode(
            int nSiError)
            ;
    virtual ref<UiContainer> OnNewUiTomographDeviceControls()
            ;

private:

    // device handle
    HANDLE _m_hSiDevice = INVALID_HANDLE_VALUE;

    // low level response reader
    sbuf TryReadFromTomograph(
            int nBytes);

    // compatible firmware?
    bool CheckTomographVersions();

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
