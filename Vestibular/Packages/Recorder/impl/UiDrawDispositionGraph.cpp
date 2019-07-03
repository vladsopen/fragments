// UiDrawDispositionGraph.cpp

#include "CProject.h"
#include "CDrawDisposition.h"
#include "CSkinizerCommonIfaceGp.h"
#include "CRulersForRoutes.h"
#include "CColorizerCommonIfaceGp.h"

void UiDrawDispositionGraph::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rDrawDisposition->_m_ptrmapUiDrawDispositionGraph += rUi;
    }

    //
    // Square Parent
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->OnPaint = [=]
        {
            draw dc = GetOnPaintDc();

            m_rDrawDisposition->            
                DrawDispositionFeet(
                    dc,
                    rUi);
        }

        //
        // Square 
        //
        {
            ref<CUiPanelGp> rUi;

            rUi->OnModifyShape = [=]
            {
                m_rDrawDisposition->            
                    HandleSquareAreaOnModifyShape(
                        rUi,
                        0.1); // numReserveForFeet
            }

            //
            // Top pad to match horizontal Ruler
            //
            {
                ref<CUiPanelGp> rUi;

                rUi->SetAlikeX(_m_alikexHorizontalRuler);
            }

            NL();

            //
            // Vertical Ruler
            //
            {
                ref<CUiPanelGp> rUi;

                rUi->SetAlikeX(_m_alikexVerticalRuler);

                //
                // The Bar
                //
                {
                    ref<UiContainer> rUi = 
                        m_rDrawDisposition->
                            x_rRulersForRoutes->
                                NewUiRulersForRoutesVertical();
                }
            }

            //
            // Graph Border
            //
            {
                ref<CUiPanelGp> rUi;

                int nBorder = 1;
                rUi->SetBorderDepth(nBorder);
                rUi->SetBorderColor(rUi->GetColorizer()->GetBorderAndSeparatorColor());
                rUi->SetBorderStyleToFlat();

                rUi->SetMarginAtLeft(nBorder);
                rUi->SetMarginAtRight(nBorder);
                rUi->SetMarginAtTop(nBorder);
                rUi->SetMarginAtBottom(nBorder);

                //
                // Image
                //
                {
                    ref<CUiPanelGp> rUi;

                    rUi->SetPreferredX(oo);
                    rUi->SetPreferredY(oo);
                    //rUi->SetMinX(off);
                    //rUi->SetMinY(off);
                    //rUi->SetMaxX(off);
                    //rUi->SetMaxY(off);

                    rUi->OnSize = [=]
                    {
                        m_rDrawDisposition->            
                            x_sizeLastGraphBack = 
                                rUi->GetInstantSize();
                    }

                    rUi->OnPaint = [=]
                    {
                        draw dc = GetOnPaintDc();

                        m_rDrawDisposition->            
                            DrawDispositionGraph(
                                dc,
                                rUi);
                    }
                }
            }

            //
            // Right pad to match vertical Ruler
            //
            {
                ref<CUiPanelGp> rUi;

                rUi->SetAlikeX(_m_alikexVerticalRuler);
            }

            NL();

            //
            // a pad under vertical ruler
            //
            {
                ref<CUiPanelGp> rUi;

                rUi->SetAlikeX(_m_alikexVerticalRuler)
            }

            //
            // Horizontal Ruler
            //
            {
                ref<CUiPanelGp> rUi;

                rUi->SetAlikeX(_m_alikexHorizontalRuler);

                //
                // The Bar
                //
                {
                    ref<UiContainer> rUi = 
                        m_rDrawDisposition->
                            x_rRulersForRoutes->
                                NewUiRulersForRoutesHorizontal();
                }
            }

            //
            // a pad to the size of vertical ruler pad
            //
            {
                ref<CUiPanelGp> rUi;

                rUi->SetAlikeX(_m_alikexVerticalRuler)
            }

        }
    }
}
