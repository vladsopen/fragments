// CSendAlarm.cpp

#include "CProject.h"
#include "CSendAlarm.h"
#include "CTaskIface.h"
#include "VirtOs_CAppGp.h"
#include "CShellAutoDeleteTempIfaceGp.h"
#include "CIoCommonIface.h"
#include "VirtOs_file.h"
#include "CLogTypeIface.h"
#include "CSenderSessionImpl.h"
#include "CRandomIfaceGp.h"
#include "CPatientLinkIface.h"
#include "CLoaderIface.h"
#include "CAuthDeviceReceiverIface.h"
#include "VirtOs_ask.h"
#include "CNetLocatorIfaceGp.h"
#include "CNetDriverSendatumIfaceGp.h"
#include "CNetPumpInIfaceGp.h"
#include "CNetProtocolAbstractIfaceGp.h"
#include "CFeedIoProtocol.h"
#include "CQuestHandler.h"
#include "CFileMultipartSplitIfaceGp.h"

CSendAlarm::CSendAlarm()
{
    _init_AttachToSendAlarm();
}

CSendAlarm::~CSendAlarm()
{
    rASSERT(!_m_bSendAlarmOpened);
}

void CSendAlarm::_init_AttachToSendAlarm()
{
    x_rQuestHandler->_x_pSendAlarm = this;
}

void CSendAlarm::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToSendAlarm();

    rFAIL("make sure it is OK to expose CSendAlarm!");
}

void CSendAlarm::InitSendAlarm()
{
    rASSERT(!_m_bOneTimeInitSendAlarmOk);
    _m_bOneTimeInitSendAlarmOk = true;
    _m_bSendAlarmOpened = true;

    x_rCurrentTask->x_sTaskType =  
        CTaskIface::C_sTaskTypeIsAlarm;

    x_rCurrentTask->x_sAlarm = 
        CTaskIface::C_sAlarmIsEcgArrhythmia;

    x_rCurrentTask->x_sAlarmClass = 
        CTaskIface::C_sAlarmClassIsPause;

    x_rCurrentTask->x_nBatteryPercent = 100;

    RenderFlashEcg();

    x_rQuestHandler->
        InitQuestHandler();

    _m_ontimerAlarmUpload.
        SetTimeoutInTicks(
            500,
            this);
}

void CSendAlarm::CloseSendAlarm()
{
    if (_m_bSendAlarmOpened)
    {
        _m_bSendAlarmOpened = false;

        x_rQuestHandler->
            CloseQuestHandler();
    }
}

void CSendAlarm::RenderFlashEcg()
{
    // Fake ECG data.
    // Patient table view is smart enough to switch to text mode.
    // It will be reported as corrupt data with ASCIIable preview in debug mode.

    str sRender;

    ref<CRandomIfaceGp> rRandom;

    int nInLine = 0;
    repeat(iPoint, 20)
    {
        static array<str> s_asCurve = 
            array<str>()
                << "-_|_|_"
                << "_/|__|"
                << "_=_|_-"
                << "_|_|_|"
                << "_|__|_"
                << "|_(_|_"
                << "_|_|_|"
                << "-__/__"
                << "_|_|__"
                << "_|__)_"
                << "___|__"
                << "_-_|_|"
                << "|__|__"
                << "-___|_"
                ;

        sRender += 
            s_asCurve.
                GetAt(
                    rRandom->
                        GenerateNextRandomInt(
                            0, 
                            s_asCurve.GetCount() - 1));

        nInLine++;
        if (nInLine > 3)
        {
            nInLine = 0;
            sRender += "\n";
        }
    }

    if (x_sFlashData.Get().Len() > 0)
    {
        x_sFlashData += "\n";
    }

    x_sFlashData += sRender.GetStripBoth(CHARMAP("\n"));

    // update and scroll to the end of the edit box
    ref<CUiPanelGp> rUiBox;
    for (
        iter iBox;
        _m_ptrmapUiEcgBox.Iterate(out iBox, out rUiBox);)
    {
        rUiBox->UpdateTreeLayout();
    }

    // update and scroll to the end of the edit box
    ref<CUiEditGp> rUiEdit;
    for (
        iter iEdit;
        _m_ptrmapUiEcgEdit.Iterate(out iEdit, out rUiEdit);)
    {
        rUiEdit->UpdateTreeLayout();
        rUiEdit->ScrollToShowPoint(
            point(0, rUiEdit->GetInstantSize().h));
    }
}

[...]