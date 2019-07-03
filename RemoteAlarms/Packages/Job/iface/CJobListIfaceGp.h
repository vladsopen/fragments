// CJobListIfaceGp.h
#pragma once

class CJobStateIfaceGp;
class CPatInfoSessionIfaceGp;

//
// CJobListIfaceGp -
//
//   Maps all existing in/out tasks
//
ASSUME_IMPL_FOR_IFACE(CJobListIface, CJobListIfaceGp)
ASSUME_IMPL_FOR_IFACE(CJobListImpl, CJobListIface)

class CJobListIfaceGp : public object
{
public:
    CJobListIfaceGp();
    //~CJobListIfaceGp();
    //NEW_LOCAL(CJobListImpl, CJobListIfaceGp)
    //NEW_GP(Ralarm, CJobListImpl, CJobListIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CPatInfoSessionIfaceGp> _x_pPatInfoSession
            xauto(Get, Set);
    ref<CPatInfoSessionIfaceGp> GetPatInfoSession()
            return
                _x_pPatInfoSession.Get().Valid();

    // For tracing changes
    utc x_utcLastAddedToJobList = utc()
            xauto(Get);
    utc x_utcLastPollImportAttachments = utc()
            xauto(Get);

// Operations

    // One-time opener
    //void InitJobList();

    // Cleanup
    //void CloseJobList();

    // Iterator
    queryorcount QueryJobs(
            out iter& out_i,
            out ref<CJobStateIfaceGp>& out_rIterJob,
            str sJobKey)
            vhook;

// UI

    // JobList
    ref<UiContainer> NewUiJobListMain(
            ptr<CPatInfoSessionIfaceGp> pPatInfoSession)
            vhook;
    bool x_bExpandBox = false
            xauto(Get);
    void x_bExpandBox_Set( 
            bool value)
            vhook;
    ptrmap<CUiPanelGp> _m_ptrmapUiJobListParent;

protected:
    virtual queryorcount OnQueryJobs(
            out iter& out_i,
            out ref<CJobStateIfaceGp>& out_rIterJob,
            str sJobKey)
            v1pure;
    virtual ref<UiContainer> OnNewUiJobListMain(
            ptr<CPatInfoSessionIfaceGp> pPatInfoSession)
            v1pure;
    virtual void Onx_bExpandBox_Set( 
            bool value)
            v1pure;
private:
    //bool _m_bOneTimeInitJobListIfaceGpOk = false;
    //bool _m_bJobListIfaceGpOpened = false;

    void _init_AttachToJobListIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
