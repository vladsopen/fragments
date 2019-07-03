// UiDemoControl.cpp

#include "CProject.h"
#include "CDemoImpl.h"
#include "CDemoExitType.h"
#include "CProtocolIface.h"

void UiDemoControl::Ui()
{
    str sModality = m_rDemo->x_rExternalProtocol->x_sIniModality;
    ref<CProtocolIface> rProtocol = 
        m_rDemo->x_rExternalProtocol;

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rDemo->_m_ptrmapUiDemoControl += rUi;
    }

    //
    // Title 
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetText(sModality);
    }

    NL();

    //
    // Startup info
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetGroupBox(true);

        //
        // Title 
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                "Requested Command:");
        }

        NL();

        //
        // Parsed Info
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText( 
                (m_rDemo->x_bCreatingNewSample? 
                    "Create new" 
                    : 
                    "Open existing\n"
                        "  prefix: <" + 
                        RichSuccessBold(
                            rProtocol->x_sIniOpenPrefix) +
                        ">,\n"
                        "  extension: <" + 
                        RichSuccessBold(
                            rProtocol->x_sIniOpenFileExtension) +
                        ">,\n"
                        "  revision: <" +
                        RichSuccessBold(
                            rProtocol->x_sIniUseFileRevision) +
                        ">"
                ));
        }
    }

    NL();

    //
    // Result control
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetGroupBox(true);

        //
        // Title 
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                "Select result mode:");
        }

        NL();

        // iterate all exit types
        {
            type<CDemoExitType> typeIterExit;
            for (
                iter i;
                type<CDemoExitType>()->
                    GetDerivedList().
                        Iterate(
                            out i,
                            out typeIterExit);)
            {
                //
                // (o) Next exit
                //
                {
                    ref<UiContainer> rUi = 
                        m_rDemo->
                            NewUiDemoExitRadio(
                                typeIterExit);
                }

                NL();
            }
        }
    }

    NL();

    //
    // Data 
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetGroupBox(true);

        //
        // Title 
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText(
                "Sample data:");
        }

        NL();

        //
        // Data controls
        //
        {
            ref<UiContainer> rUi = 
                m_rDemo->
                    NewUiDemoData();
        }
    }

    NL();

}
