// CNetSyncProgressImpl.cpp

#include "CNetProject.h"
#include "CNetSyncProgressImpl.h"
#include "VirtOs_dir.h"
#include "VirtOs_CFileList.h"
#include "CFileMultipartMergeIfaceGp.h"

CNetSyncProgressImpl::CNetSyncProgressImpl()
{
    _init_AttachToNetSyncProgressImpl();
}

void CNetSyncProgressImpl::_init_AttachToNetSyncProgressImpl()
{
    //x_r->_x_p = this;
}

CNetSyncProgressImpl::~CNetSyncProgressImpl()
{
    //rASSERT(!_m_bNetSyncProgressImplOpened);
}

num CNetSyncProgressImpl::OnCalcNetSyncProgress01()
{
    rASSERT(_m_bOneTimeInitNetSyncProgressImplOk);

    // update file list
    str sPart;
    int nPending = 0;
    for (
        iter i;
        _m_textSyncUploads.
            Iterate(
                out i,
                out sPart);)
    {
        path pathPart = sPart;

        bool bPresent = false;

        if (pathPart != "")
        {
            if (file(pathPart).IsExist() ||
                (
                    m_pathAlternateExt != "" &&
                    file(pathPart.GetAppendedPath(m_pathAlternateExt)).IsExist()
                ))
            {
                nPending++;
                bPresent = true;
            }
        }

        [...]

        if (!bPresent)
        {
            _m_textSyncUploads.SetWord(i, "");
        }
    }

    // all done?
    if (nPending == 0)
    {
        _m_textSyncUploads = null();
    }

    int nTotal = _m_textSyncUploads.GetWordCount();
    if (nTotal == 0)
    {
        return 1.0;
    }
    else
    {
        return
            math::GDivNot0(
                (num) nTotal - nPending,
                (num) nTotal,
                or_return 1.0);
    }
}

void CNetSyncProgressImpl::OnCancelNetSync()
{
    if (!_m_bOneTimeInitNetSyncProgressImplOk)
    {
        return;
    }

    // delete all parts, create a marker instead of the last one
    str sPart;
    bool bMarkerCreated = false;
    for (
        iter i;
        _m_textSyncUploads.
            IterateBackwards(
                out i,
                out sPart);)
    {
        path pathPart = sPart;

        // account for packed files
[...]