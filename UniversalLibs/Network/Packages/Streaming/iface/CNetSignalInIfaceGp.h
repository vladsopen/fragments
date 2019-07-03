// CNetSignalInIfaceGp.h
#pragma once

#include "CNetSignalCommonIfaceGp.h"

class CNetLocatorIfaceGp;

//
// CNetSignalInIfaceGp -
//
//   Signal slice receiver.
//
//ASSUME_IMPL_FOR_IFACE(CNetSignalInImpl, CNetSignalInIfaceGp)

class CNetSignalInIfaceGp : public CNetSignalCommonIfaceGp
{
public:
    CNetSignalInIfaceGp();
    //NEW_LOCAL(CNetSignalInImpl, CNetSignalInIfaceGp)
    NEW_GP(Net, CNetSignalInImpl, CNetSignalInIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // see base:
    // Allow network signal routing
    //bool x_bNetSignalEnabled = false
    //        xauto(Get, Set);

    // Limit memory consumption for signals not read by the receiving app
    int x_nSubchannelReceptionCacheBytes = 
            // for scanner: 
            5 * 64 /*256*/ * 128
            //1 * 1024 * 1024,
            xauto(Get, Set)
            xassert(value > 0);

    // Shift timing to ensure we have received enough data for smooth playback
    static const tick C_tickDefaultDelaySignalInput = 200;
    tick x_tickDelaySignalInput = C_tickDefaultDelaySignalInput
            xauto(Get, Set)
            xassert(true);

    // Send data to app a fast as it comes
    bool x_bEnableAutoRemoteTimeSync = true
            xauto(Get, Set);

    // Avoid errors when app doesn't read on time
    bool x_bIgnoreSubchannelCacheOverflow = 
            true
            //false,
            xauto(Get, Set);

    // Emulator option not to delete files 
    //   will cause infinite reread
    bool x_bDeleteReceivedFiles = true
            xauto(Get, Set);

    // An option to hide junk data from the receivers
    bool x_bFeedSignalToApp = true
            xauto(Get, Set);

    // Pump off the junk
    bool x_bDiscardDownloadedSignal = false
            xauto(Get, Set);

// Operations

    // Open channel.
    //   You must also set x_bNetSignalEnabled.
    void InitNetSignalIn(
            ref<CNetLocatorIfaceGp> rLocator)
            vhook;

    // Close
    void CloseNetSignalIn()
            vhook;

    // Read if ready
    sbuf ReadNetSignalPortion(
            int iSubchannel,
            int nMinBytes,
            int nMaxBytes,
            out int& out_nErrorCount)
            vhook;

// UI

protected:
    virtual void OnInitNetSignalIn(
            ref<CNetLocatorIfaceGp> rLocator)
            v1pure;
    virtual void OnCloseNetSignalIn()
            v1pure;
    virtual sbuf OnReadNetSignalPortion(
            int iSubchannel,
            int nMinBytes,
            int nMaxBytes,
            out int& out_nErrorCount)
            v1pure;
private:
    //bool _m_bOneTimeInitNetSignalInIfaceGpOk = false;

    void _init_AttachToNetSignalInIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
