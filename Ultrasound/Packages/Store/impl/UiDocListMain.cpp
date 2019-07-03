// UiDocListMain.cpp

#include "CUsmainProject.h"
#include "CDocListImpl.h"
#include "CDocImpl.h"
#include "CDocFormatGp.h"
#include "CMatrixFormatGp.h"
#include "CCustomizeDocIfaceGp.h"

NOTEST;

void UiDocListMain::Ui()
{
    int nColumns = 
        IsPrintOrPreview()?
            m_rDocList->x_nPrintMediaColumns
            :
            1;

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rDocList->_m_pUiDocListMain = rUi;

        // allow main window to stretch to full screen in
        //   an emergency when no docs exist
        rUi->SetMaxX(oo);
        rUi->SetMaxY(oo);

        rUi->SetAlignContentToTop();
    }

    /*
    //
    // Add Media Button
    //
    {
        ref<CUiButtonGp> rUi;

        rUi->SetText("New Recorder");

        rUi->OnClick = [=]
        {
//debug:

//debug.
        }
    }
    */

    //
    // List
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetAlignContentToTop();

        // All our medias
        {
            // debug skip to test extra floater
            if (sys::GIsAccess(...) &&
                sys::GIsAccess(...) &&
                sys::GIsAccess(...))
            {
                NOTEST;

                debug("Doc.List./show.SkipUiUpdate:/4e").
                    FloatNote("Alt-Shift-U skip doc list update");
                return;
            }

            ref<CDocImpl> rIterDoc;
            bool bNeedDeferredNewLine = false;
            int iColumn = 0;
            int nFoundVisible = 0;
            for (
                iter i;
                m_rDocList->
                    IterateDocs(
                        out i,
                        out rIterDoc);)
            {
                //sync: doc_list
                if (!rIterDoc->_x_bDocExcludedFromList &&
                    !rIterDoc->_x_rCustomizeDoc->x_bHideAboveTimeBar)
                {
                    nFoundVisible++;

                    //
                    // Next Media
                    //
                    {
                        ref<UiContainer> rUi = 
                            rIterDoc->
                                NewUiDocMain();
                    }

                    iColumn++;

                    if (iColumn >= nColumns ||
                        bNeedDeferredNewLine)
                    {
                        if (rIterDoc->x_bDocEndsRowInDocList)
                        {
                            NL();

                            iColumn = 0;
                        }
                        else
                        {
                            bNeedDeferredNewLine = true;
                        }
                    }
                }
            }

            if (nFoundVisible == 0)
            {
                //
                // No docs msg
                //
                {
                    ref<CUiStaticGp> rUi;

                    rUi->SetText(
                        "   " +
                            TRANSLATE(
                                "No media w[... UI-string-removed]");

                    rUi->SetTextColor(rgb(100, 100, 180));
                }
            }
        }
    }
}
