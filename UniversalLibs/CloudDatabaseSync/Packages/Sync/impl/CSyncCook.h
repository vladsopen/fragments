// CSyncCook.h
#pragma once

class CNetSyncAbstractIfaceGp;
class CNetImpl;

//
// CSyncCook - 
//
//   (de)Compress/(de)Crypt network packages
//
//ASSUME_IMPL_FOR_IFACE(CSyncCookImpl, CSyncCook)

class CSyncCook : public object
{
public:
    CSyncCook();
    //~CSyncCook();
    //NEW_LOCAL(CSyncCookImpl, CSyncCook)
    //NEW_GP(CloudSync, CSyncCookImpl, CSyncCook)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CNetImpl> _x_pNet
            xauto(Get, Set);
    ref<CNetImpl> GetNet()
            return
                _x_pNet.Get().Valid();

// Operations

    // One-time opener
    //void InitSyncCook();

    // Cleanup
    //void CloseSyncCook();

    // Pack
    str CookActionsBeforeUpload(
            str sActions,
            ref<CNetSyncAbstractIfaceGp> rNetSyncAbstract);

    // UnPack
    str UncookActionsAfterDownload(
            str sActions,
            ref<CNetSyncAbstractIfaceGp> rNetSyncAbstract);

// UI

protected:
private:
    //bool _m_bOneTimeInitSyncCookOk = false;
    //bool _m_bSyncCookOpened = false;

    bool IsLzmaOn();

    void _init_AttachToSyncCook();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
