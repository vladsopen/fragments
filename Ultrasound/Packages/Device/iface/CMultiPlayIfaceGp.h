// CMultiPlayIfaceGp.h
#pragma once

#include "SuperSharedUsoundConst.h"

class CDocIfaceGp;

//
// CMultiPlayIfaceGp -
//
//   Plays back multiple Docs with audio Parallelizer controlled.
//
ASSUME_IMPL_FOR_IFACE(CMultiPlayIface, CMultiPlayIfaceGp)
ASSUME_IMPL_FOR_IFACE(CMultiPlayImpl, CMultiPlayIface)

class CMultiPlayIfaceGp : public object
{
public:
    CMultiPlayIfaceGp();
    //NEW_LOCAL(CMultiPlayImpl, CMultiPlayIfaceGp)
    NEW_GP(Usmain, CMultiPlayImpl, CMultiPlayIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // on?
    bool IsMultiPlaying()
            vhook;

// Operations

    // You can either
    //   - select a single doc: a recorder or a saved window
    //   - select multiple *recorder* docs for *different* channel
    void InitiateDocIntoPlayback(
            ref<CDocIfaceGp> rDoc,
            EUsoundPlaybackRange eUsoundPlaybackRange,
            percent percentVolume)
            vhook;

    // You must call InitiateDocIntoPlayback()
    //   before every call to StartMultiPlay() to select at least one doc.
    void StartMultiPlay()
            vhook;

    // Stop playback in all started docs
    void EndMultiPlay()
            vhook;

    // To change volume without restart
    void SetMultiPlayVolume(
            percent percentVolume)
            vhook;

// UI

protected:
    virtual bool OnIsMultiPlaying()
            v1pure;
    virtual void OnInitiateDocIntoPlayback(
            ref<CDocIfaceGp> rDoc,
            EUsoundPlaybackRange eUsoundPlaybackRange,
            percent percentVolume)
            v1pure;
    virtual void OnStartMultiPlay()
            v1pure;
    virtual void OnEndMultiPlay()
            v1pure;
    virtual void OnSetMultiPlayVolume(
            percent percentVolume)
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
