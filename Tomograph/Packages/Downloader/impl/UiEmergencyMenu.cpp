// UiEmergencyMenu.cpp

#include "CProject.h"
#include "CDownloaderImpl.h"
#include "CDeviceManagerIface.h"
#include "VirtOs_LegacyResource.h"

void UiEmergencyMenu::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rDownloader->_m_pUiEmergencyMenu = rUi;
    }

    //
    // Erase Device
    //
    {
        ref<CUiMenuItemGp> rUi;

        rUi->SetText(
            TRANSLATE(
                "Erase Tomo[... UI-string-removed]");

        rUi->SetIcon(RES__ERASER_16x16());

        rUi->OnClick = [=]
        {
            ACT("Erasing...", 0, 0);

            m_rDeviceManager->EraseInEmergency();
        }
    }

    NL();

    //
    // Open Test
    //
    {
        ref<CUiMenuItemGp> rUi;

        rUi->SetText("Open Tomographics Test");

        rUi->SetIcon((res) RES_TINY_SETTING);

        rUi->OnClick = [=]
        {
            ACT("Searching...", 0, 0);

            sys::GOpenGuiProgram(
                CProject::GGetIt().
                    GetSharedStoragePath().
                        GetAppendedPath("Modules").
                        GetAppendedPath("Tomographics015.exe"),
                sys::E_OpenWindowState_Normal);
        }
    }
}
