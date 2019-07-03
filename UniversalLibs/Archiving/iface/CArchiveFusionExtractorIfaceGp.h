// CArchiveFusionExtractorIfaceGp.h
#pragma once

#include "CArchiveFusionBaseIfaceGp.h"

//
// CArchiveFusionExtractorIfaceGp - see base class
SEE_ALSO(CArchiveFusionCreatorIfaceGp)
//
//ASSUME_IMPL_FOR_IFACE(CArchiveFusionExtractorImpl, CArchiveFusionExtractorIfaceGp)

class CArchiveFusionExtractorIfaceGp : public CArchiveFusionBaseIfaceGp
{
public:
    CArchiveFusionExtractorIfaceGp();
    //~CArchiveFusionExtractorIfaceGp();
    //NEW_LOCAL(CArchiveFusionExtractorImpl, CArchiveFusionExtractorIfaceGp)
    NEW_GP(SysCommon, CArchiveFusionExtractorImpl, CArchiveFusionExtractorIfaceGp)
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

    // Complete?
    bool x_bFusionExtractorDone = false
            xauto(Get, Set);

    // Result
    str x_sFusionExtractorError = ""
            xauto(Get, Set)
            xassert(true);

    // Rename dirs when extracting to avoid overwrite
    str x_sAppendToDirNames = ""
            xauto(Get, Set)
            xassert(true);

// Operations

    // One-time opener
    void StartArchiveFusionExtractor(
            path pathBall,
            path pathUnpackToFolder,
            ref<CAppEventFloaterIfaceGp> rProgress)
            vhook;

    // Cleanup
    void CloseArchiveFusionExtractor()
            vhook;

// UI

protected:
    SEE_ALSO(CArchiveFusionExtractorImpl)

    virtual void OnStartArchiveFusionExtractor(
            path pathBall,
            path pathUnpackToFolder,
            ref<CAppEventFloaterIfaceGp> rProgress)
            v1pure;
    virtual void OnCloseArchiveFusionExtractor()
            v1pure;

private:
    //bool _m_bOneTimeInitArchiveFusionExtractorIfaceGpOk = false;
    //bool _m_bArchiveFusionExtractorIfaceGpOpened = false;

    void _init_AttachToArchiveFusionExtractorIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
