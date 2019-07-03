// UiUmoveSimpleDemoSetup.cpp

#include "CProject.h"
#include "CUmoveTest.h"

void UiUmoveSimpleDemoSetup::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rUmoveTest->_m_ptrmapUiUmoveSimpleDemoSetup += rUi;
    }

    //
    // "Virtual Position:"
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetText(
            "Virtual Position: " + 
                Str(m_rUmoveTest->x_iSimpleDemoPos) + 
                " ");
    }

    //
    // Virtual Position Input
    //
    {
        ref<CUiEditGp> rUi;

        rUi->SetText(Str(m_rUmoveTest->x_iSimpleDemoPos));

        rUi->OnChange = [=]
        {
            m_rUmoveTest->x_iSimpleDemoPos = 
                rUi->InputInt(
                    0,
                    oo,
                    m_rUmoveTest->x_iSimpleDemoPos);

            m_rUmoveTest->UpdateForNewSimepleDemoPositions();
        }
    }

    { ref<CUiSpinGp> rUi; }

    NL();

    //
    // "Virtual Range:"
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetText(
            "Virtual Range: " + 
                Str(m_rUmoveTest->x_nSimpleDemoRange) + 
                " ");
    }

    //
    // Virtual Range Input
    //
    {
        ref<CUiEditGp> rUi;

        rUi->SetText(Str(m_rUmoveTest->x_nSimpleDemoRange));

        rUi->OnChange = [=]
        {
            m_rUmoveTest->x_nSimpleDemoRange = 
                rUi->InputInt(
                    0,
                    oo,
                    m_rUmoveTest->x_nSimpleDemoRange);

            m_rUmoveTest->UpdateForNewSimepleDemoPositions();
        }
    }

    { ref<CUiSpinGp> rUi; }

    NL();

    /*
    //
    // [Resample 1]
    //
    {
        ref<CUiButtonGp> rUi;

        rUi->SetText("Resample Array (Simple)");

        rUi->OnClick = [=]
        {
            m_rMuldivResample->
                ResampleArraySimple();
        }
    }

    NL();

    //
    // [Resample 2]
    //
    {
        ref<CUiButtonGp> rUi;

        rUi->SetText("Resample Array (Interpolate)");

        rUi->OnClick = [=]
        {
            m_rMuldivResample->
                ResampleArrayInterpolate();
        }
    }

    NL();

    //
    // "Report"
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetText(m_rMuldivResample->x_sReport);
    }
    */
}
