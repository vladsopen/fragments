// CFeedOnlineCallbackIfaceGp.h
#pragma once

class CRangesIfaceGp;
class CAxisTypeIfaceGp;
class CIndexTypeIfaceGp;

//
// CFeedOnlineCallbackIfaceGp -
//
//   Client-defined callback receiving Balance data.
//     Must be registered in CFeedRegistryIfaceGp.
//
//ASSUME_IMPL_FOR_IFACE(CFeedOnlineCallbackImpl, CFeedOnlineCallbackIfaceGp)

class CFeedOnlineCallbackIfaceGp : public object
{
public:
    CFeedOnlineCallbackIfaceGp();
    //NEW_LOCAL(CFeedOnlineCallbackImpl, CFeedOnlineCallbackIfaceGp)
    //NEW_GP(Balance, CFeedOnlineCallbackImpl, CFeedOnlineCallbackIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

    // Override this to receive the streams identified with typeIndexType
    //   The value is split into optional numerical and string parts:
    //   - numInUiUnits - defined only for numerical indexes (or set to OO)
    //   - richAppend - optional string value (or empty)
    // typeAxisType is either
    //   - X-value: type<CAxisTypeIfaceGp>()->AsAxisTypeX()
    //   - Y-value: type<CAxisTypeIfaceGp>()->AsAxisTypeY()
    //   - axis-neutral value: null()
    void FeedIndex(
            type<CIndexTypeIfaceGp> typeIndexType,
            type<CAxisTypeIfaceGp> typeAxisType,
            num numInUiUnits,
            rich richAppend,
            ref<CRangesIfaceGp> rRanges)
            vhook;

// UI

protected:
    virtual void OnFeedIndex(
            type<CIndexTypeIfaceGp> typeIndexType,
            type<CAxisTypeIfaceGp> typeAxisType,
            num numInUiUnits,
            rich richAppend,
            ref<CRangesIfaceGp> rRanges)
            v1pure;
private:
    //bool _m_bOneTimeInitFeedOnlineCallbackIfaceGpOk = false;

    void _init_AttachToFeedOnlineCallbackIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
