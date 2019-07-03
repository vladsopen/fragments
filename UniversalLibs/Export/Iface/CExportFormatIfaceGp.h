// CExportFormatIfaceGp.h
#pragma once

class CExportWriterIfaceGp;
class CExportScalarTypeIfaceGp;

//
// CExportFormatIfaceGp -
//
//   Abstract format drivers receiving format-independent data and commands and sending
//     the formatted result to a CExportWriterIfaceGp implementation.
//
// no impl assumed
//ASSUME_IMPL_FOR_IFACE(CExportFormatImpl, CExportFormatIfaceGp)

class CExportFormatIfaceGp : public object
{
public:
    CExportFormatIfaceGp();
    //NEW_LOCAL(CExportFormatImpl, CExportFormatIfaceGp)
    //NEW_GP(SysCommon, CExportFormatImpl, CExportFormatIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

    //
    // Global File-scope Header and Ending
    //

    // Start of file
    void HandleFileHeaderStart(
            ref<CExportWriterIfaceGp> rExportWriter)
            vhook;

    // Header attributes to the format controller
    void SetFileHeaderAuthorName(
            str sDocumentAuthorName,
            ref<CExportWriterIfaceGp> rExportWriter)
            vhook;

    // End of file header
    void HandleFileHeaderEnd(
            ref<CExportWriterIfaceGp> rExportWriter)
            vhook;

    // End the document
    void HandleDocumentEnd(
            ref<CExportWriterIfaceGp> rExportWriter)
            vhook;

    //
    // Page-global statements
    //

    // Start of page
    void HandlePageHeaderStart(
            ref<CExportWriterIfaceGp> rExportWriter)
            vhook;

    // Page attributes
    void SetPageName(
            str sGridPageName,
            ref<CExportWriterIfaceGp> rExportWriter)
            vhook;

    // End of page header
    void HandlePageHeaderEnd(
            ref<CExportWriterIfaceGp> rExportWriter)
            vhook;

    // End of page
    void HandlePageEnd(
            ref<CExportWriterIfaceGp> rExportWriter)
            vhook;

    //
    // Grid row statament
    //

    // Start a row
    void HandleGridRowHeaderStart(
            ref<CExportWriterIfaceGp> rExportWriter)
            vhook;

    // Set row header attributes
        // -none yet-

    // End row header
    void HandleGridRowHeaderEnd(
            ref<CExportWriterIfaceGp> rExportWriter)
            vhook;

    // End row itself
    void HandleGridRowEnd(
            ref<CExportWriterIfaceGp> rExportWriter)
            vhook;

    //
    // Grid cell
    //

    // Cell content with Excel-like formula
    void WriteGridScalar(
            str sFormula,
            str sValue,
            type<CExportScalarTypeIfaceGp> typeExportScalarType,
            ref<CExportWriterIfaceGp> rExportWriter)
            vhook;

// UI

protected:
    virtual void OnHandleFileHeaderStart(
            ref<CExportWriterIfaceGp> rExportWriter)
            v1pure;
    virtual void OnSetFileHeaderAuthorName(
            str sDocumentAuthorName,
            ref<CExportWriterIfaceGp> rExportWriter)
            v1pure;
    virtual void OnHandleFileHeaderEnd(
            ref<CExportWriterIfaceGp> rExportWriter)
            v1pure;
    virtual void OnHandleDocumentEnd(
            ref<CExportWriterIfaceGp> rExportWriter)
            v1pure;
    virtual void OnHandlePageHeaderStart(
            ref<CExportWriterIfaceGp> rExportWriter)
            v1pure;
    virtual void OnSetPageName(
            str sGridPageName,
            ref<CExportWriterIfaceGp> rExportWriter)
            v1pure;
    virtual void OnHandlePageHeaderEnd(
            ref<CExportWriterIfaceGp> rExportWriter)
            v1pure;
    virtual void OnHandlePageEnd(
            ref<CExportWriterIfaceGp> rExportWriter)
            v1pure;
    virtual void OnHandleGridRowHeaderStart(
            ref<CExportWriterIfaceGp> rExportWriter)
            v1pure;
    virtual void OnHandleGridRowHeaderEnd(
            ref<CExportWriterIfaceGp> rExportWriter)
            v1pure;
    virtual void OnHandleGridRowEnd(
            ref<CExportWriterIfaceGp> rExportWriter)
            v1pure;
    virtual void OnWriteGridScalar(
            str sFormula,
            str sValue,
            type<CExportScalarTypeIfaceGp> typeExportScalarType,
            ref<CExportWriterIfaceGp> rExportWriter)
            v1pure;
private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToExportFormatIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
