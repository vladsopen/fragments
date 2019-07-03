// CSyncOutImpl.cpp

#include "CProject.h"
#include "CSyncOutImpl.h"
#include "CNetImpl.h"
#include "CSessionIface.h"
#include "CNetSyncOutIfaceGp.h"
#include "CNetLocatorIfaceGp.h"
#include "CActionIface.h"
#include "VirtOs_file.h"
#include "CNetDriverIfaceGp.h"
#include "CStoreIface.h"
#include "CBasicClassIface.h"
#include "CBasicNodeIfaceGp.h"
#include "CSyncCook.h"
#include "CNetEnablePumpIfaceGp.h"
#include "CGlobalConfigIface.h"
#include "CFileMultipartSplitIfaceGp.h"
#include "VirtOs_CAppGp.h"
#include "CAppInstallIfaceGp.h"
#include "CAppInstallInternalIfaceGp.h"

void CSyncOutImpl::OnFlushActionUpload()
{
    if (m_sMergedActionsToUpload == "")
    {
        return;
    }

    //*****************************************************************
    //
    // SAFETY CRITICAL FILE CREATION
    //
    //   If anything fails here we have to fatally abort the 
    //   whole process otherwise we making the local DB out of
    //   sync with the server.
    //
    //*****************************************************************

    hookrfail hookrfailScope(
        &GHookFailOutboxSave,
        str(),
        NULL);

    // debug test rFAIL handler
    {
        str sTestFail = 
            CProject::GGetIt().x_rGlobalConfig->
                x_sTestSyncOutboxFail;
        if (sTestFail != "")
        {
            rFAIL(sTestFail);
        }
    }

    ref<CNetImpl> rNet = 
        _x_pNet.Get().Valid();
    ref<CStoreIface> rStore = 
        rNet->GetStore();
    ref<CSessionIface> rSession = 
        rNet->GetSession();
    ref<CBasicClassIface> rBasicClass = 
        rSession->x_rSessionBasicClass;

    // Compress/Encrypt
    ref<CSyncCook> rSyncCook = 
        rNet->x_rSyncCook;
    str sCooked = 
        rSyncCook->
            CookActionsBeforeUpload(
                m_sMergedActionsToUpload,
                x_rNetSyncOut);

    // save action script into the file
    str sSaveError;
    Save[...]

    // Send to network
    ref<CNetSyncProgressIfaceGp> rNetProgress;
    x_rNetSyncOut->
        StartFileSync(
            pathOutFile,
            "", // sRestoreIntoSubdirs,
            m_unidefDeferExtraHeader,
            "", // sSecurityId
            ptr<CFileMultipartSplitIfaceGp>(),
            out rNetProgress);

    // clear temp file, still critical
    {
        str sDeleteError;
        file(pathOutFile).EraseFile(out sDeleteError);
        if (sDeleteError != "")
        {
            rNet->
                HandleNetError(
                    CNetErrorsIfaceGp::E_NetErrorFrom_Local,
                    CNetErrorsIfaceGp::E_NetErrorLevel_Warning,
                    false, // bForceFloater
                    "Cannot delete temp database update upload file.",
                    sDeleteError, // str sDetails,
                    "ActionsExport", // sContext,
                    x_rNetSyncOut);

            return;
        }
    }

    //*****************************************************************
    // SAFETY CRITICAL DESTRUCTORS HERE
    //*****************************************************************
}

void CSyncOutImpl::HandleUploadTimer()
{
    rASSERT(_m_bSyncOutImplOpened);

    bool bHaveJobs = false;
    ForcePollSyncOut(
        out bHaveJobs);
}

void CSyncOutImpl::ForcePollSyncOut(
        out bool& out_bHaveJobs)
{
    rASSERT(_m_bSyncOutImplOpened);

    out_bHaveJobs = 
        x_rNetSyncOut->
            PollNetSyncOut();
}

[...]