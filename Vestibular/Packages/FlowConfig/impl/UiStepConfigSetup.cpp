// UiStepConfigSetup.cpp

#include "CProject.h"
#include "CStepConfigImpl.h"
#include "CFlowConfigImpl.h"
#include "CDbStepConfigIface.h"
#include "CDemandPostureTypeIface.h"
#include "CDemandEyesTypeIface.h"
#include "CDizzyPaintTypeIface.h"
#include "CDemandFeetTypeIface.h"
#include "CDemandHeadTypeIface.h"

void UiStepConfigSetup::Ui()
{
    const size C_sizeLargeIcons = size(40, 40);

    ref<CFlowConfigImpl> rFlowConfig = 
        m_rStepConfig->_x_pFlowConfig.Get().Valid();
    ref<CDbStepConfigIface> rDbStepConfig = 
        m_rStepConfig->
            x_rDbStepConfig;
    bool bConfigStageTiming =
        rFlowConfig->
            _x_bAllowStageTimingConfig;
                        
    //bool bSelected = 
    //    rFlowConfig->x_iCurrentStep == m_rStepConfig->x_iStep;

    scale scaleCaptionHeight = scale(70);

    // step setup redesigned, aligned

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rStepConfig->_m_ptrmapUiStepConfigSetup += rUi;
        rFlowConfig->
            AddUiUsingPreview(
                rUi);

        rUi->SetAlignContentToTop();
    }

    //
    // Main Options (left column)
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetDialogSubPanel();
        //rUi->SetAlignContentToTop();

        rUi->SetMaxX(off);

        if (!m_rStepConfig->x_bStepInUse)
        {
            rUi->SetDisableInfo(
                CStepConfigImpl::C_slocStepIsNotInUse);
        }

        //
        // N
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                "" +
                    TRANSLATE(
                        "Stag[... UI-string-removed]");

            rUi->SetFontSize(
                //bSelected? 
                40 
                //: 20
                );

            rUi->SetPreferredY(scaleCaptionHeight);
        }

        NL();

        /*
        //
        // Extra info
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                "" +
                    (m_rStepConfig->x_bStepInUse?
                        ""
                        :
                        "" + CStepConfigImpl::C_slocStepIsNotInUse));

            rUi->SetFontSize(20);
            rUi->SetFontBold(true);
        }
        */

        NL();

        //
        // "Duration"
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                "" +
                    TRANSLATE(
                        "Duration[... UI-string-removed]");

            rUi->SetAlikeX(_m_alikexLabelsLeft);
        }

        if (bConfigStageTiming)
        {
            //
            // ____ s
            //
            {
                ref<CUiEditGp> rUi;

                rUi->SetText(
                    Str(
                        rDbStepConfig->
                            x_numStepSeconds));

                //rUi->SetFontSize(35);

                rUi->OnChange = [=]
                {
                    rDbStepConfig->
                            x_numStepSeconds = 
                        rUi->InputInt(
                            1, 
                            60, 
                            (int) 
                                rDbStepConfig->
                                        x_numStepSeconds);

                    m_rStepConfig->
                        HandleStepConfigChange();
                }
            }

            { ref<CUiSpinGp> rUi; }
        }
        else
        {
            //
            // Fixed s
            //
            {
                ref<CUiStaticGp> rUi;

                rUi->SetText(
                    Str(
                        rDbStepConfig->
                            x_numStepSeconds));
            }
        }

        //
        // "sec"
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                "" +
                    TRANSLATE(
                        "sec[... UI-string-removed]");
        }

        NL();

        //
        // "Pause Before"
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                "" +
                    TRANSLATE(
                        "Pause:[... UI-string-removed]");

            rUi->SetAlikeX(_m_alikexLabelsLeft);
        }

        if (bConfigStageTiming)
        {
            //
            // ____ s
            //
            {
                ref<CUiEditGp> rUi;

                rUi->SetText(
                    Str(
                        rDbStepConfig->
                            x_numPauseSecondsBeforeStep));

                //rUi->SetFontSize(35);

                rUi->OnChange = [=]
                {
                    rDbStepConfig->
                            x_numPauseSecondsBeforeStep = 
                        rUi->InputInt(
                            1, 
                            60, 
                            (int) 
                                rDbStepConfig->
                                        x_numPauseSecondsBeforeStep);

                    m_rStepConfig->
                        HandleStepConfigChange();
                }
            }

            { ref<CUiSpinGp> rUi; }
        }
        else
        {
            //
            // Fixed s
            //
            {
                ref<CUiStaticGp> rUi;

                rUi->SetText(
                    Str(
                        rDbStepConfig->
                            x_numPauseSecondsBeforeStep));
            }
        }

        //
        // "sec"
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                "" +
                    TRANSLATE(
                        "sec[... UI-string-removed]");
        }

        NL();

        if (bConfigStageTiming)
        {
            //
            // [x] x_bAutoStartStep
            //
            {
                ref<CUiCheckGp> rUi;

                rUi->SetText(
                    "" +
                        TRANSLATE(
                            "Automatic [... UI-string-removed]");

                rUi->SetTip(
                    TRANSLATE(
                        "If this is[... UI-string-removed]");

                rUi->SetCheck(
                    rDbStepConfig->
                        x_bAutoStartStep);

                rUi->OnClick = [=]
                {
                    rDbStepConfig->
                        x_bAutoStartStep ^= 1;

                    m_rStepConfig->
                        HandleStepConfigChange();
                }
            }
        }

        NL();

        //
        // "Posture"
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                CDemandPostureTypeIface::C_slocPosture + ":");

            rUi->SetAlikeX(_m_alikexLabelsLeft);
        }

        //
        // Posture [v]
        //
        {
            ref<CUiDropDownGp> rUi;

            rUi->SetIcon(
                rDbStepConfig->
                    x_typeDemandPostureType->
                        GetDemandPostureImage());

            rUi->SetIconSize(C_sizeLargeIcons);

            rUi->OnClick = [=]
            {
                ref<UiContainer> rUiFrame = 
                    m_rStepConfig->
                        NewUiStepConfigSelectPosture();

                rUiFrame->CreateDropDownAnchoredTo(this);
            }
        }

        NL();


        //
        // "Head"
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                "" +
                    TRANSLATE(
                        "Head[... UI-string-removed]");

            rUi->SetAlikeX(_m_alikexLabelsLeft);
        }

        //
        // Head [v]
        //
        {
            ref<CUiDropDownGp> rUi;

            rUi->SetIcon(
                rDbStepConfig->
                    x_typeDemandHeadType->
                        GetDemandHeadImage());

            rUi->SetIconSize(C_sizeLargeIcons);

            rUi->OnClick = [=]
            {
                ref<UiContainer> rUiFrame = 
                    m_rStepConfig->
                        NewUiStepConfigSelectHead();

                rUiFrame->CreateDropDownAnchoredTo(this);
            }
        }

        NL();

        //
        // "Feet"
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                "" +
                    TRANSLATE(
                        "Feet[... UI-string-removed]");

            rUi->SetAlikeX(_m_alikexLabelsLeft);
        }

        //
        // Feet [v]
        //
        {
            ref<CUiDropDownGp> rUi;

            rUi->SetIcon(
                rDbStepConfig->
                        x_typeDemandFeetType->
                            GetDemandFeetImage());

            rUi->SetIconSize(C_sizeLargeIcons);

            rUi->OnClick = [=]
            {
                ref<UiContainer> rUiFrame = 
                    m_rStepConfig->
                        NewUiStepConfigSelectFeet();

                rUiFrame->CreateDropDownAnchoredTo(this);
            }
        }

        NL();

        //
        // "Eyes"
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                "" +
                    TRANSLATE(
                        "Eyes[... UI-string-removed]");

            rUi->SetAlikeX(_m_alikexLabelsLeft);
        }

        //
        // Eyes [v]
        //
        {
            ref<CUiDropDownGp> rUi;

            rUi->SetIcon(
                rDbStepConfig->
                        x_typeDemandEyesType->
                            GetEyeStateImage());

            rUi->SetIconSize(C_sizeLargeIcons);

            rUi->OnClick = [=]
            {
                ref<UiContainer> rUiFrame = 
                    m_rStepConfig->
                        NewUiStepConfigSelectEyes();

                rUiFrame->CreateDropDownAnchoredTo(this);
            }
        }

        NL();
    }

    //
    // pad
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetText(" ");
    }

    //
    // Stripe Options (right column)
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetDialogSubPanel();
        //rUi->SetAlignContentToTop();

        //
        // "Paint"
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                "" +
                    TRANSLATE(
                        "Stimulus[... UI-string-removed]");

            rUi->SetFontBold(true);

            rUi->SetPreferredY(scaleCaptionHeight);
        }

        //
        // Paint [v]
        //
        {
            ref<CUiDropDownGp> rUi;

            rUi->SetText(
                rDbStepConfig->
                    x_typeDizzyPaintType->
                        GetPaintTypeAsString());

            rUi->SetIcon(
                rDbStepConfig->
                    x_typeDizzyPaintType->
                        GetPaintTypeIcon());
            rUi->SetIconSize(size(32, 32));

            rUi->OnClick = [=]
            {
                ref<UiContainer> rUiFrame = 
                    m_rStepConfig->
                        NewUiStepConfigSelectPaint();

                rUiFrame->CreateDropDownAnchoredTo(this);
            }
        }

        NL();

        //
        // "Stripe count"
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                "" +
                    TRANSLATE(
                        "Stripe cou[... UI-string-removed]");

            rUi->SetAlikeX(_m_alikexLabelsRight);
        }

        //
        // ____ s
        //
        {
            ref<CUiEditGp> rUi;

            rUi->SetText(
                Str(
                    rDbStepConfig->
                            x_numStripeCount));

            //rUi->SetFontSize(35);

            rUi->OnChange = [=]
            {
                rDbStepConfig->
                        x_numStripeCount = 
                    rUi->InputInt(
                        1, 
                        60, 
                        (int) 
                            rDbStepConfig->
                                    x_numStripeCount);

                m_rStepConfig->
                    HandleStepConfigChange();
            }
        }

        { ref<CUiSpinGp> rUi; }

        NL();

        //
        // "Stripe speed"
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                "" +
                    TRANSLATE(
                        "Stripe spe[... UI-string-removed]");

            rUi->SetAlikeX(_m_alikexLabelsRight);
        }

        //
        // ____ s
        //
        {
            ref<CUiEditGp> rUi;

            rUi->SetText(
                Str(
                    rDbStepConfig->
                            x_numStripeSpeedInSeconds));

            //rUi->SetFontSize(35);

            rUi->OnChange = [=]
            {
                rDbStepConfig->
                        x_numStripeSpeedInSeconds = 
                    rUi->InputInt(
                        1, 
                        // 60 exposes a width +/- 1 bug in 3D
                        30,
                        //60, 
                        (int) 
                            rDbStepConfig->
                                    x_numStripeSpeedInSeconds);

                m_rStepConfig->
                    HandleStepConfigChange();
            }
        }

        { ref<CUiSpinGp> rUi; }

        //
        // "sec"
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                "" +
                    TRANSLATE(
                        "sec[... UI-string-removed]");
        }

        NL();
    }
}
