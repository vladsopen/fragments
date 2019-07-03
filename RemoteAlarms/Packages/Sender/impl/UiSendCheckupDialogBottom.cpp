// UiSendCheckupDialogBottom.cpp

#include "CProject.h"
#include "CSendCheckup.h"
#include "CSkinizerCommonIfaceGp.h"
#include "CSenderSessionImpl.h"
#include "CTaskIface.h"

void UiSendCheckupDialogBottom::Ui()
{
    ref<CSenderSessionImpl> rSenderSession = 
        m_rSendCheckup->GetSenderSession();
    ref<CTaskIface> rTask = 
        m_rSendCheckup->x_rCheckupSendingTask;

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rSendCheckup->_m_ptrmapUiSendCheckupDialogBottom += rUi;
    }

    //
    // panel for buttons
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->GetSkinizer()->EqualizeClosePanel(rUi);

        //
        // [Go] button
        //
        {
            ref<CUiDialogOkGp> rUi;

            rUi->SetIdleUpdate(true);

            rUi->GetSkinizer()->EqualizeDialogCloseButton(rUi);

            rUi->SetText(
                TRANSLATE(
                    "Send[... UI-string-removed]");

            rUi->SetTip(
                TRANSLATE(
                    "Start data[... UI-string-removed]");

            rUi->OnClick = [=]
            {
                rUi->CloseFrame();

                m_rSendCheckup->
                    HandleGoSend();
            }
        }

        //
        // [Cancel] button
        //
        {
            ref<CUiDialogCancelGp> rUi;

            rUi->GetSkinizer()->EqualizeDialogCloseButton(rUi);

            rUi->OnClick = [=]
            {
                rUi->CloseFrame();
            }
        }
    }
}
