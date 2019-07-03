// CPlayback.h
#pragma once

class CIoControlIfaceGp;
class CDocImpl;
class CAudioSliceGp;

//
// CPlayback - 
//
//   Controls playback in a Doc
//
//ASSUME_IMPL_FOR_IFACE(CPlaybackImpl, CPlayback)

class CPlayback : public object
{
public:
    CPlayback();
    //NEW_LOCAL(CPlaybackImpl, CPlayback)
    //NEW_GP(MyProject, CPlaybackImpl, CPlayback)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Backref to parent document 
    ptr<CDocImpl> _x_pParentDoc
            xauto(Get, Set);

    bool IsPlaying();

// Operations

    // Start Doc playback
    void PrepareToStartPlayback(
            ref<CAudioSliceGp> rAudioSlice,
            percent percentVolume);

    // Chenge without restart
    void SetPlaybackVolume(
            percent percentVolume);

    // End Doc playback
    void StopPlayback();

// UI

    // PlaybackStop
    ref<UiContainer> NewUiPlaybackStop();
    ptrmap<CUiPanelGp> _m_ptrmapUiPlaybackStop;

protected:
private:

    bool m_bPlayingBack = false;
    path m_pathTempFileHeap;

    ref<CIoControlIfaceGp> _m_rIoControl;

    int m_iSaveScroll = 0;

    // File saver
    path SaveTempPlaybackFileHeap(
            ref<CAudioSliceGp> rAudioSlice);

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiPlaybackStop.cpp)
class UiPlaybackStop : public UiContainer
{
public:
    virtual void Ui();

    ref<CPlayback> m_rPlayback;
};
