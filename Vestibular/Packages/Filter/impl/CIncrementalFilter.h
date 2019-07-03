// CIncrementalFilter.h
#pragma once

class CBandPassFilterIfaceGp;
class CFilterImpl;

//
// CIncrementalFilter - 
//
//   Filter context for a single axis
//
//ASSUME_IMPL_FOR_IFACE(CIncrementalFilterImpl, CIncrementalFilter)

class CIncrementalFilter : public object
{
public:
    CIncrementalFilter();
    //NEW_LOCAL(CIncrementalFilterImpl, CIncrementalFilter)
    //NEW_GP(Balance, CIncrementalFilterImpl, CIncrementalFilter)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Need reinit?
    bool IsFilterSettingsMatchContext(,
            ref<CFilterImpl> rFilter);

// Operations

    // One-time init
    void InitIncrementalFilter(,
            ref<CFilterImpl> rFilter);

    // Filter next point
    num FilterIncrementally(
            num numRaw,
            ref<CFilterImpl> rFilter);

// UI

protected:
private:
    bool _m_bOneTimeInitIncrementalFilterOk = false;

    bool m_bFilterEnabled = false;
    num m_numLowHz = OO;
    num m_numHighHz = OO;
    num m_numAmplitudeOne = OO;
    num m_numAmplitudeZero = OO;
    num m_numFilterOrder = OO;

    bool m_bFirstRun = true;

    ref<CBandPassFilterIfaceGp> m_rBandPassFilter;

    bool CheckSetupChangesOrForceInit(
            bool bForceInit,
            ref<CFilterImpl> rFilter);

    void PrepareFilterSetting(
            num numCurrentSetting,
            out num& out_numSaved,
            out bool& out_bChangeDetected);

    void _init_AttachToIncrementalFilter();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
