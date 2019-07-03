// UiAppEventFloater.cpp

#include "CGuiPlusProject.h"
#include "VirtOs_LegacyResource.h"
#include "CAppEventFloaterImpl.h"
#include "CAppEventImpl.h"
#include "VirtOs_ask.h"
#include "CSkinizerCommonIfaceGp.h"

// *** stop adding stuff here! time to decomposite ***

void UiAppEventFloater::Ui()
{
    int nEvents = 
        m_rAppEventFloater->
            GetSavedTotalAppEventCount();

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        if (m_bEmbedded)
        {
            m_rAppEventFloater->_m_ptrmapUiMainEmbedded += rUi;
        }
        else
        {
            m_rAppEventFloater->_m_ptrmapUiMainFloater += rUi;
        }

        if (!debug("Floater.DontSetImmuneToCriticalModals?").InputBool())
        {
            // A hack to allow us to move it out of the way when it activates
            //   itself overlaying a critical modal window.
            // As the floater does not create any modals (except save dialog)
            //   from which it should be locked out it may be safe:
            // (this won't work for system boxes with a message loop 
            //   - LockFui, MS boxes, ask)
            rUi->SetImmuneToCriticalModals(true);
        }
    }

    //
    // Outer Back
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetBackColor(m_rAppEventFloater->GetFloaterBackColor());
        rUi->SetBackAlpha(
            percent(20));
            //percent(30));
            //percent(65));

        if (!m_bEmbedded)
        {
            //
            // Floater Window Caption
            //
            {
                ref<CUiCaptionGp> rUi;

                //
                // Windows
                //
                {
                    ref<CUiStaticGp> rUi;

                    m_rAppEventFloater->_m_ptrmapUiAppEventFloaterCaptionText += rUi;

                    rUi->SetText(
                        m_rAppEventFloater->x_slocFloaterWindowCaption +
                            (nEvents && m_rAppEventFloater->x_bMinimizeContent?
                                " [" +
                                RichBold(Str(nEvents)) +
                                "]"
                                :
                                ""));

                    rUi->SetIcon(
                        m_rAppEventFloater->x_bMinimizeContent?
                            (res) RES_ipWARN
                            :
                            null());
                }

                //
                // Padding
                //
                {
                    ref<CUiStaticGp> rUi;

                    rUi->SetMaxX(oo);
                }

                //if (!m_rAppEventFloater->x_bExclusiveModalEventFloater)
                if (m_rAppEventFloater->x_bEnableEndUserCollapseButton)
                {
                    //
                    // Hide Button
                    //
                    {
                        ref<CUiButtonGp> rUi;

                        rUi->SetText(
                            TRANSLATE(
                                "Shrink/Exp[... UI-string-removed]");
                        rUi->SetIcon((res) RES_WIN_MIN);

                        rUi->SetCaptionPart(false);
                        rUi->SetAlignInParentToRight();

                        rUi->OnClick = [=]
                        {
                            m_rAppEventFloater->x_bMinimizeContent =
                                !m_rAppEventFloater->x_bMinimizeContent;
                        }
                    }
                }

                if (m_rAppEventFloater->x_bEnableEndUserCloseButton
                    ||
                    m_rAppEventFloater->x_richStopPrompt != "")
                {
                    { ref<CUiSeparateColumnsGp> rUi; }

                    if (m_rAppEventFloater->x_richStopPrompt != "")
                    {
                        //
                        // Custom Close
                        //
                        {
                            ref<CUiButtonGp> rUi;

                            rUi->GetSkinizer()->EqualizeCaptionCloseButton(rUi);

                            rUi->OnClick = [=]
                            {
                                if (m_rAppEventFloater->HandleAllowStop())
                                {
                                    rUi->CloseFrame();
                                }
                            }
                        }
                    }
                    else
                    {
                        //
                        // Close
                        //
                        {
                            ref<CUiCloseGp> rUi;

                            rUi->OnClick = [=]
                            {
                                rUi->CloseFrame();
                            }
                        }
                    }
                }
                else
                if (debug::GIsAllowed...())
                {
                    { ref<CUiSeparateColumnsGp> rUi; }

                    //
                    // Forced Close
                    //
                    {
                        ref<CUiCloseGp> rUi;

                        rUi->SetBackColor(rgb(255, 0, 255));
                        rUi->SetIcon((res) RES_Debug);

                        rUi->OnClick = [=]
                        {
                            rUi->CloseFrame();
                        }
                    }
                }
            }
        }

        NL();

        if (m_rAppEventFloater->x_bMinimizeContent)
        {
            ASSERT_SOFT(!m_bEmbedded); // what then?
            return;
        }

        //
        // Client Area Back
        //
        {
            ref<CUiPanelGp> rUi;

            m_rAppEventFloater->_m_ptrmapUiAppEventFloaterClient += rUi;

            rUi->SetDialogRootPanel(1.0);

            //
            // large message used to kill the window
            //
            {
                ref<CUiScrollablePanelGp> rUi;

                //
                // Info Text and error count
                //
                {
                    ref<CUiStaticGp> rUi;

                    m_rAppEventFloater->_m_ptrmapUiMainAppEventText += rUi;

                    str sMainInfo = 
                        m_rAppEventFloater->GetMainDescriptionText();

                    str sCounterInfo;
                    if (nEvents)
                    {
                        sCounterInfo +=
                            "(" +
                                TRANSLATE(
                                    "Tota[... UI-string-removed]");

                        sCounterInfo =
                            "\v\n\v\n" +
                                RichAlignRight(RichSize(18, sCounterInfo));
                    }

                    rUi->SetText(
                         sMainInfo +
                            sCounterInfo);

                    rUi->SetIcon(
                        nEvents?
                            m_rAppEventFloater->_x_resMainLargeFloaterIcon
                            :
                            res());
                    //rUi->SetFontSize(25);

                    rUi->SetPadX(scale(30));
                    rUi->SetMarginAtLeft(scale(25));
                    rUi->SetMarginAtRight(scale(30));

                    rUi->SetWrapText(true);
                    rUi->SetMinX(10);
                    rUi->SetPreferredX(scale(600));
                    rUi->SetMaxX(oo);

                    // Skin!
                    //rUi->SetTextColor(m_rAppEventFloater->GetFloaterTextColor());
                }
            }

            NL();

            int nProgressIndent = 25;

            //
            // indent for progress
            //
            {
                ref<CUiStaticGp> rUi;

                rUi->SetMinX(nProgressIndent);
            }

            //
            // Progressbar
            //
            {
                ref<CUiProgressGp> rUi;

                m_rAppEventFloater->_m_ptrmapUiMainAppEventProgress += rUi;

                //rUi->SetMaxX(oo);
        //debug:
                //rUi->SetLegacyClusterCount(35);
        //debug.

                rUi->SetTotal(
                    m_rAppEventFloater->x_nnProgressTotal);
                rUi->SetDone(
                    m_rAppEventFloater->x_nnProgressDone);

                rUi->SetMaxX(oo);

                //rUi->SetTimerInTicks(50);

                rUi->OnTimer = [=]
                {
                    //rUi->InvalidateState();
                }
            }

            //
            // pad
            //
            {
                ref<CUiStaticGp> rUi;

                rUi->SetMinX(nProgressIndent);
                //rUi->SetMaxX(oo);
            }

            NL();

            //
            // Event list controls
            //
            {
                ref<CUiPanelGp> rUi;

                rUi->SetHidden(nEvents == 0);

                //
                // Export Button
                //
                {
                    ref<CUiButtonGp> rUi;

                    rUi->SetText(
                        TRANSLATE(
                            "Save Repor[... UI-string-removed]");
                    rUi->SetIcon((res) RES_FILE_SAVE_SM);

                    rUi->SetAlignInParentToRight();
                    rUi->SetWindowAlpha(percent(80));

                    rUi->OnClick = [=]
                    {
                        m_rAppEventFloater->ShowExportedEventReport();
                    }
                }

                //
                // Clear Button
                //
                {
                    ref<CUiButtonGp> rUi;

                    //rUi->SetText()
                    rUi->SetIcon((res) RES_KILL_ALL_SM);

                    rUi->SetAlignInParentToRight();
                    rUi->SetWindowAlpha(percent(60));
                    rUi->SetTip(
                        TRANSLATE(
                            "Clear list[... UI-string-removed]");

                    rUi->OnClick = [=]
                    {
                        if (ask(
                            TRANSLATE(
                                "Clear list[... UI-string-removed]");
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

                rUi->SetMinY(0);
                rUi->SetMaxY(
                    nEvents? (pix) scale(350) : (pix) 0);

                //
                // List Scroll
                //
                {
                    ref<CUiScrollablePanelGp> rUi;

                    //
                    // List Content
                    //
                    {
                        ref<CUiPanelGp> rUi;

                        m_rAppEventFloater->_m_ptrmapUiAppEventList += rUi;

                        rUi->SetPadY(1);
                        rUi->SetBorderStyleToSunken();
                        rUi->SetBorderDepth(2);
                        rUi->SetBackColor(rgb(255, 253, 253));
                        rUi->SetBackAlpha(percent(95));
                        rUi->SetMarginAtLeft(scale(20));
                        rUi->SetMarginAtRight(3);
                        rUi->SetMarginAtTop(3);
                        rUi->SetMarginAtBottom(3);

                        // List all errors
                        if (m_rAppEventFloater->GetListedEventCount() > 1)
                        {
                            ref<CAppEventImpl> rIterAppEvent;
                            for (iter i;
                                m_rAppEventFloater->
                                    IterateEvents(
                                        out i,
                                        out rIterAppEvent);)
                            {
                                //
                                // Next Event
                                //
                                {
                                    ref<UiContainer> rUi =
                                        rIterAppEvent->NewUiAppEventInfo(
                                            i, // iEvent
                                            m_rAppEventFloater);
                                }

                                NL();
                            }

                            NL();

                            //
                            // End of List to scroll to
                            //
                            {
                                ref<CUiPanelGp> rUi;

                                m_rAppEventFloater->_m_ptrmapUiAppEventListEnd += rUi;
                            }
                        }
                    }
                }
            }

            if (m_rAppEventFloater->x_bEnableEndUserCloseButton
                &&
                m_rAppEventFloater->x_richStopPrompt == "")
            {
                NL();

                //
                // Friendly Close
                //
                {
                    ref<CUiDialogCloseGp> rUi;

                    rUi->OnClick = [=]
                    {
                        rUi->CloseFrame();
                    }
                }
            }
        }
    }
}
