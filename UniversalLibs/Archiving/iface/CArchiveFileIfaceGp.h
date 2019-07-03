// CArchiveFileIfaceGp.h
#pragma once

//
// CArchiveFileIfaceGp -
//
//   Single file entry in ArchiveFusion tarball.
SEE_ALSO(CArchiveFusionCreatorIfaceGp)
SEE_ALSO(CArchiveFusionExtractorIfaceGp)
//
ASSUME_IMPL_FOR_IFACE(CArchiveFileImpl, CArchiveFileIfaceGp)

class CArchiveFileIfaceGp : public object
{
public:
    CArchiveFileIfaceGp();
    //~CArchiveFileIfaceGp();
    //NEW_LOCAL(CArchiveFileImpl, CArchiveFileIfaceGp)
    NEW_GP(SysCommon, CArchiveFileImpl, CArchiveFileIfaceGp)
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

    // Add a file extracted in the same partial tree path it is located
    void SetFullSourcePathInRootFolder(
            path pathFull,
            path pathSourceRoot,
            out str& out_sError)
            vhook;

    // Add a file arbitrarely moved during extraction
    void SetFullSourcePathAndRelativeTarget(
            path pathFull,
            path pathRelativeTarget,
            out str& out_sError)
            vhook;

    // While packing our own growing ulog
    bool x_bIgnoreFileSizeChanges = false
            xauto(Get, Set);

// Operations

    // One-time opener
    //void InitArchiveFile();

    // Cleanup
    //void CloseArchiveFile();

// UI

protected:
    SEE_ALSO(CArchiveFileImpl)

    virtual void OnSetFullSourcePathInRootFolder(
            path pathFull,
            path pathSourceRoot,
            out str& out_sError)
            v1pure;
    virtual void OnSetFullSourcePathAndRelativeTarget(
            path pathFull,
            path pathRelativeTarget,
            out str& out_sError)
            v1pure;

private:
    //bool _m_bOneTimeInitArchiveFileIfaceGpOk = false;
    //bool _m_bArchiveFileIfaceGpOpened = false;

    void _init_AttachToArchiveFileIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
