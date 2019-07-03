// UiUmoveTestDemoSetup.cpp

#include "CProject.h"
#include "CUmoveTest.h"
#include "CDemoVehicle.h"
#include "CDemoVehicleList.h"
#include "CUmoveIfaceGp.h"

void UiUmoveTestDemoSetup::Ui()
{
    ref<CDemoVehicle> rDemoVehicle = 
        m_rUmoveTest->x_rDemoVehicleList->GetGuineaPigVehicle();

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rUmoveTest->_m_ptrmapUiUmoveTestDemoSetup += rUi;

        rUi->SetMaxX(off);
        rUi->SetAlignInParentToTop();

        rUi->SetDialogSubPanel();
    }

    //
    // [Make Horizonal Slider]
    //
    {
        ref<CUiButtonGp> rUi;

        rUi->SetText("Make Horizonal Slider");

        rUi->SetMaxX(oo);

        rUi->OnClick = [=]
        {
            m_rUmoveTest->x_bShowSliderWrapper = true;
            m_rUmoveTest->x_bGenerateHorizontalSlider = true;
            m_rUmoveTest->x_bGenerateVerticalSlider = false;

            rDemoVehicle->x_rUmove->x_sizeLogicalUmoveRange = size(30, 0);
            rDemoVehicle->x_rUmove->x_pointLogicalVehiclePos = point(0, 0);
            rDemoVehicle->x_rUmove->x_sizeVehicle = size(50, 0);
            rDemoVehicle->x_bStretchHeightAsForVerticalMarker = true;
            rDemoVehicle->x_bStretchWidthAsForHorizontalMarker = false;
            rDemoVehicle->x_pointForbiddenPlace = point(off, off);
            rDemoVehicle->x_bUnmovable = false;

            m_rUmoveTest->UpdateAllTestUi();
        }
    }

    NL();

    //
    // [Make Vertical Slider]
    //
    {
        ref<CUiButtonGp> rUi;

        rUi->SetText("Make Vertical Slider");

        rUi->SetMaxX(oo);

        rUi->OnClick = [=]
        {
            m_rUmoveTest->x_bShowSliderWrapper = true;
            m_rUmoveTest->x_bGenerateHorizontalSlider = false;
            m_rUmoveTest->x_bGenerateVerticalSlider = true;

            rDemoVehicle->x_rUmove->x_sizeLogicalUmoveRange = size(0, 30);
            rDemoVehicle->x_rUmove->x_pointLogicalVehiclePos = point(0, 0);
            rDemoVehicle->x_rUmove->x_sizeVehicle = size(0, 50);
            rDemoVehicle->x_bStretchHeightAsForVerticalMarker = false;
            rDemoVehicle->x_bStretchWidthAsForHorizontalMarker = true;
            rDemoVehicle->x_pointForbiddenPlace = point(off, off);
            rDemoVehicle->x_bUnmovable = false;

            m_rUmoveTest->UpdateAllTestUi();
        }
    }

    NL();

    //
    // [Reset]
    //
    {
        ref<CUiButtonGp> rUi;

        rUi->SetText("Reset to 2D Grid");

        rUi->SetMaxX(oo);

        rUi->OnClick = [=]
        {
            m_rUmoveTest->x_rDemoVehicleList->RenderDemoVehicles();

            m_rUmoveTest->x_bShowSliderWrapper = true;
            m_rUmoveTest->x_bGenerateHorizontalSlider = false;
            m_rUmoveTest->x_bGenerateVerticalSlider = false;

            m_rUmoveTest->UpdateAllTestUi();
        }
    }

    NL();

    //
    // [x] Show Slider Wrapper
    //
    {
        ref<CUiCheckGp> rUi;

        rUi->SetText("Slider Wrapper & Buttons");

        rUi->SetCheck(m_rUmoveTest->x_bShowSliderWrapper);

        rUi->OnClick = [=]
        {
            m_rUmoveTest->x_bShowSliderWrapper ^= 1;
            m_rUmoveTest->UpdateAllTestUi();
        }
    }

    NL();

    //
    // [x] Generate Horizontal Slider
    //
    {
        ref<CUiCheckGp> rUi;

        rUi->SetText("Horizontal Slider");

        rUi->SetCheck(m_rUmoveTest->x_bGenerateHorizontalSlider);

        rUi->OnClick = [=]
        {
            m_rUmoveTest->x_bGenerateHorizontalSlider ^= 1;

            if (m_rUmoveTest->x_bGenerateHorizontalSlider)
            {
                m_rUmoveTest->x_bShowSliderWrapper = true;
                m_rUmoveTest->x_bGenerateVerticalSlider = false;
            }

            m_rUmoveTest->UpdateAllTestUi();
        }
    }

    NL();

    //
    // [x] Generate Vertical Slider
    //
    {
        ref<CUiCheckGp> rUi;

        rUi->SetText("Vertical Slider");

        rUi->SetCheck(m_rUmoveTest->x_bGenerateVerticalSlider);

        rUi->OnClick = [=]
        {
            m_rUmoveTest->x_bGenerateVerticalSlider ^= 1;

            if (m_rUmoveTest->x_bGenerateVerticalSlider)
            {
                m_rUmoveTest->x_bShowSliderWrapper = true;
                m_rUmoveTest->x_bGenerateHorizontalSlider = false;
            }

            m_rUmoveTest->UpdateAllTestUi();
        }
    }

    NL();
}
