// CExportFormatAsGridExcelXmlIfaceGp.h
#pragma once

#include "CExportFormatIfaceGp.h"

//
// CExportFormatAsGridExcelXmlIfaceGp - 
//
//   Excel 2003 XML table.
//
ASSUME_IMPL_FOR_IFACE(CExportFormatAsGridExcelXmlImpl, CExportFormatAsGridExcelXmlIfaceGp)

class CExportFormatAsGridExcelXmlIfaceGp : public CExportFormatIfaceGp
{
public:
    CExportFormatAsGridExcelXmlIfaceGp();
    //NEW_LOCAL(CExportFormatAsGridExcelXmlImpl, CExportFormatAsGridExcelXmlIfaceGp)
    NEW_GP(SysCommon, CExportFormatAsGridExcelXmlImpl, CExportFormatAsGridExcelXmlIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Line breaks
    str x_sCosmeticLineBreak = C_szDosFileNewLine
            xauto(Get, Set)
            xassert(value != "");

// Operations

// UI

protected:
private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToExportFormatAsGridExcelXmlIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
