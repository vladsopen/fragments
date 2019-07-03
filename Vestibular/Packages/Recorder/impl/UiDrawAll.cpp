// UiDrawAll.cpp

#include "CProject.h"
#include "CDrawAll.h"
#include "CRecorderImpl.h"
#include "CDrawRoute.h"
#include "CDrawVelocity.h"
#include "CDrawTable.h"
#include "CDrawDisposition.h"
#include "CDrawFreq.h"
#include "CDrawCoords.h"
#include "CSessionIface.h"
#include "CPatientLinkIface.h"
#include "CPatientSessionIface.h"

void UiDrawAll::Ui()
{
    ref<CRecorderImpl> rRecorder = 
        m_rDrawAll->_x_pRecorder.Get().Valid();
    ref<CSessionIface> rSession =
        rRecorder->_x_pSession.Get().Valid();

    scale scalePad = scale(10);

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rDrawAll->_m_ptrmapUiDrawAll += rUi;

        rUi->SetPadX(scalePad);
        rUi->SetPadY(scalePad);

        rUi->SetMarginAtTop(5);
    }

    //
    // Left
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetAlignContentToCenterY();

        rUi->SetPadX(scalePad);
        rUi->SetPadY(scalePad);

        rUi->SetPreferredX(0);
        rUi->SetMaxX(oo);

        //
        // Disposition Back
        //
        {
            ref<CUiPanelGp> rUi;

            EqualizeRouteGraphSizes(rUi);

            //
            // Disposition
            //
            {
                ref<UiContainer> rUi =
                    rRecorder->
                        x_rDrawDisposition->
                            NewUiDrawDispositionGraph();
            }
        }

        NL();

        //
        // pad
        //
        {
            ref<CUiStaticGp> rUi;
        }

        NL();

        //
        // Route Back
        //
        {
            ref<CUiPanelGp> rUi;

            EqualizeRouteGraphSizes(rUi);

            //
            // Route
            //
            {
                ref<UiContainer> rUi =
                    rRecorder->
                        x_rDrawRoute->
                            NewUiDrawMainGraph();
            }
        }
    }

    //
    // Right
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetAlignContentToCenterY();

        rUi->SetPadX(scalePad);
        rUi->SetPadY(scalePad);

        rUi->SetPreferredX(0);
        rUi->SetMaxX(oo);

        if (!rSession->
                x_rPatientSession->
                    x_bSessionForPatientPicture)
        {
            //
            // Scroll Back
            //
            {
                ref<CUiPanelGp> rUi;

                rUi->SetMinY(
                    scale(debug("Table.ScaleY?").InputIntOrSet(200)));

                //
                // Scroll
                //
                {
                    ref<CUiScrollablePanelGp> rUi;

                    //
                    // Table
                    //
                    {
                        ref<UiContainer> rUi =
                            rRecorder->
                                x_rDrawTable->
                                    NewUiDrawMainGraph();
                    }
                }
            }

            NL();

            //
            // pad
            //
            {
                ref<CUiStaticGp> rUi;
            }

            NL();
        }

        //
        // Coordinates
        //
        {
            ref<UiContainer> rUi =
                rRecorder->
                    x_rDrawCoords->
                        NewUiDrawMainGraph();
        }

        NL();

        //
        // Velocity
        //
        {
            ref<UiContainer> rUi =
                rRecorder->
                    x_rDrawVelocity->
                        NewUiDrawMainGraph();
        }

        NL();

        //
        // pad
        //
        {
            ref<CUiStaticGp> rUi;
        }

        NL();

        //
        // Freq
        //
        {
            ref<UiContainer> rUi =
                rRecorder->
                    x_rDrawFreq->
                        NewUiDrawMainGraph();
        }
    }
}

void UiDrawAll::EqualizeRouteGraphSizes(
        ref<CUiPanelGp> rUi)
{
    rUi->SetMinY(10);
    rUi->SetMaxY(oo);
    rUi->SetPreferredY(oo);
}

