// CNetSignalOutIfaceGp.h
#pragma once

#include "CNetSignalCommonIfaceGp.h"

class CNetLocatorIfaceGp;

//
// CNetSignalOutIfaceGp -
//
//   Signal slice sender.
//
//ASSUME_IMPL_FOR_IFACE(CNetSignalOutImpl, CNetSignalOutIfaceGp)

class CNetSignalOutIfaceGp : public CNetSignalCommonIfaceGp
{
public:
    CNetSignalOutIfaceGp();
    //NEW_LOCAL(CNetSignalOutImpl, CNetSignalOutIfaceGp)
    NEW_GP(Net, CNetSignalOutImpl, CNetSignalOutIfaceGp)
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

    // Limits output file size.
    //   This is not exact, can be a few bytes larger/smaller
    int x_nMaxNetSignalOutFileBytes = 
            // we still need to wait for 4 seconds
            400 * 1024 // * 1024,
            //100 * 1024, // * 1024,
            xauto(Get, Set)
            xassert(value > 0);

    // Limits file accumulation time
    tick x_tickMaxNetSignalOutFileDelay = 400
            xauto(Get, Set)
            xassert(value > 0);

    // Larger values may produce more compact files
    tick x_tickMaxSyncError = 10
            xauto(Get, Set)
            xassert(value >= 0);

    // Compression option
    bool x_bNetCompressSignal = true
            xauto(Get, Set);

    // Testing larger transfers
    int x_nAddDummyMbytes = 0
            xauto(Get, Set)
            xassert(value >= 0);

// Operations

    // Open channel.
    //   You must also set x_bNetSignalEnabled. 
    void InitNetSignalOut(
            ref<CNetLocatorIfaceGp> rLocator)
            vhook;

    // Close
    void CloseNetSignalOut()
            vhook;

    // Queue for sending
    void SendNetSignalPortion(
            int iSubchannel,
            sbuf sbufSignal,
            int nErrorCount)
            vhook;

// UI

protected:
    virtual void OnInitNetSignalOut(
            ref<CNetLocatorIfaceGp> rLocator)
            v1pure;
    virtual void OnCloseNetSignalOut()
            v1pure;
    virtual void OnSendNetSignalPortion(
            int iSubchannel,
            sbuf sbufSignal,
            int nErrorCount)
            v1pure;
private:
    //bool _m_bOneTimeInitNetSignalOutIfaceGpOk = false;

    void _init_AttachToNetSignalOutIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
