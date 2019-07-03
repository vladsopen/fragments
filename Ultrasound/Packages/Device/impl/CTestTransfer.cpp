// CTestTransfer.cpp

#include "CProject.h"
#include "CTestTransfer.h"
#include "CXvendor.h"
#include "CUsbForEmulator.h"
#include "CRawAudioSink.h"
#include "CErrorReporterIface.h"
#include "CIntercomDeviceIface.h"
#include "CSoundIface.h"
#include "CGlobalConfigIface.h"
#include "CDeviceSessionImpl.h"
#include "CIoHole.h"
#include "CGlobalConvertersIface.h"
#include "CRandomIfaceGp.h"
#include "CGlobalAppTypeIfaceGp.h"
#include "CMandateIfaceGp.h"

CTestTransfer::CTestTransfer()
{
}

void CTestTransfer::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CTestTransfer!");
}

void CTestTransfer::RunStressTest()
{
    NOTEST;

    ref<CRandomIfaceGp> rRandom;
    //rRandom->RandomizeToSeed(0);

    static int s_nRuns = 0;
    static int s_nIterations = 0;
    s_nRuns++;
    sys::GDisplayDebugInRelease(
        10,
        50,
        "Raw Read Stress " + 
            Str(s_nRuns) + 
            " runs, total " + 
            Str(s_nIterations) + 
            " iterations   ");

    ref<CDeviceSessionImpl> rDeviceSession;
    ref<CXvendor> rXvendor = 
        rDeviceSession->x_rXvendor;
    ref<CUsbForEmulator> rUsb;
    rDeviceSession->x_rUsbHardware = rUsb;
    rDeviceSession->_x_rUsbActive = rUsb;
    ref<CRawAudioSink> rRawAudioSink = 
        rDeviceSession->x_rRawAudioSink;
    ref<CErrorReporterIface> rErrorReporter = 
        rDeviceSession->x_rErrorReporterForDeviceSession;
    ref<CIntercomDeviceIface> rIntercomDevice = 
        rDeviceSession->x_rIntercomDevice;
    ref<CSoundIface> rSound = 
        rDeviceSession->x_rSound;

    rRawAudioSink->_x_pTestTransfer = this;

    str sError;
    rUsb->InitUsb(out sError, rDeviceSession);
    rASSERT(sError == "");

    rXvendor->x_nPollClusters = 
        rRandom->GenerateNextRandomInt(1, 4);

    // select request size as the base for all numbers
    {
        int iRandom = rRandom->GenerateNextRandomInt(1, 4);
        if (iRandom == 1)
        {
            rXvendor->x_nMinAudioAndRasterRequestBytes = 64;
        }
        else
        if (iRandom == 2)
        {
            rXvendor->x_nMinAudioAndRasterRequestBytes = 128;
        }
        else
        if (iRandom == 3)
        {
            rXvendor->x_nMinAudioAndRasterRequestBytes = 256;
        }
        else
        {
            rXvendor->x_nMinAudioAndRasterRequestBytes = 1024;
        }
    }

    rUsb->x_nAudioBytesPerDeviceCycle = 
        rXvendor->x_nMinAudioAndRasterRequestBytes *
            rRandom->GenerateNextRandomInt(5, 10);

    rUsb->x_nDivisorForPointerInAccumulator = 
        1 << ((rXvendor->x_nMinAudioAndRasterRequestBytes / 64) % 3);

    rUsb->x_nMaxBytesPerRequest = 
        rXvendor->x_nMinAudioAndRasterRequestBytes * 
            rRandom->GenerateNextRandomInt(1, 5) *
            rXvendor->x_nPollClusters;
            */
    rXvendor->x_nPollClusters = C_nMaxClustersPerChannel;
    rXvendor->x_nMinAudioAndRasterRequestBytes = 2048;
    rUsb->x_nAudioBytesPerDeviceCycle = 65536;
    rUsb->x_nDivisorForPointerInAccumulator = 8;
    rUsb->x_nMaxBytesPerRequest = 65536;
    int nMaxElapse = 100; //rUsb->x_nAudioBytesPerDeviceCycle / 17;

    // Enable idle polling
    rDeviceSession->SetDeviceOnlineMode(true);

    // first poll will set read pointer
    rXvendor->
        HandleOnlineIdle(
            rDeviceSession);

    repeat(iOnlineIteration, rRandom->GenerateNextRandomInt(10, 1000))
    {
        s_nIterations++;

        int nElapse = 
            rRandom->GenerateNextRandomInt(
                0, 
                nMaxElapse);

        // move device pointer
        rUsb->
            ElapseVirtualTime(
                nElapse);

        // pool
        rXvendor->
            HandleOnlineIdle(
                rDeviceSession);

        // check result
        CheckInputForAllClusters(rUsb);
    }
}

void CTestTransfer::RunOnlineTest()
{
    ref<CDeviceSessionImpl> rDeviceSession;
    ref<CXvendor> rXvendor = 
        rDeviceSession->x_rXvendor;
    ref<CUsbForEmulator> rUsb;
    rDeviceSession->x_rUsbHardware = rUsb;
    rDeviceSession->_x_rUsbActive = rUsb;
    ref<CRawAudioSink> rRawAudioSink = 
        rDeviceSession->x_rRawAudioSink;
    ref<CErrorReporterIface> rErrorReporter = 
        rDeviceSession->x_rErrorReporterForDeviceSession;
    ref<CIntercomDeviceIface> rIntercomDevice = 
        rDeviceSession->x_rIntercomDevice;
    ref<CSoundIface> rSound = 
        rDeviceSession->x_rSound;

    // we test not-init error here
    assertoff aoNoUsbInit("x_bUsbInitCalled");

    rRawAudioSink->_x_pTestTransfer = this;

    assertoff aoSmallRequest("value % 64 == 0");
    rXvendor->x_nMinAudioAndRasterRequestBytes = 4;
    rXvendor->x_nPollClusters = C_nMaxClustersPerChannel;
    rUsb->x_nMaxBytesPerRequest = 
        10 * rXvendor->x_nPollClusters;

    rUsb->x_nAudioBytesPerDeviceCycle = 18;
    rUsb->x_nDivisorForPointerInAccumulator = 2;

    CProject::GGetIt().
        x_rGlobalConfig->
            _x_nSharedClusterCacheUseBytes = 
                64;

    // make playable portion as small as possible
    CProject::GGetIt().
        x_rGlobalConfig->
            _x_numMinPlayableMilliseconds = 
                0.1; // 17.6 bytes
                // 44100 * 4 * m / 1000.

    // make junctions as small as possible
    CProject::GGetIt().
        x_rGlobalConfig->
            _x_nSmoothJunctionsStereoSamples = 
                2;

    //rDeviceSession->InitDeviceSession();
    ref<CMandateIfaceGp> rMandate;
    rMandate->x_typeGlobalAppType = 
        (type<CGlobalAppTypeIfaceGp>)
            type<CGlobalAppForParted>();
    rDeviceSession->x_rMandate = 
        rMandate;

    // initialization logic for filters
    rSound->
        SetAudioVolumeScannerScaleAndSampling(
            50, // int nVolumePercent,
            4000, // int nScannerScaleHz,
            32001); // int nScannerSampling

    array<int> anElapse;
    anElapse.Add(3);
    anElapse.Add(6);
    anElapse.Add(9);
    anElapse.Add(5);
    anElapse.Add(8);
    anElapse.Add(1);
    anElapse.Add(10);
    anElapse.Add(10);
    anElapse.Add(1);
    anElapse.Add(1);
    anElapse.Add(0);
    anElapse.Add(1);
    anElapse.Add(1);
    anElapse.Add(14);
    anElapse.Add(0);
    anElapse.Add(14);
    anElapse.Add(5);
    anElapse.Add(13);

    // Enable idle polling
    rDeviceSession->SetDeviceOnlineMode(true);

    /*
    // add smth to init read
    rUsb->
        ElapseVirtualTime(8);

    // first poll will set read pointer
    rXvendor->
        HandleOnlineIdle(
            rDeviceSession);

    rUsb->
        ResetRandomByteSequence();
    */

    repeat(iOnlineIteration, anElapse.GetCount())
    {
        int iElapse = anElapse.GetAt(iOnlineIteration);

        if (iOnlineIteration == 1)
        {
            // late open
            rASSERT(!rUsb->IsUsbOpen());

            assertoff aoNoBurn("bSuccededAndNotLocked");
            str sError = "";
            rUsb->InitUsb(out sError, rDeviceSession);
            rASSERT(sError.FindPos("Firmware error") >= 0);
            rASSERT(rUsb->IsUsbOpen());
        }
        else
        if (iOnlineIteration == 2)
        {
            // reopen erroe
            str sError = "";
            rUsb->InitUsb(out sError, rDeviceSession);
            rASSERT(sError.FindPos("USB already open") >= 0);
        }

        if (iOnlineIteration == anElapse.GetCount() - 1)
        {
            // offline
            rDeviceSession->SetDeviceOnlineMode(false);
        }

        // move device pointer
        rUsb->
            ElapseVirtualTime(
                iElapse);

        // pool
        {
            assertoff aoNoSharedBuffer("_m_bSharedWavesInitOk");

            rXvendor->
                HandleOnlineIdle(
                    rDeviceSession);
        }

        // check result
        CheckInputForAllClusters(rUsb);
    }
}

void CTestTransfer::CheckInputForAllClusters(
        ref<CUsbForEmulator> rUsb)
{
    // nothing sent?
    if (!m_asbufRawAudioSendToSinkForClusters.GetCount())
    {
        return;
    }

    int nBytesReceived = -1;
    sbuf sbufIterCluster;
    for(
        iter i;
        m_asbufRawAudioSendToSinkForClusters.
            Iterate(
                out i,
                out sbufIterCluster);)
    {
        if (nBytesReceived == -1)
        {
            nBytesReceived = sbufIterCluster.Len();
        }
        else
        {
            // we receive equal amount for all 
            rASSERT(nBytesReceived == sbufIterCluster.Len());
        }

        CheckClusterRawAudio(i, sbufIterCluster, rUsb);
    }

    m_iRandomBytePosition += nBytesReceived;
    m_asbufRawAudioSendToSinkForClusters.RemoveAll();
}

void CTestTransfer::CheckClusterRawAudio(
        int iCluster,
        sbuf sbufCluster,
        ref<CUsbForEmulator> rUsb)
{
    NOTEST;

    bool bMismatchFound = false;
    sbuf sbufExpected;
    sbufExpected.ReserveAlloc(sbufCluster.Len());
    repeat(iByte, sbufCluster.Len())
    {
        char cPoint = sbufCluster.GetAt(iByte);

        int iClusterBank = 
            CGlobalConvertersIface::GConvertClusterBufferToClusterBank(
                iCluster);

        char cExpected = 
            rUsb->
                GenerateRandomRawAudio(
                    iClusterBank,
                    m_iRandomBytePosition + iByte);

        sbufExpected.AppendChar(cExpected);
        if (cPoint != cExpected)
        {
            rFAIL("raw audio read byte mismatch");
            bMismatchFound = true;
        }
    }

    if (bMismatchFound)
    {
        TESTLOG(
            "",
            "\n******* ERROR BYTE MISMATCH:\n"
                "Expected: " + sbufExpected.GetHexDump(" ") + "\n" +
                "Received: " + sbufCluster.GetHexDump(" ") + "\n");
    }
}

void CTestTransfer::OnHandleRawAudioSendToSinkForCluster(
        int iCluster,
        sbuf sbufRawAudio)
{
    if (iCluster >= m_asbufRawAudioSendToSinkForClusters.GetCount())
    {
        m_asbufRawAudioSendToSinkForClusters.
            SetCount(
                iCluster + 1);
    }

    TESTLOG(
        "",
        "Received audio cluster [" + Str(iCluster) + "]:\n" +
            "   " + sbufRawAudio.GetHexDump(" ") +
            "\n");

    sbuf sbufCluster = 
        m_asbufRawAudioSendToSinkForClusters.
            GetAt(iCluster);

    m_asbufRawAudioSendToSinkForClusters.
        SetAt(  
            iCluster,
            sbuf(sbufCluster + sbufRawAudio));
}

void CTestTransfer::OnTestClass()
{
    debug::GRedirectTestLog(
        CIoHole::C_sIoHoleTestLog,
        debug::GGetTestLogRedirect(""));

    assertoff aoNoCloseUsb("!x_bUsbInitCalled");
    assertoff aoNoCloseSound("close WaveOutQueue object before destructor!");

    RunOnlineTest();
}
