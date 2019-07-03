// UiFilterExperimenter.cpp

#include "CProject.h"
#include "CFilterImpl.h"
#include "VirtOs_CAppGp.h"
#include "CUvOptionsIfaceGp.h"
#include "CUvSkinOptionsIfaceGp.h"
#include "CSkinizerCommonIfaceGp.h"
#include "CDbConfigIface.h"

void UiFilterExperimenter::Ui()
{
    ref<CDbConfigIface> rDbConfig = 
        m_rFilter->GetDbConfigForFilter();
    sloc slocDisableRange = 
        rDbConfig->x_bBandFilterEnabled?
            ""
            :
            TRANSLATE(
                "Filter is [... UI-string-removed]");

    GetUiContainerSkinizer()->
        x_bShowLineartUnderCaption = false;

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rFilter->_m_ptrmapUiFilterExperimenter += rUi;

        rUi->SetSkinizerAndDefaultColorizer(
            CAppGp::GGet()->
                x_rUvOptions->
                    x_rUvSkinOptions->
                        NewSelectedSkinizerForDialogs());

        rUi->GetSkinizer()->
            EqualizeFrameBorder(rUi);

        /*
        size sizeScreen =
            sys::GGetPrimaryMonitorResolution();

        rUi->SetMinX(sizeScreen.w / 3);
        rUi->SetMinY(sizeScreen.h / 3);
        */

        rUi->OnCloseFrame = [=]
        {
            m_rFilter->
                NormalizeFilterBounds();
        }
    }

    //
    // Skinned Caption
    //
    {
        ref<UiContainer> rUi = 
            GetUiContainerSkinizer()->
                NewUiCommonCaption(
                    TRANSLATE(
                        "Filter[... UI-string-removed]");
    }

    NL();

    //
    // Main
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetDialogRootPanel();

        //
        // "Range:"
        //
        {
            ref<CUiCheckGp> rUi;

            rUi->SetText(
                TRANSLATE(
                    "Range:[... UI-string-removed]");

            rUi->SetTip(
                TRANSLATE(
                    "Turn filte[... UI-string-removed]");

            rUi->SetCheck(rDbConfig->x_bBandFilterEnabled);

            rUi->OnClick = [=]
            {
                rDbConfig->x_bBandFilterEnabled ^= 1;

                m_rFilter->
                    HandleFilterSetupChange(
                        rDbConfig);
            }
        }

        //
        // ____ min Hz
        //
        {
            ref<CUiEditGp> rUi;

            rUi->SetText(Str(rDbConfig->x_numBandLowHz));

            rUi->SetTip(
                TRANSLATE(
                    "Minimum fr[... UI-string-removed]");

            rUi->SetDisableInfo(slocDisableRange);

            rUi->SetPreferredX(scale(60));

            rUi->OnChange = [=]
            {
                rDbConfig->x_numBandLowHz = 
                    rUi->InputNum(
                        0, 
                        m_rFilter->GetFilterMaxHz(), 
                        rDbConfig->x_numBandLowHz);

                m_rFilter->
                    HandleFilterSetupChange(
                        rDbConfig);
            }
        }

        //
        // Spin min
        //
        { 
            ref<CUiSpinGp> rUi; 

            rUi->SetStep(0.1);
        }

        //
        // ____ max Hz
        //
        {
            ref<CUiEditGp> rUi;

            rUi->SetText(Str(rDbConfig->x_numBandHighHz));

            rUi->SetTip(
                TRANSLATE(
                    "Maximum fr[... UI-string-removed]");

            rUi->SetDisableInfo(slocDisableRange);

            rUi->SetPreferredX(scale(50));

            rUi->OnChange = [=]
            {
                rDbConfig->x_numBandHighHz = 
                    rUi->InputNum(
                        0, 
                        m_rFilter->GetFilterMaxHz(), 
                        rDbConfig->x_numBandHighHz);

                m_rFilter->
                    HandleFilterSetupChange(
                        rDbConfig);
            }
        }

        //
        // Spin max
        //
        { 
            ref<CUiSpinGp> rUi; 

            rUi->SetStep(1.0);
        }

        //
        // "Hz"
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                TRANSLATE(
                    "Hz[... UI-string-removed]");
        }

        NL();

        //
        // Close
        //
        {
            ref<CUiDialogCloseGp> rUi;

            rUi->GetSkinizer()->
                EqualizeDialogCloseButton(rUi);

            rUi->OnClick = [=]
            {
                rUi->CloseFrame();
            }
        }
    }
}
