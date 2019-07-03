// UiDownloaderList.cpp

#include "CProject.h"
#include "CDownloaderImpl.h"
#include "CBodySchemeIface.h"
#include "CGlobalConfigIface.h"
#include "VirtOs_LegacyResource.h"
#include "CWizardStepType.h"
#include "CSkinizerCommonIfaceGp.h"
#include "CSkinizerCommonIface.h"

void UiDownloaderList::Ui()
{
    ref<CBodySchemeIface> rBodyScheme = 
        m_rDownloader->GetGlobalConfig()->x_rBodyScheme;

    str sProfileName = 
        rBodyScheme->GetCurrentBodyProfileNameForIndication();

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rDownloader->_m_pUiDownloaderList = rUi;

        rUi->SetDialogRootPanel(1.5);
        rUi->SetMarginAtLeft(rUi->GetMarginAtLeft() * 2);

        // Current profile autoupdate
        rUi->SetTimerInTicks(100);
        rUi->OnTimer = [=]
        {
            // do we need to update for the new selection?
            if (m_sRecentProfileName != sProfileName)
            {
                m_sRecentProfileName = sProfileName;
                rUi->InvalidateContent();
            }
        }
    }

    //
    // Profile Caption
    //
    {
        ref<CUiPanelGp> rUi;

        //
        // Just an icon
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetAlignContentToCenterX();
            rUi->SetAlignInParentToCenterY();

            rUi->SetIcon((res) RES_SAMPLER_TOMO);

            rUi->SetMarginAtLeft(scale(5));
            rUi->SetMarginAtRight(scale(5));
        }

        //
        // Profile Menu
        //
        {
            ref<CUiPanelGp> rUi;

            rUi->SetMinX(off);


            //
            // Prompt
            //
            {
                ref<CUiStaticGp> rUi;

                rUi->SetText(
                    TRANSLATE(
                        "Current Pr[... UI-string-removed]");

                rUi->SetFontBold(true);
            }

            NL();

            //
            // Indent
            //
            {
                ref<CUiStaticGp> rUi;

                rUi->SetText("  ");
            }

            if (m_rDownloader->
                    x_typeWizardStepType->
                        IsProfileChangeEnabled())
            {
                //
                // Profile Selector
                //
                {
                    ref<UiContainer> rUi = 
                        rBodyScheme->
                            NewUiBodySchemeProfileSelector(
                                m_rDownloader);
                }
            }
            else
            {
                //
                // Profile Name Indicator
                //
                {
                    ref<CUiStaticGp> rUi;

                    rUi->SetText(sProfileName);

                    rUi->SetFontSize(25);
                    rUi->SetFontBold(true);
                    rUi->SetTextColor(CSkinizerCommonIface::C_rgbLargeText);
                }
            }
        }
    }

    NL();

    //
    // List Back
    //
    {
        ref<CUiPanelGp> rUi;

        //
        // Point List
        //
        {
            ref<CUiScrollablePanelGp> rUi;

            // Standard-List: Border and Background
            rUi->GetSkinizer()->EqualizeListScrollablePanel(rUi);

            rUi->GetScrollClip()->
                SetBackAlphaByte(
                    //alpha(debug("Skin.ListBackAlpha?").InputInt()));
                    CSkinizerCommonIface::C_alphaListBack);

            // Tomogram or Point list
            {
                int nTomogramCount = m_rDownloader->GetTomogramCount();
                int nBodyPointCount = rBodyScheme->GetCurrentBodyProfilePointCount();
                repeat(i, math::GMax(nTomogramCount, nBodyPointCount))
                {
                    //
                    // Point or Tomogram Row
                    //
                    {
                        ref<UiContainer> rUi = 
                            m_rDownloader->
                                NewUiDownloaderPointRow(
                                    i);
                    }

                    NL();
                }
            }
        }
    }
}
