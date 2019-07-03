// CNetSyncAbstractIfaceGp.h
#pragma once

#include "CNetErrorsIfaceGp.h"

class CNetEnablePumpIfaceGp;
class CNetLocatorIfaceGp;

//
// CNetSyncAbstractIfaceGp -
//
//   Common Iface for both In and Out directions
SEE_ALSO(CNetSyncCommon) // not related
//

class CNetSyncAbstractIfaceGp : public object
{
public:
    CNetSyncAbstractIfaceGp();
    //~CNetSyncAbstractIfaceGp();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Init status
    bool IsNetSyncInitOk()
            vhook;

    // Network path. Set before init.
    ref<CNetLocatorIfaceGp> x_rNetLocator
            xauto(Get, Set);

    // Online enabler
    ref<CNetEnablePumpIfaceGp> x_rNetEnablePumpSync
            xabstract;
    ref<CNetEnablePumpIfaceGp> x_rNetEnablePumpSync_Get()
            vhook;
    void x_rNetEnablePumpSync_Set(
            ref<CNetEnablePumpIfaceGp> rNetEnablePump)
            vhook;

    // on off shortcut for the pump
    bool x_bEnableNetSyncPumping //= false
            xabstract;
    bool x_bEnableNetSyncPumping_Get()
            vhook;
    void x_bEnableNetSyncPumping_Set(
            bool bEnable)
            vhook;

// Operations

    // One-time opener
    //void InitNetSyncAbstract();

    // Cleanup
    //void CloseNetSyncAbstract();

    // Log error (to Display or optionally to the floater if enabled)
    void LogSyncErrorAsNet(
            CNetErrorsIfaceGp::ENetErrorFrom eNetErrorFrom,
            CNetErrorsIfaceGp::ENetErrorLevel eNetErrorLevel,
            sloc slocFriendly,
            str sDetails,
            str sContext)
            vhook;

// UI

protected:
    virtual bool OnIsNetSyncInitOk()
            v1pure;
    virtual ref<CNetEnablePumpIfaceGp> Onx_rNetEnablePumpSync_Get()
            v1pure;
    virtual void Onx_rNetEnablePumpSync_Set(
            ref<CNetEnablePumpIfaceGp> rNetEnablePump)
            v1pure;
    virtual bool Onx_bEnableNetSyncPumping_Get()
            v1pure;
    virtual void Onx_bEnableNetSyncPumping_Set(
            bool bEnable)
            v1pure;
    virtual void OnLogSyncErrorAsNet(
            CNetErrorsIfaceGp::ENetErrorFrom eNetErrorFrom,
            CNetErrorsIfaceGp::ENetErrorLevel eNetErrorLevel,
            sloc slocFriendly,
            str sDetails,
            str sContext)
            v1pure;
private:
    //bool _m_bOneTimeInitNetSyncAbstractIfaceGpOk = false;
    //bool _m_bNetSyncAbstractIfaceGpOpened = false;

    void _init_AttachToNetSyncAbstractIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
