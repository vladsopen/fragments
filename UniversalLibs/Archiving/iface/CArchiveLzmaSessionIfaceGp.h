// CArchiveLzmaSessionIfaceGp.h
#pragma once

INCLUDABILITY(
    AllowPath "/SysCommon/Packages/Archiving/",
    AllowPath "/Net/Packages/",
    AllowPath "/CloudSync/Packages/",
    AllowPath "/SysCommon/Packages/",
    );

#include "CArchiveBasicSessionIfaceGp.h"

class CArchiveCallbackIfaceGp;
class CArchiveLzmaOptionsIfaceGp;

//
// CArchiveLzmaSessionIfaceGp -
//
//   Buffer/file compressor/decompressor using LZMA - default 7zip method.
//     Encapsulates LL Sevenz.lib.
//
//ASSUME_IMPL_FOR_IFACE(CArchiveLzmaSessionImpl, CArchiveLzmaSessionIfaceGp)

class CArchiveLzmaSessionIfaceGp : public CArchiveBasicSessionIfaceGp
{
public:
    CArchiveLzmaSessionIfaceGp();
    //NEW_LOCAL(CArchiveLzmaSessionImpl, CArchiveLzmaSessionIfaceGp)
    NEW_GP(SysCommon, CArchiveLzmaSessionImpl, CArchiveLzmaSessionIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // See x_bCreatePrivateHeader
    int JustGetDefaultCompressedHeaderBytes()
            vhook;

// Attributes

// Operations

    //
    // Notes:
    //   * Only one operation per CArchiveLzmaSession object is allowed.
    //   * File-to-file and in-memory compression use the same archive image format.
    //   * Compressed file will contain all metadata required for decompression.
    //   * Actual header format is controlled by x_bShowPublic7zipHeader.
    //   * pArchiveLzmaOptions must be defined for compressing and
    //       must be null() for decompressing.
    //

    // File --> File
    //   Note: the archive does not contain any file info, only single plain buffer.
    void RunArchiveFileToFile(
            EArchiveCommand eArchiveCommand,
            ptr<CArchiveLzmaOptionsIfaceGp> pArchiveLzmaOptions,
            path pathIn,
            path pathOut,
            out str& out_sError)
            vhook;

    //void RunArchiveInMemory()
        SEE_ALSO(CArchiveBasicSessionIfaceGp)

    // Callback-based for large streams
    //   * Safe threading obviously is not possible here with application
    //      defined callbacks.
    void RunArchiveWithCallback(
            EArchiveCommand eArchiveCommand,
            ptr<CArchiveLzmaOptionsIfaceGp> pArchiveLzmaOptions,
            big nnSourceBytes,
            ref<CArchiveCallbackIfaceGp> rArchiveCallback,
            out str& out_sError)
            vhook;

// UI

protected:
    virtual int OnJustGetDefaultCompressedHeaderBytes()
            v1pure;
    virtual void OnRunArchiveFileToFile(
            EArchiveCommand eArchiveCommand,
            ptr<CArchiveLzmaOptionsIfaceGp> pArchiveLzmaOptions,
            path pathIn,
            path pathOut,
            out str& out_sError)
            v1pure;
    virtual void OnRunArchiveWithCallback(
            EArchiveCommand eArchiveCommand,
            ptr<CArchiveLzmaOptionsIfaceGp> pArchiveLzmaOptions,
            big nnSourceBytes,
            ref<CArchiveCallbackIfaceGp> rArchiveCallback,
            out str& out_sError)
            v1pure;
private:
    //bool _m_bOneTimeInitLzmaIfaceGpOk = false;

    void _init_AttachToLzmaIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
