// CTomographCommon.cpp

#include "CProject.h"
#include "CTomographCommon.h"
#include "CDeviceManagerImpl.h"
#include "CUsbFrameFromTomograph.h"
#include "CDownloaderIface.h"
#include "CCurveIface.h"
#include "SharedConst.h"

CTomographCommon::CTomographCommon()
{
}

void CTomographCommon::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CTomographCommon!");
}

bool CTomographCommon::ReportSiUsbError(
        str sFunction,
        int nSiError,
        sloc sFriendlyFinalStatus,
        str sMoreErrorInfo)
{
    rASSERT(sFunction != "");
    rASSERT(sFriendlyFinalStatus != "");

    // dont forget /ERR:XXXX/
    rASSERT(sMoreErrorInfo.FindPos('/') >= 0 && sMoreErrorInfo.FindPos(':') > 0);

    if (nSiError == C_nSiSuccess)
    {
        return false; // nothing bad to report
    }
    else
    {
        x_richUsbError = 
            RichSize(30, sFriendlyFinalStatus) + 
                RichSize(
                    18,
                    "\n" + 
                    sFunction + ": " + 
                    StringizeSiUsbErrorCode(nSiError) + 
                    (sMoreErrorInfo != ""? "\n" + sMoreErrorInfo : ""));

        TESTLOG("", " *** SiUsbError *** " + x_richUsbError + "\n");

        return true; // reporting an error
    }
}

void CTomographCommon::ParseTomographFrameAndCallbackWithResults(
        ref<CUsbFrameFromTomograph> rUsbFrameFromTomograph,
        ref<CDownloaderIface> rDownloader)
{
    ref<CDeviceManagerImpl> rDeviceManager = 
        _x_pDeviceManager.Get().Valid();

    sbuf sbufRawFrame = rUsbFrameFromTomograph->x_sbufRawUsbFrame;
    int nFrameLength = sbufRawFrame.Len();
    if (!rUsbFrameFromTomograph->IsCompleteUsbFrameFromTomograph() ||
        nFrameLength != GetTomographFrameTotalBytes())
    {
        ReportTomographCallErrorOnce(
            " [Cannot parse incomplete frame " + 
                Str(sbufRawFrame.Len()) + " bytes] ");

        return;
    }

    // Parse frame header
    rASSERT(x_nTomographFrameHeaderBytes == 4);
    int iParseByte = 0;
    int nFrameCountLowByte = (unsigned char) sbufRawFrame.GetAt(iParseByte);
    iParseByte++;
[...]

    // Check for inconsistencies
    str sFormatError;

    // Alexey had it fixed, no checks needed
    nFrameCountHighByte; nFrameCountLowByte;
    /*
    // Check FrameCount
    int nFrameCount = SHORT_HILO(nFrameCountHighByte, nFrameCountLowByte);
    nFrameCount = 0;
    if (nFrameCount < CUsbFrameFromTomograph::C_iFirstTomographFrameIndex)
    {
        sFormatError = 
            " [Invalid frame count: " + Str(nFrameCount) + "] ";
    }
    */

    // Old format?
    if (ReportSiUsbError(
            "Parse-Frame",
            nSignatureEe == 0xff && nSignature00 == 0x00?
                C_nSiResponseParserError 
                :
                C_nSiSuccess,
            TRANSLATE(
                "Tomograph [... UI-string-removed]");
    }

    // Check Signature
    if (nSignatureEe != 0xee || nSignature00 != 0x00)
    {
        sFormatError = 
            " [Invalid frame signature:"
                " 0x" + Hex00(nSignatureEe, 2) + 
                " 0x" + Hex00(nSignature00, 2) + 
                "] ";
    }

    // Report any errors
    if (ReportSiUsbError(
            "Parse-Frame",
            sFormatError != ""?
                C_nSiResponseParserError 
                :
                C_nSiSuccess,
            TRANSLATE(
                "Receiving [... UI-string-removed]");
    }

    // The following is coded completely different from the Tomographics test code
    //   to be sure there are no errors in the data interprepation.

    // Convert data stream pointer to an array of short-int channel pairs
    CASSERT(sizeof(int) == 4);
    int* pnAmplitudePair = (int*)
        (sbufRawFrame.c() + x_nTomographFrameHeaderBytes);
    int nAmplitudePairCount = 
        x_nTomographFrameDataBytes / sizeof(int);
    rASSERT(
        int(nAmplitudePairCount * sizeof(int)) == 
            x_nTomographFrameDataBytes);

    TESTLOG(
        "DownloadDetails",
        "Going to parse " + Str(nAmplitudePairCount) + 
            " amplitude pairs from the frame.\n");

    // Parse the data stream and send it to the app
    repeat(iPair, nAmplitudePairCount)
    {
        // Next pair as 4 bytes
        int nAmplitudePair = pnAmplitudePair[iPair];
        CASSERT(sizeof(nAmplitudePair) == 4);

        // Exract shorts
        int nAmplitudeAt0 = (unsigned short) INT_LO(

[...]

            // Finalize this Tomogram in the downloader
            rDownloader->
                HandleNextTomogramComplete(
                    rDeviceManager);

            break;
        }
    }
}

