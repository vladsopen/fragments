// CArchiveFusionCreatorIfaceGp.h
#pragma once

#include "CArchiveFusionBaseIfaceGp.h"

class CArchiveBasicOptionsIfaceGp;
class CAppEventFloaterIfaceGp;
class CArchiveFileIfaceGp;

//
// CArchiveFusionCreatorIfaceGp - see base class
SEE_ALSO(CArchiveFusionExtractorIfaceGp)
//
//ASSUME_IMPL_FOR_IFACE(CArchiveFusionCreatorImpl, CArchiveFusionCreatorIfaceGp)

class CArchiveFusionCreatorIfaceGp : public CArchiveFusionBaseIfaceGp
{
public:
    CArchiveFusionCreatorIfaceGp();
    //~CArchiveFusionCreatorIfaceGp();
    //NEW_LOCAL(CArchiveFusionCreatorImpl, CArchiveFusionCreatorIfaceGp)
    NEW_GP(SysCommon, CArchiveFusionCreatorImpl, CArchiveFusionCreatorIfaceGp)
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

    // Pack small crap just for testing
    bool x_bDryRunFusion = false
            xauto(Get, Set);

    // Extension is used if temp file is created internally
    path x_pathDefaultBallExt = ".tempball"
            xauto(Get, Set)
            xassert(value.Len() >= 2 && value.GetFileExt() == value);

// Operations

    // One-time opener
    void InitArchiveFusionCreator(
            path pathForceTempBall)
            vhook;

    // Cleanup, delete the temp file
    void CloseArchiveFusionCreator()
            vhook;

    // Add next file
    void FuseArchiveFile(
            ref<CArchiveFileIfaceGp> rArchiveFile,
            out str& out_sError)
            vhook;

    // After all files added tarball file must be ready
    path FinalizeArchiveFusion(
            ref<CArchiveBasicOptionsIfaceGp> rArchiveBasicOptions,
            ref<CAppEventFloaterIfaceGp> rProgress,
            out str& out_sError)
            vhook;

// UI

protected:
    SEE_ALSO(CArchiveFusionCreatorImpl)

    virtual void OnInitArchiveFusionCreator(
            path pathForceTempBall)
            v1pure;
    virtual void OnCloseArchiveFusionCreator()
            v1pure;
    virtual void OnFuseArchiveFile(
            ref<CArchiveFileIfaceGp> rArchiveFile,
            out str& out_sError)
            v1pure;
    virtual path OnFinalizeArchiveFusion(
            ref<CArchiveBasicOptionsIfaceGp> rArchiveBasicOptions,
            ref<CAppEventFloaterIfaceGp> rProgress,
            out str& out_sError)
            v1pure;

private:
    //bool _m_bOneTimeInitArchiveFusionCreatorIfaceGpOk = false;
    //bool _m_bArchiveFusionCreatorIfaceGpOpened = false;

    void _init_AttachToArchiveFusionCreatorIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
