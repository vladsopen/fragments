// CTestPatientLink.h
#pragma once

//
// CTestPatientLink - test Patients communication modes
//
//ASSUME_IMPL_FOR_IFACE(CTestPatientLinkImpl, CTestPatientLink)

class CTestPatientLink : public object
{
public:
    CTestPatientLink();
    //NEW_LOCAL(CTestPatientLinkImpl, CTestPatientLink)
    //NEW_GP(CustomApps, CTestPatientLinkImpl, CTestPatientLink)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

// UI

protected:
private:
    //bool _m_bOneTimeInitTestPatientLinkOk = false;

    void _init_AttachToTestPatientLink();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    virtual void OnTestClass();
};
