// CViewCache.h
#pragma once

class CImmediatePaintIfaceGp;
class CLocalPaletteConfigIface;
class CGlobalPaletteConfigIface;
class CMatrixFormatGp;
class CMatrixSliceIface;
class CImageImpl;
class CViewState;
class CStoreIface;

//
// CViewCache - 
//
//   Media Picture cache.
//   It copies data from Store and caches the media 
//     in scaled and colored format.
//   ViewPassed pos and ViewScrollPos also apply.
//
//ASSUME_IMPL_FOR_IFACE(CViewCacheImpl, CViewCache)

class CViewCache : public object
{
public:
    CViewCache();
    //NEW_LOCAL(CViewCacheImpl, CViewCache)
    //NEW_GP(MyProject, CViewCacheImpl, CViewCache)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Backpointer to parent to simplify repaint callback calls from everywhere
    ptr<CImageImpl> _x_pParentImage
            xauto(Get, Set);

    //
    // Image States
    //

    // Enable controls over media image
    bool x_bOnImageControls = true
            xauto(Get, Set);

    // Enable incremental update
    bool x_bEnableIncrementalUpdate = true
            xauto(Get, Set);

    // !pixel cache for incremental mode SetPixel
    bool x_bEnableThinRawPicture = true
            xauto(Get, Set);

    // Passed position
    bool x_bDrawPassedPos = true
            xauto(Get, Set);

    // Incremental debug
    bool x_bDrawIncrementalMargins = true
            xauto(Get, Set);

    // Mouse controlled generator
    bool x_bInteractiveGenerator = false
            xauto(Get, Set);

    // Unsafe Pixel optimization
    bool x_bUseOptimizedPixelAddress = true
            xauto(Get, Set);

    // Profiling 
    tick _x_tickPerScreen = 0
            xauto(Get, Set)
            xassert(true);
    tick _x_tickStartScreen = 0
            xauto(Get, Set)
            xassert(true);

    // Debug
    bool IsTurboViewCache();

// Operations

    // Resize callback
    void ResizeViewCache(
            size sizeUi);

    // Called on all external changes to image and state which 
    //   would require total cache re-rendering
    void InvalidateViewCache();

    // Online optimization: can we expect parial update?
    bool IsCacheCompletelyUpToDate();

    // Online optimization: temporarily dont invalidate
    bool LockCacheAndWindowInvalidates(
            bool bLock);

    // Online optimization
    void RenderOnlyNextOnlineSliceAndIgnoreInvalidates();

    // Handles debug fourier format change
    void ResizeViewCacheAfterFormatChange();

    // Rerender in new colors
    void InvalidatePictureAfterPaletteChange();

    // Rerender in new colors
    void InvalidatePictureAfterBaselineMoved();

    // x_bOnImageControls option change
    void InvalidateAfterOnImageTestControlsChanged();

    // Update paintable comments
    void UpdateMediaCommentsNow();

    // UT Support
    void UpdateContentAndCacheNowForTestsOnly();

    // Redraw callback
    void PaintFullMediaUiImage(
            draw dc,
            size sizeUi);

    // Update if invalidated
    void EnsureViewCacheUpdated();

// UI

    // ViewCache
    ref<UiContainer> NewUiViewCache();
    ptr<CUiPanelGp> _m_pUiViewCache;

    // ViewCacheMediaLayer
    ref<UiContainer> NewUiViewCacheMediaLayer();
    ptr<CUiPanelGp> _m_pUiViewCacheMediaLayer;
    void HandleViewCacheMouse(
            EMouse eMouse,
            point pointClientMousePos,
            ref<CUiPanelGp> rUi);

    // ViewCacheMenu
    ref<UiContainer> NewUiViewCacheMenu();
    ref<UiContainer> NewUiViewCacheControlsOverMedia();
    ptr<CUiPanelGp> _m_pUiViewCacheMenu;

protected:
private:

    // The image corresponding to the storage state
    ref<CPicture> _m_rRawPicture;

    // Optimized version containing only one pixel for faster PutPixel
    ref<CPicture> _m_rRawPicture1pixel;

    // The image corresponding to the view size
    ref<CPicture> _m_rScaledPicture;

    // Preallocate this for Render
    ref<CImmediatePaintIfaceGp> _m_rImmediatePaintObjectCache;

    // Actual taget UI size
    size _m_sizeUi = size();

    // Invalidate flag
    bool _m_bUpToDate = false;
    bool _m_bRenderOnlyNextOnlineSliceNextTime = false;
    bool _m_bLockCacheAndWindowInvalidates = false;

    // Update the cache
    void RerenderViewCache();

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiViewCache.cpp)
class UiViewCache : public UiContainer
{
public:
    virtual void Ui();

    ref<CViewCache> m_rViewCache;
};

// (UiViewCacheMenu.cpp)
class UiViewCacheMenu : public UiContainer
{
public:
    virtual void Ui();

    ref<CViewCache> m_rViewCache;
    bool m_bShowOverMedia = false;

private:
    bool _m_bShowPrivateHacks = false;
    bool _m_bShowRareHacks = false;
};

// (UiViewCacheMediaLayer.cpp)
class UiViewCacheMediaLayer : public UiContainer
{
public:
    virtual void Ui();

    ref<CViewCache> m_rViewCache;

private:
    int m_iDisketteBlinkPhase = 0;
};
