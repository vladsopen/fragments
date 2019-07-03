// UiUmoveTestSliderWrapper.cpp

#include "CProject.h"
#include "CUmoveTest.h"
#include "CSkinizerCommonIfaceGp.h"
#include "CDemoVehicle.h"
#include "CDemoVehicleList.h"
#include "CUmoveIfaceGp.h"
#include "CColorizerCommonIfaceGp.h"

void UiUmoveTestSliderWrapper::Ui()
{
    ref<CDemoVehicle> rDemoVehicle = 
        m_rUmoveTest->x_rDemoVehicleList->GetGuineaPigVehicle();

    // current position of the main vehicle
    point pointVirtual = 
        rDemoVehicle->x_rUmove->x_pointLogicalVehiclePos;

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rUmoveTest->_m_ptrmapUiUmoveTestSliderWrapper += rUi;
    }

    //
    // Slider-style Additional External Controls around Umove parent
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetAlignContentToCenterX();

        if (!m_rUmoveTest->x_bGenerateHorizontalSlider)
        {
            //
            // Top Slider Button
            //
            {
                ref<CUiButtonGp> rUi;

                //rUi->SetText("+");
                rUi->SetIcon(RES__ARROW_TINY_UP());

                if (m_rUmoveTest->x_bGenerateVerticalSlider)
                {
                    rUi->SetMaxX(oo);
                }

                if (pointVirtual.y <= 0)
                {
                    rUi->SetDisableInfo("At the top");
                }

                rUi->OnClick = [=]
                {
                    pointVirtual.y--;
                    rDemoVehicle->x_rUmove->x_pointLogicalVehiclePos =
                        pointVirtual;

                    m_rUmoveTest->UpdateForNewVehiclePositions();
                }
            }
        }

        NL();

        if (!m_rUmoveTest->x_bGenerateVerticalSlider)
        {
            //
            // Left Slider Button
            //
            {
                ref<CUiButtonGp> rUi;

                EqualizeSliderPlusMinus(rUi);

                rUi->SetText("-");
                //rUi->SetIcon(RES__ARROW_TINY_LEFT());

                if (m_rUmoveTest->x_bGenerateHorizontalSlider)
                {
                    rUi->SetMaxY(oo);
                }

                if (pointVirtual.x <= 0)
                {
                    rUi->SetDisableInfo("At the leftmost");
                }

                rUi->OnClick = [=]
                {
                    pointVirtual.x--;
                    rDemoVehicle->x_rUmove->x_pointLogicalVehiclePos =
                        pointVirtual;

                    m_rUmoveTest->UpdateForNewVehiclePositions();
                }
            }
        }

        //
        // Umove Parent Zone where stuff moves around
        //
        {
            ref<UiContainer> rUi = 
                m_rUmoveTest->NewUiUmoveTestMainArena();
        }

        if (!m_rUmoveTest->x_bGenerateVerticalSlider)
        {
            //
            // Right Slider Button
            //
            {
                ref<CUiButtonGp> rUi;

                EqualizeSliderPlusMinus(rUi);

                rUi->SetText("+");
                //rUi->SetIcon(RES__ARROW_TINY_RIGHT());

                if (m_rUmoveTest->x_bGenerateHorizontalSlider)
                {
                    rUi->SetMaxY(oo);
                }

                if (pointVirtual.x >= 
                        rDemoVehicle->x_rUmove->
                            x_sizeLogicalUmoveRange.Get().w - 1)
                {
                    rUi->SetDisableInfo("At the rightmost");
                }

                rUi->OnClick = [=]
                {
                    pointVirtual.x++;
                    rDemoVehicle->x_rUmove->x_pointLogicalVehiclePos =
                        pointVirtual;

                    m_rUmoveTest->UpdateForNewVehiclePositions();
                }
            }
        }

        NL();

        if (!m_rUmoveTest->x_bGenerateHorizontalSlider)
        {
            //
            // Bottom Slider Button
            //
            {
                ref<CUiButtonGp> rUi;

                //rUi->SetText("-");
                rUi->SetIcon(RES__ARROW_TINY_DOWN());

                if (m_rUmoveTest->x_bGenerateVerticalSlider)
                {
                    rUi->SetMaxX(oo);
                }

                if (pointVirtual.y >= 
                        rDemoVehicle->x_rUmove->
                            x_sizeLogicalUmoveRange.Get().h - 1)
                {
                    rUi->SetDisableInfo("At the bottom");
                }

                rUi->OnClick = [=]
                {
                    pointVirtual.y++;
                    rDemoVehicle->x_rUmove->x_pointLogicalVehiclePos =
                        pointVirtual;

                    m_rUmoveTest->UpdateForNewVehiclePositions();
                }
            }
        }
    }
}

void UiUmoveTestSliderWrapper::EqualizeSliderPlusMinus(
        ref<CUiButtonGp> rUi)
{
    // "+"/"-" text
    rUi->SetFontSize(35);
    rUi->SetFontBold(true);

    rUi->SetMinX(off);
    rUi->SetPreferredX(scale(40));
    rUi->SetAlignContentToCenterX();

    rUi->SetMinY(off);

    rUi->SetTextColor(rUi->GetColorizer()->GetButtonShadowAndSoftTextColor());
}

