// CJobStateIface.h
#pragma once

#include "CJobStateIfaceGp.h"

class CTaskIface;

//
// CJobStateIface -
//
//   Current task attributes
//

class CJobStateIface : public CJobStateIfaceGp
{
public:
    CJobStateIface();
    //~CJobStateIface();
    NEW_LOCAL(CJobStateImpl, CJobStateIface)
    //NEW_GP(Ralarm, CJobStateImpl, CJobStateIface)
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

    // Id/path
    str GetJobTaskId(
            out str& out_sError)
            vhook;
    path GetJobTaskFolder(
            out str& out_sError)
            vhook;
    //static path GGetFolderByTaskId(
    //        str sTaskId,
    //        ref<CIoCommonIface> rIoCommon);
        // see Impl

    // In or Out?
    //type<CJobDirectionIface> x_typeJobDirection
    //        xauto(Get, Set);

    // Quest job can be both!
    bool x_bUploadJob = false
            xauto(Get, Set);
    bool x_bDownloadJob = false
            xauto(Get, Set);
    // Quest can be imported incomplete
    bool x_bContinuousReceptionJob = false
            xauto(Get, Set);

    // Unique ID (not sequential)
    //int x_iJob = -1
    //        xauto(Get, Set),
    //        xassert(value >= 0);

    // Short task info
    str x_sJobShort = ""
            xauto(Get, Set)
            xassert(true);

    // Optional task data
    ref<CTaskIface> x_rTask
            xauto(Get, Set);

    // From inbox?
    path x_pathMetaFile = ""
            xauto(Get, Set);

    // Primarty checkup part to be imported
    path x_pathReceivedCheckupInitialFile = ""
            xauto(Get, Set);

    // Large attachment file (fusion is deleted in PatInfo mode!)
    path x_pathReceivedAttachmentFile = ""
            xauto(Get, Set);

    // Failed
    sloc x_slocJobErrorVeryShort = ""
            xauto(Get, Set);

    // Debug
    str x_sJobDebugReport = ""
            xauto(Get, Set);

    // Short to click
        SEE_ALSO(GetJobStatusReport)
    str GetReportInList(
            str sNewLineSeparator)
            vhook;

// Operations

    // One-time opener
    //void InitJobState();

    // Cleanup
    //void CloseJobState();

    // Navigation helper
    void SaveJobSampleKey(
            str sSampleKey)
            vhook;

    // Reply tracer
    void SaveJobCheckupKey(
            str sCheckupKey)
            vhook;

    // Debug
    void SaveJobDebugReport(
            str sReport)
            vhook;

    // Caller from receiver parser
    void NoteJobAttachmentDownloaded()
            vhook;

// UI

protected:
    SEE_ALSO(CJobStateImpl)

    virtual str OnGetJobTaskId(
            out str& out_sError)
            v1pure;
    virtual path OnGetJobTaskFolder(
            out str& out_sError)
            v1pure;
    virtual str OnGetReportInList(
            str sNewLineSeparator)
            v1pure;
    virtual void OnSaveJobSampleKey(
            str sSampleKey)
            v1pure;
    virtual void OnSaveJobCheckupKey(
            str sCheckupKey)
            v1pure;
    virtual void OnSaveJobDebugReport(
            str sReport)
            v1pure;
    virtual void OnNoteJobAttachmentDownloaded()
            v1pure;

private:
    //bool _m_bOneTimeInitJobStateIfaceOk = false;
    //bool _m_bJobStateIfaceOpened = false;

    void _init_AttachToJobStateIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
