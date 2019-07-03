// CPlayBase.h
#pragma once

class CWaveOutQueueIfaceGp;
class CReadingIface;
class CRangesIface;

//
// CPlayBase - 
//
//   Audible Feedback Effect.
//     Extended by specific classes.
//
//ASSUME_IMPL_FOR_IFACE(CPlayBaseImpl, CPlayBase)

class CPlayBase : public object
{
public:
    CPlayBase();
    //NEW_LOCAL(CPlayBaseImpl, CPlayBase)
    //NEW_GP(Balance, CPlayBaseImpl, CPlayBase)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // WaveOut
    static const int C_nPlayBaseWaveFreq = 44100;
    static const tick C_tickPerBlock = 300;
    static const int C_nBuffersAhead = 1;

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

    void InitPlayBaseOnce();

    // Data receiving
    void PlayNextFreq(
            num numFreq,
            num numMinFreq,
            num numMaxFreq,
            num numVolumeMaxAt,
            num numBlackmanAlpha);

    // interferes with audio prompts
    // Muto on pause
    //void SetPlaybackAudible(
    //        bool bAudible);

// UI

protected:
private:
    bool _m_bOneTimeInitPlayBaseOk = false;

    // sound
    ref<CWaveOutQueueIfaceGp> m_rWaveOutQueue;
    utc m_utcLastBufferAdded;
    utc m_utcFirstBufferAdded;
    int m_nBlocksAdded = 0;
    num m_numSinPhase = 0;
    num m_numPrevFreq = 0;
    int m_nPrevVolume = 0;
    num m_numFreqSum = 0;
    int m_nFreqCount = 0;
    array<short> _m_ashortCacheWaveBuffer;
    sbuf _m_sbufCacheWaveBuffer;
    percent _m_percentVolumeSet = percent(-1);

    void _init_AttachToPlayBase();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
