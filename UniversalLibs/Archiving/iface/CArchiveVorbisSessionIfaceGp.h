// CArchiveVorbisSessionIfaceGp.h
#pragma once

#include "CArchiveBasicSessionIfaceGp.h"

class CArchiveCallbackIfaceGp;
class CArchiveVorbisOptionsIfaceGp;

//
// CArchiveVorbisSessionIfaceGp -
//
//   Vorbis/Ogg compressor/decoder.
//
//ASSUME_IMPL_FOR_IFACE(CArchiveVorbisSessionImpl, CArchiveVorbisSessionIfaceGp)

class CArchiveVorbisSessionIfaceGp : public CArchiveBasicSessionIfaceGp
{
public:
    CArchiveVorbisSessionIfaceGp();
    //NEW_LOCAL(CArchiveVorbisSessionImpl, CArchiveVorbisSessionIfaceGp)
    NEW_GP(SysCommon, CArchiveVorbisSessionImpl, CArchiveVorbisSessionIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

    // Callback-based for large streams of unknown size.
    void RunStreamingCallback(
            EArchiveCommand eArchiveCommand,
            ptr<CArchiveVorbisOptionsIfaceGp> pArchiveVorbisOptions,
            ref<CArchiveCallbackIfaceGp> rArchiveCallback,
            out str& out_sError)
            vhook;

// UI

protected:
    virtual void OnRunStreamingCallback(
            EArchiveCommand eArchiveCommand,
            ptr<CArchiveVorbisOptionsIfaceGp> pArchiveVorbisOptions,
            ref<CArchiveCallbackIfaceGp> rArchiveCallback,
            out str& out_sError)
            v1pure;
private:
    //bool _m_bOneTimeInitArchiveVorbisSessionIfaceGpOk = false;

    void _init_AttachToArchiveVorbisSessionIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
