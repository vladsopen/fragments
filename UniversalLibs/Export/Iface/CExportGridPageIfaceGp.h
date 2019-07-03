// CExportGridPageIfaceGp.h
#pragma once

class CExportGridRowIfaceGp;

//
// CExportGridPageIfaceGp -
//
//   Some formats (Excel) may support multiple pages per file.
//     Create at least one page from CExportGridFileIfaceGp to start populating
//     it with grid items.
//
ASSUME_IMPL_FOR_IFACE(CExportGridPageImpl, CExportGridPageIfaceGp)

class CExportGridPageIfaceGp : public object
{
public:
    CExportGridPageIfaceGp();
    // created only from CExportGridFileIfaceGp methods
    //NEW_LOCAL(CExportGridPageImpl, CExportGridPageIfaceGp)
    //NEW_GP(SysCommon, CExportGridPageImpl, CExportGridPageIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    //
    // Global page properties.
    //   Fill-in before adding rows.
    //

    // Excel spreadsheet pages have names
    str x_sGridPageName = ""
            xauto(Get, Set)
            xassert(true);

// Operations

    // Start next data row
    ref<CExportGridRowIfaceGp> AddGridRow()
            vhook;

// UI

protected:
    virtual ref<CExportGridRowIfaceGp> OnAddGridRow()
            v1pure;
private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToExportGridPageIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
