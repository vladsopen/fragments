// UiJobListOptionsMenu.cpp

#include "CProject.h"
#include "CJobListImpl.h"
#include "CSessionIface.h"
#include "CLogin.h"
#include "CJobFilterSetup.h"
#include "VirtOs_ask.h"

void UiJobListOptionsMenu::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rJobList->_m_ptrmapUiJobListOptionsMenu += rUi;
    }

    //
    // Filters
    //
    {
        ref<UiContainer> rUi = 
            m_rJobList->
                x_rJobFilterSetup->
                    NewUiJobFilterSetupMenu();
    }
    
    NL();

    { ref<CUiSeparateRowsGp> rUi; }

    NL();

    //
    // [Kill all]
    //
    {
        sloc slocWhat =
            TRANSLATE(
                "Delete all[... UI-string-removed]");

        ref<CUiMenuItemGp> rUi;

        rUi->SetText(
            TRANSLATE(
                "Clea[... UI-string-removed]");

        rUi->SetIcon(RES__DELETE_L());

        rUi->SetTip(
            slocWhat);

        rUi->OnClick = [=]
        {
            if (
                ask(
                    slocWhat + "?").
                    _unsafe_ask_Delete()._unsafe_ask_Cancel() == 
                    ask::Compare()._unsafe_ask_Delete())
            {
                m_rJobList->
                    KillAllJobsToTheDeath();
            }
        }
    }

    NL();

    { ref<CUiSeparateRowsGp> rUi; }

    NL();

    //
    // [Setup]
    //
    {
        ref<CUiMenuItemGp> rUi;

        rUi->SetText(
            TRANSLATE(
                "Authentica[... UI-string-removed]");

        rUi->SetIcon(RES__LOCK_L());

        rUi->SetTip(
            TRANSLATE(
                "User login[... UI-string-removed]");

        rUi->OnClick = [=]
        {
            m_rJobList->
                GetSession()->
                    x_rLogin->
                        ShowBasicDialogWindow();
        }
    }
   
    NL();
}
