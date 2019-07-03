// CNetIfaceGp.h
#pragma once

class CNetDisplayIfaceGp;
class CLoginIfaceGp;

//
// CNetIfaceGp -
//
//   Encapsulates SyncIn and SyncOut and provides
//     common network syncronizatino stuff.
//
ASSUME_IMPL_FOR_IFACE(CNetIface, CNetIfaceGp)
ASSUME_IMPL_FOR_IFACE(CNetImpl, CNetIface)

class CNetIfaceGp : public object
{
public:
    CNetIfaceGp();
    //~CNetIfaceGp();
    //NEW_LOCAL(CNetImpl, CNetIfaceGp)
    NEW_GP(CloudSync, CNetImpl, CNetIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // For subordinate subchannels
    str GetDatabaseChannel()
            vhook;

    // Needed for Fileman
    ref<CLoginIfaceGp> GetNetLogin()
            vhook;

    // To facilitate app cache updates
    int GetImportCount()
            vhook;

    // For sending out BupService reports
    void EnableOnlyNetUploadPump()
            vhook;

    // Current status for all pipes
    //   NOTE: this is also attached for filesync
    // (a ptr<> is just to break the dependency on Net for apps)
    ptr<CNetDisplayIfaceGp> GetNetDisplay()
            vhook;

// Operations

// UI

protected:
    virtual str OnGetDatabaseChannel()
            v1pure;
    virtual ref<CLoginIfaceGp> OnGetNetLogin()
            v1pure;
    virtual int OnGetImportCount()
            v1pure;
    virtual void OnEnableOnlyNetUploadPump()
            v1pure;
    virtual ptr<CNetDisplayIfaceGp> OnGetNetDisplay()
            v1pure;
private:
    //bool _m_bOneTimeInitNetIfaceGpOk = false;
    //bool _m_bNetIfaceGpOpened = false;

    void _init_AttachToNetIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
