// CSyncInIface.h
#pragma once

class CStoreIface;
class CNetImpl;
class CNetIface;

//
// CSyncInIface -
//
//   Downloads DB update Actions from server for merging into the local DB.
//
ASSUME_IMPL_FOR_IFACE(CSyncInImpl, CSyncInIface)

class CSyncInIface : public object
{
public:
    CSyncInIface();
    //~CSyncInIface();
    NEW_LOCAL(CSyncInImpl, CSyncInIface)
    //NEW_GP(CloudSync, CSyncInImpl, CSyncInIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CNetIface> _x_pNet
            xauto(Get, Set);

// Operations

    // After imported data is commited we must clear imports
    void DeleteAllImportedFiles()
            vhook;

// UI

    // Status bar
    ref<UiContainer> NewUiSyncInStatus()
            vhook;

protected:
    SEE_ALSO(CSyncInImpl) // F12-lookup

    virtual void OnDeleteAllImportedFiles()
            v1pure;
    virtual ref<UiContainer> OnNewUiSyncInStatus()
            v1pure;

private:
    //bool _m_bOneTimeInitSyncInIfaceOk = false;
    //bool _m_bSyncInIfaceOpened = false;

    void _init_AttachToSyncInIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
