// CExportFormatAsGridRichImpl.cpp

#include "CSysCommonProject.h"
#include "CExportFormatAsGridRichImpl.h"
#include "VirtOs_CAppGp.h"
#include "VirtOs_CGlobalUi.h"
#include "CExportWriterIfaceGp.h"
#include "CExportScalarTypeIfaceGp.h"

CExportFormatAsGridRichImpl::CExportFormatAsGridRichImpl()
{
    _init_AttachToExportFormatAsGridRichImpl();
}

void CExportFormatAsGridRichImpl::_init_AttachToExportFormatAsGridRichImpl()
{
    //x_r->_x_p = this;
}

void CExportFormatAsGridRichImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToExportFormatAsGridRichImpl();

    rFAIL("make sure it is OK to expose CExportFormatAsGridRichImpl!");
}

str CExportFormatAsGridRichImpl::GetSafetyCellContent(
        str sUnsafe)
{
    str sSafe = 
        CAppGp::GGet()->x_rUi->ConvertToPlainText(sUnsafe);

    return 
        sSafe;
}

void CExportFormatAsGridRichImpl::OnHandleFileHeaderStart(
        ref<CExportWriterIfaceGp> rExportWriter)
{
    rExportWriter;
}

void CExportFormatAsGridRichImpl::OnSetFileHeaderAuthorName(
        str sDocumentAuthorName,
        ref<CExportWriterIfaceGp> rExportWriter)
{
    rExportWriter;

    m_sDocumentAuthorName = sDocumentAuthorName;
}

void CExportFormatAsGridRichImpl::OnHandleDocumentEnd(
        ref<CExportWriterIfaceGp> rExportWriter)
{
    rExportWriter;
}

void CExportFormatAsGridRichImpl::OnHandlePageHeaderStart(
        ref<CExportWriterIfaceGp> rExportWriter)
{
    rExportWriter;
}

void CExportFormatAsGridRichImpl::OnSetPageName(
        str sGridPageName,
        ref<CExportWriterIfaceGp> rExportWriter)
{
    rExportWriter;

    m_sPageName = sGridPageName;
}

void CExportFormatAsGridRichImpl::OnHandlePageHeaderEnd(
        ref<CExportWriterIfaceGp> rExportWriter)
{
    rExportWriter;
}

void CExportFormatAsGridRichImpl::OnHandlePageEnd(
        ref<CExportWriterIfaceGp> rExportWriter)
{
    rExportWriter;
}

void CExportFormatAsGridRichImpl::OnHandleGridRowHeaderStart(
        ref<CExportWriterIfaceGp> rExportWriter)
{
    str sStart = 
        "\vtrowd \vclcbpat0 ";

    // Relative width
    int nRelativeRichTableColumnWidthInTwips = 0;

    repeat(iColumn, x_nTableColumnCount)
    {
        // Increase relative width
        nRelativeRichTableColumnWidthInTwips +=
            x_nTableColumnWidthInTwips;

        // Column header
        sStart +=
            "\vcellx" +
                Str(nRelativeRichTableColumnWidthInTwips) +
                " ";
    }

    rExportWriter->
        AppendText(
            sStart);
}

void CExportFormatAsGridRichImpl::OnHandleGridRowHeaderEnd(
        ref<CExportWriterIfaceGp> rExportWriter)
{
    rExportWriter;
}

void CExportFormatAsGridRichImpl::OnHandleGridRowEnd(
        ref<CExportWriterIfaceGp> rExportWriter)
{
    str sEnd = 
        "\vrow ";

    rExportWriter->
        AppendText(
            sEnd);
}

void CExportFormatAsGridRichImpl::OnWriteGridScalar(
        str sFormula,
        str sValue,
        type<CExportScalarTypeIfaceGp> typeExportScalarType,
        ref<CExportWriterIfaceGp> rExportWriter)
{
    // Not used
    typeExportScalarType;

    //
    // Write supported content
    //

    str sCell =
        RichAlignCenter(GetSafetyCellContent(sValue)) +
            "\vcell ";

    rExportWriter->
        AppendText(
            sCell);
}

[...]