// CUcomDriverDeviceForEmulator.cpp

#include "CProject.h"
#include "CUcomDriverDeviceForEmulator.h"
#include "CUcomDriverSlotForEmulator.h"
#include "CUcomEmulatorCommon.h"
#include "CUcomDeviceIfaceGp.h"
#include "CRangesIface.h"
#include "CPostural.h"
#include "CRandomIfaceGp.h"

CUcomDriverDeviceForEmulator::CUcomDriverDeviceForEmulator()
{
}

void CUcomDriverDeviceForEmulator::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CUcomDriverDeviceForEmulator!");
}

void CUcomDriverDeviceForEmulator::ConnectToEmulatorSlot(
        ref<CUcomDriverSlotForEmulator> rUcomDriverSlotForEmulator,
        ref<CUcomDeviceIfaceGp> rUcomDevice,
        out str& out_sError)
{
    rUcomDevice;

    //
    // Connection function is not virtual.
    //   It called from 
    //     virtual CUcomDriverSlotForEmulator::OnNewUcomDeviceForSlot()
    //   and is typesafe.
    //
    // A Ucom device is opened upon client's call to 
    //     NewUcomDeviceMatchingAt()
    //   and must get the device ready to read/write or return an error status.
    //
    // Use connection information saved into the slot object by your enumeratior
    //   (usually device index or PnP path is enough to identify it here)
    //

    str sDevicePath = rUcomDriverSlotForEmulator->x_sDetectedDevicePath;

    if (sDevicePath == "")
    {
        out_sError = 
            "Emulator open error " + 
                //sys::GGetLastOsError() + 
                " <" + sDevicePath + "> "
                "/ERR:KDOE/";
    }

    // init time sync
    m_utcThisSecondStart = sys::GGetUtc();
    m_nnSentThisSecond = 0;
    m_nnNeedToSend = 0;

    // init fixed random seq
    m_rRandom->
        RandomizeToSeed(0);
}

void CUcomDriverDeviceForEmulator::OnCloseUcomDeviceDriver()
{
    //if (_m_hHidFileRead != INVALID_HANDLE_VALUE)
    //{
    //    CloseHandle(_m_hHidFileRead);
    //    _m_hHidFileRead = INVALID_HANDLE_VALUE;
    //}

    _m_nInputQueueLengthLastSet = -1;
}

void CUcomDriverDeviceForEmulator::OnSetUcomDeviceDriverReadAndWriteTimeouts(
        tick tickReadTimeout,
        tick tickWriteTimeout)
{
    tickReadTimeout;
    tickWriteTimeout;

    // NOP 

    /*
    // Set timeouts
    str sError;
    if (ReportEmulatorDeviceError(
            FT_SetTimeouts(
                _m_hEmulatorDevice,
                GTickToInt(tickReadTimeout), 
                GTickToInt(tickWriteTimeout)),
            "Emulator communication timeouts could not be set. " 
                "/ERR:UFTM/",
            out sError))
    {
        rFAIL(sError);
    }
    */
}

void CUcomDriverDeviceForEmulator::OnSetInputQueueLength(
        int nCount)
{
    // update only if changed
    if (_m_nInputQueueLengthLastSet != nCount)
    {
        _m_nInputQueueLengthLastSet = nCount;

        //HidD_GetNumInputBuffers(
        //    _m_hHidFileRead,
        //    &ulongCheck));
    }
}

void CUcomDriverDeviceForEmulator::OnOutputToUcomDeviceDriver(
        EUcomIoRequest eUcomIoRequest,
        sbuf sbufPacket,
        int nReportId,
        out int& out_nBytesWritten,
        out str& out_sError)
{
    if (eUcomIoRequest == E_UcomIoRequest_HidFeature)
    {
        TESTLOG(
            "",
            "Emulator sends SetHidFeature "
                "(" + Str(nReportId) + ")<" + 
                sbufPacket.GetReplaced('\0', '`') + 
                ">\n")

        //if (!SetHidFeature(
        //{
        //    out_sError = 
        //        "Failed to output " + 
        //}

        out_nBytesWritten = sbufPacket.Len();
        out_sError = "";
    }
    else
    if (eUcomIoRequest == E_UcomIoRequest_Generic)
    {
        TESTLOG(
            "",
            "Emulator sends packet " + 
                "(" + Str(nReportId) + ")<" + 
                sbufPacket.GetReplaced('\0', '`') + 
                ">\n")

        //if (!WriteFile(
        //{
        //    out_sError = 
        //        "Failed to output " + 
        //}

        out_nBytesWritten = sbufPacket.Len();
        out_sError = "";
    }
    else
    {
        rFAIL("unknown eUcomIoRequest");
    }
}

void CUcomDriverDeviceForEmulator::OnGetQueueStatusFromUcomDeviceDriver(
        out int& out_nBytesReady,
        out EUcomDriverQueueStatus& out_eUcomDriverQueueStatus)
{
    out_nBytesReady;

    // not implemented for Emulator
    out_eUcomDriverQueueStatus = 
        CUcomDriverDeviceIfaceGp::E_UcomDriverQueueStatus_NotAvailable;

    //
    // Here we should do all the necessary software queue checks
    //   and return the number of bytes already ready to be read
    //

    //// defaults
    //out_eUcomDriverQueueStatus = E_UcomDriverQueueStatus_NotAvailable; // Emulator :(
    //out_nBytesReady = -1;

    //// Check Receive Queue
    //str sError;
    //if (ReportEmulatorDeviceError(
    //        FT_GetQueueStatus(
    //            _m_hEmulatorDevice, 
    //            (DWORD*) &out_nBytesReady),
    //        "Cannot check input queue for Emulator device. " 
    //            "/ERR:UFQC/",
    //        out sError))
    //{
    //    rFAIL(sError);
    //    return;
    //}
}

sbuf CUcomDriverDeviceForEmulator::GenerateNextEmulatedInputPacketOrEmpty()
{
    int nSamplesPerSecond = C_nSamplesPerSecond + 1;

    // a second passed?
    utc utcNow = sys::GGetUtc();
    if (utcNow - m_utcThisSecondStart > 1000)
    {
        // defer all samples we have not sent yet for the later
        m_nnNeedToSend += 
            math::GMulDiv(
                (big) nSamplesPerSecond,
                GTickToBig(utcNow - m_utcThisSecondStart),
                (big) 1000)
                - 
                m_nnSentThisSecond;

        // start next second
        m_nnSentThisSecond = 0;
        m_utcThisSecondStart = utcNow;
    }

    // limit to the buffer length
    if (m_nnNeedToSend > _m_nInputQueueLengthLastSet)
    {
        m_nnNeedToSend = _m_nInputQueueLengthLastSet;
    }

    big nnNeedThisSecond = 
        math::GMulDiv(
            GTickToBig(utcNow - m_utcThisSecondStart),
            (big) nSamplesPerSecond,
            (big) 1000);

    if (m_nnNeedToSend > 0)
    {
        m_nnNeedToSend--;
    }
    else
    if (nnNeedThisSecond > m_nnSentThisSecond)
    {
        m_nnSentThisSecond++;
    }
    else
    {
        return sbuf("");
    }

    return 
        DoGenerateNextEmulatedInputPacket();
}

sbuf CUcomDriverDeviceForEmulator::DoGenerateNextEmulatedInputPacket()
{
    point pointEmulate = point();
    size sizeRange = sys::GGetPrimaryMonitorResolution();

    if (!debug::GIsTest())
    {
        pointEmulate = sys::GGetMousePosAsynchronously();
    }
    else
    {
        pointEmulate = 
            point(
                m_rRandom->GenerateNextRandomInt(-10, 1000),
                m_rRandom->GenerateNextRandomInt(-10, 1000));
    }

    ref<CRangesIface> rRanges =
        x_rEmulateRanges;

    int nPosX = (int)
        math::GMulDiv(
            (num) pointEmulate.x,
            rRanges->x_numMaxXmm - 
                rRanges->x_numMinXmm,
            (num) sizeRange.w)
            +
            (int) rRanges->x_numMinXmm;

    int nPosY = (int)
        math::GMulDiv(
            (num) sizeRange.h - pointEmulate.y,
            rRanges->x_numMaxYmm - 
                rRanges->x_numMinYmm,
            (num) sizeRange.h)
            +
            (int) rRanges->x_numMinYmm;

    num numKilo = 
        math::GMulDiv(
            (num) pointEmulate.x + pointEmulate.y,
            200.,
            (num) sizeRange.w + sizeRange.h);

    // const values will force redetect
    numKilo += 
        m_rRandom->GenerateNextRandomInt(-10, 10) / 10.;
    int nDelta = 2;
    nPosX += 
        m_rRandom->GenerateNextRandomInt(-nDelta, nDelta);
    nPosY += 
        m_rRandom->GenerateNextRandomInt(-nDelta, nDelta);

    str sFormat = 
        "% " + Str(CPostural::C_nPosturalReadingHexAsciiChars) + "x";

    sbuf sbufPackage = sbuf(
        str() + 
            StrChar('=', CPostural::C_nPosturalReadPackageStartOffset) +
            CPostural::C_sPosturalResponseWeight + 
            StrFormat(
                int(numKilo * CPostural::C_nPosturalReadingFixedPoint),
                sFormat.c()) + 
            CPostural::C_sPosturalResponseWeight + 
            StrFormat(
                nPosX * CPostural::C_nPosturalReadingFixedPoint,
                sFormat.c()) + 
            CPostural::C_sPosturalResponseWeight + 
            StrFormat(
                nPosY * CPostural::C_nPosturalReadingFixedPoint,
                sFormat.c())
            );

    return sbufPackage;
}

[...]