// CExportFormatAsGridPlainTextIfaceGp.h
#pragma once

#include "CExportFormatIfaceGp.h"

//
// CExportFormatAsGridPlainTextIfaceGp - 
//
//   CSV or tab-separated txt.
//
ASSUME_IMPL_FOR_IFACE(CExportFormatAsGridPlainTextImpl, CExportFormatAsGridPlainTextIfaceGp)

class CExportFormatAsGridPlainTextIfaceGp : public CExportFormatIfaceGp
{
public:
    CExportFormatAsGridPlainTextIfaceGp();
    //NEW_LOCAL(CExportFormatAsGridPlainTextImpl, CExportFormatAsGridPlainTextIfaceGp)
    NEW_GP(SysCommon, CExportFormatAsGridPlainTextImpl, CExportFormatAsGridPlainTextIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // CSV or tab separation
    str x_sExportValueSeparator = "\t"
            xauto(Get, Set)
            xassert(value != "");

    // Line breaks
    str x_sExportLineBreak = C_szDosFileNewLine
            xauto(Get, Set)
            xassert(value != "");

// Operations

// UI

protected:
private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToExportFormatAsGridPlainTextIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
