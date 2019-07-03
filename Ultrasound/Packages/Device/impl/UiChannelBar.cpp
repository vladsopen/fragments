// UiChannelBar.cpp

#include "CUsmainProject.h"
#include "CChannel.h"
#include "CRawCapture.h"
#include "CDetailTypeIface.h"
#include "CAdjusterTypeImpl.h"
#include "CCustomizeHardwareImpl.h"
#include "CDeviceCommon.h"
#include "CHardwareImpl.h"
#include "CSkinizerCommonIfaceGp.h"
#include "CColorizerCommonIfaceGp.h"

void UiChannelBar::Ui()
{
    // Get visibility info
    ref<CDeviceCommon> rDeviceCommon = 
        m_rChannel->_x_pParentDevice.Get().Valid();
    ref<CHardwareImpl> rHardware = 
        rDeviceCommon->_x_pParentHardware.Get().Valid();
    ref<CCustomizeHardwareImpl> rCustomizeHardware = 
        (ref<CCustomizeHardwareIface>) rHardware->_x_rCustomizeHardware;

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rChannel->_m_ptrmapUiChannelBar += rUi;

        //rUi->SetGroupBox(true);

        rUi->SetPadY(2 + (pix) scale(3));
    }

    if (rCustomizeHardware->CountVisibleChannels() > 1 ||
        m_rChannel->x_iChannel > 0)
    {
        //
        // Group Caption
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                "" + 
                    TRANSLATE(
                        "Channe[... UI-string-removed]");

            rUi->SetFontBold(true);
            rUi->SetFontSize(25);
            rUi->SetTextColor(rUi->GetColorizer()->GetButtonShadowAndSoftTextColor());
            rUi->SetAlignContentToRight();
        }
    }

    NL();

    // Device controls
    if (!m_typeDetailType->IsAuxiliaryWindow())
    {
        type<CAdjusterTypeIfaceGp> typeIterAdjusterTypeIface;
        for (
            iter i;
            type<CAdjusterTypeImpl>()->
                GetDerivedList().
                    Iterate(
                        out i,
                        out typeIterAdjusterTypeIface);)
        {
            type<CAdjusterTypeImpl> typeIterAdjusterType = 
                typeIterAdjusterTypeIface;

            // Hidden? (Gain with AGC)
            if (!typeIterAdjusterType->
                    IsAdjusterUiVisible(
                        rDeviceCommon,
                        m_rChannel->x_iChannel))
            {
                continue;
            }

            int iForCluster = typeIterAdjusterType->GetClusterNumberAdjusterSpecificToOr(-1);
            if (iForCluster >= 0 &&
                !rCustomizeHardware->
                    IsClusterAdjustersVisible(
                        m_rChannel->x_iChannel,
                        iForCluster))
            {
                continue;
            }

            //
            // Next Parameter
            //
            {
                ref<UiContainer> rUi =
                    typeIterAdjusterType->
                        NewUiAdjusterInChannelBar(
                            m_rChannel,
                            m_typeDetailType);
            }

            NL();
        }
    }

    // All raw captures in the channel
    if (m_typeDetailType->CanSeeHacks())
    {
        ref<CRawCapture> rIterRawCapture;
        for (
            iter i;
            m_rChannel->
                IterateChannelCaptures(
                    out i,
                    out rIterRawCapture);)
        {
            //
            // Next Capture
            //
            {
                ref<UiContainer> rUi = 
                    rIterRawCapture->
                        NewUiRawCaptureBar(
                            m_typeDetailType);
            }
            
            NL();
        }
    }
}
