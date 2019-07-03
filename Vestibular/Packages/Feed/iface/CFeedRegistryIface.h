// CFeedRegistryIface.h
#pragma once

#include "CFeedRegistryIfaceGp.h"

class CFeedStateIfaceGp;
class CRangesIfaceGp;
class CAxisTypeIfaceGp;
class CIndexTypeIfaceGp;

//
// CFeedRegistryIface - see ifacegp
//

class CFeedRegistryIface : public CFeedRegistryIfaceGp
{
public:
    CFeedRegistryIface();
    NEW_LOCAL(CFeedRegistryImpl, CFeedRegistryIface)
    //NEW_GP(Balance, CFeedRegistryImpl, CFeedRegistryIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Get-accessors for handlers
    ptr<CFeedOnlineCallbackIfaceGp> _x_pMonitoringFeed_Get()
            return 
                this->_x_pMonitoringFeed;
    ptr<CFeedStateIfaceGp> _x_pMonitoringFeedStateForMath_Get()
            return 
                this->_x_pMonitoringFeedStateForMath;
    ptr<CFeedStateIfaceGp> _x_pMonitoringFeedStateForGames_Get()
            return 
                this->_x_pMonitoringFeedStateForGames;

// Operations

    // Calls FeedIndex() on all external callbacks
    void SendIndexToFeed(
            type<CIndexTypeIfaceGp> typeIndexType,
            type<CAxisTypeIfaceGp> typeAxisType,
            num numInUiUnits,
            rich richAppend,
            ref<CRangesIfaceGp> rRanges)
            vhook;

    // State feeds for math and games
    ref<CFeedStateIfaceGp> GetFeedStateForMath()
            vhook;
    ref<CFeedStateIfaceGp> GetFeedStateForGames()
            vhook;

// UI

protected:
    virtual void OnSendIndexToFeed(
            type<CIndexTypeIfaceGp> typeIndexType,
            type<CAxisTypeIfaceGp> typeAxisType,
            num numInUiUnits,
            rich richAppend,
            ref<CRangesIfaceGp> rRanges)
            v1pure;
    virtual ref<CFeedStateIfaceGp> OnGetFeedStateForMath()
            v1pure;
    virtual ref<CFeedStateIfaceGp> OnGetFeedStateForGames()
            v1pure;
private:
    //bool _m_bOneTimeInitFeedRegistryIfaceOk = false;

    void _init_AttachToFeedRegistryIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
