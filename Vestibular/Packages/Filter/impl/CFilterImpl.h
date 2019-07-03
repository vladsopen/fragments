// CFilterImpl.h
#pragma once

#include "CFilterIface.h"

class CIncrementalFilter;
class CDbConfigIface;

//
// CFilterImpl - see Iface
//

class CFilterImpl : public CFilterIface
{
public:
    CFilterImpl();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // for incremental and offline
    static const int C_nFilterOrder = 11;
    static const num C_numFilterOne = 1.0000;
    static const num C_numFilterZero = 0.00001;

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Local config access
    ref<CDbConfigIface> GetDbConfigForFilter();

    // Min/Max sane freq
    num GetFilterMinHz();
    num GetFilterMaxHz();
    num GetFilterMinBandDeltaHz();

    // Shortcuts for config validator
    //num GetNormalizedLowHz();
    //num GetNormalizedHighHz();
    // see iface

// Operations

    // Min/max auto fix
    void NormalizeFilterBounds();

    // Update everything dependent
    void HandleFilterSetupChange(
            ref<CDbConfigIface> rDbConfig);

// UI

    str GetFilterSetupReport();
    res GetFilterIcon();

    // FilterExperimenter
    void OpenUiFilterExperimenter();
    ref<UiContainer> NewUiFilterExperimenter();
    ptrmap<CUiPanelGp> _m_ptrmapUiFilterExperimenter;

    // FilterSetup
    //ref<UiContainer> NewUiFilterSetup();
    ptrmap<CUiPanelGp> _m_ptrmapUiFilterSetup;

protected:
    virtual num OnGetNormalizedLowHz()
            ;
    virtual num OnGetNormalizedHighHz()
            ;
    virtual void OnGetSafeNormalizedHighAndLowHz(
            out num& out_numLowHz,
            out num& out_numHighHz)
            ;
    virtual void OnFilterOfflineReadingCache(
            rowset<CDbReadingIface> rowsetRawReading,
            out rowset<CDbReadingIface>& out_rowsetFilteredReading)
            ;
    virtual bool OnFilterOnlineIncremental(
            ref<CReadingIface> rReading,
            out ref<CReadingIface>& out_rFilteredReading)
            ;
    virtual void OnResetFilterForNewRecording()
            ;
    virtual void OnInvalidateFilterCache()
            ;
    virtual ref<UiContainer> OnNewUiFilterSetup()
            ;
private:
    //bool _m_bOneTimeInitFilterImplOk = false;

    bool m_bFilterCacheInvalidated = true;

    ref<CIncrementalFilter> m_rIncrementalFilterX;
    ref<CIncrementalFilter> m_rIncrementalFilterY;

    void _init_AttachToFilterImpl();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiFilterSetup.cpp)
class UiFilterSetup : public UiContainer
{
public:
    virtual void Ui();

    ref<CFilterImpl> m_rFilter;

private:
};

// (UiFilterExperimenter.cpp)
class UiFilterExperimenter : public UiContainer
{
public:
    virtual void Ui();

    ref<CFilterImpl> m_rFilter;

private:
};
