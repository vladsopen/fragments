// CCustomizerIfaceGp.h
#pragma once

class CJobStateIfaceGp;

//
// CCustomizerIfaceGp - 
//
//   External slave Ralarm controls / callback
//

class CCustomizerIfaceGp : public object
{
public:
    CCustomizerIfaceGp();
    //~CCustomizerIfaceGp();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);
    //ref<> Get ()
    //        return
    //            _x_p .Get().Valid();

    // For using Ralarm as a tool
    bool x_bWithoutPatInfo = false
            xauto(Get, Set);

    // Disable this if you don't show blocking UI
    bool x_bEnableUnwelcome = true
            xauto(Get, Set);

    // Main app wants to control large attachemt downloads
    //   (by default it's by SampleKey)
    // This won't affect automatic download restart if already
    //   started in a previous session (checked by dir.Exists)
    bool x_bAutoAttachmentDownloadStart = true
            xauto(Get, Set);

    // Instead of import into PatInfo
    path x_pathJustMoveAttachmentsToFolder = path()
            xauto(Get, Set)
            xassert(true);

    // Local files
    path x_pathAlternateTasksRoot = path()
            xauto(Get, Set)
            xassert(true);

    // Ending for local data dir - AppLogo is the default
    str x_sAlternateLocalAppName = ""
            xauto(Get, Set)
            xassert(true);

    // Net channel
    str x_sAlternateNetChannelRoot = ""
            xauto(Get, Set)
            xassert(true);

    // Net reception mask
    str x_sForceReceiverMetaPregFilter = ""
            xauto(Get, Set)
            xassert(true);

    // We abuse Ralarm for local backup jobs
        SEE_ALSO(x_bEnableNetworking)
    bool x_bEnableNetworking = true
            xauto(Get, Set)
            xassert(true);

    // Backup dont need background complications
    bool x_bEnableSenderParapack = true
            xauto(Get, Set)
            xassert(true);

// Operations

    // One-time opener
    //void InitCustomizer();

    // Cleanup
    //void CloseCustomizer();

    // User clicks a job in the box
    void HandleJobClick(
            ref<CJobStateIfaceGp> rJobState,
            out bool& out_bDoneSomething)
            vhook;

// UI

protected:
    SEE_ALSO(CUmailCustomizer)

    virtual void OnHandleJobClick(
            ref<CJobStateIfaceGp> rJobState,
            out bool& out_bDoneSomething)
            v1null;

private:
    //bool _m_bInited = false;
    //bool _m_bOpened = false;

    void _init_AttachToCustomizerIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

