// CArchiveDupeSessionIfaceGp.h
#pragma once

#include "CArchiveBasicSessionIfaceGp.h"

//
// CArchiveDupeSessionIfaceGp - 
//
//   Transparent duplicator fake-packer.
//     It is used to polymorphically bypass real packing.
//
//ASSUME_IMPL_FOR_IFACE(CArchiveDupeSessionIface, CArchiveDupeSessionIfaceGp)
//ASSUME_IMPL_FOR_IFACE(CArchiveDupeSessionImpl, CArchiveDupeSessionIface)

class CArchiveDupeSessionIfaceGp : public CArchiveBasicSessionIfaceGp
{
public:
    CArchiveDupeSessionIfaceGp();
    //~CArchiveDupeSessionIfaceGp();
    //NEW_LOCAL(CArchiveDupeSessionImpl, CArchiveDupeSessionIfaceGp)
    NEW_GP(SysCommon, CArchiveDupeSessionImpl, CArchiveDupeSessionIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

// Operations

// UI

protected:
    SEE_ALSO(CArchiveDupeSessionImpl)

private:
    //bool _m_bOneTimeInitArchiveDupeSessionIfaceGpOk = false;
    //bool _m_bArchiveDupeSessionIfaceGpOpened = false;

    void _init_AttachToArchiveDupeSessionIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
