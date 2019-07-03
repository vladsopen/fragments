// CNetStatusIfaceGp.h
#pragma once

//
// CNetStatusIfaceGp - 
//
//   Asynchronous communication error and completion status.
//
//ASSUME_IMPL_FOR_IFACE(CNetStatusImpl, CNetStatusIfaceGp)

class CNetStatusIfaceGp : public object
{
public:
    CNetStatusIfaceGp();
    //NEW_LOCAL(CNetStatusImpl, CNetStatusIfaceGp)
    //NEW_GP(Net, CNetStatusImpl, CNetStatusIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // For timeout check
    utc x_utcBoxTransferStarted
            xauto(Get, Set);

    // Check this before interpreting error info as final.
    //   This is set to true after both successful and failed operation.
    bool x_bCommunicationFinished = false
            xauto(Get, Set);

    // Main error status. If empry = OK
    str x_sCommunicationError = ""
            xauto(Get, Set);
            
    // Append this to full error description for debugging.
    //   May contain str/sloc progress info if supported by the driver.
    str x_sOptionalExtraInfo = ""
            xauto(Get, Set);

    // Only if driver supports it 
    num x_numOptionalProgressPercent = -1.0
            xauto(Get, Set)
            xassert(value >= -1.0);

// Operations

// UI

protected:
private:
    //bool _m_bOneTimeInitNetStatusIfaceGpOk = false;

    void _init_AttachToNetStatusIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
