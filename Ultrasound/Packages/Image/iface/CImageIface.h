// CImageIface.h
#pragma once

#include "UsmainSharedUi.h"

class CPaletteVirtualizerIface;
class CDbDocIface;
class CUdbMirrorIfaceGp;
class CDocIface;

//
// CImageIface - see ifacegp
//
ASSUME_IMPL_FOR_IFACE(CImageImpl, CImageIface)

class CImageIface : public object
{
public:
    CImageIface();
    NEW_LOCAL(CImageImpl, CImageIface)
    //NEW_GP(MyProject, CImageImpl, CImageIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);
    bool MirrorDbDocImage(
            ref<CUdbMirrorIfaceGp> rUdbMirror,
            ref<CDbDocIface> rDbDoc)
            vhook;

// Constants

// Attributes

    // Backref to parent document in which context we render the image
    ptr<CDocIface> _x_pParentDoc
            xauto(Get, Set);

    // View width control
    void SetViewWidthInMatrixSlices(
            int nSlices)
            vhook;
    int GetViewWidthInMatrixSlices()
            vhook;

    // Scroll pos - horizontal image shift in media slices
    void SetViewScrollInMatrixSlicesWuu(
            int nSlices)
            vhook;
    void SetViewScrollInMatrixSlicesWithSliderUpdate(
            int nSlices)
            vhook;
    num PrepareOrSendViewScrollRange(
            bool bSend)
            vhook;
    int GetViewScrollInMatrixSlices()
            vhook;

    // Screen passed is still require for CalcupSystem
    int GetCycledScreenSliceCounterForIndexRecalc()
            vhook;

    // Profiling support
    bool _x_bShowProfilingTimingOnMedia = false
            xauto(Get, Set);

    // UT support
    bool _x_bTestLogPalettePaintOperations = false
            xauto(Get, Set);
    bool _x_bTestLogMediaDetails = true
            xauto(Get, Set);
    bool _x_bTestLogCurveDetails = true
            xauto(Get, Set);
    size _x_sizeTestFixedCacheUi = size(-1, -1)
            xauto(Get, Set)
            xassert(true);

    // Fullscreen switch
    bool x_bImageFullscreen = false
            xauto(Get);
    void x_bImageFullscreen_Set(
            bool value)
            vhook;

    // Replaces colors for RASTER
    void SetNewImagePaletteVirtualizer(
            ref<CPaletteVirtualizerIface> rNewPaletteVirtualizer)
            vhook;

// Operations

    // When a Doc undergoea x_bIndependentRecorderTimeBars = false
    //   transition it wants to make sure the global timebars are
    //   in sync with it.
    void ForceBroadcastMyTimebarSyncToEveryone()
            vhook;

    // Init for online and set default width
    void PrepareToViewRecordingAtWidth(
            int nDefaultViewWidthSlices)
            vhook;

    // Normalize recording for offline mode
    void PrepareOfflineScroll()
            vhook;

    // Full update handler
    void UpdateAfterMediaEditInStore()
            vhook;

    // For debug we need to handler this
    void UpdateAfterMediaFormatChanged()
            vhook;

    // After gamma edits
    void UpdateAfterPaletteChange()
            vhook;

    // After SetMidlline in storage
    void UpdateAfterBaselineMoved()
            vhook;

    // Online update
    void HandleNextOnlineSliceAppended()
            vhook;

    // UT Support
    void UpdateImageCompletelyNowForTestsOnly()
            vhook;

    // Media commanent
    void UpdateAfterOnMediaTextChanged()
            vhook;

    // Selection controls
    void UpdateAfterPatientSelectionChanged(
            bool bSelected)
            vhook;

    // For Store save cut off optimization
    void EmulateViewRedrawToMarkVisibleSlices()
            vhook;

    // Clear-Store handler
    void ResetForClearStore()
            vhook;

    // Factory defaults for palette
    void ResetImagePaletteToFactoryDefaults()
            vhook;

// UI

    // All UI corresponding to media rectangle
    ref<UiContainer> NewUiImageMain()
            vhook;

    // ColoringBar
    ref<UiContainer> NewUiImagePaletteBar()
            vhook;

    // Limited mode
    ref<UiContainer> NewUiImageLimitedPreview()
            vhook;

    // Debug
    ref<UiContainer> NewUiImageHackMenu()
            vhook;

    // Bottom media window part used as the synchronized timebar
    ref<UiContainer> NewUiImageSeparateTimeBar()
            vhook;

protected:
    virtual bool OnMirrorDbDocImage(
            ref<CUdbMirrorIfaceGp> rUdbMirror,
            ref<CDbDocIface> rDbDoc)
            v1pure;
    virtual void OnSetViewWidthInMatrixSlices(
            int nSlices)
            v1pure;
    virtual int OnGetViewWidthInMatrixSlices()
            v1pure;
    virtual void OnSetViewScrollInMatrixSlicesWuu(
            int nSlices)
            v1pure;
    virtual void OnSetViewScrollInMatrixSlicesWithSliderUpdate(
            int nSlices)
            v1pure;
    virtual num OnPrepareOrSendViewScrollRange(
            bool bSend)
            v1pure;
    virtual int OnGetViewScrollInMatrixSlices()
            v1pure;
    virtual int OnGetCycledScreenSliceCounterForIndexRecalc()
            v1pure;
    virtual void Onx_bImageFullscreen_Set(
            bool value)
            v1pure;
    virtual void OnSetNewImagePaletteVirtualizer(
            ref<CPaletteVirtualizerIface> rNewPaletteVirtualizer)
            v1pure;
    virtual void OnForceBroadcastMyTimebarSyncToEveryone()
            v1pure;
    virtual void OnPrepareToViewRecordingAtWidth(
            int nDefaultViewWidthSlices)
            v1pure;
    virtual void OnPrepareOfflineScroll()
            v1pure;
    virtual void OnUpdateAfterMediaEditInStore()
            v1pure;
    virtual void OnUpdateAfterMediaFormatChanged()
            v1pure;
    virtual void OnUpdateAfterPaletteChange()
            v1pure;
    virtual void OnUpdateAfterBaselineMoved()
            v1pure;
    virtual void OnHandleNextOnlineSliceAppended()
            v1pure;
    virtual void OnUpdateImageCompletelyNowForTestsOnly()
            v1pure;
    virtual void OnUpdateAfterOnMediaTextChanged()
            v1pure;
    virtual void OnUpdateAfterPatientSelectionChanged(
            bool bSelected)
            v1pure;
    virtual void OnEmulateViewRedrawToMarkVisibleSlices()
            v1pure;
    virtual void OnResetForClearStore()
            v1pure;
    virtual void OnResetImagePaletteToFactoryDefaults()
            v1pure;
    virtual ref<UiContainer> OnNewUiImageMain()
            v1pure;
    virtual ref<UiContainer> OnNewUiImagePaletteBar()
            v1pure;
    virtual ref<UiContainer> OnNewUiImageLimitedPreview()
            v1pure;
    virtual ref<UiContainer> OnNewUiImageHackMenu()
            v1pure;
    virtual ref<UiContainer> OnNewUiImageSeparateTimeBar()
            v1pure;

private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
