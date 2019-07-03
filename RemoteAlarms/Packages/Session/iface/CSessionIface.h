// CSessionIface.h
#pragma once

#include "CSessionIfaceGp.h"

class CWaveDocIface;
class CIoCommonIface;
class CLogin;
class CJobListIface;
class CUserListIface;
class CReceiverSessionIface;
class CSenderSessionIface;
class CPatientLinkIface;
class CPatientLinkSampleIface;

//
// CSessionIface - see iface
//

class CSessionIface : public CSessionIfaceGp
{
public:
    CSessionIface();
    NEW_LOCAL(CSessionImpl, CSessionIface)
    //NEW_GP(Ralarm, CSessionImpl, CSessionIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Global object access encapsulation
    ref<CPatientLinkIface> GetPatientLink()
            vhook;

    // Network
    ref<CSenderSessionIface> x_rSenderSession
            xauto(Get);
    ref<CReceiverSessionIface> x_rReceiverSession
            xauto(Get);

    // ECG, etc
    //ref<CWaveDocIface> x_rWaveDoc
    //        xauto(Get);

    // All users
    ref<CUserListIface> x_rUserList
            xauto(Get);

    // Inbox|Outbox
    ref<CJobListIface> x_rJobList
            xauto(Get, Set);

    // Login
    ref<CLogin> x_rLogin
            xauto(Get, Set);

    // Common ExpertMail network I/O
    ref<CIoCommonIface> x_rIoCommon
            xauto(Get);

// Operations

// UI

    // (for main window caption)
    ref<UiContainer> NewUiSessionPatientLink()
            vhook;

    // Common Status / Login button
    ref<UiContainer> NewUiSessionCornerPopup()
            vhook;

protected:
    virtual ref<CPatientLinkIface> OnGetPatientLink()
            v1pure;
    virtual ref<UiContainer> OnNewUiSessionPatientLink()
            v1pure;
    virtual ref<UiContainer> OnNewUiSessionCornerPopup()
            v1pure;
private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToSessionIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
