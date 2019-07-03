// UiNetSyncOutStatus.cpp

#include "CNetProject.h"
#include "CNetSyncOutImpl.h"
#include "CNetBinalIface.h"
#include "CSkinizerCommonIfaceGp.h"
#include "CNetLocatorIfaceGp.h"
#include "CNetSyncCommon.h"
#include "CNetUiConfigIface.h"
#include "CNetGlobalConfigIface.h"

void UiNetSyncOutStatus::Ui()
{
    int nLocalOutboxFiles = 
        m_rNetSyncOut->
            GetLastLocalOutboxFileCount();

    big nnOutboxBytes = 
        m_rNetSyncOut->
            GetLastLocalOutboxFileBytes();
    big nnOutboxKb = 
        nnOutboxBytes / 1024;

    int nUploadedFiles = 
        m_rNetSyncOut->
            GetLastLocalUploadedFileCount();

    ref<CNetUiConfigIface> rNetUiConfig = 
        CNetProject::GGetIt().x_rNetGlobalConfig->x_rNetUiConfig;

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rNetSyncOut->_m_ptrmapUiNetSyncOutStatus += rUi;

        rUi->SetTimerInTicks(2000);

        // align with the centered Easy Display
        rUi->SetAlignContentToCenterX();

        m_rNetSyncOut->
            x_rNetSyncCommon->
                EqualizeSyncDirectionProgressBar(
                    rUi);

        rUi->OnTimer = [=]
        {
            rUi->UpdateTreeState();
        }

        rUi->OnMouse = [=]
        {
            if (GetOnMouseEvent() == E_Mouse_LeftButtonUp &&
                sys::GIsAccess(...))
            {
                m_rNetSyncOut->x_bShowSyncOutDebug ^= 1;
                rUi->InvalidateContent();
            }
        }
    }

    if (rNetUiConfig->_x_bVerbosePumpStatus)
    {
        //
        // Status
        //
        {
            ref<CUiStaticGp> rUi;

            str sSamples = RichBold(Str(nLocalOutboxFiles));
            rUi->SetText(
                str() +
                    m_rNetSyncOut->
                        x_rNetLocator->
                            _x_slocRecordsInUploadQueue
                    /*
                    TRANSLATE(
                        "Records in[... UI-string-removed]");

            rUi->SetFontSize(
                m_rNetSyncOut->x_nNetSyncOutFontSize);

            rUi->SetTip(
                TRANSLATE(
                    "Current se[... UI-string-removed]");
        }

        NL();
    }

    //
    // Progress Bar
    //
    {
        ref<CUiProgressGp> rUi;

        if (nLocalOutboxFiles == 0)
        {
            nUploadedFiles = 0;
        }

        rUi->SetTotal(nUploadedFiles + nLocalOutboxFiles);
        rUi->SetDone(nUploadedFiles);
    }

    if (m_rNetSyncOut->x_bShowSyncOutDebug)
    {
        NL();

        //
        // Session
        //
        {
            ref<UiContainer> rUi = 
                m_rNetSyncOut->
                    x_rNetBinalForSyncOut->
                        NewUiNetBinalMain();
        }
    }
}
