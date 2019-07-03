// CExportWriterToSbufIfaceGp.h
#pragma once

#include "CExportWriterIfaceGp.h"

//
// CExportWriterToSbufIfaceGp -
//
//   Implements in-memory destination for a text output stream.
//
ASSUME_IMPL_FOR_IFACE(CExportWriterToSbufImpl, CExportWriterToSbufIfaceGp)

class CExportWriterToSbufIfaceGp : public CExportWriterIfaceGp
{
public:
    CExportWriterToSbufIfaceGp();
    //NEW_LOCAL(CExportWriterToSbufImpl, CExportWriterToSbufIfaceGp)
    NEW_GP(SysCommon, CExportWriterToSbufImpl, CExportWriterToSbufIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Read result
    sbuf GetWrittenString()
            vhook;

// Operations

// UI

protected:
    virtual sbuf OnGetWrittenString()
            v1pure;
private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToExportWriterToSbufIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
