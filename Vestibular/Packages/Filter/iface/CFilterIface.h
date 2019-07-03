// CFilterIface.h
#pragma once

class CReadingIface;
class CSessionIface;
class CDbReadingIface;

//
// CFilterIface -
//
//   Manages signal band-pass filtration.
//
ASSUME_IMPL_FOR_IFACE(CFilterImpl, CFilterIface)

class CFilterIface : public object
{
public:
    CFilterIface();
    NEW_LOCAL(CFilterImpl, CFilterIface)
    //NEW_GP(Balance, CFilterImpl, CFilterIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // GSS for FFT
    static const num C_numLowestUsefulFreq = 0.05;

// Attributes

    // parent
    ptr<CSessionIface> _x_pSession
            xauto(Get, Set);

    // Shortcuts for config validator
    num GetNormalizedLowHz()
            vhook;
    num GetNormalizedHighHz()
            vhook;
    void GetSafeNormalizedHighAndLowHz(
            out num& out_numLowHz,
            out num& out_numHighHz)
            vhook;

// Operations

    // Offline DB-array filter
    //  note: rowset<> stuff is obsolete!
    void FilterOfflineReadingCache(
            rowset<CDbReadingIface> rowsetRawReading,
            out rowset<CDbReadingIface>& out_rowsetFilteredReading)
            vhook;

    // Online filter
    bool FilterOnlineIncremental(
            ref<CReadingIface> rReading,
            out ref<CReadingIface>& out_rFilteredReading)
            vhook;

    // Reset incremental context
    void ResetFilterForNewRecording()
            vhook;

    // Force recalc next time
    void InvalidateFilterCache()
            vhook;

// UI

    // Config dialog insertion
    ref<UiContainer> NewUiFilterSetup()
            vhook;

protected:
    virtual num OnGetNormalizedLowHz()
            v1pure;
    virtual num OnGetNormalizedHighHz()
            v1pure;
    virtual void OnGetSafeNormalizedHighAndLowHz(
            out num& out_numLowHz,
            out num& out_numHighHz)
            v1pure;
    virtual void OnFilterOfflineReadingCache(
            rowset<CDbReadingIface> rowsetRawReading,
            out rowset<CDbReadingIface>& out_rowsetFilteredReading)
            v1pure;
    virtual bool OnFilterOnlineIncremental(
            ref<CReadingIface> rReading,
            out ref<CReadingIface>& out_rFilteredReading)
            v1pure;
    virtual void OnResetFilterForNewRecording()
            v1pure;
    virtual void OnInvalidateFilterCache()
            v1pure;
    virtual ref<UiContainer> OnNewUiFilterSetup()
            v1pure;
private:
    //bool _m_bOneTimeInitFilterIfaceOk = false;

    void _init_AttachToFilterIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
