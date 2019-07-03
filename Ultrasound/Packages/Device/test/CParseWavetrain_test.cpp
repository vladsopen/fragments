// CParseWavetrain_test.cpp

#include "CProject.h"
#include "CParseWavetrain.h"
#include "CRandomIfaceGp.h"

static sbuf GEmulateReadDevice(
        sbuf sbufParse,
        int nReadBytes,
        out int& out_nReading)
{
    rASSERT(nReadBytes > 0);

    sbuf sbufRead = (sbuf)
        sbufParse.
            GetMid(out_nReading, nReadBytes);

    out_nReading += nReadBytes;

    // padding
    if (sbufRead != "" &&
        sbufRead.Len() < nReadBytes)
    {
        sbuf sbufPad = StrChar('/', nReadBytes);
        sbufRead = sbufPad.GetOverlay(sbufRead, 0);
    }

    return sbufRead;
}

void CParseWavetrain::_test_GParse(
        const array<str>& asInserts)
{
    sbuf C_sbufSource = (sbuf)
        "Idiocy is the new smartness (:";
      // 0123456789.123456789.123456789[30]
      // [_)(_][.)(.][-)(-][=)(=][:)(:]

    const int C_nAlarmSwitchInBlock = 2;

    //
    // Prepare Formatted Source
    //

    int nActiveClusters = 2;
    int nUsefulPerCluster = 3;
    int nBlocks = 5;
    int nSourceLength = C_sbufSource.Len();
    ASSERT(nSourceLength == nBlocks * nUsefulPerCluster * nActiveClusters);

    sbuf sbufParse;
    str sJunk;
    array<sbuf> asbufExpectedBlocks;
    repeat(iBlock, nBlocks + 1)
    {
        // insert junk
        str sInsert = asInserts.GetAtOr(iBlock, "");
        sJunk += "<" + sInsert + ">";

        if (iBlock < nBlocks)
        {
            sbuf sbufExpectedBlock;

            repeat(iCluster, nActiveClusters)
            {
                // header
                sbuf sbufHeader = 
                    CParseWavetrain::GComposeEmulatedHeader(
                        iCluster,
                        iBlock >= C_nAlarmSwitchInBlock?
                            0 // alarm on
                            :
                            -1, // alarm off
                        (iBlock + 1));

                sbufParse += sbufHeader;

                // useful data
                sbuf sbufUseful = (sbuf)
                    C_sbufSource.
                        GetMid(
                            iBlock * nUsefulPerCluster +
                                iCluster * nUsefulPerCluster,
                            nUsefulPerCluster);

                sbufParse += sbufUseful;
                sbufExpectedBlock += sbufUseful;
            }

            asbufExpectedBlocks.Add(sbufExpectedBlock);
        }
    }

    //
    // Init Parser
    //

    ref<CParseWavetrain> rParseWavetrain;
    sbuf sbufClustersJoined = 
        StrChar(
            '*', 
            nUsefulPerCluster * 
                C_nAskBlocksAtOnce
            );

    //TRACE((sJunk + "\n").c());

    //
    // Parse Loop
    //

    sbuf sbufResult;
    int iReceivingBlock = 0;
    sbuf sbufCollectBlock;
    int nReading = 0;
    int iPrevBufferId = -1;
    while (true)
    {
        int nNextRead = 
            rParseWavetrain->
                GetNextReadBytes(
                    nActiveClusters,
                    nUsefulPerCluster);

        sbuf sbufRead = 
            GEmulateReadDevice(
                sbufParse,
                nNextRead,
                out nReading);

        if (sbufRead == "")
        {
            break;
        }

        int iBufferId = -1;
        if (rParseWavetrain->
                ParseIncomingStreamIntoJoinedClusterBuffers(
                    nActiveClusters,
                    nUsefulPerCluster,
                    sbufRead,
                    out sbufClustersJoined,
                    out iBufferId))
        {
            if (iBufferId <= iPrevBufferId)
            {
                iReceivingBlock++;
                sbufCollectBlock = "";
            }

            sbufCollectBlock += sbufClustersJoined;

            sbuf sbufExpect = asbufExpectedBlocks.GetAt(iReceivingBlock);
            rASSERT(
                sbufExpect.GetLeft((iBufferId + 1)) == 
                sbufCollectBlock.GetLeft((iBufferId + 1) * nUsefulPerCluster));
            sbufResult += sbufClustersJoined;

            // check alarm change
            int fAlarms = rParseWavetrain->x_fLastAlarmsStatus;
            fAlarms <<=
                CParseWavetrain::C_nStreamHeaderAlarmBitLeftShift;
            ASSERT(
                (fAlarms & F_StreamHeader_AlarmBits) == 
                    (iReceivingBlock >= C_nAlarmSwitchInBlock?
                        0
                        :
                        F_StreamHeader_AlarmBits));

            iPrevBufferId = iBufferId;
        }
    }

    rASSERT(sbufResult == C_sbufSource);
}

void CParseWavetrain::OnTestClass()
{
    _test_GParse(array<str>() << "" << "" << "" << "" << "");
    _test_GParse(array<str>() << "R" << "" << "" << "" << "");
    _test_GParse(array<str>() << "12" << "" << "" << "" << "");
    _test_GParse(array<str>() << "123" << "" << "" << "" << "");
    _test_GParse(array<str>() << "1234" << "" << "" << "" << "");
    _test_GParse(array<str>() << "12345" << "" << "" << "" << "");
    _test_GParse(array<str>() << "123456" << "" << "" << "" << "");
    _test_GParse(array<str>() << "1234567" << "" << "" << "" << "");

    _test_GParse(
        array<str>() << 
            StrChar('R', 8) << 
            StrChar('R', 21) << 
            StrChar('R', 28) << 
            StrChar('R', 24) << 
            StrChar('R', 6) << 
            StrChar('R', 25));

    _test_GParse(array<str>() << "" << "R" << "" << "" << "");
    _test_GParse(array<str>() << "" << "" << "R" << "" << "");
    _test_GParse(array<str>() << "1" << "2" << "" << "" << "");
    _test_GParse(array<str>() << "1" << "22" << "" << "" << "");
    _test_GParse(array<str>() << "1" << "2" << "333" << "" << "");
    _test_GParse(array<str>() << "" << "2222" << "3" << "" << "");
    _test_GParse(array<str>() << "" << "333" << "4444" << "55555" << "666666");
    _test_GParse(array<str>() << "111" << "222" << "" << "333" << "444");
    _test_GParse(array<str>() << "1234567" << "8" << "9" << "00" << "111111111111111");
    _test_GParse(array<str>() << "11" << "22" << "333333333333333333" << "1" << "444");
    _test_GParse(array<str>() << "1" << "2233333333333333333" << "" << "44444444444" << "");

    // half-header test
    {
        int iCluster = 0;
        int iBlock = 0;
        int fAlarmAndClusterIndex = iCluster;
        ASSERT(!(fAlarmAndClusterIndex & ~F_StreamHeader_ValidClusterIndexBits));
        fAlarmAndClusterIndex |= F_StreamHeader_AlarmBits;
        fAlarmAndClusterIndex |= F_StreamHeader_ReservedTestNonZero;
        sbuf sbufHeader = (sbuf) StrChar('X', E_StreamHeader_count);
        sbufHeader.SetAt(E_StreamHeader_0x55, (char) 0x55);
        sbufHeader.SetAt(E_StreamHeader_0xaa, (char) 0xab);
        sbufHeader.SetAt(E_StreamHeader_AlarmAndClusterIndex, (char) fAlarmAndClusterIndex);
        sbufHeader.SetAt(E_StreamHeader_AddressHighByte, (char) (iBlock + 1));

        _test_GParse(
            array<str>() << 
                sbufHeader << "" << sbufHeader + sbufHeader << "111" << "");
        _test_GParse(
            array<str>() << 
                "123" + sbufHeader << sbufHeader << sbufHeader << sbufHeader << sbufHeader);
    }

    //
    // Stress test
    //

    int iLoop = 0;
    //math::GRandomizeGlobalRandomSeed16bit(12345);
    ref<CRandomIfaceGp> rRandom;
    rRandom->RandomizeToSeed(12345);

    while (true)
    {
        iLoop++;

        if (iLoop % 1000 == 0)
        {
            // **** END STRESS TEST ****
            break;

            TRACE(("-----Loop " + Str(iLoop) + "....\n").c());
        }

        array<str> asJunk;
        repeat(iBlock, 10)
        {
            int nJunks = rRandom->GenerateNextRandomInt(0, 35);
            if (rRandom->GenerateNextRandomInt(0, 5) == 3)
            {
                nJunks = 0;
            }
            else
            if (rRandom->GenerateNextRandomInt(0, 3) == 1)
            {
                nJunks = rRandom->GenerateNextRandomInt(0, 5) * 3;
            }
            else
            if (rRandom->GenerateNextRandomInt(0, 4) == 1)
            {
                nJunks = rRandom->GenerateNextRandomInt(0, 3) * 4;
            }

            asJunk.Add(StrChar('R', nJunks));
        }

        _test_GParse(asJunk);
    }

[...]

