// CFileNodeIfaceGp.h
#pragma once

#include "CAbstractNodeIfaceGp.h"

//
// CFileNodeIfaceGp -
//
//   Encapsulates a db record for an attached/synchronized file.
//
ASSUME_IMPL_FOR_IFACE(CFileNodeIface, CFileNodeIfaceGp)
ASSUME_IMPL_FOR_IFACE(CFileNodeImpl, CFileNodeIface)

class CFileNodeIfaceGp : public CAbstractNodeIfaceGp
{
public:
    CFileNodeIfaceGp();
    //~CFileNodeIfaceGp();
    //NEW_LOCAL(CFileNodeImpl, CFileNodeIfaceGp)
    NEW_GP(CloudSync, CFileNodeImpl, CFileNodeIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Key
    key GetFilePrimaryKey()
            vhook;

    // File name
    str GetFileName()
            vhook;

    // File dir "MyDir/SubDir"
    str GetFileDir()
            vhook;

    // The target may or may not exist locally
    path GetFileLocalPath()
            vhook;

// Operations

    // One-time opener
    //void InitFileNode();

    // Cleanup
    //void CloseFileNode();

// UI

protected:
    SEE_ALSO(CFileNodeImpl) // F12-lookup

    virtual key OnGetFilePrimaryKey()
            v1pure;
    virtual str OnGetFileName()
            v1pure;
    virtual str OnGetFileDir()
            v1pure;
    virtual path OnGetFileLocalPath()
            v1pure;

private:
    //bool _m_bOneTimeInitFileNodeIfaceGpOk = false;
    //bool _m_bFileNodeIfaceGpOpened = false;

    void _init_AttachToFileNodeIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
