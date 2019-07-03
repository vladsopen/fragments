// CImageImpl.h
#pragma once

#include "CImageIface.h"

class CGeneratorListIface;
class CColoring;
class CGlobalPaletteConfigIface;
class CLocalPaletteConfigIface;
class CViewState;
class CViewCache;
class CStoreIface;
class CDocIface;

//
// CImageImpl - see iface
//

class CImageImpl : public CImageIface
{
public:
    CImageImpl();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    //
    // External Pointers
    //

    // Parent doc we MUST be attached to
    ref<CDocIface> GetParentDoc();

    // Related store from where we get data
    ref<CStoreIface> GetRelatedStore();

    // Generator controller for our doc-session
    ref<CGeneratorListIface> DigGeneratorList();

    // Related Session from where we get config
    //ref<CSessionIface> GetRelatedSession();

    //
    // Internal Controllers
    //

    // Current 
    ref<CViewState> x_rViewState
            xauto(Get);

    // Image prepared for fast screen blit
    ref<CViewCache> x_rViewCache
            xauto(Get);

    // Image prepared for fast screen blit
    ref<CColoring> x_rColoring
            xauto(Get);

// Operations

// UI

    alikex _m_alikexScaleBar;

    // ImageMain
    //ref<UiContainer> NewUiImageMain();
    ptr<CUiPanelGp> _m_pUiImageMain;

    // ImageFullscreen
    ref<UiContainer> NewUiImageFullscreen();
    ptr<CUiPanelGp> _m_pUiImageFullscreen;
    // see iface: bool x_bImageFullscreen = false
    bool x_bImageFullscreenBare = false
            xauto(Get, Set);

    // ImageBarSystem
    ref<UiContainer> NewUiImageBarSystem();
    ptr<CUiPanelGp> _m_pUiImageBarSystem;
    void InvalidateImageBarSystemContent();

    // CornerScaleTime
    ref<UiContainer> NewUiCornerScaleTime();
    ptr<CUiPanelGp> _m_pUiCornerScaleTime;

    // ImageLimitedPreview
    //ref<UiContainer> NewUiImageLimitedPreview();
    ptr<CUiPanelGp> _m_pUiImageLimitedPreview;

    // ImageBarSystemWrap
    ref<UiContainer> NewUiImageBarSystemWrap();
    ptr<CUiPanelGp> _m_pUiImageBarSystemWrap;

    // ImageHackMenu
    //ref<UiContainer> NewUiImageHackMenu();
    ptr<CUiPanelGp> _m_pUiImageHackMenu;

    // ImageSeparateTimeBar
    //ref<UiContainer> NewUiImageSeparateTimeBar();
    ptrmap<CUiPanelGp> _m_ptrmapUiImageSeparateTimeBar;

    // ImageCornerAndTime
    ref<UiContainer> NewUiImageCornerAndTime();
    ptrmap<CUiPanelGp> _m_ptrmapUiImageCornerAndTime;

protected:
    virtual bool OnMirrorDbDocImage(
            ref<CUdbMirrorIfaceGp> rUdbMirror,
            ref<CDbDocIface> rDbDoc)
            ;
    virtual void OnSetViewWidthInMatrixSlices(
            int nSlices)
            ;
    virtual int OnGetViewWidthInMatrixSlices()
            ;
    virtual void OnSetViewScrollInMatrixSlicesWuu(
            int nSlices)
            ;
    virtual void OnSetViewScrollInMatrixSlicesWithSliderUpdate(
            int nSlices)
            ;
    virtual num OnPrepareOrSendViewScrollRange(
            bool bSend)
            ;
    virtual int OnGetViewScrollInMatrixSlices()
            ;
    virtual int OnGetCycledScreenSliceCounterForIndexRecalc()
            ;
    virtual void Onx_bImageFullscreen_Set(
            bool value)
            ;
    virtual void OnSetNewImagePaletteVirtualizer(
            ref<CPaletteVirtualizerIface> rNewPaletteVirtualizer)
            ;
    virtual void OnForceBroadcastMyTimebarSyncToEveryone()
            ;
    virtual void OnPrepareToViewRecordingAtWidth(
            int nDefaultViewWidthSlices)
            ;
    virtual void OnPrepareOfflineScroll()
            ;
    virtual void OnUpdateAfterMediaEditInStore()
            ;
    virtual void OnUpdateAfterMediaFormatChanged()
            ;
    virtual void OnUpdateAfterPaletteChange()
            ;
    virtual void OnUpdateAfterBaselineMoved()
            ;
    virtual void OnHandleNextOnlineSliceAppended()
            ;
    virtual void OnUpdateImageCompletelyNowForTestsOnly()
            ;
    virtual void OnUpdateAfterOnMediaTextChanged()
            ;
    virtual void OnUpdateAfterPatientSelectionChanged(
            bool bSelected)
            ;
    virtual void OnEmulateViewRedrawToMarkVisibleSlices()
            ;
    virtual void OnResetForClearStore()
            ;
    virtual void OnResetImagePaletteToFactoryDefaults()
            ;
    virtual ref<UiContainer> OnNewUiImageMain()
            ;
    virtual ref<UiContainer> OnNewUiImagePaletteBar()
            ;
    virtual ref<UiContainer> OnNewUiImageLimitedPreview()
            ;
    virtual ref<UiContainer> OnNewUiImageHackMenu()
            ;
    virtual ref<UiContainer> OnNewUiImageSeparateTimeBar()
            ;

private:

    // init parent back pointers in our members
    void _init_AttachProperties();

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiImageMain.cpp)
class UiImageMain : public UiContainer
{
public:
    virtual void Ui();

    ref<CImageImpl> m_rImage;
};

// (UiImageFullscreen.cpp)
class UiImageFullscreen : public UiContainer
{
public:
    virtual void Ui();

    ref<CImageImpl> m_rImage;
};

// (UiImageBarSystem.cpp)
class UiImageBarSystem : public UiContainer
{
public:
    virtual void Ui();

    ref<CImageImpl> m_rImage;
};

// (UiCornerScaleTime.cpp)
class UiCornerScaleTime : public UiContainer
{
public:
    virtual void Ui();

    ref<CImageImpl> m_rImage;
};

// (UiImageLimitedPreview.cpp)
class UiImageLimitedPreview : public UiContainer
{
public:
    virtual void Ui();

    ref<CImageImpl> m_rImage;
};

// (UiImageBarSystemWrap.cpp)
class UiImageBarSystemWrap : public UiContainer
{
public:
    virtual void Ui();

    ref<CImageImpl> m_rImage;
};

// (UiImageHackMenu.cpp)
class UiImageHackMenu : public UiContainer
{
public:
    virtual void Ui();

    ref<CImageImpl> m_rImage;
};

// (UiImageSeparateTimeBar.cpp)
class UiImageSeparateTimeBar : public UiContainer
{
public:
    virtual void Ui();

    ref<CImageImpl> m_rImage;
};

// (UiImageCornerAndTime.cpp)
class UiImageCornerAndTime : public UiContainer
{
public:
    virtual void Ui();

    ref<CImageImpl> m_rImage;
};
