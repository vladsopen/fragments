// CDemoIface.h
#pragma once

//
// CDemoIface -
//
//   Emulator session replacing an external app
//
//ASSUME_IMPL_FOR_IFACE(CDemoImpl, CDemoIface)

class CDemoIface : public object
{
public:
    CDemoIface();
    //~CDemoIface();
    NEW_LOCAL(CDemoImpl, CDemoIface)
    //NEW_GP(CustomApps, CDemoImpl, CDemoIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Check the command line for the session mode
    //   (call before Init)
    bool IsExeStartedForDemo()
            vhook;

    // Session active?
    bool IsDemoInitOk()
            vhook;

// Operations

    // One-time opener
    void InitDemo()
            vhook;

    // Cleanup
    void CloseDemo()
            vhook;

// UI

    // Main UI
    ref<UiContainer> NewUiDemoMain()
            vhook;

protected:
    SEE_ALSO(CDemoImpl)

    virtual bool OnIsExeStartedForDemo()
            v1pure;
    virtual bool OnIsDemoInitOk()
            v1pure;
    virtual void OnInitDemo()
            v1pure;
    virtual void OnCloseDemo()
            v1pure;
    virtual ref<UiContainer> OnNewUiDemoMain()
            v1pure;

private:
    //bool _m_bOneTimeInitDemoIfaceOk = false;
    //bool _m_bDemoIfaceOpened = false;

    void _init_AttachToDemoIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
