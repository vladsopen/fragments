// CFileClassIface.h
#pragma once

#include "CFileClassIfaceGp.h"

class CStatusPipeIface;

//
// CFileClassIface - see CFileClassIfaceGp
//
//ASSUME_IMPL_FOR_IFACE(CFileClassImpl, CFileClassIface)

class CFileClassIface : public CFileClassIfaceGp
{
public:
    CFileClassIface();
    //~CFileClassIface();
    NEW_LOCAL(CFileClassImpl, CFileClassIface)
    //NEW_GP(CloudSync, CFileClassImpl, CFileClassIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Attach folder to be synced in this session
    str x_sFileSyncFolderName = ""
            xauto(Get, Set)
            xassert(
                value != "" &&
                path(value).GetFileNameWithoutExt() == value);

// Operations

    //// External polling
    //void ForcePollFileClass(
    //        out bool& out_bHaveJobs)
    //        vhook;
        // see Gp

    // Feed progress to the statusbar
    void UpdateStatusPipeForFileNet(
            ref<CStatusPipeIface> rStatusPipeIn,
            ref<CStatusPipeIface> rStatusPipeOut)
            vhook;

// UI

protected:
    SEE_ALSO(CFileClassImpl) // F12-lookup

    virtual void OnUpdateStatusPipeForFileNet(
            ref<CStatusPipeIface> rStatusPipeIn,
            ref<CStatusPipeIface> rStatusPipeOut)
            v1pure;

private:
    //bool _m_bOneTimeInitFileClassIfaceOk = false;
    //bool _m_bFileClassIfaceOpened = false;

    void _init_AttachToFileClassIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
