// CSenderSessionIface.h
#pragma once

class CPiPatientIfaceGp;
class CSendParcelIface;
class CSessionIface;
class CPiCheckupIfaceGp;
class CTaskIface;

//
// CSenderSessionIface -
//
//   GSM modem emulation
//
ASSUME_IMPL_FOR_IFACE(CSenderSessionImpl, CSenderSessionIface)

class CSenderSessionIface : public object
{
public:
    CSenderSessionIface();
    //~CSenderSessionIface();
    NEW_LOCAL(CSenderSessionImpl, CSenderSessionIface)
    //NEW_GP(Ralarm, CSenderSessionImpl, CSenderSessionIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CSessionIface> _x_pSession
            xauto(Get, Set);
    ref<CSessionIface> GetSession()
            return
                _x_pSession.Get().Valid();

        SEE_ALSO(x_sSpecialSessionSeparator)
    bool x_bUseSenderAsAlarmEmulator = false
            xauto(Get, Set);

    // No packing in GSM device
    bool IsParapackEnabledForSession()
            vhook;

    // Network status
    utc GetLastSenderGoodEventUtc()
            vhook;
    utc GetLastSenderBadEventUtc()
            vhook;

// Operations

    // One-time opener
    void InitSenderSession()
            vhook;

    // Cleanup
    void CloseSenderSession()
            vhook;

    // Checkup menu handler
    void PromptToSendExpertMailCheckup(
            ref<CPiCheckupIfaceGp> rPiCheckup)
            vhook;

    // Attach parcel to sender for external tool use
    ref<CSendParcelIface> PrepareAbstractParcel()
            vhook;

    // Backup mode
    bool BackupPiPatient(
            ref<CPiPatientIfaceGp> rPiPatient)
            vhook;

// UI

    // Main
    ref<UiContainer> NewUiSenderSessionMain()
            vhook;

    // Status bar
    ref<UiContainer> NewUiSenderSessionStatus()
            vhook;

protected:
    SEE_ALSO(CSenderSessionImpl)

    virtual bool OnIsParapackEnabledForSession()
            v1pure;
    virtual utc OnGetLastSenderGoodEventUtc()
            v1pure;
    virtual utc OnGetLastSenderBadEventUtc()
            v1pure;
    virtual void OnInitSenderSession()
            v1pure;
    virtual void OnCloseSenderSession()
            v1pure;
    virtual void OnPromptToSendExpertMailCheckup(
            ref<CPiCheckupIfaceGp> rPiCheckup)
            v1pure;
    virtual ref<CSendParcelIface> OnPrepareAbstractParcel()
            v1pure;
    virtual bool OnBackupPiPatient(
            ref<CPiPatientIfaceGp> rPiPatient)
            v1pure;
    virtual ref<UiContainer> OnNewUiSenderSessionMain()
            v1pure;
    virtual ref<UiContainer> OnNewUiSenderSessionStatus()
            v1pure;

private:
    //bool _m_bOneTimeInitSenderSessionIfaceOk = false;
    //bool _m_bSenderSessionIfaceOpened = false;

    void _init_AttachToSenderSessionIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
