// CArchiveBasicSessionIfaceGp.h
#pragma once

class CArchiveBasicOptionsIfaceGp;

//
// CArchiveBasicSessionIfaceGp -
//
//   Common archiving session (pure abastract)
//
//ASSUME_IMPL_FOR_IFACE(CArchiveSessionImpl, CArchiveBasicSessionIfaceGp)

class CArchiveBasicSessionIfaceGp : public object
{
public:
    CArchiveBasicSessionIfaceGp();
    //NEW_LOCAL(CArchiveSessionImpl, CArchiveBasicSessionIfaceGp)
    //NEW_GP(SysCommon, CArchiveSessionImpl, CArchiveBasicSessionIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // Operation selector
    enum EArchiveCommand
    {
        E_ArchiveCommand_Compress,
        E_ArchiveCommand_Decompress,
    };

    // This option allows moving all LZMA processing logic into a
    //   temporary thread separate from the default app thread.
    //   See also x_nMaxCpuThreads which is an internal 7z LZMA feature
    //   enabling some parallel encoding operations on Windows.
    enum EArchiveMainThread
    {
        E_ArchiveMainThread_UseDefault,
        E_ArchiveMainThread_BranchNew,
    };

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

    // Memory --> Memory. Optimized. Use with caution!
    //   * You MUST be sure you always have more than enough memory!
    //      Running memory-to-memory can be slightly faster ONLY FOR SMALL
    //      blocks for which it is very inefficient to create temporary files.
    //   * You MUST preallocate out_sbufResult before decompression to
    //      maximum decompressed size you can handle.
    //      In all cases where you are not sure how much space to reserve
    //      you are probably handling blocks too large to be safe and efficient for
    //      in-memory compression. By consuming too much RAM you will only slow down or
    //      crash your app!
    void RunArchiveInMemory(
            EArchiveCommand eArchiveCommand,
            ptr<CArchiveBasicOptionsIfaceGp> pArchiveBasicOptions,
            sbuf sbufSource,
            out sbuf& out_sbufResult,
            out str& out_sError)
            vhook;

// UI

protected:
    virtual void OnRunArchiveInMemory(
            EArchiveCommand eArchiveCommand,
            ptr<CArchiveBasicOptionsIfaceGp> pArchiveBasicOptions,
            sbuf sbufSource,
            out sbuf& out_sbufResult,
            out str& out_sError)
            v1pure;
private:
    //bool _m_bOneTimeInitArchiveSessionIfaceGpOk = false;

    void _init_AttachToArchiveSessionIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
