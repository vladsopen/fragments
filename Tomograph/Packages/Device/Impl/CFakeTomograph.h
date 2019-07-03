// CFakeTomograph.h
#pragma once

class CTomogramIface;
class CFakeTomographMemory;
class CUsbFrameFromTomograph;
class CTomographEmulator;

//
// CFakeTomograph -
//
//   Fictitious Tomograph device for CTomographEmulator.
//   Imitates real device state and responses.
//

class CFakeTomograph : public object
{
public:
    CFakeTomograph();
    //NEW_LOCAL(CFakeTomographImpl, CFakeTomograph)
    //NEW_GP(MyProject, CFakeTomographImpl, CFakeTomograph)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Emulating USB-online status (to make OpenFakeTomograph() fail)
    bool x_bUsbPlugedIn = false
            xauto(Get, Set);

    /*
    // Emulating USB-online status (to make OpenFakeTomograph() fail)
    bool x_bTomographPowered = false
            xauto(Get);
    void x_bTomographPowered_Set(
            bool value);
    */

    // Signal to the emulator to try to switch to the real device
    //bool x_bFakeTomographWindowOpened = false
            //xauto(Get, Set);

    // Tracing iface opne status
    bool x_bTomographUsbOpen = false
            xauto(Get, Set);

    // Current sample being transfered (not transfered yet)
    int x_iSendingTomogram = 0
            xauto(Get, Set)
            xassert(value >= -1);

    // How many bytes from x_iSendingTomogram we have packaged into frames and sent
    int x_iOffsetInSendingTomogram = 0
            xauto(Get, Set)
            xassert(value >= -1);

    // Every frame has an index
    int x_iFrameCount = CUsbFrameFromTomograph::C_iFirstTomographFrameIndex
            xauto(Get, Set)
            xassert(value >= CUsbFrameFromTomograph::C_iFirstTomographFrameIndex);

    // Emulator source data
    ref<CFakeTomographMemory> x_rFakeTomographMemory
            xauto(Get);

    // Random Mode
    bool x_bReplyWithRandomSamples = false
            xauto(Get, Set);

    // Obsolete frame header
    bool x_bObsoleteHeaderSignature = false
            xauto(Get, Set);

    // Marker Editor Limit
    int x_nMaxEditMarkerCount = 2
            xauto(Get, Set)
            xassert(value >- 0 && value < oo);

    // Erros status
    str GetUsbClientErrorLog();

// Operations

    // Emulate USB Open
    bool OpenFakeTomograph(
            ref<CTomographEmulator> rTomographEmulator);

    // Data read emulation
    sbuf ReadNextFakeTomographFrame(
            ref<CTomographEmulator> rTomographEmulator);

    // App faults
    void LogUsbClientError(
            sloc slocError);

    // App activation handler
    void HandleAppActivationStateChange(
            CProject::EActivationStateChange eActivationStateChange,
            ref<CTomographEmulator> rTomographEmulator);

// UI

    // Simple Tomogram editor
    void HandleMouseOverTomogram(
            ref<CTomogramIface> rTomogram,
            EMouse eMouse,
            point pointClientMousePos,
            size sizeUiSize);

    // FakeTomographFrame
    //void OpenUiFakeTomographFrame();
    //void CloseUiFakeTomographFrame();
    ref<UiContainer> NewUiFakeTomographFrame(
            ref<CTomographEmulator> rTomographEmulator);
    ptr<CUiPanelGp> _m_pUiFakeTomographFrame;

    // UI blinking support
    int x_iBlinkCycle = 0
            xauto(Get, Set)
            xassert(value >= 0);

    // PC/USB status
    void PaintPcAndPlugs(
            draw dc,
            size sizeUi);

    // Vertical panel at left undef ON/OFF button
    alikex _m_alikexOnOffButtonHanging;

    // Main help line
    rich GetMainHelpStatusLine();

    // FakeTomographConnectionControls
    ref<UiContainer> NewUiFakeTomographConnectionControls();
    ptr<CUiPanelGp> _m_pUiFakeTomographConnectionControls;

    // FakeTomographAdvancedSide
    ref<UiContainer> NewUiFakeTomographAdvancedSide();
    ptr<CUiPanelGp> _m_pUiFakeTomographAdvancedSide;

    // FakeTomographCommunicationStatus
    ref<UiContainer> NewUiFakeTomographCommunicationStatus();
    ptr<CUiPanelGp> _m_pUiFakeTomographCommunicationStatus;

protected:
private:
    str m_sClientErrorLog;
    int m_iEditMarker = 0;

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiFakeTomographFrame.cpp)
class UiFakeTomographFrame : public UiContainer
{
public:
    virtual void Ui();

    ref<CFakeTomograph> m_rFakeTomograph;
    ref<CTomographEmulator> m_rTomographEmulator;
};

// (UiFakeTomographConnectionControls.cpp)
class UiFakeTomographConnectionControls : public UiContainer
{
public:
    virtual void Ui();

    ref<CFakeTomograph> m_rFakeTomograph;
};

// (UiFakeTomographAdvancedSide.cpp)
class UiFakeTomographAdvancedSide : public UiContainer
{
public:
    virtual void Ui();

    ref<CFakeTomograph> m_rFakeTomograph;
};

// (UiFakeTomographCommunicationStatus.cpp)
class UiFakeTomographCommunicationStatus : public UiContainer
{
public:
    virtual void Ui();

    ref<CFakeTomograph> m_rFakeTomograph;
};
