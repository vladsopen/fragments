// UiProxyMain.cpp

#include "CProject.h"
#include "CProxyImpl.h"
#include "CGlobalConfigIface.h"
#include "CPatientLinkSampleIface.h"
#include "CAppEventFloaterIfaceGp.h"
#include "CLogTypeIface.h"
#include "CParaApiIface.h"
#include "CPatientLinkIface.h"
#include "VirtOs_dir.h"
#include "CSequence.h"
#include "CProtocolIface.h"

void UiProxyMain::Ui()
{
    ref<CGlobalConfigIface> rGlobalConfig = 
        CProject::GGetIt().x_rGlobalConfig;

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rProxy->_m_ptrmapUiProxyMain += rUi;

        /*
        // for demo only
        rUi->SetBorderColor(rgb(0, 0, 255));
        rUi->SetBorderDepth(5);
        rUi->SetBorderStyleToFlat();
        */

        rUi->SetDialogRootPanel(); // some padding
        rUi->SetAlignContentToCenterY();
    }

    //
    // Title
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetText("Protocol Debugger");

        rUi->SetFontSize(30);
    }

    //
    // Right-aligned toolbar
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetMaxX(oo);
        rUi->SetAlignContentToRight();

        rUi->SetPadX(2);

        /*
        //
        // Test Event Log
        //
        {
            ref<CUiButtonGp> rUi;

            rUi->SetText("Demo Event");

            rUi->SetIcon(RES__INFO_BALLOON_L());

            rUi->OnClick = [=]
            {
                type<CLogTypeIface>()->
                    AsLogTypeForDemoError()->
                        LogEvent(
                            TRANSLATE(
                                "Demo event[... UI-string-removed]");
            }
        }
        */

        /*
        //
        // Show patient card
        //
        {
            ref<CUiButtonGp> rUi;

            rUi->SetText(
                TRANSLATE(
                    "Card[... UI-string-removed]");

            rUi->SetTip(
                TRANSLATE(
                    "Show patie[... UI-string-removed]");

            rUi->SetIcon(RES__EDIT_COMMENT_L());

            rUi->OnClick = [=]
            {
                m_rProxy->
                    GetPatientLink()->
                        ShowPatientCard();
            }
        }
        */
    }

    NL();

    //
    // [Browse sample dir]
    //
    {
        ref<CUiButtonGp> rUi;

        rUi->SetIdleUpdate(true);

        rUi->SetText(
            "Explore Data");

        rUi->SetTip(
            "Open current working data folder");

        if (m_rProxy->x_rProtocol->x_pathIniSampleFolder == "")
        {
            rUi->SetDisableInfo("Not yet defined");
        }

        rUi->OnClick = [=]
        {
            sys::GOpenExplorer(
                m_rProxy->x_rProtocol->x_pathIniSampleFolder,
                sys::E_ExplorerView_Tree,
                sys::E_ExplorerOpen_Folder);
        }
    }

    //
    // [Browse .ini dir]
    //
    {
        ref<CUiButtonGp> rUi;

        rUi->SetIdleUpdate(true);

        rUi->SetText(
            "Explore .ini");

        rUi->SetTip(
            "Open current temp communication folder");

        if (m_rProxy->x_pathProtocolIni == "")
        {
            rUi->SetDisableInfo("Not yet defined");
        }

        rUi->OnClick = [=]
        {
            sys::GOpenExplorer(
                m_rProxy->x_pathProtocolIni,
                sys::E_ExplorerView_Tree,
                sys::E_ExplorerOpen_SelectInParent);
        }
    }

    //
    // [PatientAPI-Debug]
    //
    {
        ref<UiContainer> rUi =
            m_rProxy->
                GetPatientLink()->
                    NewUiPatientDebug();
    }

    //
    // Patients Session Info
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetIdleUpdate(true);

        // dump Patients sample status to verify all flags
        ref<CPatientLinkSampleIface> rPatientLinkSample = 
            m_rProxy->_x_rPatientLinkSample;

        str sStatus = RichBold("Link Status: ");
        if (rPatientLinkSample->IsProxyAttachedToPatientSample())
        {
            sStatus += "Patients linked sample. ";

            if (rPatientLinkSample->IsProxyCreatedByPatientLink())
            {
                sStatus += "Loaded from Patients ";
            }
            if (rPatientLinkSample->IsPatientSampleOpenedByPatientIo())
            {
                sStatus += "by PatientIo-OpenSample. ";
            }
            if (rPatientLinkSample->IsPatientSamplePaintedByPatientIo())
            {
                sStatus += "by PatientIo-PaintSample. ";
            }
            if (rPatientLinkSample->IsPatientSampleViewedByPatientEmbed())
            {
                sStatus += "by PatientEmbed. ";
            }
        }
        else
        {
            sStatus += "New unsaved sample.";
            rASSERT(!rPatientLinkSample->IsProxyCreatedByPatientLink());
            rASSERT(!rPatientLinkSample->IsPatientSampleOpenedByPatientIo());
            rASSERT(!rPatientLinkSample->IsPatientSamplePaintedByPatientIo());
            rASSERT(!rPatientLinkSample->IsPatientSampleViewedByPatientEmbed());
        }

        rUi->SetText(
            sStatus);
                //+
                //"\n" +
                //m_rProxy->
                //    GetPatientLink()->
                //        GetCurrentPatientNameAgeSexAndId());
    }

    NL();

    //
    // pad
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetText("");
        rUi->SetMaxY(oo);
    }

    NL();

    //
    // Steps
    //
    {
        ref<UiContainer> rUi =
            m_rProxy->
                x_rSequence->
                    NewUiSequenceList();
    }

    NL();
}
