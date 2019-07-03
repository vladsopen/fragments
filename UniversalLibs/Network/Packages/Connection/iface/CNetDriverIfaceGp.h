// CNetDriverIfaceGp.h
#pragma once

class CLoginIfaceGp;
class CNetHelperDriverIfaceGp;
class CNetCallbackCommonIfaceGp;
class CNetCallbackInIfaceGp;
class CNetConnectionInIfaceGp;
class CNetBoxAbstractionIfaceGp;
class CNetLocatorIfaceGp;
class CNetCallbackOutIfaceGp;
class CNetConnectionOutIfaceGp;

//
// CNetDriverIfaceGp -
//
//   Specific low level communication driver attached to In or Out connection.
//

class CNetDriverIfaceGp : public object
{
public:
    CNetDriverIfaceGp();
    ~CNetDriverIfaceGp();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // ***************************************************************
    //  WARNING: all properties get reset on stalled driver restart!
    // ***************************************************************

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // see ShutdownNetDriver() which sets this before and
    //   after virtual function processing
    bool x_bNetDriverShutdownCalled = false
            xauto(Get, Set);
    bool x_bNetDriverShutdownComplete = false
            xauto(Get, Set);

    // Tests try to close PingPong driver twice causing an assert failure
    int x_nTestHackForPingPongDontShutdown = 0
            xauto(Get, Set)
            xassert(value >= 0);

    // Automatic from classinfo
    str GetNetDriverName();

    // User for in or/and out
    bool x_bHadConnectionsOut = false
            xauto(Get, Set);
    bool x_bHadConnectionsIn = false
            xauto(Get, Set);

    // ***************************************************************
    //  WARNING: all properties get reset on stalled driver restart!
    // ***************************************************************

// Operations

    //
    // Virtual Driver Handlers
    //

    // It is recommended that a driver should support multiple connections,
    //   althoug this is not required if an app would never expect multiple connections
    //   for a specific kind of transfer.
    bool OpenAttachedNetConnectionOut(
            ptr<CNetConnectionOutIfaceGp> pOut,
            ptr<CNetCallbackOutIfaceGp> pCallbackOut,
            ref<CNetLocatorIfaceGp> rLocator,
            out str& out_sError)
            vhooked;

    // It is recommended that a driver should support multiple connections,
    //   althoug this is not required if an app would never expect multiple connections
    //   for a specific kind of transfer.
    bool OpenAttachedNetConnectionIn(
            ptr<CNetConnectionInIfaceGp> pIn,
            ptr<CNetCallbackInIfaceGp> pCallbackIn,
            ref<CNetLocatorIfaceGp> rLocator,
            out str& out_sError)
            vhooked;

    // App is required to call CloseNetConnectionOut which calls this
    bool CloseAttachedNetConnectionOut(
            ref<CNetConnectionOutIfaceGp> rConnectionOut)
            vhooked;

    // App is required to call CloseNetConnectionIn which calls this
    bool CloseAttachedNetConnectionIn(
            ref<CNetConnectionInIfaceGp> rConnectionIn)
            vhooked;

    // Driver object owner is required to call this directly.
    //   No connections are allowed after this.
    //   IfaceGp handles shutdown flags so impls should not have to worry
    //   about multiple calls and reentry.
    bool ShutdownNetDriver()
            vhooked;

    // App is required to call CloseNetConnectionOut which calls this
    bool SendNetBoxViaConnection(
            ref<CNetBoxAbstractionIfaceGp> rBox,
            ref<CNetConnectionOutIfaceGp> rConnectionOut,
            ref<CNetCallbackOutIfaceGp> rCallbackOut,
            ref<CNetLocatorIfaceGp> rLocator)
            vhooked;

    // Caller either via internal timer (CNetHelperDriverIfaceGp)
    //   or from app calling
    bool PollNetDriverOut(
            ref<CNetHelperDriverIfaceGp> rHelperDriver,
            out bool& out_bHaveJobs)
            vhook;
    bool PollNetDriverIn(
            ref<CNetHelperDriverIfaceGp> rHelperDriver,
            out bool& out_bHaveJobs)
            vhook;

    //
    // Internal Driver Helpers
    //

    // Save error state in all places and return the text
    str ReportNetDriverError(
            str sDetails,
            ptr<CNetCallbackCommonIfaceGp> pToCallback,
            ptr<CNetBoxAbstractionIfaceGp> pToBox,
            ref<CNetLocatorIfaceGp> rLocator)
            vhooked;

    // For CNetSyncInImpl::OnForceSkipToNewestNow()
    void HandleForceSkipToNewestNow()
            vhook;

// UI

    // Optional internal status and config additional to
    ref<UiContainer> NewUiNetDriverExtendedDash()
            vhook;

protected:
    virtual bool OnOpenAttachedNetConnectionOut(
            ptr<CNetConnectionOutIfaceGp> pOut,
            ptr<CNetCallbackOutIfaceGp> pCallbackOut,
            ref<CNetLocatorIfaceGp> rLocator,
            out str& out_sError)
            v1pure;
    virtual bool OnOpenAttachedNetConnectionIn(
            ptr<CNetConnectionInIfaceGp> pIn,
            ptr<CNetCallbackInIfaceGp> pCallbackIn,
            ref<CNetLocatorIfaceGp> rLocator,
            out str& out_sError)
            v1pure;
    virtual bool OnCloseAttachedNetConnectionOut(
            ref<CNetConnectionOutIfaceGp> rConnectionOut)
            v1pure;
    virtual bool OnCloseAttachedNetConnectionIn(
            ref<CNetConnectionInIfaceGp> rConnectionIn)
            v1pure;
    virtual bool OnShutdownNetDriver()
            v1null;
    virtual bool OnSendNetBoxViaConnection(
            ref<CNetBoxAbstractionIfaceGp> rBox,
            ref<CNetConnectionOutIfaceGp> rConnectionOut,
            ref<CNetCallbackOutIfaceGp> rCallbackOut,
            ref<CNetLocatorIfaceGp> rLocator)
            v1pure;
    virtual bool OnPollNetDriverOut(
            ref<CNetHelperDriverIfaceGp> rHelperDriver,
            out bool& out_bHaveJobs)
            v1null;
    virtual bool OnPollNetDriverIn(
            ref<CNetHelperDriverIfaceGp> rHelperDriver,
            out bool& out_bHaveJobs)
            v1null;
    virtual str OnReportNetDriverError(
            str sDetails,
            ptr<CNetCallbackCommonIfaceGp> pToCallback,
            ptr<CNetBoxAbstractionIfaceGp> pToBox,
            ref<CNetLocatorIfaceGp> rLocator)
            v1st;
    virtual void OnHandleForceSkipToNewestNow()
            v1null;
    virtual ref<UiContainer> OnNewUiNetDriverExtendedDash()
            v1null;
private:
    bool _m_bHadConnections = false;

    void _init_AttachToNetDriverIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
