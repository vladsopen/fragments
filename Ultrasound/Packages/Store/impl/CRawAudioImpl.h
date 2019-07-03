// CRawAudioImpl.h
#pragma once

#include "CRawAudioIface.h"

class CAudioSliceGp;
class CStoreImpl;
class CRawWaveImpl;

//
// CRawAudioImpl - see iface
//

class CRawAudioImpl : public CRawAudioIface
{
public:
    CRawAudioImpl();
    ~CRawAudioImpl();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Overflow the storgage twice this size and it will purge half data
    int CalcMaxStoredAudioComposites();

    // Count shorts / 2
    int GetAudioCompositePairCount();

    // Read a quadrature,
    //   encapsulating internal circular buffer, emulating shifted buffer
    int GetAudioAt(
            int iChannel,
            int iAt);

    // Change a quadrature,
    //   encapsulating internal circular buffer, emulating shifted buffer
    void SetAudioAt(
            int iChannel,
            int iAt,
            int nAmplitude);

    // Current selection
    //link<CRawWaveImpl> x_linkSelection
    //        xauto(Get);
    //void x_linkSelection_Set(link<CRawWaveImpl> value); // UI update

    // Get whole stored audio ref (opmimization)
    //ref<CAudioSliceGp> GetInternalRawAudioArrayAsInterleaved();

    // Parent backref
    ptr<CStoreImpl> _x_pParentStore
            xauto(Get, Set);

// array<> Publishing

    int GetWaveChannelCount()
        { return m_aRawWave.GetCount(); }
    // void SetCount(int nNewSize)
    //     { return m_aRawWave.SetCount(nNewSize); }
    ref<CRawWaveImpl> GetWaveChannelAt(int iAt)
        { return m_aRawWave.GetAt(iAt); }
    // void SetAt(int iAt,
    //     { m_aRawWave.SetAt(iAt,
    //void AddWaveChannel(ref<CRawWaveImpl> value)
    //     { m_aRawWave.Add(value); }
    // void InsertAt(int iAt,
    //     { m_aRawWave.InsertAt(iAt,
    // void RemoveAt(int iAt,
    //     { m_aRawWave.RemoveAt(iAt,
    // void RemoveAll()
    //     { m_aRawWave.RemoveAll(); }
    bool IterateWaveChannels(out iter& out_i, out ref<CRawWaveImpl>& out_value)
        { return m_aRawWave.Iterate(out out_i, out out_value); }
    // see also IterateBackwards()

// Operations

    // Start online
    void PreallocAudioBeforeStart();

    // Online audio acquisition
    void AddNextOnlineRawComposites(
            ref<CAudioSliceGp> rAddAudioSlice);

    // For Audio Playback.
    //   A temporary ref<> returned,
    //   the client can save it as ref<> _m_r during the playback.
    ref<CAudioSliceGp> NewAudioCompositeLinearBufferCopy(
            int iMinVirtualMatrixPosFrom);
    ref<CAudioSliceGp> NewAudioCompositeLinearBufferCopyCutToView();
    ref<CAudioSliceGp> NewAudioCompositeLinearBufferCopyCutArea(
            int iStartAfterVirtualCompositeOffset,
            int iEndBeforeVirtualCompositeOffset);
    void GetVisibleCompositeRange(
            //int iScrollAtMatrix,
            //int nViewMatrixWidth,
            out int& out_iFirstVisibleVirtualOffset,
            out int& out_nVisibleCompositeCount);

    // Slices to quadratures range
    void ConvertFromSliceToCompositeRange(
            int iScrollAtMatrix,
            int nViewMatrixWidth,
            out int& out_iFirstVisibleVirtualOffset,
            out int& out_nVisibleCompositeCount);

    // Load PCM
    void ImportRawAudio(
            ref<CAudioSliceGp> rAudioSlice);

    // Handle OFF
    void FlushRawAudioToTempFile();

    // Done with the recording
    void ClearAudioSliceAndTempFile();

    // Tmp file preload
    void ReloadAudioSliceFromTempFile();
    void ReleaseMemoryPreloadedFromTempFile();

// UI

    // RawAudioDirectView
    ref<UiContainer> NewUiRawAudioDirectView();
    ptr<CUiPanelGp> _m_pUiRawAudioDirectView;
    bool x_bShowSlowDebugGraphs = true
            xauto(Get, Set);
    void InvalidateAudioDebugGraphContent();
    void InvalidateAudioDebugGraphPaint();

protected:
private:

    // wave channel objects
    array< ref<CRawWaveImpl> > m_aRawWave;

    // packed wave data
    //TODO: we've got to start saving it!
    ref<CAudioSliceGp> _m_rAudioSlice;
    int _m_nAudioSlicesFlushedToTempFile = 0;
    path _m_pathSavedToTemp;
    int GetAudioSliceShortCount();

    // circular pointr in _m_rAudioSlice
    int m_iNextFreeCircularAudioWord = 0;
    int m_nLoopsAround = 0;

    // fast graph update
    tick _m_tickLastDebugGraphAutoUpdate = 0;

    // Circular buffer helper
    int TranslatePlainIndexToCircularBuffer(
            int iAt);

    // Prealloc
    void EnsureAudioBufferPreallocated();

    // init parent back pointers in our members
    void _init_AttachProperties();

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiRawAudioDirectView.cpp)
class UiRawAudioDirectView : public UiContainer
{
public:
    virtual void Ui();

    ref<CRawAudioImpl> m_rRawAudio;

private:
    bool _m_bWasTurbo = false;
};
