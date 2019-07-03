// CProxyIfaceGp.h
#pragma once

//
// CProxyIfaceGp -
//
//   CustomApps global context.
//
ASSUME_IMPL_FOR_IFACE(CProxyIface, CProxyIfaceGp)
ASSUME_IMPL_FOR_IFACE(CProxyImpl, CProxyIface)

class CProxyIfaceGp : public object
{
public:
    CProxyIfaceGp();
    //NEW_LOCAL(CProxyImpl, CProxyIfaceGp)
    NEW_GP(CustomApps, CProxyImpl, CProxyIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Init ok?
    bool IsProxyOpened()
            vhook;

// Operations

    // Open CustomApps context
    void InitProxy(
            path pathOpenExistingAttachment,
            str sAttachmentRevision,
            str sAttachmentExtra)
            vhook;

    // Close CustomApps context
    void CloseProxy()
            vhook;

    // External polling
    void HandleProxyIdle(
            out CProject::EIdleNext& out_eIdleNextMayBeUpgraded)
            vhook;

// UI

    // Main UI
    ref<UiContainer> NewUiProxyMain()
            vhook;

protected:
    virtual bool OnIsProxyOpened()
            v1pure;
    virtual void OnInitProxy(
            path pathOpenExistingAttachment,
            str sAttachmentRevision,
            str sAttachmentExtra)
            v1pure;
    virtual void OnCloseProxy()
            v1pure;
    virtual ref<UiContainer> OnNewUiProxyMain()
            v1pure;
    virtual void OnHandleProxyIdle(
            out CProject::EIdleNext& out_eIdleNextMayBeUpgraded)
            v1pure;
private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToProxyIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
