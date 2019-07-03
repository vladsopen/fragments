// UiStepConfigCompactAttribs.cpp

#include "CProject.h"
#include "CStepConfigImpl.h"
#include "CDbStepConfigIface.h"
#include "CDemandPostureTypeIface.h"
#include "CDemandEyesTypeIface.h"
#include "CDizzyPaintTypeIface.h"
#include "CFlowConfigImpl.h"
#include "CDemandFeetTypeIface.h"
#include "CDemandHeadTypeIface.h"

void UiStepConfigCompactAttribs::Ui()
{
    ref<CFlowConfigImpl> rFlowConfig = 
        m_rStepConfig->_x_pFlowConfig.Get().Valid();

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rStepConfig->_m_ptrmapUiStepConfigCompactAttribs += rUi;
    }

    //
    // Disableable Part
    //
    {
        ref<CUiPanelGp> rUi;

        if (!m_rStepConfig->x_bStepInUse)
        {
            rUi->SetDisableInfo(
                CStepConfigImpl::C_slocStepIsNotInUse);
        }

        //
        // "Info"
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                "(" +
                    Str(
                        m_rStepConfig->
                            x_rDbStepConfig->
                                x_numStepSeconds) + 
                    " " +
                    TRANSLATE(
                        "s.[... UI-string-removed]");

            rUi->SetTip(
                "" +
                    TRANSLATE(
                        "Duration[... UI-string-removed]");

            rUi->SetMinX(scale(60));
        }

        //
        // Posture
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetIcon(
                m_rStepConfig->
                    x_rDbStepConfig->
                        x_typeDemandPostureType->
                            GetDemandPostureImage());

            rUi->SetIconSize(size(32, 32));

            rUi->SetTip(
                "" +
                    CDemandPostureTypeIface::C_slocPosture);
        }

        //
        // Head
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetIcon(
                m_rStepConfig->
                    x_rDbStepConfig->
                        x_typeDemandHeadType->
                            GetDemandHeadImage());

            rUi->SetIconSize(size(32, 32));

            rUi->SetTip(
                "" +
                    TRANSLATE(
                        "Head[... UI-string-removed]");
        }

        //
        // Feet
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetIcon(
                m_rStepConfig->
                    x_rDbStepConfig->
                        x_typeDemandFeetType->
                            GetDemandFeetImage());

            rUi->SetIconSize(size(32, 32));

            rUi->SetTip(
                "" +
                    TRANSLATE(
                        "Feet[... UI-string-removed]");
        }

        //
        // Eyss
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetIcon(
                m_rStepConfig->
                    x_rDbStepConfig->
                        x_typeDemandEyesType->
                            GetEyeStateImage());

            rUi->SetTip(
                "" +
                    TRANSLATE(
                        "Eyes[... UI-string-removed]");
        }

        //
        // Animation
        //
        {
            ref<CUiStaticGp> rUi;

            /*
            rUi->SetText(
                m_rStepConfig->
                    x_rDbStepConfig->
                        x_typeDizzyPaintType->
                            GetPaintTypeAsString());
                            */

            rUi->SetIcon(
                m_rStepConfig->
                    x_rDbStepConfig->
                        x_typeDizzyPaintType->
                            GetPaintTypeIcon());
            rUi->SetIconSize(size(32, 32));

            rUi->SetTip(
                "" +
                    TRANSLATE(
                        "Stimulus[... UI-string-removed]");
        }
    }
}
