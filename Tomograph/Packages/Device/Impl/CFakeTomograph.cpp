// CFakeTomograph.cpp

#include "CProject.h"
#include "CFakeTomograph.h"
#include "CTomographUsb.h"
#include "CTomographEmulator.h"
#include "CFakeTomographMemory.h"
#include "CMarkerIface.h"
#include "CTomogramIface.h"
#include "CMarkerListIface.h"
#include "CCurveIface.h"
#include "CUsbFrameFromTomograph.h"
#include "CChannelTypeIface.h"

CFakeTomograph::CFakeTomograph()
{
}

void CFakeTomograph::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CFakeTomograph!");
}

bool CFakeTomograph::OpenFakeTomograph(
        ref<CTomographEmulator> rTomographEmulator)
{
    //x_rFakeTomographMemory->LoadFakeTomographMemory();
    x_rFakeTomographMemory->
        EnsureFakeTomographMemoryLoaded(
            rTomographEmulator);

    return x_bUsbPlugedIn; // && x_bTomographPowered;
}

rich CFakeTomograph::GetMainHelpStatusLine()
{
    if (!x_bUsbPlugedIn)
    {
        return 
            RichTextColor(            
                rgb(200, 30, 30),
                TRANSLATE(
                    "Please con[... UI-string-removed]");
    }

    /*
    if (!x_bTomographPowered)
    {
        return 
            RichTextColor(            
                rgb(180, 60, 30),
                TRANSLATE(
                    "Please tur[... UI-string-removed]");
    }
    */

    if (!x_bTomographUsbOpen)
    {
        return 
            RichTextColor(            
                rgb(10, 150, 10),
                TRANSLATE(
                    "Device is [... UI-string-removed]");
    }
    else
    {
        return 
            RichTextColor(            
                rgb(10, 10, (x_iBlinkCycle % 2)? 210 : 100),
                TRANSLATE(
                    "Device is [... UI-string-removed]");
    }
}

sbuf CFakeTomograph::ReadNextFakeTomographFrame(
        ref<CTomographEmulator> rTomographEmulator)
{
    //
    // Send fake data back
    //

    if (x_bReplyWithRandomSamples)
    {
        FAIL("todo");
    }

    // source tomogram(s)

    ref<CTomogramIface> rCurrentTomogram = 
        x_rFakeTomographMemory->
            GetTomogramAtOr(
                x_iSendingTomogram, 
                ref<CTomogramIface>());

    ref<CTomogramIface> rNextTomogram = 
        x_rFakeTomographMemory->
            GetTomogramAtOr(
                x_iSendingTomogram + 1, 
                ref<CTomogramIface>());

    sbuf sbufFrame;
    sbufFrame.ReserveAlloc(
        rTomographEmulator->GetTomographFrameTotalBytes());

    //
    // Create a header
    //

    // Frame index
    {
        int iFrameCount = x_iFrameCount;
        rASSERT(iFrameCount < 32000 && iFrameCount >= 0);
        sbufFrame.AppendChar((char) SHORT_LO(iFrameCount));
        sbufFrame.AppendChar((char) SHORT_HI(iFrameCount));
        iFrameCount++;
        x_iFrameCount = iFrameCount;
    }

    // Signature (fake!)
    sbufFrame.AppendChar(x_bObsoleteHeaderSignature? '\xff' : '\xee');
    sbufFrame.AppendChar('\x00');
    
    rASSERT(
        sbufFrame.Len() == rTomographEmulator->x_nTomographFrameHeaderBytes);

    //
    // Append the Signal
    //

    // Calculate amplitude per frame ratio
    CASSERT(sizeof(int) == 4);
    const int C_nAmplitudePairCount = 
        rTomographEmulator->x_nTomographFrameDataBytes / sizeof(int);
    rASSERT(
        int(C_nAmplitudePairCount * sizeof(int)) == 
            rTomographEmulator->x_nTomographFrameDataBytes);

    ref<CTomogramIface> rSourceTomogram = rCurrentTomogram;
    bool bOutOfValidData = false;
    int iReadAt = x_iOffsetInSendingTomogram;
    repeat(iPoint, C_nAmplitudePairCount)
    {
        // Try to read next amplitude
        int nAmplitudeAt0 = -1;
        int nAmplitudeAt1 = -1;

        if (!bOutOfValidData)
        {
            if (!rSourceTomogram->
                    TryReadAmplitudePairAt(
                        iReadAt,
                        out nAmplitudeAt0,
                        out nAmplitudeAt1))
            {
                // Switch to the next tomogram

                // test B)
                x_iSendingTomogram += 1;
                //x_iSendingTomogram = x_iSendingTomogram + 1;
[...]

