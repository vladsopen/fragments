// CSessionIfaceGp.h
#pragma once

class CSendParcelIfaceGp;
class CJobListIfaceGp;
class CCustomizerIfaceGp;

//
// CSessionIfaceGp -
//
//   Ralarm global context.
//
ASSUME_IMPL_FOR_IFACE(CSessionIface, CSessionIfaceGp)
ASSUME_IMPL_FOR_IFACE(CSessionImpl, CSessionIface)

class CSessionIfaceGp : public object
{
public:
    CSessionIfaceGp();
    //NEW_LOCAL(CSessionImpl, CSessionIfaceGp)
    NEW_GP(Ralarm, CSessionImpl, CSessionIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Init ok?
    bool IsSessionOpened()
            vhook;

    // UI in DB?
    bool JustIsAdded()
            vhook;

    // For the task box access
    ref<CJobListIfaceGp> GetJobList()
            vhook;

    // External controls for slave Ralarm mode
    ref<CCustomizerIfaceGp> x_rCustomizer
            xauto(Get, Set);

    // Networking state
    utc GetLastGoodEventUtc()
            vhook;
    utc GetLastBadEventUtc()
            vhook;
    utc GetLastDownloadReceivedUtc()
            vhook;
    bool IsDownloadListGoodAndComplete()
            vhook;

// Operations

    // Open Ralarm context
    void InitSession()
            vhook;

    // Close Ralarm context
    void CloseSession()
            vhook;

    // External polling
    void HandleSessionIdle(
            out CProject::EIdleNext& out_eIdleNextMayBeUpgraded)
            vhook;

    // Init abstract upload parcel
    ref<CSendParcelIfaceGp> NewSendParcel()
            vhook;

// UI

    // Main UI
    ref<UiContainer> NewUiSessionMain()
            vhook;

    // General Net Display
    ref<UiContainer> NewUiReceiverSessionDisplay()
            vhook;

    // Unwelcome screen
        SEE_ALSO(x_bEnableUnwelcome)
    bool IsUnwelcomeIn()
            vhook;
    ref<UiContainer> NewUiUnwelcome()
            vhook;

protected:
    virtual bool OnIsSessionOpened()
            v1pure;
    virtual bool OnJustIsAdded()
            v1pure;
    virtual ref<CJobListIfaceGp> OnGetJobList()
            v1pure;
    virtual utc OnGetLastGoodEventUtc()
            v1pure;
    virtual utc OnGetLastBadEventUtc()
            v1pure;
    virtual utc OnGetLastDownloadReceivedUtc()
            v1pure;
    virtual bool OnIsDownloadListGoodAndComplete()
            v1pure;
    virtual void OnInitSession()
            v1pure;
    virtual void OnCloseSession()
            v1pure;
    virtual void OnHandleSessionIdle(
            out CProject::EIdleNext& out_eIdleNextMayBeUpgraded)
            v1pure;
    virtual ref<CSendParcelIfaceGp> OnNewSendParcel()
            v1pure;
    virtual ref<UiContainer> OnNewUiSessionMain()
            v1pure;
    virtual ref<UiContainer> OnNewUiReceiverSessionDisplay()
            v1pure;
    virtual bool OnIsUnwelcomeIn()
            v1pure;
    virtual ref<UiContainer> OnNewUiUnwelcome()
            v1pure;
private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToSessionIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
