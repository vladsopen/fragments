// CNetErrorsIface.h
#pragma once

#include "CNetErrorsIfaceGp.h"

class CNetDisplayIfaceGp;
class CNetLocatorIfaceGp;

//
// CNetErrorsIface -
//
//    Error reporting.
//
ASSUME_IMPL_FOR_IFACE(CNetErrorsImpl, CNetErrorsIface)

class CNetErrorsIface : public CNetErrorsIfaceGp
{
public:
    CNetErrorsIface();
    NEW_LOCAL(CNetErrorsImpl, CNetErrorsIface)
    //NEW_GP(Net, CNetErrorsImpl, CNetErrorsIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

    // Logger
    void LogNetError(
            ENetErrorFrom eNetErrorFrom,
            ENetErrorLevel eNetErrorLevel,
            sloc slocFriendly,
            str sDetails,
            str sContext,
            ref<CNetDisplayIfaceGp> rNetDisplay)
            vhook;

    // Save sequence to ulog
    void LogNextPacketBlock(
            str sPathInfo,
            ref<CNetDisplayIfaceGp> rNetDisplay)
            vhook;

// UI

protected:
    virtual void OnLogNetError(
            ENetErrorFrom eNetErrorFrom,
            ENetErrorLevel eNetErrorLevel,
            sloc slocFriendly,
            str sDetails,
            str sContext,
            ref<CNetDisplayIfaceGp> rNetDisplay)
            v1pure;
    virtual void OnLogNextPacketBlock(
            str sPathInfo,
            ref<CNetDisplayIfaceGp> rNetDisplay)
            v1pure;
private:
    //bool _m_bOneTimeInitNetErrorsIfaceOk = false;

    void _init_AttachToNetErrorsIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
