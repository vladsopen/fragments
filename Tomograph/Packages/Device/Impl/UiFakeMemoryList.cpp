// UiFakeMemoryList.cpp

#include "CProject.h"
#include "CFakeTomographMemory.h"
#include "CTomogramIface.h"
#include "CFakeTomograph.h"
#include "CTomographEmulator.h"

void UiFakeMemoryList::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rFakeTomographMemory->_m_pUiFakeMemoryList = rUi;

        rUi->SetAlignContentToTop();
    }

    //
    // Left Column
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetAlignContentToTop();
        rUi->SetPadY(2);

        //
        // Sample Selection Status Panel
        //
        {
            ref<UiContainer> rUi = 
                m_rFakeTomograph->x_rFakeTomographMemory->
                    NewUiFakeMemorySelectionAttributes();
        }

        NL();

        /*
        //
        // Caption
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                TRANSLATE(
                    "Test Sampl[... UI-string-removed]");

            rUi->SetFontBold(true);
        }
        */

        //
        // Explorer
        //
        {
            ref<CUiButtonGp> rUi;

            rUi->SetText(
                TRANSLATE(
                    "Explorer",[... UI-string-removed]");

            rUi->SetIcon(RES__EXPLORER_16x16());

            rUi->OnClick = [=]
            {
                m_rFakeTomographMemory->
                    OpenExplorerForFlashFolder(
                        m_rTomographEmulator);
            }
        }
    }

    //
    // Scroller
    //
    {
        ref<CUiScrollablePanelGp> rUi;

        // outer border
        ref<CUiPanelGp> rUiScrollClip = rUi->GetScrollClip();
        rUiScrollClip->SetBorderStyleToSunken();
        rUiScrollClip->SetBorderDepth(2);

        // accentuate scrollable area
        rUiScrollClip->SetBackColor(rgb(255, 255, 255));
        rUiScrollClip->SetBackAlpha(percent(50));

        // the list
        {
            ref<CTomogramIface> rIterTomogram;
            for (iter i; m_rFakeTomographMemory->IterateTomograms(out i, out rIterTomogram);)
            {
                //
                // Sample row
                //
                {
                    ref<UiContainer> rUi = 
                        m_rFakeTomographMemory->
                            NewUiFakeMemoryRow(
                                rIterTomogram);
                }

                NL();
            }
        }
    }
}
