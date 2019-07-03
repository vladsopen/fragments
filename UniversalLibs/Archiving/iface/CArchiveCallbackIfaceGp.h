// CArchiveCallbackIfaceGp.h
#pragma once

//
// CArchiveCallbackIfaceGp -
//
//   Handles input/output streams and progress status for concurrent
//     block-by-block archiving operations.
//
//ASSUME_IMPL_FOR_IFACE(CArchiveCallbackImpl, CArchiveCallbackIfaceGp)
// app defined.

class CArchiveCallbackIfaceGp : public object
{
public:
    CArchiveCallbackIfaceGp();
    //NEW_LOCAL(CArchiveCallbackImpl, CArchiveCallbackIfaceGp)
    //NEW_GP(SysCommon, CArchiveCallbackImpl, CArchiveCallbackIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

    // App handler must assign exactly nReadBytes bytes of the next
    //   data block to out_sbufNextBlock.
    //   The exception is the very last data block which cah be
    //   less than nReadBytes or zero length.
    //   Setting out_sError to non-empty will abort archiving procedure.
    void ReadArchiveStream(
            int nReadBytes,
            out sbuf& out_sbufNextBlock,
            out str& out_sError)
            vhook;

    // App handler appends the given block to its output stream.
    //   Setting out_sError to non-empty will abort archiving procedure.
    void WriteArchiveStream(
            sbuf sbufNextBlock,
            out str& out_sError)
            vhook;

// UI

protected:
    virtual void OnReadArchiveStream(
            int nReadBytes,
            out sbuf& out_sbufNextBlock,
            out str& out_sError)
            v1pure;
    virtual void OnWriteArchiveStream(
            sbuf sbufNextBlock,
            out str& out_sError)
            v1pure;
private:
    //bool _m_bOneTimeInitArchiveCallbackIfaceGpOk = false;

    void _init_AttachToArchiveCallbackIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
