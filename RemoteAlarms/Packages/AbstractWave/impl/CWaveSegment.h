// CWaveSegment.h
#pragma once

class CWaveChannelType;
class CWaveLayerType;
class CWaveChannelImpl;
class CAudioSliceGp;
class CWaveDocImpl;

//
// CWaveSegment - 
//
//   A preparsed vertical multichannel waveform block, event or marker
//
//ASSUME_IMPL_FOR_IFACE(CWaveSegmentImpl, CWaveSegment)

class CWaveSegment : public object
{
public:
    CWaveSegment();
    //~CWaveSegment();
    //NEW_LOCAL(CWaveSegmentImpl, CWaveSegment)
    //NEW_GP(Ralarm, CWaveSegmentImpl, CWaveSegment)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CWaveDocImpl> _x_pWaveDoc
            xauto(Get, Set);
    ref<CWaveDocImpl> GetWaveDoc()
            return
                _x_pWaveDoc.Get().Valid();

    // Cached from file?
    bool x_bSegmentLoaded = false
            xauto(Get);
    bool EnsureSegmentLoaded();

    // For a channel?
    int x_iChannelAffinity = -1
            xauto(Get, Set)
            xassert(value >= -1);
    type<CWaveChannelType> GetSegmentChannelType()
            return
                CWaveChannelImpl::GGetWaveChannelType(x_iChannelAffinity);

    // Order
    big x_iiWaveSegment = -1
            xauto(Get, Set)
            xassert(value >= 0);

    // ASCII info / debug
    str x_sSegmentAsText = ""
            xauto(Get, Set)
            xassert(true);
    str x_sSegmentDebug = ""
            xauto(Get, Set)
            xassert(true);

    // Where is
    big x_iiSegmentStartInFile = -1
            xauto(Get, Set)
            xassert(value >= 0);
    big x_nnSegmentBytesInFile = -1
            xauto(Get, Set)
            xassert(value >= 0);

    // Width
    tick x_tickSegmentDuration = -1
            xauto(Get, Set)
            xassert(value >= -1);

    // Wave
    ref<CAudioSliceGp> _x_rSegmentAudioSlice
            xauto(Get, Set);

// Operations

    // One-time opener
    //void InitWaveSegment();

    // Cleanup
    //void CloseWaveSegment();

// UI

    // WaveSegmentGraph
    ref<UiContainer> NewUiWaveSegmentGraph(
            type<CWaveLayerType> typeWaveLayerType);
    ptrmap<CUiPanelGp> _m_ptrmapUiWaveSegmentGraph;

    void DrawSliceGraph(
            draw dc,
            ref<CUiPanelGp> rUi);

protected:
private:
    //bool _m_bOneTimeInitWaveSegmentOk = false;
    //bool _m_bWaveSegmentOpened = false;

    int m_nDrawCount = 0;

    void _init_AttachToWaveSegment();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiWaveSegmentGraph.cpp)
class UiWaveSegmentGraph : public UiContainer
{
public:
    virtual void Ui();

    ref<CWaveSegment> m_rWaveSegment;
    type<CWaveLayerType> m_typeWaveLayerType;

private:
};
