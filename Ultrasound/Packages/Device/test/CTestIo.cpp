// CTestIo.cpp

#include "CProject.h"
#include "CTestIo.h"
#include "CIoHole.h"
#include "CIoControlImpl.h"
#include "CIntercomClientImpl.h"
#include "CDeviceSessionImpl.h"
#include "CUsbForEmulator.h"
#include "CUsbCommon.h"
#include "CPostDeviceState.h"
#include "CClientSessionImpl.h"
#include "CGlobalAppTypeIfaceGp.h"
#include "CMandateIfaceGp.h"

CTestIo::CTestIo()
{
}

void CTestIo::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CTestIo!");
}

void CTestIo::OnTestClass()
{
    debug::GRedirectTestLog(
        CIoHole::C_sIoHoleTestLog,
        debug::GGetTestLogRedirect(""));

    ref<CIoControlImpl> rIoControl;
    ref<CDeviceSessionImpl> rDeviceSession;
    ref<CIntercomClientImpl> rIntercomClient = 
        rIoControl->x_rIntercomClient;
    ref<CUsbForEmulator> rUsbForEmulator;

    ref<CClientSessionImpl> rClientSession;
    rIoControl->x_rClientSession = 
        rClientSession;

    // switch session to USB emulator
    //rDeviceSession->x_rUsb = 
        //rUsbForEmulator;
    rDeviceSession->x_rUsbHardware = rUsbForEmulator;
    rDeviceSession->_x_rUsbActive = rUsbForEmulator;

    // switch itercom to local device session 
    //   because globals are forbidden for us
    rIntercomClient->_x_pTestDeviceSession = 
        rDeviceSession;

    //
    // Alive?
    //

    TESTLOG(
        "",
        "\n"
            "---- Alive? ----"
            "\n\n");

    rASSERT(
        rIoControl->
            IsAlive() 
                !=
                C_nAliveSignature);

    //
    // Open bypassing DeviceSession
    //

    /*
    TESTLOG(
        "",
        "\n"
            "---- I/O Open (Direct) ----"
            "\n\n");

    rASSERT(rIoControl->GetOpenError() != "");
    str sError;
    rASSERT(
        rIoControl->
            OpenXbus(
                out sError));
    rASSERT(sError == "No parallel init attempted. /ERR:PANI/");
    rASSERT(rIoControl->GetOpenError() != "");
    */

    type<CGlobalAppTypeIfaceGp> typeGlobalAppType = 
        type<CGlobalAppForTests>();
    ref<CMandateIfaceGp> rMandate;
    rMandate->x_typeGlobalAppType = typeGlobalAppType;

    //
    // Open via ClientSession
    //

    TESTLOG(
        "",
        "\n"
            "---- I/O Open (via DeviceSession - Fail on firmware) ----"
            "\n\n");

    {
        assertoff aoNoBurn("bSuccededAndNotLocked");

        rDeviceSession->
            InitDeviceSession(
                rMandate);

        rASSERT(rIoControl->GetOpenError() == "");
    }

    rASSERT(
        rIoControl->
            IsAlive() 
                ==
                C_nAliveSignature);

    /*
    //
    // Open via ClientSession
    //

    TESTLOG(
        "",
        "\n"
            "---- I/O Open (via DeviceSession - Skipping firmware) ----"
            "\n\n");

    {
        assertoff aoNoBurn("sEmulatorInitError == \"\"");

        // force reinit byt firmware will be skipped
        rUsbForEmulator->x_bUsbInitCalled = false;

        rDeviceSession->
            InitDeviceSession(
                rMandate);

        rASSERT(rIoControl->GetOpenError() == "");
    }

    //
    // ReOpen
    //

    TESTLOG(
        "",
        "\n"
            "---- I/O ReOpen ----"
            "\n\n");

    sError = "";
    rASSERT(
        rIoControl->
            OpenXbus(
                out sError));
    rASSERT(sError == ""); // was open by session call "USB already open");
    rASSERT(rIoControl->GetOpenError() == "");
    */

    //
    // Queue
    //

    TESTLOG(
        "",
        "\n"
            "---- Channel Count ----"
            "\n\n");

    rIoControl->
        SetScannerChannelCount(
            2);

    //
    // Queue
    //

    TESTLOG(
        "",
        "\n"
            "---- Get Delay ----"
            "\n\n");

    rASSERT(
        rIoControl->
            GetAudioDelayInMilliseconds()
            ==
            0);

    //
    // Queue
    //

    TESTLOG(
        "",
        "\n"
            "---- I/O Queue Check ----"
            "\n\n");

    rASSERT(
        rIoControl->
            GetXbusQueueBytes()
            ==
            0);

    //
    // Write
    //

    TESTLOG(
        "",
        "\n"
            "---- I/O Write ----"
            "\n\n");

    sbuf sbufWrite = (sbuf) "123456";
    rASSERT(
        rIoControl->
            WriteXbus(
                sbufWrite));

    //
    // Queue
    //

    TESTLOG(
        "",
        "\n"
            "---- I/O Queue Check 2 ----"
            "\n\n");

    rASSERT(
        rIoControl->
            GetXbusQueueBytes()
            ==
            sbufWrite.Len());

    //
    // Read
    //

    TESTLOG(
        "",
        "\n"
            "---- I/O Read ----"
            "\n\n");

    sbuf sbufRead;
    rASSERT(
        rIoControl->
            ReadXbus(
                out sbufRead,
                sbufWrite.Len()));
    rASSERT(sbufRead == sbufWrite);

    //
    // BitBang
    //

    TESTLOG(
        "",
        "\n"
            "---- I/O BitBang ----"
            "\n\n");

    rASSERT(
        rIoControl->
            SetXbusBitBang(
                true,
                222));

    //
    // Get State 
    //

    TESTLOG(
        "",
        "\n"
            "---- I/O GetState ----"
            "\n\n");

    rASSERT(
        rIoControl->
            GetXbusBusState()
                ==
                222);

    //
    // Online Mode
    //

    TESTLOG(
        "",
        "\n"
            "---- Online ----"
            "\n\n");

    rIoControl->
        SetOnlineMode(
            true);

    //
    // Alarm ON
    //

    TESTLOG(
        "",
        "\n"
            "---- Enable Idle Scan ----"
            "\n\n");

    rIoControl->
        EnableIdleDeviceStatusScan(
            true);

    //
    // Close
    //

    TESTLOG(
        "",
        "\n"
            "---- I/O Close ----"
            "\n\n");

    rASSERT(
        rIoControl->
            CloseXbusDevice());
    str sCloseError = 
        rIoControl->GetOpenError();
    //rASSERT(sCloseError == "");

    TESTLOG(
        "",
        "\n"
            "Error: <" + sCloseError + ">"
            "\n\n");

    //
    // Audio
    //

    TESTLOG(
        "",
        "\n"
            "---- Audio ----"
            "\n\n");

    TESTLOG("", "Reset Filters..." "\n");
    {
        assertoff aoPercent("nVolumeInPercents <= 100");
        assertoff aoFilter1("m_nSamplingFreqInHz == 32001");
        assertoff aoFilter2("numCutoffFreq < numSamplingFreq / 2");
        assertoff aoFilter3("numFraction < 0.5 && numFraction > 0");
        rIoControl->
            SetAudioVolumeScannerScaleAndSampling(
                110,
                1000,
                12345);
    }

    TESTLOG("", "Left - ON..." "\n");
    rIoControl->
        EnableAudioChannelLeft(
            true);

    TESTLOG("", "Right OFF..." "\n");
    rIoControl->
        EnableAudioChannelRight(
            false);

    TESTLOG("", "Select (wrong) iClusterBuffer..." "\n");
    {
        assertoff aoBufferRange(
            "iPlayClusterBuffer >= -1 && iPlayClusterBuffer < C_nMaxClusterBuffers");
        assertoff aoChannelNumber(
            "iChannel >= 0 && iChannel < C_nMaxChannels");
        assertoff aoClusterNumber(
            "iCluster >= 0 && iCluster < C_nMaxClustersPerChannel");
        assertoff aoClusterBufferNumber(
            "iClusterBuffer >= 0 && iClusterBuffer < C_nMaxClusterBuffers");

        rIoControl->
            SelectAudioChannelAndCluster(
                50,
                25);
    }

    TESTLOG("", "Monitor - ON..." "\n");
    rIoControl->
        SetAudioForMonitorMode(
            true);

    //
    // Close/TurnOff via ClientSession
    //

    TESTLOG(
        "",
        "\n"
            "---- I/O TurnOff and Terminate (via DeviceSession) ----"
            "\n\n");

    rDeviceSession->TerminateDeviceSession();

[...]