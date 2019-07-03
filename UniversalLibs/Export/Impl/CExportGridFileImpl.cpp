// CExportGridFileImpl.cpp

#include "CSysCommonProject.h"
#include "CExportGridFileImpl.h"
#include "CExportGridPageImpl.h"
#include "CExportFormatIfaceGp.h"
#include "CExportWriterIfaceGp.h"

CExportGridFileImpl::CExportGridFileImpl()
{
    _init_AttachToExportGridFileImpl();
}

CExportGridFileImpl::~CExportGridFileImpl()
{
    rASSERT(!_m_bOneTimeInitOk || _m_bFinalized);
}

void CExportGridFileImpl::_init_AttachToExportGridFileImpl()
{
    //x_r->_x_p = this;
}

void CExportGridFileImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToExportGridFileImpl();

    rFAIL("make sure it is OK to expose CExportGridFileImpl!");
}

void CExportGridFileImpl::OnStartExportGridFile(
        ref<CExportFormatIfaceGp> rNewExportFormat,
        ref<CExportWriterIfaceGp> rNewExportWriter)
{
    rASSERT(!_m_bFinalized);

    // init once
    if failed(!_m_bOneTimeInitOk)
    {
        return;
    }
    _m_bOneTimeInitOk = true;

    // save callbacks
    this->x_rExportFormat = rNewExportFormat;
    this->x_rExportWriter = rNewExportWriter;
}

ref<CExportGridPageIfaceGp> CExportGridFileImpl::OnAddGridPage()
{
    rASSERT(!_m_bFinalized);

    // end last page if any
    m_rCurrentExportGridPage->CloseExportGridPage();

    // header before first page
    EnsureHeaderWritten();

    // create a new one
    m_rCurrentExportGridPage = null();
    m_rCurrentExportGridPage->_x_pExportGridFile = this;
    m_rCurrentExportGridPage->StartExportGridPage();

    return m_rCurrentExportGridPage;
}

void CExportGridFileImpl::EnsureHeaderWritten()
{
    if (!m_bHeaderWritten)
    {
        m_bHeaderWritten = true;

        rASSERT(!_m_bFinalized);

        //
        // Prepare format controller
        //

        x_rExportFormat->
            HandleFileHeaderStart(
                x_rExportWriter);

        //
        // Send header attributes to the format controller
        //

        x_rExportFormat->
            SetFileHeaderAuthorName(
                x_sDocumentAuthorName,
                x_rExportWriter);

        //
        // End of header
        //

        x_rExportFormat->
            HandleFileHeaderEnd(
                x_rExportWriter);
    }
}

void CExportGridFileImpl::OnFinalizeExportGridFile()
{
    rASSERT(!_m_bFinalized);

    _m_bFinalized = true;

    EnsureHeaderWritten();

    // end last page if any
    m_rCurrentExportGridPage->CloseExportGridPage();

    // end the document
    x_rExportFormat->
        HandleDocumentEnd(
            x_rExportWriter);
}

