// CNetStatsIfaceGp.h
#pragma once

//
// CNetStatsIfaceGp -
//
//   Transfer statistics. Easy-to-reset object.
//
ASSUME_IMPL_FOR_IFACE(CNetStatsImpl, CNetStatsIfaceGp)

class CNetStatsIfaceGp : public object
{
public:
    CNetStatsIfaceGp();
    //NEW_LOCAL(CNetStatsImpl, CNetStatsIfaceGp)
    NEW_GP(Net, CNetStatsImpl, CNetStatsIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // stats
    int x_nTransferredIdleCount = 0
            xauto(Get, Set)
            xassert(value >= 0);
    int x_nTransferredPollingCount = 0
            xauto(Get, Set)
            xassert(value >= 0);
    int x_nTransferredBoxCount = 0
            xauto(Get, Set)
            xassert(value >= 0);
    big x_nnTransferredBytes = 0
            xauto(Get, Set)
            xassert(value >= 0);
    int x_nRemoteBoxesPending = 0
            xauto(Get, Set)
            xassert(value >= 0);
    int x_nRemoteDeltaSeconds = 0
            xauto(Get, Set)
            xassert(value >= 0);
    utc x_utcFirstTransfer = null()
            xauto(Get, Set)
            xassert(true);
    utc x_utcLastTransfer = null()
            xauto(Get, Set)
            xassert(true);
    utc x_utcLastProgress = null()
            xauto(Get, Set)
            xassert(true);
    tick x_tickLastTransferDuration = null()
            xauto(Get, Set)
            xassert(true);
    utc x_utcLastTransferOrProgressOrInit_Get()
            vhook;

    // Debug indication
    str x_sStintInitLog = ""
            xauto(Get, Set)
            xassert(true);

// Operations

    // Set a short description
    void InitNetStats(
            str sPurpose)
            vhook;

    // Add to stats
    void HandlePollingEvent()
            vhook;
    void HandleTransferEvent(
            int nBytes)
            vhook;

    // Clear math but not app defined captions
    void ResetNetStats()
            vhook;

    // Compression ratios and timind
    str x_sCompressionError = ""
            xauto(Get, Set)
            xassert(true);
    void HandleStartArchiving()
            vhook;
    void HandleEndArchiving(
            int nInBytes,
            int nOutBytes)
            vhook;
    str GetCompressionReport()
            vhook;

    // A helper
    static num GCalcNextAverage(
            num numPrevAverage,
            num numNextPoint,
            int nWindow);

// UI

    // Indication
    ref<UiContainer> NewUiNetStatsIndicator()
            vhook;

protected:
    virtual utc Onx_utcLastTransferOrProgressOrInit_Get()
            v1pure;
    virtual void OnInitNetStats(
            str sPurpose)
            v1pure;
    virtual void OnHandlePollingEvent()
            v1pure;
    virtual void OnHandleTransferEvent(
            int nBytes)
            v1pure;
    virtual void OnResetNetStats()
            v1pure;
    virtual void OnHandleStartArchiving()
            v1pure;
    virtual void OnHandleEndArchiving(
            int nInBytes,
            int nOutBytes)
            v1pure;
    virtual str OnGetCompressionReport()
            v1pure;
    virtual ref<UiContainer> OnNewUiNetStatsIndicator()
            v1pure;
private:
    //bool _m_bOneTimeInitNetStatsIfaceGpOk = false;

    void _init_AttachToNetStatsIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
