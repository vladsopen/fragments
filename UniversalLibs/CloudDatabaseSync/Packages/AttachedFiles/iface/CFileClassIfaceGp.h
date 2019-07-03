// CFileClassIfaceGp.h
#pragma once

#include "CAbstractClassIfaceGp.h"

class CFileNodeIfaceGp;

//
// CFileClassIfaceGp -
//
//   Fileman context for Session
//
ASSUME_IMPL_FOR_IFACE(CFileClassIface, CFileClassIfaceGp)
ASSUME_IMPL_FOR_IFACE(CFileClassImpl, CFileClassIface)

class CFileClassIfaceGp : public CAbstractClassIfaceGp
{
public:
    CFileClassIfaceGp();
    //~CFileClassIfaceGp();
    //NEW_LOCAL(CFileClassImpl, CFileClassIfaceGp)
    NEW_GP(CloudSync, CFileClassImpl, CFileClassIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Public "Dropbox"
    path GetLocalFileSyncFolder()
            vhook;

// Operations

    // Query Nodes
    bool IterateFileNodes(
            out ref<CQueryIfaceGp>& out_rQuery,
            out ref<CFileNodeIfaceGp>& out_rFileNode)
            vhook;

    // By primary key
    bool QueryFileNodeByKey(
            key keyLookup,
            out ref<CFileNodeIfaceGp>& out_rFileNode)
            vhook;

    // Allows to disable local file polling while
    //   app creates new files
    //   (added for CheckupStarted/CheckupFinished)
    void SetFileLocalUploaderScanDisableInfo(
            str sDisableInfo)
            vhook;

    // External polling
    void ForcePollFileClass(
            out bool& out_bHaveJobs)
            vhook;

// UI

protected:
    SEE_ALSO(CFileClassImpl) // F12-lookup

    virtual path OnGetLocalFileSyncFolder()
            v1pure;
    virtual bool OnIterateFileNodes(
            out ref<CQueryIfaceGp>& out_rQuery,
            out ref<CFileNodeIfaceGp>& out_rFileNode)
            v1pure;
    virtual bool OnQueryFileNodeByKey(
            key keyLookup,
            out ref<CFileNodeIfaceGp>& out_rFileNode)
            v1pure;
    virtual void OnSetFileLocalUploaderScanDisableInfo(
            str sDisableInfo)
            v1pure;
    virtual void OnForcePollFileClass(
            out bool& out_bHaveJobs)
            v1pure;

private:
    //bool _m_bOneTimeInitFileClassIfaceGpOk = false;
    //bool _m_bFileClassIfaceGpOpened = false;

    void _init_AttachToFileClassIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
