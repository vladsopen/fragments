// CNetLocatorIfaceGp.h
#pragma once

class CNetProtocolAbstractIfaceGp;
class CLoginIfaceGp;
class CNetDisplayIfaceGp;
class CNetDirectionIfaceGp;
class CNetHelperLocatorIfaceGp;

//
// CNetLocatorIfaceGp -
//
//   Connection path/target information, context and security
//     information for the communication driver.
//
//ASSUME_IMPL_FOR_IFACE(CNetLocatorImpl, CNetLocatorIfaceGp)

class CNetLocatorIfaceGp : public object
{
public:
    CNetLocatorIfaceGp();
    //NEW_LOCAL(CNetLocatorImpl, CNetLocatorIfaceGp)
    //NEW_GP(Net, CNetLocatorImpl, CNetLocatorIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // This can be a local folder for in/out box files
    path _x_pathLocalTempRoot = ""
            xauto(Get, Set);
    // Adds "/Upload"-style subfolder to pumped dir
    bool _x_bAppendDirectionToLocalPath = true
            xauto(Get, Set);
    path GetLocalTempPathForChannel(
            type<CNetDirectionIfaceGp> typeNetDirection);
            // see GetNetConnectionDirection()

    // Optional backup place to move all uploaded files
    path _x_pathLocalUploadedBackup = ""
            xauto(Get, Set);

    // We abuse [Logo] for local backup jobs
        SEE_ALSO(x_bEnablePumping)
        SEE_ALSO(x_bEnableNetPumping)
    bool x_bEnableNetworking = true
            xauto(Get, Set);
            SEE_ALSO(C_sNoNetworkingChannelKiller)

    // Driver dependent channel ID.
    str x_sRemoteChannel = ""
            xauto(Get, Set);

    // Initial server set
    str x_sServerDomainsOrIps = 
[...]

    // Auth
    ref<CLoginIfaceGp> x_rServerLogin
            xauto(Get, Set);

    // Formats
    ref<CNetProtocolAbstractIfaceGp> _x_rNetProtocolCallback
            xauto(Get, Set);

    // Must be cleared by the driver after it makes sure all remote caches 
    //   are cleared
    bool _x_bStartNewStint = true
            xauto(Get, Set);
    // For cleaning up streaming buffers but not for Patients/CloudSync sync!
    bool x_bAllowAutoDeleteOldFiles = true
            xauto(Get, Set);

    // Auto-ON on start
    bool x_bAutoActivateForApp = false
            xauto(Get, Set);

    // Parallel package packer (in and out)
    bool x_bParapack = false
            xauto(Get, Set);

    // Virtual file filter.
    //   By default IsAbbrev() test on x_sPacketFilter
    bool IsRemotePacketNameMatchesFilter(
            str sPacket)
            vhook;

    // Ext to add to streamed downloads
    path x_pathForceInboxExt = ""
            xauto(Get, Set);

    // Simplified status indicator for the end-user
    //   (may be shared with Supervisor, Signal and other objects)
    ref<CNetDisplayIfaceGp> _x_rNetDisplay
            xauto(Get);
    void _x_rNetDisplay_Set(
            ref<CNetDisplayIfaceGp> value);

    // Common impl for all locator subclasses
    ref<CNetHelperLocatorIfaceGp> x_rNetHelperLocator
            xauto(Get);

// Operations

// UI

protected:
    virtual bool OnIsRemotePacketNameMatchesFilter(
            str sPacket)
            v1st;
private:
    //bool _m_bOneTimeInitNetLocatorIfaceGpOk = false;

    void _init_AttachToNetLocatorIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
