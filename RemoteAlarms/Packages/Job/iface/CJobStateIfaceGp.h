// CJobStateIfaceGp.h
#pragma once

//
// CJobStateIfaceGp -
//
//   External access to Ralarm task list.
//
//     See QueryJobs()
//
ASSUME_IMPL_FOR_IFACE(CJobStateIface, CJobStateIfaceGp)
ASSUME_IMPL_FOR_IFACE(CJobStateImpl, CJobStateIface)

class CJobStateIfaceGp : public object
{
public:
    CJobStateIfaceGp();
    //~CJobStateIfaceGp();
    //NEW_LOCAL(CJobStateImpl, CJobStateIfaceGp)
    //NEW_GP(Ralarm, CJobStateImpl, CJobStateIfaceGp)
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

    // Task ID
    key GetJobKey()
            vhook;

    // Direction
    //   Quest job can be both!
    bool IsJobForDownload()
            vhook;
    bool IsJobForUpload()
            vhook;

    // Permanantly canelled?
    bool IsJobAborted()
            vhook;

    // Temp closed?
    bool IsJobPaused()
            vhook;

    // App files (the parcel)
    //  * immediatelly available short data
    //  * large attchment
    path GetJobHeaderPath()
            vhook;
    path GetJobAttachmentPath()
            vhook;

    // Used by rems, send to tip
    str x_sForcedJobDecision = ""
            xauto(Get, Set)
            xassert(true);

// Operations

    // Get the large part (until aborted)
    void StartJobAttachmentDownload()
            vhook;

    // Kill
    void AbortJob()
            vhook;

    // Temporary stop
    void PauseJobInSession()
            vhook;

// UI

    // Progress and status
    ref<UiContainer> NewUiJobStateNetworkRow()
            vhook;

protected:
    SEE_ALSO(CJobStateImpl)

    virtual key OnGetJobKey()
            v1pure;
    virtual bool OnIsJobForDownload()
            v1pure;
    virtual bool OnIsJobForUpload()
            v1pure;
    virtual bool OnIsJobAborted()
            v1pure;
    virtual bool OnIsJobPaused()
            v1pure;
    virtual path OnGetJobHeaderPath()
            v1pure;
    virtual path OnGetJobAttachmentPath()
            v1pure;
    virtual void OnStartJobAttachmentDownload()
            v1pure;
    virtual void OnAbortJob()
            v1pure;
    virtual void OnPauseJobInSession()
            v1pure;
    virtual ref<UiContainer> OnNewUiJobStateNetworkRow()
            v1pure;
private:
    //bool _m_bInited = false;
    //bool _m_bOpened = false;

    void _init_AttachToJobStateIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
