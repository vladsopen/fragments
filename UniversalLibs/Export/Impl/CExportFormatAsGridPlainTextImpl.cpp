// CExportFormatAsGridPlainTextImpl.cpp

#include "CSysCommonProject.h"
#include "CExportFormatAsGridPlainTextImpl.h"
#include "CExportScalarTypeIfaceGp.h"
#include "CExportWriterIfaceGp.h"

CExportFormatAsGridPlainTextImpl::CExportFormatAsGridPlainTextImpl()
{
    _init_AttachToExportFormatAsGridPlainTextImpl();
}

void CExportFormatAsGridPlainTextImpl::_init_AttachToExportFormatAsGridPlainTextImpl()
{
    //x_r->_x_p = this;
}

void CExportFormatAsGridPlainTextImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToExportFormatAsGridPlainTextImpl();

    rFAIL("make sure it is OK to expose CExportFormatAsGridPlainTextImpl!");
}

void CExportFormatAsGridPlainTextImpl::OnHandleFileHeaderStart(
        ref<CExportWriterIfaceGp> rExportWriter)
{
    rExportWriter;
}

void CExportFormatAsGridPlainTextImpl::OnSetFileHeaderAuthorName(
        str sDocumentAuthorName,
        ref<CExportWriterIfaceGp> rExportWriter)
{
    rExportWriter;
}

void CExportFormatAsGridPlainTextImpl::OnHandleFileHeaderEnd(
        ref<CExportWriterIfaceGp> rExportWriter)
{
    rExportWriter;
}

void CExportFormatAsGridPlainTextImpl::OnHandleDocumentEnd(
        ref<CExportWriterIfaceGp> rExportWriter)
{
    rExportWriter;
}

void CExportFormatAsGridPlainTextImpl::OnHandlePageHeaderStart(
        ref<CExportWriterIfaceGp> rExportWriter)
{
    rExportWriter;
}

void CExportFormatAsGridPlainTextImpl::OnSetPageName(
        str sGridPageName,
        ref<CExportWriterIfaceGp> rExportWriter)
{
    rExportWriter;
}

void CExportFormatAsGridPlainTextImpl::OnHandlePageHeaderEnd(
        ref<CExportWriterIfaceGp> rExportWriter)
{
    rExportWriter;
}

void CExportFormatAsGridPlainTextImpl::OnHandlePageEnd(
        ref<CExportWriterIfaceGp> rExportWriter)
{
    rExportWriter;
}

void CExportFormatAsGridPlainTextImpl::OnHandleGridRowHeaderStart(
        ref<CExportWriterIfaceGp> rExportWriter)
{
    rExportWriter;
}

void CExportFormatAsGridPlainTextImpl::OnHandleGridRowHeaderEnd(
        ref<CExportWriterIfaceGp> rExportWriter)
{
    rExportWriter;
}

void CExportFormatAsGridPlainTextImpl::OnHandleGridRowEnd(
        ref<CExportWriterIfaceGp> rExportWriter)
{
    rExportWriter;
}

void CExportFormatAsGridPlainTextImpl::OnWriteGridScalar(
        str sFormula,
        str sValue,
        type<CExportScalarTypeIfaceGp> typeExportScalarType,
        ref<CExportWriterIfaceGp> rExportWriter)
{
    //
    // Write supported content
    //

    // value
    rExportWriter->
        AppendText(
            sValue);

    // separator
    rExportWriter->
        AppendText(
            x_sExportValueSeparator);

    //
    // Unsupported features
    //

    // formulas are not supported
    rExportWriter->
        ReportIgnoredOutput(
            "CellFormula",
            sFormula);

    // data types are not supported
    rExportWriter->
        ReportIgnoredOutput(
            "CellType",
            typeExportScalarType->GetObjectClassInfo().GetClassNameInProcess());
}

