// CExportGridFileIfaceGp.h
#pragma once

class CExportGridPageIfaceGp;
class CExportWriterIfaceGp;
class CExportFormatIfaceGp;

//
// CExportGridFileIfaceGp -
//
//   Table/grid data exporter main controller.
//     Initiates and finalizes the export transaction and serves as the root for
//     all necessary substructures.
//
//   Format and output methods are abstracted by virtualozed
//     CExportFormatIfaceGp - actual format driver (XML/XLS, CVS, HTML <table>, ...)
//     CExportWriterIfaceGp - abstract formatted text destination (memory, file, ...)
//
ASSUME_IMPL_FOR_IFACE(CExportGridFileImpl, CExportGridFileIfaceGp)

class CExportGridFileIfaceGp : public object
{
public:
    CExportGridFileIfaceGp();
    //NEW_LOCAL(CExportGridFileImpl, CExportGridFileIfaceGp)
    NEW_GP(SysCommon, CExportGridFileImpl, CExportGridFileIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // no parent. create unattached and call Start.
    //ptr<> _x_p
    //        xauto(Get, Set);

    //
    // Global file document properties.
    //   Fill-in before adding pages.
    //

    // Some formats support fancy headers
    str x_sDocumentAuthorName = ""
            xauto(Get, Set)
            xassert(true);

// Operations

    // Init and set the output drivers.
    //   Create new objects for callbacks, it saves refs.
    void StartExportGridFile(
            ref<CExportFormatIfaceGp> rNewExportFormat,
            ref<CExportWriterIfaceGp> rNewExportWriter)
            vhook;

    // Start a page (usually only one per file)
    ref<CExportGridPageIfaceGp> AddGridPage()
            vhook;

    // Prepare the result.
    //   Only after this call you can expect the Writer to
    //   have complete the output.
    void FinalizeExportGridFile()
            vhook;

// UI

protected:
    virtual void OnStartExportGridFile(
            ref<CExportFormatIfaceGp> rNewExportFormat,
            ref<CExportWriterIfaceGp> rNewExportWriter)
            v1pure;
    virtual ref<CExportGridPageIfaceGp> OnAddGridPage()
            v1pure;
    virtual void OnFinalizeExportGridFile()
            v1pure;
private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToExportGridFileIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
