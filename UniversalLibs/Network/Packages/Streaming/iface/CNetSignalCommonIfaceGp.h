// CNetSignalCommonIfaceGp.h
#pragma once

class CNetDisplayIfaceGp;
class CNetLocatorIfaceGp;
class CNetStatsIfaceGp;
class CNetPumpCommonIfaceGp;

//
// CNetSignalCommonIfaceGp -
//
//   Common stuff for both IN and OUT signal transfer channels.
//
//ASSUME_IMPL_FOR_IFACE(CNetSignalCommonImpl, CNetSignalCommonIfaceGp)

class CNetSignalCommonIfaceGp : public object
{
public:
    CNetSignalCommonIfaceGp();
    //NEW_LOCAL(CNetSignalCommonImpl, CNetSignalCommonIfaceGp)
    //NEW_GP(Net, CNetSignalCommonImpl, CNetSignalCommonIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    static const str C_sSignalFileNameTimeFormat = "yymmdd-HHMMSS-TTT";

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Allow network signal routing
    bool x_bNetSignalEnabled
            xabstract;
    bool x_bNetSignalEnabled_Get();
    void x_bNetSignalEnabled_Set(
            bool value);

    // Show/hide UI for this direction
    bool x_bShowNetSignalUi = false
            xauto(Get, Set);

    // Stats
    ref<CNetStatsIfaceGp> x_rNetStatsHook
            xauto(Get);
    ref<CNetStatsIfaceGp> x_rNetStatsNetwork
            xauto(Get);

    // Associated locator
    ptr<CNetLocatorIfaceGp> GetNetSignalLocator();

    // Keep delay at minimum, delete extra data.
    //   Mutually exclusive with
    //     x_bFreezeAppFeed
    // moved to Locator
    //bool x_bChaseRealTime = true
    //        xauto(Get, Set);

// Operations

    // Open/close CCES UI sections when floater appears
    bool InitUiIsOpen()
            vhook;

// UI

    // Supervisor panel
    ref<UiContainer> NewUiNetSignalEasySetup()
            vhook;

    // Settings menu
    ref<UiContainer> NewUiNetSignalSetupMenuItems()
            vhook;

    // CCES
    ref<UiContainer> NewUiNetSignalMessySetup()
            vhook;

protected:
    // Access to abstract Pump properties
    ref<CNetPumpCommonIfaceGp> GetNetSignalCommonPump()
            vhook;

    virtual ref<CNetPumpCommonIfaceGp> OnGetNetSignalCommonPump()
            v1pure;
    virtual bool OnInitUiIsOpen()
            v1pure;
    virtual ref<UiContainer> OnNewUiNetSignalEasySetup()
            v1pure;
    virtual ref<UiContainer> OnNewUiNetSignalSetupMenuItems()
            v1pure;
    virtual ref<UiContainer> OnNewUiNetSignalMessySetup()
            v1pure;
private:
    //bool _m_bOneTimeInitNetSignalCommonIfaceGpOk = false;

    void _init_AttachToNetSignalCommonIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
