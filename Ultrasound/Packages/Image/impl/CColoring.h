// CColoring.h
#pragma once

#include "UsmainSharedUi.h"

class CColorListGp;
class CPaletteVirtualizerNeutralIface;
class CPaletteVirtualizerIface;
class CUmoveIfaceGp;
class CDbDocIface;
class CUdbMirrorIfaceGp;
class CDbLocalPaletteIface;
class CMatrixFormatGp;
class CGlobalConfigIface;
class CPaletteLimitTypeIface;
class CGlobalPaletteConfigIface;
class CLocalPaletteConfigIface;
class CImageImpl;

//
// CColoring - 
//
//   Runs internal palette operations for Image
//
//ASSUME_IMPL_FOR_IFACE(CColoringImpl, CColoring)

class CColoring : public object
{
public:
    CColoring();
    //NEW_LOCAL(CColoringImpl, CColoring)
    //NEW_GP(MyProject, CColoringImpl, CColoring)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);
    bool MirrorDbColoring(
            ref<CUdbMirrorIfaceGp> rUdbMirror,
            ref<CDbDocIface> rDbDoc);

// Constants

    // Slider handler
    static const scale C_scalePaletteSliderHandleHeight = prescale(12);

    // Help UV layout to get better proportion precision
    static const int C_nIncreaseProportionalHeightResolution = 1000;

    // Colors above saturation and below threshold
    static const rgb C_rgbDimmerTopPalette = rgb(255, 0, 0); //rgb(110, 12, 12);
    static const rgb C_rgbDimmerBottomPalette = rgb(0, 0, 0);

    // Emulating LegacyD logic
    static const int C_nLegacyRange = 128;

// Attributes

    // Backpointer to parent to simplify repaint callback calls from everywhere
    ptr<CImageImpl> _x_pParentImage
            xauto(Get, Set);

    // Our local palette options, loosely in-sync with global config
    ref<CLocalPaletteConfigIface> x_rLocalPaletteConfig
            xauto(Get);

    // Global palette colors
    ref<CGlobalPaletteConfigIface> GetImageGlobalPaletteConfig();

    // Global config from the session
    ref<CGlobalConfigIface> GetGlobalConfigForUpdates();

    // We need the format here for ranges
    ref<CMatrixFormatGp> GetRelatedMatrixFormat();

    // Sliders
    ref<CUmoveIfaceGp> x_rUmoveSaturation
            xauto(Get);
    ref<CUmoveIfaceGp> x_rUmoveThreshold
            xauto(Get);
    ref<CUmoveIfaceGp> x_rUmoveCenter
            xauto(Get);
    size x_sizeUmoveSliderSystemParent = size(0, 0)
            xauto(Get, Set)
            xassert(true);
    void UpdateAllSlidersUmoveState();

    // external virtual color replacer
    ref<CPaletteVirtualizerIface> x_rPaletteVirtualizer = 
            (ref<CPaletteVirtualizerIface>) 
                ref<CPaletteVirtualizerNeutralIface>()
            xauto(Get, Set);

    // Small palette bar we turn into a dropdown
    bool x_bPaletteBarToMenu = false
            xauto(Get, Set);

// Operations

    // Related global palette config
    ref<CLocalPaletteConfigIface> GetLocalPalleteForConfigSync();

    // Palette config sync
    void LoadDefaultPaletteFromConfig();
    void SaveCurrentPaletteToConfig();

    // UI up button
    void IncreasePaletteLimit(
            type<CPaletteLimitTypeIface> typePaletteLimitType);

    // UI up button
    void DecreasePaletteLimit(
            type<CPaletteLimitTypeIface> typePaletteLimitType);

    // UI up/down button
    void ChangePaletteLimit(
            type<CPaletteLimitTypeIface> typePaletteLimitType,
            int nNewLimit);

    // General updates
    void NotifyPaletteDependedAfterChange();

    // Turn into a menu?
    void DecidePaletteOnSize(
            ref<CUiPanelGp> rUi);

// UI

    // ColoringPaletteBar
    ref<UiContainer> NewUiColoringPaletteBar(
            EPaletteBarMode ePaletteBarMode);
    ptrmap<CUiPanelGp> _m_ptrmapUiColoringPaletteBar;
    ptrmap<CUiPanelGp> _m_ptrmapUiColoringPaletteBarContent;
    ptrmap<CUiPanelGp> _m_ptrmapUiColoringPaletteBarMenu;
    void InvalidateAllPaletteBarContents();
    bool IsPaletteMenuOpened();

    // ColoringMenu
    ref<UiContainer> NewUiColoringMenu();
    ptrmap<CUiPanelGp> _m_ptrmapUiColoringMenu;

    // ColoringBar
    ref<UiContainer> NewUiColoringBar(
            EPaletteBarMode ePaletteBarMode);
    ptrmap<CUiPanelGp> _m_ptrmapUiColoringBar;

    // ColoringBarLimitUpDown
    ref<UiContainer> NewUiColoringBarLimitUpDown(
            type<CPaletteLimitTypeIface> typePaletteLimitType,
            bool bCompactRulerBars);
    ptr<CUiPanelGp> _m_ptrmapUiColoringBarLimitUpDown;

    // ColoringBarSlider
    ref<UiContainer> NewUiColoringBarSlider(
            type<CPaletteLimitTypeIface> typePaletteLimitType,
            ref<CUmoveIfaceGp> rUmove,
            bool bCompactRulerBars);
    ptrmap<CUiPanelGp> _m_ptrmapUiColoringBarSlider;

    // ColoringBarGradient
    ref<UiContainer> NewUiColoringBarGradient(,
            bool bCompactRulerBars);
    ptrmap<CUiPanelGp> _m_ptrmapUiColoringBarGradient;

protected:
private:

    // UI up/down button
    void StepPaletteLimitUpOrDown(
            int nDirection,
            type<CPaletteLimitTypeIface> typePaletteLimitType);

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiColoringBar.cpp)
class UiColoringBar : public UiContainer
{
public:
    virtual void Ui();

    ref<CColoring> m_rColoring;
    EPaletteBarMode _m_ePaletteBarMode = 
            E_PaletteBarMode_MainUiFull;

private:
    void EqualizeBackOutsidePaletteRange(
            ref<CUiPanelGp> rUi,
            int nHeight);
    void PaintBackOutsidePaletteRange(
            draw dc,
            ref<CUiPanelGp> rUi,
            int iColor,
            ref<CColorListGp> rColorList,
            ref<CPaletteVirtualizerIface> rPaletteVirtualizer);

    ref<CPicture> _m_rPictureCache;
};

// (UiColoringBarLimitUpDown.cpp)
class UiColoringBarLimitUpDown : public UiContainer
{
public:
    virtual void Ui();

    ref<CColoring> m_rColoring;
    type<CPaletteLimitTypeIface> m_typePaletteLimitType;
    bool m_bCompactRulerBars = false;
};

// (UiColoringBarSlider.cpp)
class UiColoringBarSlider : public UiContainer
{
public:
    virtual void Ui();

    ref<CColoring> m_rColoring;
    type<CPaletteLimitTypeIface> m_typePaletteLimitType;
    ref<CUmoveIfaceGp> m_rUmove;
    bool m_bCompactRulerBars = false;
};

// (UiColoringBarGradient.cpp)
class UiColoringBarGradient : public UiContainer
{
public:
    virtual void Ui();

    ref<CColoring> m_rColoring;
    bool m_bCompactRulerBars = false;
};

// (UiColoringMenu.cpp)
class UiColoringMenu : public UiContainer
{
public:
    virtual void Ui();

    ref<CColoring> m_rColoring;
};

// (UiColoringPaletteBar.cpp)
class UiColoringPaletteBar : public UiContainer
{
public:
    virtual void Ui();

    ref<CColoring> m_rColoring;
    EPaletteBarMode _m_ePaletteBarMode = 
            E_PaletteBarMode_MainUiFull;
};
