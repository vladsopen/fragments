// UiFakeMemorySelectionAttributes.cpp

#include "CProject.h"
#include "CFakeTomographMemory.h"
#include "VirtOs_LegacyResource.h"
#include "CTomogramIface.h"

void UiFakeMemorySelectionAttributes::Ui()
{
    ref<CTomogramIface> rTomogram = 
        m_rFakeTomographMemory->GetTomogramAtOr(
            m_rFakeTomographMemory->x_iEditTomogram,
            ref<CTomogramIface>());

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rFakeTomographMemory->_m_pUiFakeMemorySelectionAttributes = rUi;

        rUi->SetPadX(2);
    }

    //
    // Sample Position
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetPadY(2);
        rUi->SetAlignContentToCenterX();

        //
        // #
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                TRANSLATE(
                    "Sample[... UI-string-removed]");
        }

        //
        // Input #
        //
        {
            ref<CUiEditGp> rUi;

            rUi->SetText(Str(m_rFakeTomographMemory->x_iEditTomogram));

            rUi->SetTip(
                TRANSLATE(
                    "Change the[... UI-string-removed]");

            rUi->SetPreferredX(scale(40));

            rUi->OnChange = [=]
            {
                int iNewPos = 
                    rUi->InputInt(
                        0,
                        m_rFakeTomographMemory->GetTomogramCount(),
                        m_rFakeTomographMemory->x_iEditTomogram);

                m_rFakeTomographMemory->
                    ReceiveNewPositionForTomogram(
                        rTomogram,
                        iNewPos);
            }
        }

        { ref<CUiSpinGp> rUi; }

        NL();

        //
        // Memory Size Label
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                TRANSLATE(
                    "of:[... UI-string-removed]");

            rUi->SetFontSize(18);
        }

        //
        // Input Memory Size
        //
        {
            ref<CUiEditGp> rUi;

            rUi->SetText(Str(m_rFakeTomographMemory->GetTomogramCount()));

            rUi->SetTip(
                TRANSLATE(
                    "Set flash [... UI-string-removed]");

            rUi->SetPreferredX(scale(40));

            rUi->SetFontSize(18);

            rUi->OnChange = [=]
            {
                int nNewSize = 
                    rUi->InputInt(
                        1,
                        oo,
                        oo);

                if (nNewSize < oo)
                {
                    m_rFakeTomographMemory->SetTomogramCount(nNewSize);
                }
            }
        }

        { ref<CUiSpinGp> rUi; }

        //
        // Memory Size Units
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                TRANSLATE(
                    "pcs[... UI-string-removed]");

            rUi->SetFontSize(18);
        }
    }

    //
    // Non-empry Sample has some settings visible
    //
    {
        ref<UiContainer> rUi = 
            m_rFakeTomographMemory->NewUiFakeMemorySampleSettings();
    }
}
