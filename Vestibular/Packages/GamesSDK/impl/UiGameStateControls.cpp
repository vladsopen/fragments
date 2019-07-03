// UiGameStateControls.cpp

#include "CProject.h"
#include "CGameStateImpl.h"
#include "CGamesImpl.h"
#include "CSessionIface.h"
#include "VirtOs_ask.h"
#include "CGlobalConfigIface.h"
#include "COptionsIface.h"

void UiGameStateControls::Ui()
{
    ref<CGamesImpl> rGames = 
        m_rGameState->_x_pGames.Get().Valid();
    ref<CSessionIface> rSession = 
        rGames->_x_pSession.Get().Valid();

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rGameState->_m_ptrmapUiGameStateControls += rUi;

        rUi->SetMaxX(off);
        rUi->SetMaxY(off);

        rUi->SetDialogRootPanel();
        rUi->SetDialogSubPanel();
        rUi->SetAlignContentToCenterX();
    }

    //
    // [Zero]
    //
    {
        ref<CUiButtonGp> rUi;

        EqualizeGameButton(rUi);

        rUi->SetText(
            TRANSLATE(
                "Zero[... UI-string-removed]");

        rUi->SetIcon(
            RES__WIN_MIN_RIGHT_L());

        rUi->SetTip(
            TRANSLATE(
                "Reset to z[... UI-string-removed]");

        rUi->OnClick = [=]
        {
            rGames->HandleZeroButton();
        }
    }

    //
    // [Options]
    //
    {
        ref<CUiButtonGp> rUi;

        EqualizeGameButton(rUi);

        rUi->SetText(
            TRANSLATE(
                "Option[... UI-string-removed]");

        rUi->SetTip(
            TRANSLATE(
                "Program se[... UI-string-removed]");

        rUi->SetIcon(
            RES__OPTIONS_32x32());

        //rUi->SetIconLocationAtTop();

        rUi->OnClick = [=]
        {
            ref<COptionsIface>()->
                OpenUiOptionsDialog(
                    m_rGameState);
        }
    }

    //
    // [Close]
    //
    {
        ref<CUiButtonGp> rUi;

        EqualizeGameButton(rUi);

        rUi->SetText(
            TRANSLATE(
                "Clos[... UI-string-removed]");

        rUi->SetIcon(
            RES__OK_ICON());

        rUi->SetTip(
            TRANSLATE(
                "End the tr[... UI-string-removed]");

        rUi->OnClick = [=]
        {
            m_rGameState->CloseGameState();
        }
    }

    NL();

    //
    // Secret Back
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetPreferredX(0);
        rUi->SetMaxX(oo);

        //
        // Secret Bar
        //
        {
            ref<UiContainer> rUi =
                rSession->
                    NewUiSessionSecretBar();
        }
    }
}

void UiGameStateControls::EqualizeGameButton(
        ref<CUiActionGp> rUi)
{
    rUi->SetMaxY(oo);
    rUi->SetAlikeY(_m_alikeyGameButton);
}

