// CNetStatsImpl.h
#pragma once

#include "CNetStatsIfaceGp.h"

//
// CNetStatsImpl - see Iface
//

class CNetStatsImpl : public CNetStatsIfaceGp
{
public:
    CNetStatsImpl();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // a caption
    str x_sStatsForPurpose
            xauto(Get, Set)
            xassert(true);

// Operations

// UI

    // NetStatsIndicator
    //ref<UiContainer> NewUiNetStatsIndicator();
    ptrmap<CUiPanelGp> _m_ptrmapUiNetStatsIndicator;

protected:
    virtual utc Onx_utcLastTransferOrProgressOrInit_Get()
            ;
    virtual void OnInitNetStats(
            str sPurpose)
            ;
    virtual void OnHandlePollingEvent()
            ;
    virtual void OnHandleTransferEvent(
            int nBytes)
            ;
    virtual void OnResetNetStats()
            ;
    virtual void OnHandleStartArchiving()
            ;
    virtual void OnHandleEndArchiving(
            int nInBytes,
            int nOutBytes)
            ;
    virtual str OnGetCompressionReport()
            ;
    virtual ref<UiContainer> OnNewUiNetStatsIndicator()
            ;
private:
    //bool _m_bOneTimeInitNetStatsImplOk = false;

    utc m_utcInit = null();

    // Compression profiler
    big m_nnTscStartCompress = 0;
    big m_nnTscLastAfterCompress = 0;
    num m_numAverageCompressTimePercent = 0;
    num m_numAverageCompressRatioPercent = 0;

    void _init_AttachToNetStatsImpl();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiNetStatsIndicator.cpp)
class UiNetStatsIndicator : public UiContainer
{
public:
    virtual void Ui();

    ref<CNetStatsImpl> m_rNetStats;

private:
};
