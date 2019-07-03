// UiChannelAdjustParam.cpp

#include "CUsmainProject.h"
#include "CChannel.h"
#include "CDeviceCommon.h"
#include "CAdjusterTypeImpl.h"
#include "CSliderIfaceGp.h"
#include "CSkinizerCommonIfaceGp.h"
#include "CArgUsoundAdjusterStateGp.h"
#include "CColorizerCommonIfaceGp.h"

void UiChannelAdjustParam::Ui()
{
    ref<CDeviceCommon> rDeviceCommon = 
        m_rChannel->_x_pParentDevice.Get().Valid();

    // read current setting
    ref<CArgUsoundAdjusterStateGp> rArgUsoundAdjusterState = 
        rDeviceCommon->
            GetDeviceChannelAdjuster(
                m_rChannel->x_iChannel,
                m_typeIterAdjusterType);

    str sForUi = rArgUsoundAdjusterState->_x_richAdjusterValueForUi;
    str sUiUnits = rArgUsoundAdjusterState->_x_slocAdjusterUiUnits;
    int iInternalPos = rArgUsoundAdjusterState->_x_iAdjusterInternalPos;
    int nInternalRange = rArgUsoundAdjusterState->_x_nAdjusterInternalRange;
    rASSERT(rArgUsoundAdjusterState->_x_nAdjusterInternalSmallStep == 1);
    rASSERT(rArgUsoundAdjusterState->_x_nAdjusterInternalLargeStep == 1);

    //
    // Static Slider Paramenters
    //

    m_rSlider->x_nSmallDelta = 1;;
    m_rSlider->x_nLargeDelta = m_rSlider->x_nSmallDelta;
    m_rSlider->x_pixThumbWidth = (pix) scale(C_scaleThumbAndButtonsWidth); // can be variable as well

    //
    // Update Slider State
    //

    m_rSlider->x_iLogicalSliderPos =
        iInternalPos;

    m_rSlider->x_iLogicalSliderMin =  
        0;

//debug:
    if (!debug::GIsTest() &&
        debug("Device.ThumbLimitTest?").InputBool())
    {
        m_rSlider->x_iLimitMovementMin =
            math::GAbs(m_typeIterAdjusterType->GetAdjusterNameForUi().CalcSimpleCrc())
                % 5;
        m_rSlider->x_iLimitMovementMax =  
            5 + 
                math::GAbs((m_typeIterAdjusterType->GetAdjusterNameForUi()).CalcSimpleCrc()) 
                % 5;
    }
//debug.

    m_rSlider->x_iLogicalSliderMax =  
        nInternalRange - 1;

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rChannel->_m_ptrmapUiChannelAdjustParam += rUi;
    }

    //
    // Label
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetText(
            "" +
                m_typeIterAdjusterType->
                    GetAdjusterNameForUi() + 
                m_typeIterAdjusterType->
                    GetAdjusterNameEndingForUi(
                        rDeviceCommon,
                        m_rChannel->x_iChannel) + 
                ": " + 
                RichBold(sForUi) + 
                " " + 
                sUiUnits);
    }

    NL();

    //
    // Free-style Device Control Slider
    //
    {
        ref<CUiPanelGp> rUi;

        // Generic Slider attributes
        m_rSlider->EqualizeOuterSliderBarPanel(rUi);

        // Lock width (application controlled)
        //rUi->SetPreferredX(nSliderWidth);
        rUi->SetMaxX(oo);
        rUi->SetMinX(off);

        // Flexible height (application controlled)
        //rUi->SetMinY(nMinSliderHeight);
        rUi->SetMaxY(off);
        //rUi->SetPreferredY(nSliderHeight);

        //
        // Left Button
        //
        {
            ref<CUiButtonGp> rUi;

            EqualizeSliderPlusMinus(rUi);

            rUi->SetText("-");

            rUi->OnClick = [=]
            {
                // Generic Slider logic
                m_rSlider->HandleFirstArrowButton();

                // Send new slider position to the application
                rDeviceCommon->
                    SetDeviceChannelAdjuster(
                        m_rChannel->x_iChannel,
                        m_typeIterAdjusterType,
                        m_rSlider->x_iLogicalSliderPos);
            }
        }

        //
        // Slider Rail Panel
        //
        {
            ref<CUiPanelGp> rUi;

            // Border settings according to skin preference

            // Skin
            rUi->GetSkinizer()->EqualizeGroupBorder(rUi);

            // Generic Slider attributes
            m_rSlider->EqualizeSliderRail(rUi);

            rUi->OnSize = [=]
            {
                // Just send it to Slider and forget
                m_rSlider->HandleRailOnSize(rUi);
            }

            rUi->OnClick = [=]
            {
                // Change the position if necessary
                if (m_rSlider->HandleRailOnClick(rUi))
                {
                    rDeviceCommon->
                        SetDeviceChannelAdjuster(
                            m_rChannel->x_iChannel,
                            m_typeIterAdjusterType,
                            m_rSlider->x_iLogicalSliderPos);
                }
            }

            // added for new visual rail features
            rUi->OnPaint = [=]
            {
                draw dc;
                dc.HandleDrawUi(this);

                m_rSlider->HandleRailOnPaint(rUi, dc);
            }

            //
            // Moving Thumb Button
            //
            {
                ref<CUiButtonGp> rUi;

                // Important Slider Setup for the Thumb button
                m_rSlider->EqualizeThumb(rUi);

                rUi->SetPreferredY(scale(20));

                rUi->OnModifyShape = [=]
                {
                    // Just send it to Slider and forget
                    m_rSlider->HandleThumbOnModifyShape(rUi);
                }

                rUi->OnMouse = [=]
                {
                    // Slider will initiate drag routine if reqired
                    if (m_rSlider->HandleThumbOnMouse(rUi, GetOnMouseEvent()))
                    {
                        // Set new position if changed
                        rDeviceCommon->
                            SetDeviceChannelAdjuster(
                                m_rChannel->x_iChannel,
                                m_typeIterAdjusterType,
                                m_rSlider->x_iLogicalSliderPos);
                    }
                }
            }
        }

        //
        // Right Button
        //
        {
            ref<CUiButtonGp> rUi;

            EqualizeSliderPlusMinus(rUi);

            rUi->SetText("+");

            rUi->OnClick = [=]
            {
                // Generic Slider logic
                m_rSlider->HandleSecondArrowButton();

                // Send new slider position to the application
                rDeviceCommon->
                    SetDeviceChannelAdjuster(
                        m_rChannel->x_iChannel,
                        m_typeIterAdjusterType,
                        m_rSlider->x_iLogicalSliderPos);
            }
        }
    }
}

void UiChannelAdjustParam::EqualizeSliderPlusMinus(
        ref<CUiButtonGp> rUi)
{
    // Generic Slider attributes
    m_rSlider->EqualizeArrowButton(rUi);

    // "+"/"-" text
    rUi->SetFontSize(35);
    rUi->SetFontBold(true);

    rUi->SetMinX(off);
    rUi->SetPreferredX(C_scaleThumbAndButtonsWidth);
    rUi->SetAlignContentToCenterX();

    rUi->SetMinY(off);

    rUi->SetTextColor(rUi->GetColorizer()->GetButtonShadowAndSoftTextColor());
}

