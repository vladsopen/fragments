// UiTunerProfileListBox.cpp

#include "CDucalcProject.h"
#include "CMediaDocIfaceGp.h" //(dont_uninclude)
#include "CTunerProfileList.h"
#include "CTunerProfile.h"

void UiTunerProfileListBox::Ui()
{
    {
        ref<CUiPanelGp> rUi = this;

        m_rTunerProfileList->_m_pUiTunerProfileListBox = rUi;

        rUi->SetBackColor(rgb(252, 252, 255));
        rUi->SetBackAlpha(percent(99));
        rUi->SetBorderDepth(1);
        rUi->SetBorderStyleToSunken();
        rUi->SetMaxX(oo);
        rUi->SetMaxY(oo);

        int nMargins = 3;
        rUi->SetMarginAtLeft(nMargins);
        rUi->SetMarginAtRight(nMargins);
        rUi->SetMarginAtTop(nMargins);
        rUi->SetMarginAtBottom(nMargins);
    }

    //
    // List
    //
    {
        ref<CUiScrollablePanelGp> rUi;

        rUi->SetPadY(1);

        // List all items
        {
            ref<CTunerProfile> rIterTunerProfile;
            for (iter i; 
                m_rTunerProfileList->Iterate(
                    out i, 
                    out rIterTunerProfile);)
            {
                //
                // Next Profile Name
                //
                {
                    ref<UiContainer> rUi =
                        rIterTunerProfile->NewTunerProfileNameRowUi(
                            m_rTunerProfileList);
                }

                NL();
            }
        }
    }

    // DO NOT PUT ANY UIs in here - access to them would be very slow (see loop above)
}
