// UiSessionStatusMenu.cpp

#include "CProject.h"
#include "CSessionImpl.h"
#include "CStoreIface.h"
#include "CUbroRefineIfaceGp.h"

void UiSessionStatusMenu::Ui()
{
    ref<CStoreIface> rStore = 
        m_rSession->x_rStore;

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rSession->_m_ptrmapUiSessionStatusMenu += rUi;
    }

    //
    // [Dump Journal]
    //
    {
        ref<CUiMenuItemGp> rUi;

        rUi->SetText(
            "Dump Journal");

        rUi->SetTip(
            "Dump CloudSync Journal");

        rUi->OnClick = [=]
        {
            m_rSession->x_rStore->DumpJournal();
        }
    }

    NL();

    //
    // [Main UI]
    //
    {
        ref<CUiMenuItemGp> rUi;

        rUi->SetText(
            "CloudSync Ubrowser");

        rUi->SetTip(
            "CloudSync object manager");

        rUi->OnClick = [=]
        {
            ref<UiContainer> rUiFrame = 
                m_rSession->
                    NewUiSessionFrame(
                        m_rUbroRefine);

            rUiFrame->CreateFloater();
        }
    }

    NL();

    //
    // [x] Flush
    //
    {
        ref<CUiCheckGp> rUi;

        rUi->SetText("Allow Flush Commit");

        rUi->SetTip(
            "An option to temporarily extend Store auto transaction");

        rUi->SetCheck(rStore->x_bEnableFlushCommit);

        rUi->OnClick = [=]
        {
            rStore->x_bEnableFlushCommit ^= 1;
        }
    }
}
