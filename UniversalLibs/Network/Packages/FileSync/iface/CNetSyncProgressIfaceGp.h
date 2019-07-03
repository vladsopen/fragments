// CNetSyncProgressIfaceGp.h
#pragma once

//
// CNetSyncProgressIfaceGp -
//
//   Exposable part transfer progress status
//
//ASSUME_IMPL_FOR_IFACE(CNetSyncProgressIface, CNetSyncProgressIfaceGp)
//ASSUME_IMPL_FOR_IFACE(CNetSyncProgressImpl, CNetSyncProgressIface)

class CNetSyncProgressIfaceGp : public object
{
public:
    CNetSyncProgressIfaceGp();
    //~CNetSyncProgressIfaceGp();
    //NEW_LOCAL(CNetSyncProgressImpl, CNetSyncProgressIfaceGp)
    NEW_GP(Net, CNetSyncProgressImpl, CNetSyncProgressIfaceGp)
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

    // Init ok?
    bool IsNetSyncProgressOpen()
            vhook;

    // 0.0 ... 1.0 (slow!)
    num CalcNetSyncProgress01()
            vhook;

// Operations

    // Cleanup
    //void CloseNetSyncProgress();

    // Delete incomplete parts
    void CancelNetSync()
            vhook;

// UI

protected:
    SEE_ALSO(CNetSyncProgressImpl)

    virtual bool OnIsNetSyncProgressOpen()
            v1pure;
    virtual num OnCalcNetSyncProgress01()
            v1pure;
    virtual void OnCancelNetSync()
            v1pure;

private:
    //bool _m_bOneTimeInitNetSyncProgressIfaceGpOk = false;
    //bool _m_bNetSyncProgressIfaceGpOpened = false;

    void _init_AttachToNetSyncProgressIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
