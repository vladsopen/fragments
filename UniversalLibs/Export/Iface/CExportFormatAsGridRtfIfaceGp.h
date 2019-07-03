// CExportFormatAsGridRichIfaceGp.h
#pragma once

#include "CExportFormatIfaceGp.h"

//
// CExportFormatAsGridRichIfaceGp - 
//
//   Rich Text Format export parameters
//
ASSUME_IMPL_FOR_IFACE(CExportFormatAsGridRichImpl, CExportFormatAsGridRichIfaceGp)

class CExportFormatAsGridRichIfaceGp : public CExportFormatIfaceGp
{
public:
    CExportFormatAsGridRichIfaceGp();
    //NEW_LOCAL(CExportFormatAsGridRichImpl, CExportFormatAsGridRichIfaceGp)
    NEW_GP(SysCommon, CExportFormatAsGridRichImpl, CExportFormatAsGridRichIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Must be set explicitly, affects auto-width
    int x_nTableColumnCount = 1
            xauto(Get, Set)
            xassert(value > 0);

    // Columnt width in pixels
    int x_nTableColumnWidthInTwips = 1200
            xauto(Get, Set)
            xassert(value > 0);

// Operations

// UI

protected:
private:
    //bool _m_bOneTimeInitExportFormatAsGridRichIfaceGpOk = false;

    void _init_AttachToExportFormatAsGridRichIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
