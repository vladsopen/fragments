// CColoring.cpp

#include "CUsmainProject.h"
#include "CColoring.h"
#include "CGlobalPaletteConfigIface.h"
#include "CImageImpl.h"
#include "CGlobalConfigIface.h"
#include "CLocalPaletteConfigIface.h"
#include "CPaletteLimitTypeIface.h"
#include "CMatrixFormatGp.h"
#include "CDocIface.h"
#include "CDbDocIface.h"
#include "CUmoveIfaceGp.h"
#include "CPaletteVirtualizerIface.h"
#include "CPaletteVirtualizerNeutralIface.h"
#include "CCustomizeDocIfaceGp.h"

CColoring::CColoring()
{
}

void CColoring::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    //rFAIL("make sure it is OK to expose CColoring!");
}

bool CColoring::MirrorDbColoring(
        ref<CUdbMirrorIfaceGp> rUdbMirror,
        ref<CDbDocIface> rDbDoc)
{
    //
    // We are called to save/load all values in
    //
    //     ref<CDbDocIface> rDbDoc
    //
    // which are related to this object
    //

    //ref<CUdbConnectionIfaceGp> rUdbConnection = 
        //rUdbMirror->GetMirroredUdbConnection();

    //
    // Local Palette
    //

    if (!x_rLocalPaletteConfig->
            MirrorDbLocalPalette(
                rUdbMirror,
                rDbDoc->_x_dbrefDocLocalPalette))
    {
        return false;
    }

    return true;
}

ref<CMatrixFormatGp> CColoring::GetRelatedMatrixFormat()
{
    ref<CImageImpl> rImage = _x_pParentImage.Get().Valid();

    ref<CDocIface> rDoc = rImage->GetParentDoc();

    ref<CMatrixFormatGp> rMatrixFormat =
        rDoc->
            x_rMatrixFormat;

    return rMatrixFormat;
}

ref<CGlobalConfigIface> CColoring::GetGlobalConfigForUpdates()
{
    ref<CImageImpl> rImage = 
        _x_pParentImage.Get().Valid();
    ref<CDocIface> rDoc = 
        rImage->_x_pParentDoc.Get().Valid();

    ref<CGlobalConfigIface> rGlobalConfig = 
        rDoc->GetConfigForDoc();

    return rGlobalConfig;
}

ref<CGlobalPaletteConfigIface> CColoring::GetImageGlobalPaletteConfig()
{
    ref<CGlobalConfigIface> rGlobalConfig =
        GetGlobalConfigForUpdates();

    ref<CGlobalPaletteConfigIface> rGlobalPaletteConfig = 
        // I locked the palette to linear mapping for RASTER
        //rDoc->_x_pDupomDoc?
        //    rGlobalConfig->
        //        _x_rGlobalPaletteConfigForRaster;
        //    :
            rGlobalConfig->
                _x_rGlobalPaletteConfig;

    return rGlobalPaletteConfig;
}

ref<CLocalPaletteConfigIface> CColoring::GetLocalPalleteForConfigSync()
{
    ref<CImageImpl> rImage = 
        _x_pParentImage.Get().Valid();
    ref<CDocIface> rDoc = 
        rImage->_x_pParentDoc.Get().Valid();

    // RASTER thresholds are stored separately
    if (rDoc->_x_pDupomDoc)
    {
        ref<CGlobalConfigIface> rGlobalConfig =
            GetGlobalConfigForUpdates();

        return rGlobalConfig->x_rGlobalRasterLocalPaletteConfig;
    }
    else
    {
        ref<CGlobalPaletteConfigIface> rGlobalPaletteConfig = 
            GetImageGlobalPaletteConfig();

        ref<CLocalPaletteConfigIface> rLocalPaletteConfig = 
            rGlobalPaletteConfig->
                x_rDefaultLocalPaletteSettings;

        return rLocalPaletteConfig;
    }
}

void CColoring::LoadDefaultPaletteFromConfig()
{
    TESTLOG("", "Loading Default Palette from Config...\n");

    // get global palette config
    ref<CLocalPaletteConfigIface> rGlobalLocalPaletteConfig = 
        GetLocalPalleteForConfigSync();

    // Init palette settings
    this->x_rLocalPaletteConfig.
        ExposeIn(
            rGlobalLocalPaletteConfig->
                ExposeOut());
}

void CColoring::SaveCurrentPaletteToConfig()
{
    // get global palette config
    ref<CLocalPaletteConfigIface> rGlobalLocalPaletteConfig = 
        GetLocalPalleteForConfigSync();

    // Save palette settings
    rGlobalLocalPaletteConfig->
        ExposeObjectContentInWithoutRenew(
            x_rLocalPaletteConfig->
                ExposeOut());
}

ref<UiContainer> CColoring::NewUiColoringBar(
        EPaletteBarMode ePaletteBarMode)
{
    ref<UiColoringBar> rUi;

    rUi->m_rColoring = this;
    rUi->_m_ePaletteBarMode = ePaletteBarMode;

    return rUi;
}

void CColoring::IncreasePaletteLimit(
        type<CPaletteLimitTypeIface> typePaletteLimitType)
{
    StepPaletteLimitUpOrDown(
        1,
        typePaletteLimitType);
}

void CColoring::DecreasePaletteLimit(
        type<CPaletteLimitTypeIface> typePaletteLimitType)
{
    StepPaletteLimitUpOrDown(
        -1,
        typePaletteLimitType);
}

void CColoring::StepPaletteLimitUpOrDown(
        int nDirection,
        type<CPaletteLimitTypeIface> typePaletteLimitType)
{
    rASSERT(nDirection == -1 || nDirection == 1);

    int nCurrentLimitInMediaRange = 
        typePaletteLimitType->
            GetLimitInPaletteRange(
                x_rLocalPaletteConfig,
                GetRelatedMatrixFormat());

    // We have to step twice as fast to match 
    //   LegacyD 128 range vs our 256 range
    int nCurrentLimitInLegacyRange = 
        math::GMulDiv(
            nCurrentLimitInMediaRange,
            C_nLegacyRange,
            GetRelatedMatrixFormat()->x_nMatrixSliceAmplitudeRange);

    // now we need a hack to emulate the legacy top max value of 127
    if (nCurrentLimitInLegacyRange > C_nLegacyRange - 1)
    {
        nCurrentLimitInLegacyRange = C_nLegacyRange - 1;
    }

    num numDecibel = 
        CLocalPaletteConfigIface::GConvertIndexToDecibelInLowLevelRangeRounded(
            nCurrentLimitInLegacyRange,
            C_nLegacyRange);

    numDecibel += 
        nDirection * 
            typePaletteLimitType->
                GetPaletteLimitStepInDecibel();

    int nNewLimitInLegacyRange = 
        CLocalPaletteConfigIface::GConvertDecibelToPaletteIndexInLowLevelRange(
            numDecibel,
            C_nLegacyRange);

    if (nNewLimitInLegacyRange == nCurrentLimitInLegacyRange)
    {
        nNewLimitInLegacyRange += nDirection;
    }

    // We have to step twice as fast to match 
    //   LegacyD 128 range vs our 256 range
    int nNewLimitInMediaRange = 
        math::GMulDiv(
            nNewLimitInLegacyRange,
            GetRelatedMatrixFormat()->x_nMatrixSliceAmplitudeRange,
            C_nLegacyRange);

    ChangePaletteLimit(
        typePaletteLimitType,
        nNewLimitInMediaRange);
}

void CColoring::ChangePaletteLimit(
        type<CPaletteLimitTypeIface> typePaletteLimitType,
        int nNewLimit)
{
    // change x_rLocalPaletteConfig
    typePaletteLimitType->
        SetLimitInPaletteRange(
            x_rLocalPaletteConfig,
            GetRelatedMatrixFormat(),
            nNewLimit,
            GetGlobalConfigForUpdates());

    NotifyPaletteDependedAfterChange();
}

void CColoring::NotifyPaletteDependedAfterChange()
{
    // update bar
    _m_ptrmapUiColoringBar.InvalidateTreeState();

    // save new config
    SaveCurrentPaletteToConfig();

    // update image
    _x_pParentImage.Get().Valid()->UpdateAfterPaletteChange();

    // update palette controls
    _m_ptrmapUiColoringBar.InvalidateTreeState();
    _m_ptrmapUiColoringBar.InvalidateTreeLayout();
}

void CColoring::InvalidateAllPaletteBarContents()
{
//debug:
    //_m_ptrmapUiColoringPaletteBar.InvalidateTreeState();
    //_m_ptrmapUiColoringPaletteBarContent.InvalidateContent();

    _m_ptrmapUiColoringPaletteBar.InvalidateContent();
}

bool CColoring::IsPaletteMenuOpened()
{
    return _m_ptrmapUiColoringPaletteBarMenu.GetCountClearingNulls() > 0;
}

void CColoring::DecidePaletteOnSize(
        ref<CUiPanelGp> rUi)
{
    ref<CImageImpl> rImage = 
        _x_pParentImage.Get().Valid();
    ref<CDocIface> rDoc = 
        rImage->_x_pParentDoc.Get().Valid();

    size sizeUi = rUi->GetInstantSize();

    bool bUpdateMenu = false;

    if (sizeUi.h > 0)
    {
        if (!rDoc->_x_rCustomizeDoc->x_bCompactRulerBars)
        {
            if (!x_bPaletteBarToMenu)
            {
                x_bPaletteBarToMenu = true;
                bUpdateMenu = true;
            }
        }
        else
        {
            if (x_bPaletteBarToMenu)
            {
                x_bPaletteBarToMenu = false;
                bUpdateMenu = true;
            }
        }
    }

    if (bUpdateMenu)
    {
        _m_ptrmapUiColoringPaletteBar.InvalidateContent();
    }
}

ref<UiContainer> CColoring::NewUiColoringBarLimitUpDown(
        type<CPaletteLimitTypeIface> typePaletteLimitType,
        bool bCompactRulerBars)
{
    ref<UiColoringBarLimitUpDown> rUi;

    rUi->m_rColoring = this;
    rUi->m_typePaletteLimitType = typePaletteLimitType;
    rUi->m_bCompactRulerBars = bCompactRulerBars;

    return rUi;
}

ref<UiContainer> CColoring::NewUiColoringBarSlider(
        type<CPaletteLimitTypeIface> typePaletteLimitType,
        ref<CUmoveIfaceGp> rUmove,
        bool bCompactRulerBars)
{
    ref<UiColoringBarSlider> rUi;

[...]