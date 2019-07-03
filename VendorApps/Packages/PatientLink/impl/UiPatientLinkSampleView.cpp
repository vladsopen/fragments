// UiPatientLinkSampleView.cpp

#include "CProject.h"
#include "CPatientLinkSampleImpl.h"
#include "CProxyIface.h"
#include "VirtOs_CAppGp.h"
#include "CUvOptionsIfaceGp.h"
#include "CUvSkinOptionsIfaceGp.h"
#include "CSkinizerCommonIfaceGp.h"

void UiPatientLinkSampleView::Ui()
{
    ref<CProxyIface> rProxy = 
        m_rPatientLinkSample->
            _x_pProxy.Get().Valid();

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rPatientLinkSample->_m_ptrmapUiPatientLinkSampleView += rUi;

        //DEMO: OpenSample windows looking like the source main window
        rUi->SetSkinizerAndDefaultColorizer(
            CAppGp::GGet()->
                x_rUvOptions->
                    x_rUvSkinOptions->
                        NewSelectedSkinizerForMainFrame());

        rUi->OnCloseFrame = [=]
        {
            rProxy->
                CloseProxy();
        }
    }

    //
    // Sample Window Caption
    //
    {
        ref<CUiCaptionGp> rUi;

        //
        // App Logo
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                TRANSLATE(
                    "A sample f[... UI-string-removed]");

            rUi->SetIcon(RES_APP_MAIN());
            rUi->SetMinX(0);
            rUi->SetMaxX(oo);
        }

        //
        // Close Sample
        //
        { 
            ref<CUiCloseGp> rUi; 

            rUi->OnClick = [=]
            {
                rUi->CloseFrame();
            }
        }
    }

    NL();

    //
    // Proxy UI
    //
    {
        ref<UiContainer> rUi = 
            rProxy->
                NewUiProxyMain();
    }
}
