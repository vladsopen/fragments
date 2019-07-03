// UiJobStateRow.cpp

#include "CProject.h"
#include "CJobStateImpl.h"
#include "CJobListImpl.h"
#include "CTaskIface.h"
#include "CConstIface.h"
#include "CUserListIface.h"
#include "CSessionIface.h"
#include "VirtOs_CAppGp.h"
#include "VirtOs_CGlobalUi.h"
#include "VirtOs_LegacyResource.h"
#include "CJobQuest.h"
#include "CJobSyncOutImpl.h"
#include "CJobSyncInImpl.h"

void UiJobStateRow::Ui()
{
    ref<CTaskIface> rTask = 
        m_rJobState->x_rTask;
    ref<CJobListImpl> rJobList = 
        m_rJobState->GetJobList();
    ref<CSessionIface> rSession = 
        rJobList->GetSession();
    ref<CUserListIface> rUserList = 
        rSession->x_rUserList;

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rJobState->_m_ptrmapUiJobStateRow += rUi;

        rUi->SetPreferredX(0);
        rUi->SetMaxX(oo);
    }

    //
    // Common back with optional popup
    //
    {
        ref<CUiListItemGp> rUi;

        //rUi->SetIdleUpdate(true);

        str sTip; // =
            //m_rJobState->
            //    GetDirectionReport("\n");

        sTip +=
            //"\n" +
                m_rJobState->GetReportInList("\n");

        if (CConstIface::GIsTestPublic())
        {
            sTip +=
                "\n\n" +
                    RichSize(
                        16, 
                        //Str(m_rJobState->x_iJob + 1) +
                            //": 
                            "<" + 
                            m_rJobState->x_rTask->
                                x_sModalityKey +
                            ">");
        }

        if (m_rJobState->x_sForcedJobDecision != "")
        {
            sTip = 
                RichBold(m_rJobState->x_sForcedJobDecision) + 
                    "\n\n" + sTip;
        }

        rUi->SetTip(sTip);

        rUi->SetAlignContentToTop();

        rUi->SetPadX(0);

        rUi->SetMenuArrowSideward();

        // hide menu arrow
        rUi->SetSkipDrawForeground(true);
        //rUi->_internal_TestExperiment("SkipDrawForeground", Str(1), NULL);
        //rUi->SetSelect(rUi->IsFocused());

        rUi->OnClick = [=]
        {
            CAppGp::GGet()->x_rUi->ShowWaitingMousePointer();

            if (!m_rJobState->
                    HandleJobClick() 
                ||
                m_rJobState->
                    IsJobInProgress()
                ||
                sys::GIsAccess(...)
                ||
                m_rJobState->
                    _x_bJobClosed)
            {
                ref<UiContainer> rUiFrame = 
                    m_rJobState->
                        NewUiJobStatePopup();

                rUiFrame->CreateMenuAnchoredTo(this);
            }

            rUi->InvalidateContent();
        }

        rUi->OnContext = [=]
        {
            if (sys::GIsAccess(...))
            {
                sys::GOpenTextInWordpad(
                    m_rJobState->ExposeOut().ExportAsText());
            }
        }

        //
        // Direction/Closed Icon
        //
        {
            ref<CUiStaticGp> rUi;

            EqualizeRowStatic(rUi);

            rUi->SetIcon(
                //m_rJobState->x_nRepliesToJob > 0?
                //    RES__REFRESH_S()
                //:
                m_rJobState->_x_bJobClosed?
                    RES__LAVBLK_DIALOG_BACK()
                    :
                m_rJobState->x_bJobDeleted?
                    RES__LIST_DELETE()
                    :
                m_rJobState->x_bUploadJob && 
                    m_rJobState->x_bDownloadJob?
                    RES__REFRESH_S()
                    :
                m_rJobState->x_bUploadJob?
                    RES__GO_BACK_S()
                    :
                m_rJobState->x_bDownloadJob?
                    RES__GO_NEXT_S()
                    :
                RES__BALL_M());

            rUi->SetIconSize(size(12, 12));

            rUi->SetMarginAtLeft(3);
        }

        //
        // Type Icon
        //
        {
            ref<CUiStaticGp> rUi;

            EqualizeRowStatic(rUi);

            rUi->SetIcon(
                rTask->x_sAlarm == 
                    CTaskIface::C_sAlarmIsPairing?
                (
                    rTask->x_sAuthDevice == ""?
                        RES__LOCK_L()
                        :
                        RES__MONITORING_HOME_MAIN_S()
                )
                :
                rTask->x_sAlarm == 
                    CTaskIface::C_sAlarmIsOnlineSnapshot?
                    RES__SIGNAL_S()
                :
                rTask->x_sTaskType ==
                    CTaskIface::C_sTaskTypeIsCheckup?
                (
                    m_rJobState->x_nRepliesToJob > 0?
                        RES__DOCTORS_S()
                        :
                        RES__DOCTOR_S()
                )
                :
                RES__ALARM_BELL_S());

            rUi->SetIconSize(size(16, 16));

            rUi->SetMarginAtLeft(2);
            rUi->SetMarginAtRight(2);
        }

        //
        // Error
        //
        {
            ref<CUiStaticGp> rUi;

            EqualizeRowStaticCompressable(rUi);

            rUi->SetText(m_rJobState->x_slocJobErrorVeryShort);

            rUi->SetTextColor(rgb(255, 0, 0));
        }

        //
        // Alarm
        //
        {
            ref<CUiStaticGp> rUi;

            EqualizeRowStaticCompressable(rUi);

            rUi->SetText(
                rTask->GetNiceAlarmOrClassCompatctForUi());
        }

        /*
        //
        // Class
        //
        {
            see GetNiceAlarmOrClassCompatctForUi()
        }
        */

        //
        // Why
        //
        {
            ref<CUiStaticGp> rUi;

            EqualizeRowStaticCompressable(rUi);

            rUi->SetText(
                rTask->x_sWhy);
        }

        //
        // Patient
        //
        {
            ref<CUiStaticGp> rUi;

            EqualizeRowStaticCompressable(rUi);

            rUi->SetText(
                rTask->x_sPatientName);

            rUi->SetFontItalic(true);
        }

        //if (typeDirection != type<CJobDirectionIsOut>())
        if (!m_rJobState->x_bUploadJob)
        {
            //
            // From
            //
            {
                ref<CUiStaticGp> rUi;

                EqualizeRowStaticCompressable(rUi);

                rUi->SetText(
                    rUserList->
                        LookupUserNameForUi(
                            rTask->x_keyFrom));

                rUi->SetFontItalic(true);
            }
        }

        //if (typeDirection != type<CJobDirectionIsIn>())
        if (!m_rJobState->x_bDownloadJob)
        {
            //
            // To
            //
            {
                ref<CUiStaticGp> rUi;

                EqualizeRowStaticCompressable(rUi);

                rUi->SetText(
                    rUserList->
                        LookupUserNameForUi(
                            rTask->x_keyTo));

                rUi->SetFontItalic(true);
            }
        }

        //
        // right align for the last icon
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetMaxX(oo);
        }

        //
        // Readiness Icon
        //
        {
            ref<CUiStaticGp> rUi;

            EqualizeRowStatic(rUi);

            rUi->SetIcon(
                m_rJobState->LoadSampleKey() != ""?
                    RES__NEXT_ICON()
                :
                m_rJobState->IsJobInProgress()?
                    (res) RES_WAIT
                    :
                    RES__OK_ICON());

            rUi->SetIconSize(size(16, 16));

            rUi->SetMarginAtLeft(3);
            rUi->SetMarginAtRight(3);
            //rUi->SetMarginAtTop(3);
        }

        NL();

        //
        // indent
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(" ");

            rUi->SetPreferredY(0);
            rUi->SetMinY(0);
            rUi->SetMaxY(0);
        }

        //
        // indented extra lines
        //
        {
            ref<CUiPanelGp> rUi;

            // dont go outside external box
            rUi->SetMinX(30);
            rUi->SetPreferredX(0);
            rUi->SetMaxX(oo);

            //
            // Net Status / Progress
            //
            {
                ref<UiContainer> rUi = 
                    m_rJobState->
                        NewUiJobStateNetworkRow();
            }
        }
    }
}

void UiJobStateRow::EqualizeRowStatic(
        ref<CUiActionGp> rUi)
{
    rUi->SetMouseInsensitive(true);
    rUi->SetMouseCursorTransparentLayer(true);
    rUi->SetMarginAtLeft(0);
    rUi->SetMarginAtRight(0);
    //rUi->SetPadX(0);
}

void UiJobStateRow::EqualizeRowStaticCompressable(
        ref<CUiActionGp> rUi)
{
    EqualizeRowStatic(
        rUi);

    rUi->SetMinX(0);
}

