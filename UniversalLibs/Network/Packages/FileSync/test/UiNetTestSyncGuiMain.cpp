// UiNetTestSyncGuiMain.cpp

#include "CNetProject.h"
#include "CNetTestSyncGui.h"
#include "CNetSyncOutImpl.h"
#include "CNetSyncInImpl.h"
#include "VirtOs_CPathList.h"

void UiNetTestSyncGuiMain::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rNetTestSyncGui->_m_ptrmapUiNetTestSyncGuiMain += rUi;
    }

    //
    // Window Caption
    //
    {
        ref<CUiCaptionGp> rUi;

        //
        // Logo
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetText("Test Sync");

            //rUi->SetIcon(RES_APP_MAIN());
            rUi->SetMinX(0);
            rUi->SetMaxX(off);

            //rUi->SetFontSize(16);
        }

        //
        // pad
        //
        {
            ref<CUiStaticGp> rUi;

            rUi->SetMaxX(oo);
        }

        { ref<CUiCloseGp> rUi; }
    }

    NL();

    //
    // main
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetDialogRootPanel();

        rUi->SetAlignContentToTop();

        //
        // [Send File]
        //
        {
            ref<CUiButtonGp> rUi;

            rUi->SetText(
                "Send File");

            rUi->OnClick = [=]
            {
                m_rNetTestSyncGui->
                    SendFile(
                        m_rNetTestSyncGui->x_pathSendFile);
            }
        }

        //
        // [Send Folder]
        //
        {
            ref<CUiButtonGp> rUi;

            rUi->SetText(
                "Send Folder " + 
                    "(pending: " + 
                    Str(m_rNetTestSyncGui->x_rPathListTodoSend->GetCount()) + 
                    ")");

            rUi->OnClick = [=]
            {
                m_rNetTestSyncGui->
                    SendFolder();
            }
        }

        //
        // [x] Out
        //
        {
            ref<CUiCheckGp> rUi;

            rUi->SetText("Out");

            rUi->SetCheck(
                m_rNetTestSyncGui->
                    x_rNetSyncOut->
                        x_bEnableNetSyncPumping);

            rUi->OnClick = [=]
            {
                m_rNetTestSyncGui->
                    x_rNetSyncOut->
                        x_bEnableNetSyncPumping 
                        =
                        !m_rNetTestSyncGui->
                            x_rNetSyncOut->
                                x_bEnableNetSyncPumping;
            }
        }

        //
        // [x] In
        //
        {
            ref<CUiCheckGp> rUi;

            rUi->SetText("In");

            rUi->SetCheck(
                m_rNetTestSyncGui->
                    x_rNetSyncIn->
                        x_bEnableNetSyncPumping);

            rUi->OnClick = [=]
            {
                m_rNetTestSyncGui->
                    x_rNetSyncIn->
                        x_bEnableNetSyncPumping 
                        =
                        !m_rNetTestSyncGui->
                            x_rNetSyncIn->
                                x_bEnableNetSyncPumping;
            }
        }

        NL();
    
        //
        // Out Dir
        //
        {
            ref<UiContainer> rUi = 
                m_rNetTestSyncGui->
                    NewUiNetTestSyncGuiDir(
                        m_rNetTestSyncGui->x_pathUploadTemp);
        }
   
        //
        // Out Dir
        //
        {
            ref<UiContainer> rUi = 
                m_rNetTestSyncGui->
                    NewUiNetTestSyncGuiDir(
                        m_rNetTestSyncGui->x_pathDownloadTemp);
        }
   
        //
        // In Dir
        //
        {
            ref<UiContainer> rUi = 
                m_rNetTestSyncGui->
                    NewUiNetTestSyncGuiDir(
                        m_rNetTestSyncGui->x_pathReceptionFolder);
        }
    }
}
