// CExportWriterIfaceGp.h
#pragma once

//
// CExportWriterIfaceGp -
//
//   Abstract texual data output stream.
//     Virtual implementations can write text to memory, to a file, etc.
//
// no specific impl assumed
//ASSUME_IMPL_FOR_IFACE(CExportWriterImpl, CExportWriterIfaceGp)

class CExportWriterIfaceGp : public object
{
public:
    CExportWriterIfaceGp();
    //NEW_LOCAL(CExportWriterImpl, CExportWriterIfaceGp)
    //NEW_GP(SysCommon, CExportWriterImpl, CExportWriterIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // At least Excel XML requires this, but we need to be flexible historically
    bool x_bOutputAsUtf8 = false
            xauto(Get, Set);

// Operations

    // Virtual abstract output
    void AppendText(
            str sAdd)
            vhook;

    // Debug report for features not suppoerted in a format
    //   (for advances diagnostics key may turn into a type<> in the future)
    void ReportIgnoredOutput(
            str sKey,
            str sContent)
            vhook;

// UI

protected:
    virtual void OnAppendText(
            str sAdd)
            v1pure;
    virtual void OnReportIgnoredOutput(
            str sKey,
            str sContent)
            v1null;
private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToExportWriterIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
