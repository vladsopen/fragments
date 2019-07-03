// CNetConnectionCommonIfaceGp.h
#pragma once

class CNetDirectionIfaceGp;
class CNetStatsIfaceGp;
class CNetBoxAbstractionIfaceGp;
class CNetLocatorIfaceGp;
class CNetCallbackCommonIfaceGp;
class CNetDriverIfaceGp;
class CNetHelperDriverIfaceGp;

//
// CNetConnectionCommonIfaceGp -
//
//   The base class for both In and Out connections.
//

class CNetConnectionCommonIfaceGp : public object
{
public:
    CNetConnectionCommonIfaceGp();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // init?
    bool IsIoConnectionOpen();

    // Direction
    type<CNetDirectionIfaceGp> GetNetConnectionDirection()
            vhook;

    // non-null after init
    ptr<CNetDriverIfaceGp> GetIoConnectionDriver();

    // non-null after init
    ptr<CNetLocatorIfaceGp> GetIoConnectionLocator();

    // non-null after init
    ptr<CNetCallbackCommonIfaceGp> GetIoConnectionCallback();

    // common driver functions
    ref<CNetHelperDriverIfaceGp> x_rNetHelperDriver
            xauto(Get);

    // Stats
    ref<CNetStatsIfaceGp> x_rNetStats
            xauto(Get);

// Operations

    // Called by app to facilitate data and status reception
    //   eiter directly or internally after StartAutoPollNetConnection
    bool PollNetConnection()
            vhooked;

    // Initiated automatic driver polling, no need to call
    //   PollNetConnection() from app then.
    void StartAutoPollNetConnection(
            tick tickTimeout);

    // Stats updater
    void HookBoxTransfer(
            ref<CNetBoxAbstractionIfaceGp> rBox);

protected:

    //
    // Internal Api for derived connections
    //

    void HandleInitCloudInOrOut(
            ptr<CNetDriverIfaceGp> pDriver,
            ptr<CNetCallbackCommonIfaceGp> pOn,
            ptr<CNetLocatorIfaceGp> pLocator);

// UI

public:

    // Common UI
    ref<UiContainer> NewUiNetConnectionDash()
            vhook;

protected:
    virtual type<CNetDirectionIfaceGp> OnGetNetConnectionDirection()
            v1pure;
    virtual bool OnPollNetConnection()
            v1pure;
    virtual ref<UiContainer> OnNewUiNetConnectionDash()
            v1pure;
private:
    bool _m_bOneTimeInitNetConnectionCommonIfaceGpOk = false;

    void _init_AttachToNetConnectionCommonIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
