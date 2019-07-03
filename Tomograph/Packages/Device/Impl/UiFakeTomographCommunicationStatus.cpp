// UiFakeTomographCommunicationStatus.cpp

#include "CProject.h"
#include "CFakeTomograph.h"

void UiFakeTomographCommunicationStatus::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rFakeTomograph->_m_pUiFakeTomographCommunicationStatus = rUi;
    }

    //
    // Online Status
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetText(
            m_rFakeTomograph->x_bTomographUsbOpen?
                RichTextColor(rgb(0, 0, 255),
                    TRANSLATE(
                        "Communicat[... UI-string-removed]");

        rUi->SetFontSize(16);
    }

    NL();

    //
    // Queue Status
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetText(
            m_rFakeTomograph->x_iSendingTomogram >= 0?
                TRANSLATE(
                    "Transferin[... UI-string-removed]");

        rUi->SetFontSize(16);
    }

    NL();

    //
    // Random Mode
    //
    {
        ref<CUiCheckGp> rUi;

        rUi->SetText(
            TRANSLATE(
                "Random\n[... UI-string-removed]");

        rUi->SetTip(
            TRANSLATE(
                "In this mo[... UI-string-removed]");

        rUi->SetCheck(m_rFakeTomograph->x_bReplyWithRandomSamples);

        rUi->SetFontSize(16);

        rUi->OnClick = [=]
        {
            m_rFakeTomograph->x_bReplyWithRandomSamples = 
                !m_rFakeTomograph->x_bReplyWithRandomSamples;
        }
    }

    NL();

    //
    // "Delay"
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetText(
            TRANSLATE(
                "Delay:[... UI-string-removed]");

        rUi->SetFontSize(18);
    }

    NL();

    /*
    //
    // Input Delay
    //
    {
        ref<CUiEditGp> rUi;

        rUi->SetText(Str(m_rFakeTomograph->x_tickResponseCallbackDelay));

        rUi->SetTip(
            TRANSLATE(
                "Emulate re[... UI-string-removed]");

        rUi->SetPreferredX(scale(40));

        rUi->SetFontSize(18);

        rUi->OnChange = [=]
        {
            int nNewMilliseconds = 
                rUi->InputInt(
                    0,
                    oo,
                    m_rFakeTomograph->x_tickResponseCallbackDelay);

            m_rFakeTomograph->x_tickResponseCallbackDelay = nNewMilliseconds;
        }
    }

    { ref<CUiSpinGp> rUi; }
    */

    NL();
}
