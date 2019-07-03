// CTestIo.h
#pragma once

//
// CTestIo - 
//
//   Tests I/O instructions through the shared hole to the interpreter
//
//ASSUME_IMPL_FOR_IFACE(CTestIoImpl, CTestIo)

class CTestIo : public object
{
public:
    CTestIo();
    //NEW_LOCAL(CTestIoImpl, CTestIo)
    //NEW_GP(MyProject, CTestIoImpl, CTestIo)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

// Operations

// UI

protected:
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    virtual void OnTestClass();
};
