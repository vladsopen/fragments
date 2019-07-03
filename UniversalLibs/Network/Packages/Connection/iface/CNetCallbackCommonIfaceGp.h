// CNetCallbackCommonIfaceGp.h
#pragma once

class CNetBoxAbstractionIfaceGp;

//
// CNetCallbackCommonIfaceGp -
//
//   Common app callback functions for both In and Out transfers.
//

class CNetCallbackCommonIfaceGp : public object
{
public:
    CNetCallbackCommonIfaceGp();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

    // Common In and Out error hanlder.
    //   See alse error status in the Box object.
    void HandleNetError(
            str sDetails,
            ptr<CNetBoxAbstractionIfaceGp> pToBox)
            vhook;

// UI

protected:
    virtual void OnHandleNetError(
            str sDetails,
            ptr<CNetBoxAbstractionIfaceGp> pToBox)
            v1null;
private:
    //bool _m_bOneTimeInitNetCallbackCommonIfaceGpOk = false;

    void _init_AttachToNetCallbackCommonIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
