// CNetNaturalSignalingIfaceGp.h
#pragma once

class CNetNaturalChannelTypeIfaceGp;

//
// CNetNaturalSignalingIfaceGp -
//
//   Signal transfer for non-Uport devices.
//
//ASSUME_IMPL_FOR_IFACE(CNetNaturalSignalingIface, CNetNaturalSignalingIfaceGp)
//ASSUME_IMPL_FOR_IFACE(CNetNaturalSignalingImpl, CNetNaturalSignalingIface)

class CNetNaturalSignalingIfaceGp : public object
{
public:
    CNetNaturalSignalingIfaceGp();
    //~CNetNaturalSignalingIfaceGp();
    //NEW_LOCAL(CNetNaturalSignalingImpl, CNetNaturalSignalingIfaceGp)
    NEW_GP(Net, CNetNaturalSignalingImpl, CNetNaturalSignalingIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

    // One-time opener
    //void InitNetNaturalSignaling();

    // Cleanup
    //void CloseNetNaturalSignaling();

    // Read if ready, empty sbuf() means no data
    bool IsReadingNaturalChannelSignal()
            vhook;
    sbuf ReadNaturalChannelSignal(
            type<CNetNaturalChannelTypeIfaceGp> typeNetNaturalChannelType,
            int nMinBytes,
            int nMaxBytes,
            out int& out_nErrorCount)
            vhook;

    // Queue for sending
    bool IsSendingNaturalChannelSignal()
            vhook;
    void SendNaturalChannelSignal(
            type<CNetNaturalChannelTypeIfaceGp> typeNetNaturalChannelType,
            sbuf sbufSignal,
            int nErrorCount)
            vhook;

// UI

protected:
    SEE_ALSO(CNetNaturalSignalingImpl) // F12-lookup

    virtual bool OnIsReadingNaturalChannelSignal()
            v1pure;
    virtual sbuf OnReadNaturalChannelSignal(
            type<CNetNaturalChannelTypeIfaceGp> typeNetNaturalChannelType,
            int nMinBytes,
            int nMaxBytes,
            out int& out_nErrorCount)
            v1pure;
    virtual bool OnIsSendingNaturalChannelSignal()
            v1pure;
    virtual void OnSendNaturalChannelSignal(
            type<CNetNaturalChannelTypeIfaceGp> typeNetNaturalChannelType,
            sbuf sbufSignal,
            int nErrorCount)
            v1pure;

private:
    //bool _m_bOneTimeInitNetNaturalSignalingIfaceGpOk = false;
    //bool _m_bNetNaturalSignalingIfaceGpOpened = false;

    void _init_AttachToNetNaturalSignalingIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
