// CNetConnectionOutIfaceGp.h
#pragma once

#include "CNetConnectionCommonIfaceGp.h"

class CNetBoxAbstractionIfaceGp;
class CNetCallbackOutIfaceGp;
class CNetLocatorIfaceGp;
class CNetDriverIfaceGp;

//
// CNetConnectionOutIfaceGp -
//
//   App sends a data box to [Logohere] transfer channel via
//     this abstract connection object.
//     Implementation uses an associalted driver to handle the transfer.
//     After the transfer is complete or failed the app may recevive a
//     note via CNetCallbackOutIfaceGp.
//
//ASSUME_IMPL_FOR_IFACE(CNetConnectionOutImpl, CNetConnectionOutIfaceGp)

class CNetConnectionOutIfaceGp : public CNetConnectionCommonIfaceGp
{
public:
    CNetConnectionOutIfaceGp();
    //NEW_LOCAL(CNetConnectionOutImpl, CNetConnectionOutIfaceGp)
    NEW_GP(Net, CNetConnectionOutImpl, CNetConnectionOutIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

    // Connect using a driver to a specific location linking to a status callback.
    //   You can only try to open once! Even if failed reopen in a new object!
    void InitNetConnectionOut(
            ptr<CNetDriverIfaceGp> pDriver,
            ptr<CNetCallbackOutIfaceGp> pCallbackOut,
            ref<CNetLocatorIfaceGp> rLocator,
            out str& out_sError)
            vhook;

    // Allways call this to cleanup the driver properly
    void CloseNetConnectionOut()
            vhook;

    // Start transfering (uploading) a package.
    //   Expect status and error info in the callback and Box object.
    void SendNetBox(
            ref<CNetBoxAbstractionIfaceGp> rBox)
            vhook;

// UI

protected:
    virtual void OnInitNetConnectionOut(
            ptr<CNetDriverIfaceGp> pDriver,
            ptr<CNetCallbackOutIfaceGp> pCallbackOut,
            ref<CNetLocatorIfaceGp> rLocator,
            out str& out_sError)
            v1pure;
    virtual void OnCloseNetConnectionOut()
            v1pure;
    virtual void OnSendNetBox(
            ref<CNetBoxAbstractionIfaceGp> rBox)
            v1pure;
private:
    //bool _m_bOneTimeInitNetConnectionOutIfaceGpOk = false;

    void _init_AttachToNetConnectionOutIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
