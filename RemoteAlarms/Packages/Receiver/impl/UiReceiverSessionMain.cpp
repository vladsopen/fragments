// UiReceiverSessionMain.cpp

#include "CProject.h"
#include "CReceiverSessionImpl.h"
#include "CNetSyncInIfaceGp.h"
#include "VirtOs_dir.h"
#include "CGlobalConfigIface.h"
#include "CSessionIface.h"
#include "CJobListIface.h"
#include "CImportAlarms.h"

void UiReceiverSessionMain::Ui()
{
    // *** DEBUG UI HERE! ***

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rReceiverSession->_m_ptrmapUiReceiverSessionMain += rUi;

        rUi->SetAlignContentToTop(); // ExpertMail button must stick to the job list

        rUi->SetDialogSubPanel();
    }

    /*
    //
    // Session Corner
    //
    {
        ref<UiContainer> rUi = 
            m_rReceiverSession->
                GetSession()->
                    NewUiSessionCornerPopup();
    }
    */

    //
    // Missing alarms
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetIdleUpdate(true);

        m_rReceiverSession->x_rImportAlarms->_m_ptrmapUiMissingAlarms += rUi;

        rUi->SetText(
            m_rReceiverSession->
                x_rImportAlarms->
                    GetMissingAlarmReport() 
                +
                (CProject::GGetIt().
                    x_rGlobalConfig->
                        _x_bImportInCurrentPatient?
                            RichErrorBold(" (ALL INTO CURRENT PATIENT!)")
                            :
                            ""));

        rUi->SetFontSize(16);

        rUi->SetMaxX(oo);
    }

    //
    // Net Downloader Status
    //
    {
        ref<UiContainer> rUi = 
            m_rReceiverSession->
                NewUiReceiverSessionStatus();
    }

    NL();

    if (debug::GIsAllowed...())
    {
        //
        // [Start]
        //
        {
            ref<CUiButtonGp> rUi;

            rUi->SetText("Enable");

            //rUi->SetAlikeX(m_rReceiverSession->GetSession()->x_rJobList->GetJobBoxAlikeX());

            rUi->OnClick = [=]
            {
                m_rReceiverSession->
                    StartDownload();
            }
        }

        //
        // [Browse Merged]
        //
        {
            ref<CUiButtonGp> rUi;

            rUi->SetIdleUpdate(true);

            rUi->SetText(
                "Browse Inbox");

            rUi->OnClick = [=]
            {
                path pathFolder = 
                    m_rReceiverSession->GetTempInbox();

                dir(pathFolder).CreateDir(out_IGNORE_sError);

                sys::GOpenExplorer(
                    pathFolder,
                    sys::E_ExplorerView_Tree,
                    sys::E_ExplorerOpen_Folder);
            }
        }

        //
        // [Browse Temp]
        //
        {
            ref<CUiButtonGp> rUi;

            rUi->SetIdleUpdate(true);

            rUi->SetText(
                "Browse Temp");

            rUi->OnClick = [=]
            {
                path pathFolder = 
                    m_rReceiverSession->GetTempFragmentIn();

                dir(pathFolder).CreateDir(out_IGNORE_sError);

                sys::GOpenExplorer(
                    pathFolder,
                    sys::E_ExplorerView_Tree,
                    sys::E_ExplorerOpen_Folder);
            }
        }

        //
        // progress...
        //
        {
            ref<CUiStaticGp> rUi;

            m_rReceiverSession->_m_ptrmapUiDownloadProgress += rUi;

            rUi->SetText(
                "Download " + 
                    (
                        m_rReceiverSession->
                            x_rReceiverNetSyncIn->
                                x_bEnableNetSyncPumping?
                            "is ON"
                            :
                            "is OFF"
                    )
                    +
                    " "
                    +
                    Str(
                        math::GRound(
                            100. *
                            m_rReceiverSession->
                                x_rReceiverNetSyncIn->
                                    GetDownloadProgress(),
                            0.01)) +
                    "%");
        }
    }
}
