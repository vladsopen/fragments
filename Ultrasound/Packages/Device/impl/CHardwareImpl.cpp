// CHardwareImpl.cpp

#include "CUsmainProject.h"
#include "CHardwareImpl.h"
#include "CDeviceForEmulator.h"
#include "CDeviceForWire.h"
#include "CDeviceForUscan.h"
#include "CRawCapture.h"
#include "CDeviceForLegacy.h"
#include "CDetailTypeIface.h"
#include "CCustomizeHardwareImpl.h"
#include "CRawSlotGp.h"
#include "CDocIface.h"
#include "CMultiPlayImpl.h"
#include "CHardwareAsPlayerOnly.h"
#include "VirtOs_CAppGp.h"
#include "VirtOs_CGlobalUi.h"
#include "CConpadBarIface.h"
#include "CSpeedometerImpl.h"
#include "CSendClusterIfaceGp.h"
#include "CIoControlIfaceGp.h"

CHardwareImpl::CHardwareImpl()
{
    this->_x_rCustomizeHardware = ref<CCustomizeHardwareImpl>();

    _init_AttachProperties();
}

CHardwareImpl::~CHardwareImpl()
{
    rASSERT(!_m_bHardwareInitOk);
}

void CHardwareImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachProperties();

    rFAIL("make sure it is OK to expose CHardwareImpl!");
}

void CHardwareImpl::AddDevice(
        ref<CDeviceCommon> rDeviceCommon)
{ 
    TESTLOG(
        "",
        "Adding a device " + rDeviceCommon->GetDeviceNameForUi() + 
            "\n");

    // attach
    rASSERT(rDeviceCommon->_x_pParentHardware == null());
    rDeviceCommon->_x_pParentHardware = this;

    // append
    m_aDevice.Add(rDeviceCommon); 
}

void CHardwareImpl::OnSetCustomizeHardware(
        ref<CCustomizeHardwareIfaceGp> rCustomizeHardware)
{
    this->_x_rCustomizeHardware = rCustomizeHardware;
    _init_AttachProperties();
}

ref<CDeviceCommon> CHardwareImpl::GetDeviceForClassTests()
{
    rASSERT(debug::GIsTest());

    ref<CDeviceCommon> rDeviceCommon = m_aDevice.GetAt(0);
    rASSERT(Derive<CDeviceForEmulator>(rDeviceCommon) != null());
    return rDeviceCommon;
}

bool CHardwareImpl::OnDispatchNextOnlineSend(
        EScannerDevice eScannerDevice,
        ref<CRawSlotGp> rTargetRawSlot,
        ref<CAudioSliceGp> rAudioSlice,
        ptr<CSendClusterIfaceGp> pSendCluster)
{
    //rASSERT(IsHardwareInitOk());

    if (!debug::GIsTest())
    {
        x_utcLastAudioOrRasterReceived = 
            sys::GGetUtc();
    }

    bool bDone = false;

    // single doc playback active?
    if not_null(
        ref<CDocIface> rDoc = 
            _x_pSinglePlaybackDoc)
    {
        rDoc->
            SendRawAudioCompositesToDoc(
                E_ScannerDevice_Emulator,
                rAudioSlice);
    }

    if (IsHardwareInitOk())
    {
        // Send through All devices
        ref<CDeviceCommon> rIterDeviceCommon;
        for (
            iter i;
            this->
                IterateDevices(
                    out i,
                    out rIterDeviceCommon);)
        {
            if (rIterDeviceCommon->
                    DispatchNextOnlineSendThroughRawCaptures(
                        eScannerDevice,
                        rTargetRawSlot,
                        rAudioSlice,
                        pSendCluster))
            {
                bDone = true;
            }
        }
    }

    // profiling
    if (rTargetRawSlot->x_iScannerChannel == 0 &&
        rTargetRawSlot->x_iScannerCluster == 0)
    {
        if not_null(
            ref<CDocIfaceGp> rFirstDoc = 
                rTargetRawSlot->_x_pFirstDocDispatch)
        {
            x_rSpeedometer->
                MeasureNextAudioReception(
                    rAudioSlice,
                    rFirstDoc);
        }
        else
        {
            if (!debug::GIsTest())
            {
                //FAIL("raw audio[0:0] not sent to any doc, cannot measure speed!");
            }
        }
    }

    return bDone;
}

void CHardwareImpl::OnTestClass()
{
    {
        // check destructors
        assertoff aoNoInit1("!_m_bHardwareInitOk");
        assertoff aoNoInit2("!_m_bDeviceInitOk");
        assertoff aoNoInit3("!IsNull()");

        ref<CHardwareImpl> rHardware;
        rHardware->_test_RunOnHardware();
    }
}

void CHardwareImpl::_test_RunOnHardware()
{
    CProject::EIdleNext eIdleNext = CProject::E_IdleNext_StandBy;

    {
        assertoff aoNoInit("IsHardwareInitOk()");

        // Offline
        TESTLOG("", "\n" "CHardwareImpl offline idle without device:\n")

        HandleIdleIsOnlineMode(out eIdleNext);
        ASSERT(eIdleNext == CProject::E_IdleNext_StandBy);

        // Add a device
        TESTLOG("", "\n" "CHardwareImpl add a device:\n")
        AddDevice(
            ref<CDeviceForEmulator>());

        // Bad Online
        TESTLOG("", "\n" "CHardwareImpl online no init:\n")
        m_aDevice.GetAt(0)->StartRecordingInAllCaptures();
        TESTLOG("", "\n" "online idle:\n")
        HandleIdleIsOnlineMode(out eIdleNext);
        ASSERT(eIdleNext == CProject::E_IdleNext_StandBy);

        {
            // check destructor
            assertoff aoNoInitDevice("!_m_bDeviceInitOk");

            m_aDevice.RemoveAll();
        }
    }

    // Init
    TESTLOG("", "\n" "CHardwareImpl Correct Init:\n");
    InitHardwareForNewSession();

    {
        assertoff aoNoInit("!_m_bDeviceInitOk");

        // Online
        TESTLOG("", "\n" "CHardwareImpl online no init:\n")
        m_aDevice.GetAt(0)->StartRecordingInAllCaptures();
        TESTLOG("", "\n" "online idle:\n")

        // without doc
        {
            assertoff aoNoDocs("bDocFound");
            HandleIdleIsOnlineMode(out eIdleNext);
            ASSERT(eIdleNext == CProject::E_IdleNext_RealtimeLoop);
            eIdleNext = CProject::E_IdleNext_StandBy;
        }

        // Offline
        TESTLOG("", "\n" "CHardwareImpl offline:\n")
        m_aDevice.GetAt(0)->StopRecordingInAllCaptures();
        TESTLOG("", "\n" "online idle again:\n")
        eIdleNext = CProject::E_IdleNext_StandBy;
        HandleIdleIsOnlineMode(out eIdleNext);
        ASSERT(eIdleNext == CProject::E_IdleNext_StandBy);

[...]