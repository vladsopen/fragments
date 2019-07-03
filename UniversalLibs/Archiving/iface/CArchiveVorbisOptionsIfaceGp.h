// CArchiveVorbisOptionsIfaceGp.h
#pragma once

class CWaveFormatIfaceGp;

//
// CArchiveVorbisOptionsIfaceGp - 
//
//   Vorbis compression settings.
//
//ASSUME_IMPL_FOR_IFACE(CArchiveVorbisOptionsImpl, CArchiveVorbisOptionsIfaceGp)

class CArchiveVorbisOptionsIfaceGp : public object
{
public:
    CArchiveVorbisOptionsIfaceGp();
    //NEW_LOCAL(CArchiveVorbisOptionsImpl, CArchiveVorbisOptionsIfaceGp)
    //NEW_GP(SysCommon, CArchiveVorbisOptionsImpl, CArchiveVorbisOptionsIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Sound Freq and format (weirdly, also applies to non-audio data)
    ref<CWaveFormatIfaceGp> x_rWaveFormat
            xauto(Get);

    // Variable bitrate control
    static const int C_nVorbisBitrateDontCare = -1;
    int x_nVorbisBitrateUpper = C_nVorbisBitrateDontCare
            xauto(Get, Set)
            xassert(
                value == C_nVorbisBitrateDontCare ||
                (value > 0 && value < oo));
    int x_nVorbisBitrateNominal = C_nVorbisBitrateDontCare
            xauto(Get, Set)
            xassert(
                value == C_nVorbisBitrateDontCare ||
                (value > 0 && value < oo));
    int x_nVorbisBitrateLower = C_nVorbisBitrateDontCare
            xauto(Get, Set)
            xassert(
                value == C_nVorbisBitrateDontCare ||
                (value > 0 && value < oo));

    /*
    // Alternate simple quality based setup
    static const num C_numMinVorbisQuality = -0.1;
    static const num C_numMaxVorbisQuality = 1.0;
    static const num C_numUnusedVorbisQuality = OO;
    num x_numVorbisQuality = C_numUnusedVorbisQuality
            xauto(Get, Set)
            xassert(
                value == C_numUnusedVorbisQuality
                ||
                (value >= C_numMinVorbisQuality 
                    && 
                    value <= C_numMaxVorbisQuality));
                    */

// Operations

// UI

protected:
private:
    //bool _m_bOneTimeInitArchiveVorbisOptionsIfaceGpOk = false;

    void _init_AttachToArchiveVorbisOptionsIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
