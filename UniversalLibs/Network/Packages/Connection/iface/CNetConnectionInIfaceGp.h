// CNetConnectionInIfaceGp.h
#pragma once

#include "CNetConnectionCommonIfaceGp.h"

class CNetBoxAbstractionIfaceGp;
class CNetLocatorIfaceGp;
class CNetCallbackInIfaceGp;
class CNetDriverIfaceGp;

//
// CNetConnectionInIfaceGp -
//
//   App receives data from [Logohere] transfer channel via
//     this abstract connection object.
//     Implementation uses an associalted driver to handle the transfer.
//     After the transfer is complete or failed the app may recevive a
//     note via CNetCallbackOutIfaceGp.
//
//ASSUME_IMPL_FOR_IFACE(CNetConnectionInImpl, CNetConnectionInIfaceGp)

class CNetConnectionInIfaceGp : public CNetConnectionCommonIfaceGp
{
public:
    CNetConnectionInIfaceGp();
    //NEW_LOCAL(CNetConnectionInImpl, CNetConnectionInIfaceGp)
    NEW_GP(Net, CNetConnectionInImpl, CNetConnectionInIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // When we done and successfully received all the data
    utc _x_utcLastGoodEmptyDownload = utc()
            xauto(Get, Set)
            xassert(true);

// Operations

    // Connect using a driver to a specific location linking to a status callback.
    //   You can only try to open once! Even if failed reopen in a new object!
    void InitNetConnectionIn(
            ptr<CNetDriverIfaceGp> pDriver,
            ref<CNetCallbackInIfaceGp> rCallbackIn,
            ref<CNetLocatorIfaceGp> rLocator,
            out str& out_sError)
            vhook;

    // Always call this to cleanup the driver properly
    void CloseNetConnectionIn()
            vhook;

    // Internal! Never call. Called by the callback wrapper automatically.
    void HookBeforeBoxReceived(
            ref<CNetBoxAbstractionIfaceGp> rBox)
            vhook;

// UI

protected:
    virtual void OnInitNetConnectionIn(
            ptr<CNetDriverIfaceGp> pDriver,
            ref<CNetCallbackInIfaceGp> rCallbackIn,
            ref<CNetLocatorIfaceGp> rLocator,
            out str& out_sError)
            v1pure;
    virtual void OnCloseNetConnectionIn()
            v1pure;
    virtual void OnHookBeforeBoxReceived(
            ref<CNetBoxAbstractionIfaceGp> rBox)
            v1pure;
private:
    //bool _m_bOneTimeInitNetConnectionInIfaceGpOk = false;

    void _init_AttachToNetConnectionInIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
