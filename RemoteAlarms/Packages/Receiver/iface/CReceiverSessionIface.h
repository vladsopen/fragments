// CReceiverSessionIface.h
#pragma once

class CPatInfoSessionIfaceGp;
class CIoCommonIface;
class CJobStateIface;
class CSessionIface;
class CJobListIfaceGp;
class CPiSampleIfaceGp;
class CTaskIface;

//
// CReceiverSessionIface -
//
//    Downloads ExpertMail stream and sorts to patients
//
ASSUME_IMPL_FOR_IFACE(CReceiverSessionImpl, CReceiverSessionIface)

class CReceiverSessionIface : public object
{
public:
    CReceiverSessionIface();
    //~CReceiverSessionIface();
    NEW_LOCAL(CReceiverSessionImpl, CReceiverSessionIface)
    //NEW_GP(Ralarm, CReceiverSessionImpl, CReceiverSessionIface)
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

    // debug
    ptr<CPatInfoSessionIfaceGp> _x_pAttachedPatInfoSessionForImport
            xauto(Get, Set);

    // Location for merged files
    path GetTempInbox()
            vhook;

    // Network status
    utc GetLastReceiverGoodEventUtc()
            vhook;
    utc GetLastReceiverBadEventUtc()
            vhook;
    utc GetLastReceiverDownloadReceivedUtc()
            vhook;
    bool IsReceiverDownloadListGoodAndComplete()
            vhook;

// Operations

    // One-time opener
    void InitReceiverSession()
            vhook;

    // Cleanup
    void CloseReceiverSession()
            vhook;

    // Called from Net Protocol callback upon a file reception
    //   to handle HTTP POST and header values attached to a file
    void ReceiveDownloadedTask(
            ref<CTaskIface> rTask,
            path pathDownloadedFile,
            out str& out_sRaiseNetError)
            vhook;

    // User clicked on the task
    bool TryImportJob(
            ref<CJobStateIface> rJob)
            vhook;

    // Set Net to clear stale parts
    void SetupSkipToNewest()
            vhook;

    // Scan by job list
    void PollImportJobAttachments(
            ref<CJobStateIface> rJobState)
            vhook;

// UI

    // Main
    ref<UiContainer> NewUiReceiverSessionMain()
            vhook;

    // PatientInseted sample status
    ref<UiContainer> NewUiReceiverUnderIndicators(
            ref<CPiSampleIfaceGp> rPiSample)
            vhook;

    // Status bar
    ref<UiContainer> NewUiReceiverSessionStatus()
            vhook;

    // Net Display
    ref<UiContainer> NewUiReceiverNetDisplay()
            vhook;

    // Replace Main window until connected
    bool IsShowingReceiverUnwelcome()
            vhook;
    ref<UiContainer> NewUiReceiverUnwelcome()
            vhook;

protected:
    SEE_ALSO(CReceiverSessionImpl)

    virtual utc OnGetLastReceiverGoodEventUtc()
            v1pure;
    virtual utc OnGetLastReceiverBadEventUtc()
            v1pure;
    virtual utc OnGetLastReceiverDownloadReceivedUtc()
            v1pure;
    virtual bool OnIsReceiverDownloadListGoodAndComplete()
            v1pure;
    virtual void OnInitReceiverSession()
            v1pure;
    virtual path OnGetTempInbox()
            v1pure;
    virtual void OnCloseReceiverSession()
            v1pure;
    virtual ref<UiContainer> OnNewUiReceiverSessionMain()
            v1pure;
    virtual void OnReceiveDownloadedTask(
            ref<CTaskIface> rTask,
            path pathDownloadedFile,
            out str& out_sRaiseNetError)
            v1pure;
    virtual bool OnTryImportJob(
            ref<CJobStateIface> rJob)
            v1pure;
    virtual void OnSetupSkipToNewest()
            v1pure;
    virtual void OnPollImportJobAttachments(
            ref<CJobStateIface> rJobState)
            v1pure;
    virtual ref<UiContainer> OnNewUiReceiverUnderIndicators(
            ref<CPiSampleIfaceGp> rPiSample)
            v1pure;
    virtual ref<UiContainer> OnNewUiReceiverSessionStatus()
            v1pure;
    virtual ref<UiContainer> OnNewUiReceiverNetDisplay()
            v1pure;
    virtual bool OnIsShowingReceiverUnwelcome()
            v1pure;
    virtual ref<UiContainer> OnNewUiReceiverUnwelcome()
            v1pure;

private:
    //bool _m_bOneTimeInitReceiverSessionIfaceOk = false;
    //bool _m_bReceiverSessionIfaceOpened = false;

    void _init_AttachToReceiverSessionIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
