// CFeedRegistryIfaceGp.h
#pragma once

class CFeedStateIfaceGp;
class CFeedOnlineCallbackIfaceGp;

//
// CFeedRegistryIfaceGp - 
//
//   Registers and routes feed data to external callbacks.
//
ASSUME_IMPL_FOR_IFACE(CFeedRegistryIface, CFeedRegistryIfaceGp)
ASSUME_IMPL_FOR_IFACE(CFeedRegistryImpl, CFeedRegistryIface)

class CFeedRegistryIfaceGp : public object
{
public:
    CFeedRegistryIfaceGp();
    //NEW_LOCAL(CFeedRegistryImpl, CFeedRegistryIfaceGp)
    //NEW_GP(Balance, CFeedRegistryImpl, CFeedRegistryIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    //
    // Receivers
    //

    // Main monitoring x, y and index feed
    ptr<CFeedOnlineCallbackIfaceGp> _x_pMonitoringFeed
            xauto(Set);

    // State transitions
    ptr<CFeedStateIfaceGp> _x_pMonitoringFeedStateForMath
            xauto(Set);
    ptr<CFeedStateIfaceGp> _x_pMonitoringFeedStateForGames
            xauto(Set);

// Operations

// UI

protected:
private:
    //bool _m_bOneTimeInitFeedRegistryIfaceGpOk = false;

    void _init_AttachToFeedRegistryIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
