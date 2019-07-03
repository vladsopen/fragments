// CExportGridPageImpl.cpp

#include "CSysCommonProject.h"
#include "CExportGridPageImpl.h"
#include "CExportGridRowImpl.h"
#include "CExportFormatIfaceGp.h"
#include "CExportGridFileImpl.h"
#include "CExportWriterIfaceGp.h"

CExportGridPageImpl::CExportGridPageImpl()
{
    _init_AttachToExportGridPageImpl();
}

CExportGridPageImpl::~CExportGridPageImpl()
{
    rASSERT(!_m_bOpened);
}

void CExportGridPageImpl::_init_AttachToExportGridPageImpl()
{
    //x_r->_x_p = this;
}

void CExportGridPageImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToExportGridPageImpl();

    rFAIL("make sure it is OK to expose CExportGridPageImpl!");
}

void CExportGridPageImpl::StartExportGridPage()
{
    rASSERT(!_m_bOpened);
    _m_bOpened = true;
}

void CExportGridPageImpl::EnsurePageHeaderWritten()
{
    if (_m_bHeaderWritten)
    {
        return;
    }

    _m_bHeaderWritten = true;

    // parents
    ref<CExportGridFileImpl> rExportGridFile = 
        _x_pExportGridFile.Get().Valid();

    // controllers
    ref<CExportFormatIfaceGp> rExportFormat = 
        rExportGridFile->x_rExportFormat;
    ref<CExportWriterIfaceGp> rExportWriter = 
        rExportGridFile->x_rExportWriter;

    //
    // Start page header
    //

    rExportFormat->
        HandlePageHeaderStart(
            rExportWriter);

    //
    // Set page header attributes
    //

    rExportFormat->
        SetPageName(
            x_sGridPageName,
            rExportWriter);

    //
    // End page header
    //

    rExportFormat->
        HandlePageHeaderEnd(
            rExportWriter);
}

void CExportGridPageImpl::CloseExportGridPage()
{
    if (!_m_bOpened)
    {
        return;
    }

    _m_bOpened = false;

    EnsurePageHeaderWritten();

    // end last page if any
    m_rCurrentExportGridRow->CloseExportGridRow();

    // parents
    ref<CExportGridFileImpl> rExportGridFile = 
        _x_pExportGridFile.Get().Valid();

    // controllers
    ref<CExportFormatIfaceGp> rExportFormat = 
        rExportGridFile->x_rExportFormat;
    ref<CExportWriterIfaceGp> rExportWriter = 
        rExportGridFile->x_rExportWriter;

    //
    // end page
    //

    rExportFormat->
        HandlePageEnd(
            rExportWriter);
}

ref<CExportGridRowIfaceGp> CExportGridPageImpl::OnAddGridRow()
{
    // end last page if any
    m_rCurrentExportGridRow->CloseExportGridRow();

    EnsurePageHeaderWritten();

    // create a new one
    m_rCurrentExportGridRow = null();
    m_rCurrentExportGridRow->_x_pExportGridPage = this;
    m_rCurrentExportGridRow->StartExportGridRow();

    return m_rCurrentExportGridRow;
}

