// CTomographEmulator.h
#pragma once

#include "CTomographCommon.h"

class CDeviceManagerImpl;
class CTomogramIface;
class CUsbFrameFromTomograph;
class CFakeTomograph;

//
// CTomographEmulator - created to completely emulate USB CTomographUsb device
//

class CTomographEmulator : public CTomographCommon
{
public:
    CTomographEmulator();
    ~CTomographEmulator();
    //NEW_LOCAL(CTomographEmulatorImpl, CTomographEmulator)
    //NEW_GP(MyProject, CTomographEmulatorImpl, CTomographEmulator)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Fake device context
    ref<CFakeTomograph> x_rFakeTomograph
            xauto(Get);

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
    virtual void OnHandleAppActivationStateChange(
            CProject::EActivationStateChange eActivationStateChange)
            ;
    virtual sloc OnStringizeSiUsbErrorCode(
            int nSiError)
            ;
    virtual ref<UiContainer> OnNewUiTomographDeviceControls()
            ;

private:
    bool _m_bEmulatorDeviceCreated = false;
    bool _m_bTomographReadRequested = false;

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
