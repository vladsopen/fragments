// CUportTestPacketConnector.cpp

#include "CUportProject.h"
#include "CUportTestPacketConnector.h"
#include "CUportPacketConnector.h"
#include "CRandomIfaceGp.h"
#include "CUportLog.h"

CUportTestPacketConnector::CUportTestPacketConnector()
{
    _init_AttachToUportTestPacketConnector();
}

void CUportTestPacketConnector::_init_AttachToUportTestPacketConnector()
{
    //x_r->_x_p = this;
}

void CUportTestPacketConnector::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToUportTestPacketConnector();

    rFAIL("make sure it is OK to expose CUportTestPacketConnector!");
}

static int GFindFirstNotEmpty(
        int iNext,
        array<sbuf>& asbufUsb,
        int nJumps)
{
    // find from iNext
    if (iNext >= 0)
    for (int i = iNext; i < asbufUsb.GetCount(); i++)
    {
        if (asbufUsb.GetAt(i) != "")
        {
            return i;
        }
    }

    if (nJumps > 0)
    {
        // find first not sent
        repeat(iUsb, asbufUsb.GetCount())
        {
            if (asbufUsb.GetAt(iUsb) != "")
            {
                return iUsb;
            }
        }
    }

    return -1;
}

static void _test_GRunConnectorTest(
        ref<CUportPacketConnector> rUportPacketConnector,
        bool bUi,
        int nSeed,
        int nPackages,
        int nRandomNotSend,
        int nRandomOffset,
        int nRandomSkipReceive,
        int nMaxJumps, 
        int nRandomJump,
        int nRandomJumpAccelerated,
        bool bAllowIllegalJumps)
{
    nRandomSkipReceive;

    rUportPacketConnector->
        InitUportPacketConnector();

    ref<CUportLog> rUportLog;
    rUportLog->InitUportLog();

    ref<CRandomIfaceGp> rRandom;
    rRandom->RandomizeToSeed(nSeed);

    TESTLOG(
        "",
        "\n"
            "nPackages: " + Str(nPackages) + "\n"
            "nSeed: " + Str(nSeed) + "\n"
            "x_nCounterWrapsAt: " + Str(rUportPacketConnector->x_nCounterWrapsAt) + "\n"
            "x_nFlushHolesAfter: " + Str(rUportPacketConnector->x_nFlushHolesAfter) + "\n"
            "x_nReassemblyBufferSize: " + Str(rUportPacketConnector->x_nReassemblyBufferSize) + "\n"
            );

    //
    // Init source signal
    //

    array<sbuf> asbufUsb;
    asbufUsb.SetCount(nPackages);
    repeat(i, nPackages)
    {
        if (((i + 1) & 65535) == 0)
        {
            ACT("Allocating Packages...", i, nPackages);
        }

        asbufUsb.SetAt(i, (sbuf) Str(i));
    }

    int nReceived = 0;
    int nSent = 0;
    int nAddLoops = 5;
    str sLastReceived = Str(-1);
    int nAfterRandomJump = 0;
    int nJumps = 0;
    int nValidReceived = 0;
    int nValidInRow = 0;
    static tick s_tickLastPump = 0;
    int nMaxRecoveryTime = 0;
    int nAverageRecoveryTime = 0;
    int nCurrentRecoveryTime = 0;
    int nWeirdErrors = 0;
    int iNextLogical = 0;
    int nErrorsPerSecond = 0;
    int nJumpsPerSecond = 0;
    int nCompleteRecoveries = 0;
    int nTotalLost = 0;
    while (true)
    {
        // abort when all sent and received and extra dummy-dumped
        if (nSent >= nPackages && nReceived >= nPackages)
        {
            if (!nAddLoops--)
            {
                break;
            }
        }

        //
        // Send
        //

        int nOffsetToSend = 
            rRandom->
                GenerateNextRandomInt(
                    -nRandomNotSend,
                    nRandomOffset);

        if (nOffsetToSend >= 0)
        {
            // find first not sent
            int iOrigNext = GFindFirstNotEmpty(iNextLogical, asbufUsb, nJumps);
            if (iOrigNext == -1)
            {
                iNextLogical = 0;
                iOrigNext = GFindFirstNotEmpty(iNextLogical, asbufUsb, nJumps);
            }

            int iNext = iOrigNext;

            if (nSent >= nPackages)
            {
                rASSERT(nSent == nPackages);
                rASSERT(iNext == -1);
            }
            else
            {
                rASSERT(iNext >= 0);

                // random offset if possible
                int iRandomNext = iNext + nOffsetToSend;

                bool bJumping = false;
                if ((nRandomJump || nRandomJumpAccelerated)
                    &&
                    (bAllowIllegalJumps ||
                        (
                            nAfterRandomJump == 0 &&
                            nValidReceived > rUportPacketConnector->x_nFlushHolesAfter
                        )
                    )
                    &&
                    rRandom->
                        GenerateNextRandomInt(
                            0,
                            nRandomJump + 
                                nRandomJumpAccelerated / (nSent | 1))
                        ==
                        0)
                {
                    if (nJumps < nMaxJumps)
                    {
                        iRandomNext = 
                            rRandom->
                                GenerateNextRandomInt(
                                    0,
                                    asbufUsb.GetCount() - 1);

                        bJumping = true;
                    }
                }

                if (iRandomNext < asbufUsb.GetCount() &&
                    nSent > 0)
                {
                    iNext = iRandomNext;
                }
                else
                {
                    bJumping = false;
                }

                if (bJumping)
                {
                    iNextLogical = iNext;
                    nAfterRandomJump = 
                        rUportPacketConnector->x_nFlushHolesAfter * 3 + 1;
                    nJumps++;
                    nJumpsPerSecond++;
                }

                sbuf sbufSend = asbufUsb.GetAt(iNext);
                if (sbufSend == "")
                {
                    iNext = iOrigNext;
                    sbufSend = asbufUsb.GetAt(iNext);
                }

                asbufUsb.SetAt(iNext, sbuf(""));

                rASSERT(Str(iNext) == sbufSend);

                if (iNext < iNextLogical)
                {
                    iNextLogical = iNext;
                }

                rUportPacketConnector->
                    AddUnimodPackage(
                        iNext % rUportPacketConnector->x_nCounterWrapsAt,
                        sbufSend,
                        rUportLog);
                nSent++;

                TESTLOG(
                    "",
                    " >" + sbufSend + "\n");
            }
        }

        //
        // Receive
        //

        /*
        if (rRandom->
                GenerateNextRandomInt(
                    0,
                    nRandomSkipReceive)
                == 0)
                */
        {
            int nLost = -1;
            str sReceive = 
                rUportPacketConnector->
                    PopPackageIfReady(
                        out nLost,
                        rUportLog);
            rASSERT(nLost >= 0);
            nTotalLost += nLost;

            if (sReceive != "")
            {
                nReceived++;
                rASSERT(nReceived <= nPackages);
                rASSERT(nReceived <= nSent);

                int nExpect = sLastReceived.ParseDecimal() + 1 + nLost;

                if (nExpect == sReceive.ParseDecimal())
                {
                    if (nValidInRow > rUportPacketConnector->x_nFlushHolesAfter)
                    {
                        if (nCurrentRecoveryTime > 0)
                        {
                            nCompleteRecoveries++;
                        }

                        if (nCurrentRecoveryTime > 0)
                        {
                            nAverageRecoveryTime = 
                                (nAverageRecoveryTime * 10 + nCurrentRecoveryTime) / 11;
                        }

                        nCurrentRecoveryTime = 0;
                    }

                    nValidInRow++;
                }
                else
                {
                    nValidInRow = 0;

                    nCurrentRecoveryTime++;
                    if (nCurrentRecoveryTime > nMaxRecoveryTime)
                    {
                        nMaxRecoveryTime = nCurrentRecoveryTime;

                        if (!nRandomJump)
                        {
                            rASSERT(
                                nMaxRecoveryTime < 
                                    rUportPacketConnector->x_nCounterWrapsAt);
                        }

                        if (nMaxRecoveryTime > s_nMaxRecoveryTime)
                        {
                            s_nMaxRecoveryTime = nMaxRecoveryTime;
                        }
                    }
                }

                if (nAfterRandomJump > 0)
                {
                    nAfterRandomJump--;
                    nValidReceived = 0;
                }
                else
                {
                    if (nExpect != sReceive.ParseDecimal())
                    {
                        if (!nRandomJump)
                        {
                            rASSERT(nExpect == sReceive.ParseDecimal());
                        }

                        nWeirdErrors++;
                        s_nWeirdErrors++;
                        nErrorsPerSecond++;
                    }

                    nValidReceived++;
                }

                sLastReceived = sReceive;

                TESTLOG(
                    "",
                    "        <" + sReceive + "\n");
            }
            else
            if (nJumps + nWeirdErrors + nTotalLost > 0 &&
                nSent >= nPackages &&
                nReceived < nPackages)
            {
                // if we've kade jumps we may not receive all
                nReceived = nPackages;
                TESTLOG(
                    "",
                    "Will not wait full data because of errors.\n");
            }
        }

        if (bUi &&
            sys::GGetUtc() - s_tickLastPump > 1000)
        {
            if (CProject::GGetIt().
                    PumpMessagesWithinTimeout(1) 
                    ==
                    CProject::E_MessagePump_WmQuitReceived
                ||
                (sys::GIsAccess(...) && 
                    sys::GIsAccess(...)))
            {
                return;
            }

            s_tickLastPump = sys::GGetUtc();

            ACT(
                "Stream test sent " + Str(nSent) + "\n"
                    "\n"
                    " at: " + sLastReceived + "\n"
                    "\n"
                    " wrap: " + Bytes(rUportPacketConnector->x_nCounterWrapsAt) + "\n"
                    " flush: " + Bytes(rUportPacketConnector->x_nFlushHolesAfter) + "\n"
                    " memory: " + Bytes(rUportPacketConnector->x_nReassemblyBufferSize) + "\n"
                    "\n"
                    " nPackages: " + Bytes(nPackages) + "\n"
                    " nRandomOffset: " + Bytes(nRandomOffset) + "\n"
                    " nMaxJumps: " + Bytes(nMaxJumps) + "\n"
                    " nRandomJump: " + Bytes(nRandomJump) + "\n"
                    " nRandomJumpAccelerated: " + Bytes(nRandomJumpAccelerated) + "\n"
                    " bAllowIllegalJumps: " + Str(bAllowIllegalJumps) + "\n"
                    "\n"
                    " Total Jumps:\v\t" + Bytes(nJumps) + "\n"
                    " Jumps/s:\v\t" + Bytes(nJumpsPerSecond) + "\n"
                    " Complete Recoveries:\v\t" + Bytes(nCompleteRecoveries) + "\n"
                    " Single Errors:\v\t" + Bytes(nWeirdErrors) + "\n"
                    " Ever Errors:\v\t" + Bytes(s_nWeirdErrors) + "\n"
                    " Lost:\v\t" + Bytes(nTotalLost) + "\n"
                    " Errors/s:\v\t" + Bytes(nErrorsPerSecond) + "\n"
                    " Recover in:\v\t" + Bytes(nMaxRecoveryTime) + "\n"
                    " Average Recovery:\v\t" + Bytes(nAverageRecoveryTime) + "\n"
                    " Ever Recover in:\v\t" + Bytes(s_nMaxRecoveryTime) + "\n"
                    "\n"
                    "(Ctrl-Esc stops)\n",
                nSent,
                nPackages);

            nErrorsPerSecond = 0;
            nJumpsPerSecond = 0;
        }
    }

    TESTLOG(
        "",
        "\n"
            "nMaxRecoveryTime: " + Str(nMaxRecoveryTime) + "\n"
            "nWeirdErrors: " + Str(nWeirdErrors) + "\n"
            "nTotalLost: " + Str(nTotalLost) + "\n"
            "nJumps: " + Str(nJumps) + "\n"
            );

    rASSERT(nReceived == nPackages);
    rASSERT(nReceived == nSent);

    rUportLog->ShowLog();
}

void GTestLoops(
        bool bUi,
        int nLoops,
        int nSeed,
        int nPackagesMax,
        int nMaxJumps,
        int nRandomJump)
{
    ref<CRandomIfaceGp> rRandom;
    rRandom->RandomizeToSeed(nSeed);

    repeat(i, nLoops)
    {
        ref<CUportPacketConnector> rUportPacketConnector;

        rUportPacketConnector->x_nCounterWrapsAt = 
            rRandom->
                GenerateNextRandomInt(
                    16,
                    16 + i * i);

        rUportPacketConnector->x_nFlushHolesAfter = 
            rRandom->
                GenerateNextRandomInt(
                    8,
                    math::GMax(8, rUportPacketConnector->x_nCounterWrapsAt / 4));

        rUportPacketConnector->x_nReassemblyBufferSize = 
            rRandom->
                GenerateNextRandomInt(
                    rUportPacketConnector->x_nFlushHolesAfter + 1,
                    rUportPacketConnector->x_nFlushHolesAfter * 3 + i);

        int nPackages = 
            rRandom->
                GenerateNextRandomInt(
                    0,
                    nPackagesMax);

        TESTLOG(
            "",
            "\n" 
                "Run [" + Str(i) + "]\n" +
                "   wrap: " + Str(rUportPacketConnector->x_nCounterWrapsAt) + "\n"
                "  flush: " + Str(rUportPacketConnector->x_nFlushHolesAfter) + "\n"
                " memory: " + Str(rUportPacketConnector->x_nReassemblyBufferSize) + "\n"
                "\n"
                " nPackages: " + Str(nPackages) + "\n"
                " (nPackagesMax: " + Str(nPackagesMax) + ")\n"
                "\n");

        int nRandomNotSend = 
            rRandom->
                GenerateNextRandomInt(
                    0,
                    10);

        int nRandomOffset = 
            rRandom->
                GenerateNextRandomInt(
                    0,
                    rUportPacketConnector->x_nFlushHolesAfter / 2);

        int nRandomSkipReceive = 
            rRandom->
                GenerateNextRandomInt(
                    0,
                    5);

        _test_GRunConnectorTest(
            rUportPacketConnector,
            bUi,
            // nSeed
            i,
            // nPackages,
            nPackages,
            // nRandomNotSend,
            nRandomNotSend,
            // nRandomOffset,
            nRandomOffset,
            // nRandomSkipReceive
            nRandomSkipReceive,
            nMaxJumps,
            nRandomJump,
            //nRandomJumpAccelerated
            0,
            // bAllowIllegalJumps,
            false);
    }
}

void GRunUportPacketConnectorRandomStressTest()
{
    static bool s_bInside = false;
    if (s_bInside)
    {
        s_bInside = false;
        return;
    }

    s_bInside = true;

    int nMax = 10 * 1000;
    repeat(i, nMax)
    {
        //ACT("Running UportPacketConnector Stress Test [" + Str(i) + "]", i, nMax);

        GTestLoops(
            // bUi
            true,
            i % 1000, 
            ref<CRandomIfaceGp>()->GenerateNextRandomInt(0, 1000000),
            // packages
            (100 + i * i) % 10000,
            // nMaxJumps
            1,
            i % 1000);

        if (i % 10 == 0)
        {
            if (CProject::GGetIt().
                    PumpMessagesWithinTimeout(1) 
                    ==
                    CProject::E_MessagePump_WmQuitReceived
                ||
                !s_bInside)
            {
                break;
            }
        }
    }
}

void GRunUportPacketConnectorStreamStressTest(
        bool bIdealFirst,
        bool bRareJumps,
        bool bNoJumps)
{
    static bool s_bInside = false;
    if (s_bInside)
    {
        s_bInside = false;
        return;
    }

    s_bInside = true;

    ref<CRandomIfaceGp> rRandom;
    rRandom->RandomizeToSeed(6546546);

    s_nMaxRecoveryTime = 0;
    s_nWeirdErrors = 0;

    int nMax = 10 * 1000;
    repeat(i, nMax)
    {
        ACT("Running UportPacketConnector Stream Stress Test [" + Str(i) + "]", i, nMax);

        ref<CUportPacketConnector> rUportPacketConnector;

        // defaults!
        //rUportPacketConnector->x_nCounterWrapsAt = 65536;
        //rUportPacketConnector->x_nFlushHolesAfter = 
        //rUportPacketConnector->x_nReassemblyBufferSize = 

        TESTLOG(
            "",
            "\n" 
                "Run [" + Str(i) + "]\n" +
                "   wrap: " + Str(rUportPacketConnector->x_nCounterWrapsAt) + "\n"
                "  flush: " + Str(rUportPacketConnector->x_nFlushHolesAfter) + "\n"
                " memory: " + Str(rUportPacketConnector->x_nReassemblyBufferSize) + "\n"
                "\n");

        _test_GRunConnectorTest(
            rUportPacketConnector,
            // bUi
            true, 
            // nSeed
            i,
            // nPackages,
            1000000,
            // nRandomNotSend,
            10,
            // nRandomOffset,
            bIdealFirst?
                0
                :
                6 /*rRandom->
                    GenerateNextRandomInt(
                        1,
                        rUportPacketConnector->x_nFlushHolesAfter / 3)*/,
            // nRandomSkipReceive
            10,
            // nMaxJumps
            100000000,
            // nRandomJump
            bNoJumps? 100000000 : bRareJumps? 10000 : 10,
            // nRandomJumpAccelerated,
            bNoJumps || bRareJumps? 0 : 10000000,
            // bAllowIllegalJumps
            true);

        //if (i % 10 == 0)
        {
            if (CProject::GGetIt().
                    PumpMessagesWithinTimeout(1) 
                    ==
                    CProject::E_MessagePump_WmQuitReceived
                ||
                !s_bInside)
            {
                break;
            }
        }
    }
}

void CUportTestPacketConnector::OnTestClass()
{
    GTestLoops(
        // bUi,
        false,
        50, 
        // seed
        12345, 
        // packages
        100, 
        // nMaxJumps
        1, 
        // Jump probability
        30);

    GTestLoops(
        // bUi,
        false,
        150, 
        // seed
        4567, 
        // packages
        13, 
        // nMaxJumps
        10, 
        // Jump probability
        31);
[...]