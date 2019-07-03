// UiColoringBarSlider.cpp

#include "CUsmainProject.h"
#include "CColoring.h"
#include "CDocIface.h"
#include "CImageImpl.h"
#include "CMatrixFormatGp.h"
#include "CCustomizeDocIfaceGp.h"
#include "CUmoveIfaceGp.h"
#include "CPaletteLimitTypeIface.h"
#include "CLocalPaletteConfigIface.h"
#include "CGlobalPaletteConfigIface.h"

void UiColoringBarSlider::Ui()
{
    ref<CLocalPaletteConfigIface> rLocalPaletteConfig =
        m_rColoring->x_rLocalPaletteConfig;
    ref<CGlobalPaletteConfigIface> rGlobalPaletteConfig = 
        m_rColoring->GetImageGlobalPaletteConfig();
    ref<CImageImpl> rImage = 
        m_rColoring->_x_pParentImage.Get().Valid();
    ref<CDocIface> rDoc = 
        rImage->_x_pParentDoc.Get().Valid();
    ref<CMatrixFormatGp> rMatrixFormat = 
        m_rColoring->GetRelatedMatrixFormat();

    int nSliderButtonsHeight = 
        (pix) scale(CColoring::C_scalePaletteSliderHandleHeight);

    int nTotalRange = 
        rMatrixFormat->x_nMatrixSliceAmplitudeRange;

    size sizeParentPanel = 
        m_rColoring->x_sizeUmoveSliderSystemParent;

    sizeParentPanel.h -= nSliderButtonsHeight;

    bool bCompactRulerBars = m_bCompactRulerBars;

    if (bCompactRulerBars)
    {
        return;
    }

    //
    // Init Umove before use
    //
    {
        m_rUmove->
            x_sizeLogicalUmoveRange = 
                size(
                    0, 
                    nTotalRange + 1);

        m_rUmove->
            x_pointLogicalVehiclePos = 
                point(
                    0, 
                    m_typePaletteLimitType->
                        ConvertPaletteLimitToSliderOffset(
                            nTotalRange,
                            rLocalPaletteConfig));

        m_rUmove->
            x_sizeVehicle = 
                size(
                    0, 
                    (pix) scale(CColoring::C_scalePaletteSliderHandleHeight));

        // threshold slider must always be lower than saturation slider
        m_rUmove->
            x_sizeOffsetInParentPanel = 
                size(
                    0,
                    m_typePaletteLimitType->
                        AllowSliderOffset(
                            nSliderButtonsHeight));

    }

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rColoring->_m_ptrmapUiColoringBarSlider += rUi;

        /*
        // Extract our height from proportional height
        rUi->SetPreferredY(
            CColoring::C_scalePaletteSliderHandleHeight);
        rUi->SetMinY(off);
        rUi->SetMaxY(off);

        // all available width
        rUi->SetPreferredX(0);
        rUi->SetMinX(1);
        rUi->SetMaxX(oo);
        */

        rUi->SetIndependentShape(true);

        rUi->SetMarginAtLeft(1);
        rUi->SetMarginAtRight(1);

        rUi->OnIndependentShape = [=]
        {
            // asking Umove the Vehicle position
            int iSliderOffset = 
                m_rUmove->
                    GetVehiclePosInParentPanel(
                        sizeParentPanel).
                        y;

            rUi->SetInstantPos(0, iSliderOffset);
            rUi->SetInstantSize(sizeParentPanel.w, m_rUmove->x_sizeVehicle.Get().h);
        }

        rUi->OnMouse = [=]
        {
            // drag the slider
            bool bMoved = 
                m_rUmove->
                    HandleVehicleOnMouse(
                        sizeParentPanel,
                        rUi,
                        GetOnMouseEvent());

            if (bMoved)
            {
                // get new position
                point pointNewPos = 
                    m_rUmove->
                        x_pointLogicalVehiclePos;

                // convert from logical up-down axis to palette limit value
                int iPaletteLimit = 
                    m_typePaletteLimitType->
                        ConvertSliderOffsetToPaletteLimit(
                            pointNewPos.y,
                            nTotalRange);

                // set the value (it will be checked and restricted)
                m_rColoring->
                    ChangePaletteLimit(
                        m_typePaletteLimitType,
                        iPaletteLimit);
            }
        }
    }

    //
    // Saturation Slider Button
    //
    {
        ref<CUiButtonGp> rUi;

        rUi->SetFocusable(false);

        rUi->SetCursor(m_rUmove->GetUmoveCursorForVehicle());

        // Size is defined by the parent
        rUi->SetMinX(0);
        rUi->SetMinY(0);
        rUi->SetMaxX(oo);
        rUi->SetMaxY(oo);
    }
}
