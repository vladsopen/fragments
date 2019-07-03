// CNetSyncOutImpl.cpp

#include "CNetProject.h"
#include "CNetSyncOutImpl.h"
#include "CNetBinalIface.h"
#include "CNetLocatorIfaceGp.h"
#include "CNetPumpOutIfaceGp.h"
#include "VirtOs_file.h"
#include "CNetErrorsIface.h"
#include "CFileMultipartSplitIfaceGp.h"
#include "VirtOs_text.h"
#include "CNetConnectionOutIfaceGp.h"
#include "CNetStatsIfaceGp.h"
#include "CNetSyncCommon.h"
#include "CFileBankTreeIfaceGp.h"
#include "CNetSyncProgressIfaceGp.h"
#include "CNetSyncProgressImpl.h"
#include "CNetParapackIface.h"
#include "CNetDisplayIfaceGp.h"

CNetSyncOutImpl::CNetSyncOutImpl()
{
    _init_AttachToNetSyncOutImpl();
}

void CNetSyncOutImpl::_init_AttachToNetSyncOutImpl()
{
    //x_r->_x_p = this;
}

void CNetSyncOutImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToNetSyncOutImpl();

    rFAIL("make sure it is OK to expose CNetSyncOutImpl!");
}

void CNetSyncOutImpl::OnInitNetSyncOut(
        ref<CNetDriverIfaceGp> rNewDriver)
{
    rASSERT(!_m_bOneTimeInitNetSyncOutImplOk);
    _m_bOneTimeInitNetSyncOutImplOk = true;

    TESTLOG(
        "+NetSync",
        "\n"
            "Net-Sync-Out: Init " + 
            x_rNetLocator->ExposeOut().ExportAsText() +
            "\n\n");

    // start session for OUT only
    x_rNetBinalForSyncOut->
        InitNetBinal(
            x_rNetLocator,
            (ptr<CNetLocatorIfaceGp>) null()); // pLocatorIn
    x_rNetBinalForSyncOut->
        RestartWithDriver(
            rNewDriver);

    ref<CNetPumpOutIfaceGp> rNetPumpOut = 
        x_rNetBinalForSyncOut->
            GetSessionManagedNetPumpOut();

    m_rOutboxFileBank->
        InitFileBank(
            rNetPumpOut->
                GetNetPumpTempLocalPath());

    //x_bEnablePumping = true
}

void CNetSyncOutImpl::OnCloseNetSyncOut()
{
    // csreful: close outer object (session) not inner (pump)
    x_rNetBinalForSyncOut->
        CloseNetBinal();

    m_rOutboxFileBank->
        CloseFileBank();
}

bool CNetSyncOutImpl::OnIsNetSyncInitOk()
{
    return _m_bOneTimeInitNetSyncOutImplOk;
}

int CNetSyncOutImpl::OnGetLastLocalOutboxFileCount()
{
    ref<CNetPumpOutIfaceGp> rNetPumpOut = 
        x_rNetBinalForSyncOut->
            GetSessionManagedNetPumpOut();

    return 
        rNetPumpOut->
            x_nLastLocalFileCount;
}

big CNetSyncOutImpl::OnGetLastLocalOutboxFileBytes()
{
    ref<CNetPumpOutIfaceGp> rNetPumpOut = 
        x_rNetBinalForSyncOut->
            GetSessionManagedNetPumpOut();

    return 
        rNetPumpOut->
            x_nnLastLocalFileBytes;
}

int CNetSyncOutImpl::OnGetLastLocalUploadedFileCount()
{
    ref<CNetPumpOutIfaceGp> rNetPumpOut = 
        x_rNetBinalForSyncOut->
            GetSessionManagedNetPumpOut();

    return 
        rNetPumpOut->
            GetInternalNetConnectionOut()->
                x_rNetStats->
                    x_nTransferredBoxCount;
}

num CNetSyncOutImpl::OnGetUploadProgress()
{
    int nLocalOutboxFiles = 
        GetLastLocalOutboxFileCount();

    int nUploadedFiles = 
        GetLastLocalUploadedFileCount();

    if (nLocalOutboxFiles == 0)
    {
        nUploadedFiles = 0;
    }

    //rUi->SetTotal(nUploadedFiles + nLocalOutboxFiles);
    //rUi->SetDone(nUploadedFiles);

    num numProgress = 
        math::GDiv0(
            (num) nUploadedFiles,
            (num) nUploadedFiles + nLocalOutboxFiles,
            or_return 0.);

    rASSERT(numProgress >= 0 && numProgress <= 1);

    return numProgress;
}

path CNetSyncOutImpl::OnGetInternalLocalTempPumpOutFolder()
{
    rASSERT(_m_bOneTimeInitNetSyncOutImplOk);

    ref<CNetPumpOutIfaceGp> rNetPumpOut = 
        x_rNetBinalForSyncOut->
            GetSessionManagedNetPumpOut();

    // locator path from pump
    return
        rNetPumpOut->GetNetPumpTempLocalPath();
}

ref<CNetEnablePumpIfaceGp> CNetSyncOutImpl::Onx_rNetEnablePumpSync_Get()
{
    ref<CNetPumpOutIfaceGp> rNetPumpOut = 
        x_rNetBinalForSyncOut->
            GetSessionManagedNetPumpOut();

    return
        rNetPumpOut->x_rNetEnablePump;
}

void CNetSyncOutImpl::Onx_rNetEnablePumpSync_Set(
        ref<CNetEnablePumpIfaceGp> rNetEnablePump)
{
    ref<CNetPumpOutIfaceGp> rNetPumpOut = 
        x_rNetBinalForSyncOut->
            GetSessionManagedNetPumpOut();

    rNetPumpOut->x_rNetEnablePump = 
        rNetEnablePump;
}

bool CNetSyncOutImpl::Onx_bEnableNetSyncPumping_Get()
{
    ref<CNetPumpOutIfaceGp> rNetPumpOut = 
        x_rNetBinalForSyncOut->
            GetSessionManagedNetPumpOut();

    return
        rNetPumpOut->x_bEnablePumping;
}

void CNetSyncOutImpl::Onx_bEnableNetSyncPumping_Set(
        bool bEnable)
{
    ref<CNetPumpOutIfaceGp> rNetPumpOut = 
        x_rNetBinalForSyncOut->
            GetSessionManagedNetPumpOut();

    // start pumping
    rNetPumpOut->x_bEnablePumping = 
        bEnable; //x_rNetSyncCommon->CanStartSyncPumpByDefault();
}

bool CNetSyncOutImpl::OnStartFileSync(
        path pathOriginal,
        str sRestoreIntoSubdirs,
        unidef unidefExtraHeader,
        str sSecurityId,
        ptr<CFileMultipartSplitIfaceGp> pFileMultipartSplit,
        out ref<CNetSyncProgressIfaceGp>& out_rProgress)
{
    rASSERT(_m_bOneTimeInitNetSyncOutImplOk);

    if failed(file(pathOriginal).IsExist())
    {
        return false;
    }

    ref<CNetPumpOutIfaceGp> rNetPumpOut = 
        x_rNetBinalForSyncOut->
            GetSessionManagedNetPumpOut();

    // locator path from pump
    path pathPumpOutbox = 
        rNetPumpOut->GetNetPumpTempLocalPath();

    TESTLOG(
        "+NetSync",
        "\n"
            "Net-Sync-Out: Start " + 
            pathOriginal +
            " [" + sSecurityId + "]\n"
            "   splitting to " + pathPumpOutbox +
            "\n\n");

    //
    // Move to pump outbox
    //

    ref<CFileMultipartSplitIfaceGp> rFileMultipartSplit;

    if not_null(
        ref<CFileMultipartSplitIfaceGp> rUseSplit = 
            pFileMultipartSplit)
    {
        rFileMultipartSplit = rUseSplit;
    }
    else
    {
        rFileMultipartSplit->x_bPreserveOriginalFileNameInSplitParts = false;
        rFileMultipartSplit->x_bGenerateUniquePartsBaseName = true;
    }

    str sError;
    text textPartPaths = 
        rFileMultipartSplit->
            SplitFileIntoParts(
                pathOriginal,
                sRestoreIntoSubdirs,
                x_nMultipartFileSplitBytes,
                //pathPumpOutbox,
                m_rOutboxFileBank,
                unidefExtraHeader,
                out sError);

    rASSERTM(sError == "", sError);
    if (sError != "")
    {
        CNetProject::GGetIt().
            x_rGlobalNetErrors->
                LogNetError(
                    CNetErrorsIface::E_NetErrorFrom_Local,
                    CNetErrorsIface::E_NetErrorLevel_DataLoss,
                    TRANSLATE(
                        "Failed to [... UI-string-removed]");

        return false;
    }

    rASSERT(textPartPaths.GetWordCount() > 1 || pFileMultipartSplit != null());

    ref<CNetSyncProgressImpl> rNetSyncProgress;
    rNetSyncProgress->
        InitNetSyncOutProgress(
            textPartPaths,
            x_rNetLocator->x_bParapack? 
                CNetParapackIface::C_sParapackExt
                :
                path());

    out_rProgress = rNetSyncProgress;

    return true;
}

[...]