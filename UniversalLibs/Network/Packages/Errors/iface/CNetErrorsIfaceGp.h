// CNetErrorsIfaceGp.h
#pragma once

//
// CNetErrorsIfaceGp - 
//
//   Created to hold common error enums
//
//ASSUME_IMPL_FOR_IFACE(CNetErrorsImpl, CNetErrorsIfaceGp)

class CNetErrorsIfaceGp : public object
{
public:
    CNetErrorsIfaceGp();
    //~CNetErrorsIfaceGp();
    //NEW_LOCAL(CNetErrorsImpl, CNetErrorsIfaceGp)
    //NEW_GP(Net, CNetErrorsImpl, CNetErrorsIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // Origin
    enum ENetErrorFrom
    {
        E_NetErrorFrom_Server,
        E_NetErrorFrom_Local,
    };

    // Severity
    enum ENetErrorLevel
    {
        E_NetErrorLevel_Warning,
        E_NetErrorLevel_DataLoss,
        E_NetErrorLevel_Stop,
    };

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);
    //ref<> Get ()
    //        return
    //            _x_p .Get().Valid();

// Operations

    // One-time opener
    //void InitNetErrors();

    // Cleanup
    //void CloseNetErrors();

// UI

protected:
private:
    //bool _m_bInited = false;
    //bool _m_bOpened = false;

    void _init_AttachToNetErrorsIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
