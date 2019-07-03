// CNetIface.h
#pragma once

#include "CNetIfaceGp.h"

class CUdbInitConnectIfaceGp;
class CNetEnablePumpIfaceGp;
class CNetDisplayIfaceGp;
class CStatusPipeIface;
class CLoginIfaceGp;
class CSyncOutIface;
class CSyncInIface;
class CSessionIface;

//
// CNetIface -
//
//   Encapsulates SyncIn and SyncOut and provides
//     common network syncronizatino stuff.
//

class CNetIface : public CNetIfaceGp
{
public:
    CNetIface();
    //~CNetIface();
    NEW_LOCAL(CNetImpl, CNetIface)
    //NEW_GP(CloudSync, CNetImpl, CNetIface)
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

    // Downloader
    ref<CSyncInIface> x_rSyncIn
            xauto(Get);

    // Uploader
    ref<CSyncOutIface> x_rSyncOut
            xauto(Get);

    // Synchronize ON/OFF switch
    ref<CNetEnablePumpIfaceGp> x_rNetEnablePumpSync
            xauto(Get);

    // Current status for all pipes
    ref<CNetDisplayIfaceGp> _x_rNetDisplay
            xauto(Get);
    // see Get in Gp

// Operations

    // One-time opener
    void InitNet(
            str sDatabaseChannel,
            ref<CLoginIfaceGp> rLogin,
            ref<CUdbInitConnectIfaceGp> rUdbInitConnect)
            vhook;

    // Cleanup
    void CloseNet()
            vhook;

    // External polling
    void ForcePollNet(
            out bool& out_bHaveJobs)
            vhook;

    // Feed progress to the statusbar
    void UpdateStatusPipeForMainNet(
            ref<CStatusPipeIface> rStatusPipeIn,
            ref<CStatusPipeIface> rStatusPipeOut)
            vhook;

// UI

    // Network status bar
    ref<UiContainer> NewUiNetStatus()
            vhook;

    // Inserted into hidden session menu content
    //   Create to control synchronized ON/OFF pump
    ref<UiContainer> NewUiNetPrivateMenu()
            vhook;

    // Left static
    //void EqualizeNetStatusLabel(
    //        ref<CUiPanelGp> rUi);
    static const scale C_scaleNetStatusLabelWidth = prescale(100);

protected:
    SEE_ALSO(CNetImpl) // F12-lookup

    virtual void OnInitNet(
            str sDatabaseChannel,
            ref<CLoginIfaceGp> rLogin,
            ref<CUdbInitConnectIfaceGp> rUdbInitConnect)
            v1pure;
    virtual void OnCloseNet()
            v1pure;
    virtual void OnForcePollNet(
            out bool& out_bHaveJobs)
            v1pure;
    virtual void OnUpdateStatusPipeForMainNet(
            ref<CStatusPipeIface> rStatusPipeIn,
            ref<CStatusPipeIface> rStatusPipeOut)
            v1pure;
    virtual ref<UiContainer> OnNewUiNetStatus()
            v1pure;
    virtual ref<UiContainer> OnNewUiNetPrivateMenu()
            v1pure;

private:
    //bool _m_bOneTimeInitNetIfaceOk = false;
    //bool _m_bNetIfaceOpened = false;

    void _init_AttachToNetIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
