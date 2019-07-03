// CWizardStepType.cpp

#include "CProject.h"
#include "CWizardStepType.h"
#include "CDownloaderImpl.h"
#include "CDeviceManagerIface.h"
#include "CTimerForTomographDownloader.h"
#include "CSessionIface.h"
#include "CPatientLinkIface.h"

CWizardStepType::CWizardStepType()
{
}

const typelist<CWizardStepType> CWizardStepType::GetDerivedList()
{
    typelist<CWizardStepType> typelistDerived;
    if (typelistDerived.IsNeedInit(__super::GetDerivedListStorage()))
    {
        typelistDerived.AddDerivedClassesWithFlag("flag_List_as_WizardStepType");
    }

    return typelistDerived;
}

bool CWizardStepTypeWelcome::OnTryEnterStep(
        ref<CDownloaderImpl> rDownloader,
        ref<CDeviceManagerIface> rDeviceManager)
{
    rDownloader; rDeviceManager;

    // nothing to do
    return true; // OK to stay on this step
}

bool CWizardStepTypeDownloading::OnTryEnterStep(
        ref<CDownloaderImpl> rDownloader,
        ref<CDeviceManagerIface> rDeviceManager)
{
    // Init callback timeout
    ref<CTimerForTomographDownloader> rTimerForTomograph = 
        rDownloader->x_rTimerForTomograph;
    rTimerForTomograph->
        SetTimeoutInMillisec(
            CTimerForTomographDownloader::C_tickRecommendedTomographPollingTimeout);

    // Init callback references
    rTimerForTomograph->_x_pDownloader = rDownloader;
    rTimerForTomograph->_x_pDeviceManager = rDeviceManager;

    // Open the device
    if (!rDeviceManager->InitTomograph())
    {
        return false; // stay on the first step!
    }

    // Notify PatientIo
    rDownloader->_x_pSessionTrusted.Get().Valid()->
        x_rPatientLink->
            HandleStartNewCheckup();

    // Download all of them
    rDownloader->x_nWantDownloadTomogramCount = 
        rDeviceManager->
            GetTomogramCountAndStartDownload();

    // Remove Tomograms we may have open
    rDownloader->RemoveAllTomograms();

    // Count in the queue
    if (rDownloader->x_nWantDownloadTomogramCount > 0)
    {
        rASSERT(rDownloader->GetTomogramCount() == 0);
        rDownloader->AddNewTomogram();
    }
    else
    {
        // Nothing to download, go to the last step
        VERIFY(
            rDownloader->
                GotoNextStep(
                    type<CWizardStepTypeFinalInfo>(),
                    rDeviceManager));
    }

    // update UI
    rDownloader->UpdateDownloaderUiCompletelyOnline();

    return true; // OK to stay on this step
}

bool CWizardStepTypeFinalInfo::OnTryEnterStep(
        ref<CDownloaderImpl> rDownloader,
        ref<CDeviceManagerIface> rDeviceManager)
{
    rich richReport;

    // Download all of them
    if (rDownloader->x_nWantDownloadTomogramCount == 0)
    {
        // No report
        richReport = "";

        // Call erase just as a formality
        if (rDownloader->x_bDoEraseAllWhenDone)
        {
            rDeviceManager->EraseAllTomogramsFromDevice();
        }
    }
    else
    if (rDeviceManager->GetFullDeviceErrorInfoOr("") == "")
    {
        // No errors - can erase safely
        if (rDownloader->x_bDoEraseAllWhenDone)
        {
            if (rDeviceManager->EraseAllTomogramsFromDevice())
            {
                // All fine and clean
                richReport = 
                    TRANSLATE(
                        "(Tomograph[... UI-string-removed]");
            }
            else
            {
                // Red error report
                richReport = 
                    TRANSLATE(
                        "Tomograph [... UI-string-removed]");

                richReport = RichBold(RichTextColor(rgb(255, 0, 0), richReport));
            }
        }
        else
        {
            // Red error report
            richReport = 
                TRANSLATE(
                    "Tomograph [... UI-string-removed]");

            richReport = RichBold(RichTextColor(rgb(255, 0, 255), richReport));
        }
    }

    rDownloader->x_richFinalResultInfo = richReport;

    if (rDownloader->x_sBodyPointsFailed != "")
    {
        rich richError = 
            "\n\n" +
                TRANSLATE(
                    "Cannot get[... UI-string-removed]");

        richError = RichBold(RichTextColor(rgb(255, 0, 255), richError));

        rDownloader->x_richFinalResultInfo = 
            rDownloader->x_richFinalResultInfo + 
                richError;
    }

    // nothing to do
    return true; // OK to stay on this step
}

bool CWizardStepTypeDoneAndOut::OnTryEnterStep(
        ref<CDownloaderImpl> rDownloader,
        ref<CDeviceManagerIface> rDeviceManager)
{
    rDeviceManager;

    // Close main window
    rDownloader->ShutdownDownloaderUi();

    return true; // OK to stay on this step
}

