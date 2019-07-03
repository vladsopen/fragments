// CNetTestStreamFormat.cpp

#include "CNetProject.h"
#include "CNetTestStreamFormat.h"
#include "CNetStreamFormatWriter.h"
#include "CNetStreamFormatReader.h"
#include "CTimeVirtualizerIfaceGp.h"
#include "VirtOs_CAppGp.h"
#include "CRandomIfaceGp.h"
#include "VirtOs_CGlobalUi.h"

CNetTestStreamFormat::CNetTestStreamFormat()
{
    _init_AttachToNetTestStreamFormat();
}

void CNetTestStreamFormat::_init_AttachToNetTestStreamFormat()
{
    //x_r->_x_p = this;
}

void CNetTestStreamFormat::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToNetTestStreamFormat();

    rFAIL("make sure it is OK to expose CNetTestStreamFormat!");
}

void CNetTestStreamFormat::GenerateRandomChannelContent(
        out array< array<str> >& out_aasSignals)
{
    out_aasSignals.
        SetCount(
            m_rRandom->GenerateNextRandomInt(0, 10));

    repeat(iSignal, out_aasSignals.GetCount())
    {
        array<str>& aSignal = out_aasSignals._unsafe_ElementAt(iSignal);

        aSignal.
            SetCount(
                m_rRandom->GenerateNextRandomInt(0, 10));

        repeat(iBlock, aSignal.GetCount())
        {
            str sContent;
            sContent.ReserveAlloc(1 * 1024 * 1024);

            int nChars = m_rRandom->GenerateNextRandomInt(0, 17);
            nChars = nChars * nChars; // * nChars;
            repeat(iChar, nChars)
            {
                sContent += 
                    StrChar(
                        (char) m_rRandom->GenerateNextRandomInt(0, 255));
            }

            while (m_rRandom->GenerateNextRandomInt(0, 5) == 1)
            {
                sContent += C_sAddErrorToSubchannel;
            }

            if (m_rRandom->GenerateNextRandomInt(0, 5) == 1)
            {
                sContent = C_sSkipSubchannel;
            }

            aSignal.SetAt(iBlock, sContent);
        }
    }
}

str CNetTestStreamFormat::RunRepack(
        tick tickMaxFlushDelay,
        int nMaxPortions,
        int nMaxPacketDataBytes,
        unused unusedNop)
{
    unusedNop;

    // faster time - we use delta-utc for ticks < 256
    ref<CTimeVirtualizerIfaceGp> rTimeVirtualizer = 
        CAppGp::GGet()->
            x_rTimeVirtualizer;

    // stop time auto-step at all GGetUtc() calls, we advance in manually
    rTimeVirtualizer->
        SetVirtualTimeManualUtcDelta(
            0);

    //
    // ****** Do not abuse control punctuation in examples for log readability!
    //

    array<str> asSignal0 = 
        array<str>() <<
            "Start;" <<
            C_sSkipSubchannel <<
            C_sSkipSubchannel <<
            // --- THIS IS DESIGNED TO BE 255 BYTES! ---
            "Long part which must be split into 256\n"
            "byte parts to satisfy the dblock size limit\n"
            "in the packed block header. I don't implement\n"
            "larger block size in 1.0 because of testing\n"
            "complications! The portion exceeding said limit will\n"
            "start here at closing bracket["
            // --- ---
            "]This will go to the next block" <<
            "{Third" + C_sAddErrorToSubchannel + "}" <<
            C_sSkipSubchannel <<
            "" <<
            "The End" 
            ;

    array<str> asSignal1 = 
        array<str>() <<
            "Begin;" + C_sAddErrorToSubchannel <<
            C_sSkipSubchannel <<
            "After skip." 
            ;

    array<str> asSignal2 = 
        array<str>() <<
            C_sSkipSubchannel <<
            C_sSkipSubchannel <<
            C_sSkipSubchannel <<
            C_sSkipSubchannel <<
            C_sSkipSubchannel
            ;

    array<str> asSignal3 = 
        array<str>() <<
            C_sAddErrorToSubchannel <<
            C_sAddErrorToSubchannel <<
            "" <<
            C_sAddErrorToSubchannel + C_sAddErrorToSubchannel <<
            C_sAddErrorToSubchannel <<
            C_sAddErrorToSubchannel <<
            C_sAddErrorToSubchannel <<
            C_sAddErrorToSubchannel 
            ;

    array<str> asSignal4 = 
        array<str>() <<
            C_sAddErrorToSubchannel + C_sAddErrorToSubchannel <<
            C_sSkipSubchannel <<
            C_sAddErrorToSubchannel <<
            C_sAddErrorToSubchannel <<
            C_sSkipSubchannel <<
            C_sSkipSubchannel <<
            "No more errors"
            ;

    array<str> asSignal5 = 
        array<str>() <<
            "Uninterru" <<
            "Pted" 
            ;

    //array< array<str> >& aasSignals = m_aasSignals;
    array< array<str> > aasSignals;
    aasSignals = 
        array< array<str> >() <<
            asSignal0 <<
            asSignal1 <<
            asSignal2 <<
            asSignal3 <<
            asSignal4 <<
            asSignal5
            ;

    if (m_bGenerateRandomChannelContent)
    {
        m_bGenerateRandomChannelContent = false;

        aasSignals.RemoveAll();
        GenerateRandomChannelContent(
            out aasSignals);
    }

    TESTLOG(
        "",
        "\n"
            "-------- Writing: -----------\n");

    ref<CNetStreamFormatWriter> rWriter;

    // iterate all signals untill all arrays are done
    array<str> asMergedSubchannels;
    array<int> anErrorsInSubchannel;
    asMergedSubchannels.SetCount(aasSignals.GetCount());
    anErrorsInSubchannel.SetCount(aasSignals.GetCount());
    int nPortionsFound = off;
    str sSendingLog;
    str sSignalInfo;
    for (
        int iPortion = 0; 
        nPortionsFound > 0 && iPortion < nMaxPortions; 
        iPortion++)
    {
        sSignalInfo += " [" + Str(iPortion) + "]: ";

        nPortionsFound = 0;
        repeat(iSubchannel, aasSignals.GetCount())
        {
            rTimeVirtualizer->
                AdvanceManualUtc(
                    (iPortion * iSubchannel + 100) % 500);

            array<str>& aSignal = aasSignals._unsafe_ElementAt(iSubchannel);

            // get portion or skip a subchannel
            str sPortion = aSignal.GetAtOr(iPortion, C_sSkipSubchannel);
            if (sPortion == C_sSkipSubchannel)
            {
                continue;
            }

            // count remote errrors
            int nAddErrors = 0;
            while (true)
            {
                str sLessErrors = 
                    sPortion.
                        GetReplaced(C_sAddErrorToSubchannel, "", 1);

                if (sLessErrors == sPortion)
                {
                    break;
                }
                else
                {
                    sPortion = sLessErrors;
                    nAddErrors++;
                }
            }

            // will continue looping
            nPortionsFound++;

            sPortion = sPortion.GetLeft(nMaxPacketDataBytes);

            // collect channel data for comparison with parser
            asMergedSubchannels.
                SetAt(
                    iSubchannel, 
                    asMergedSubchannels.GetAt(iSubchannel) +
                        sPortion);
            anErrorsInSubchannel.
                SetAt(
                    iSubchannel, 
                    anErrorsInSubchannel.GetAt(iSubchannel) +
                        nAddErrors);

            /*
            TRACE((
                sPortion + 
                    "\n ---- Len: " + Str(sPortion.Len()) + 
                    " -30: " + Str(sPortion.Len() - 30) + "\n\n" +
                    " [256]='" + sPortion.GetMid(256, 1) + "', "
                    " remainder-256: " + sPortion.GetTruncateLeft(256) + 
                    "\n\n" + 
                    ""
                    ).c());
                    */

            sSignalInfo += 
                Str(sPortion.Len()) + 
                    (nAddErrors > 0? "/e+" + Str(nAddErrors) + "" : "") + "; ";

            // ensure channel defined
            rWriter->
                SetCurrentNetStreamSubchannel(
                    iSubchannel);

            // set error count
            int nErrors = anErrorsInSubchannel.GetAt(iSubchannel);
            rWriter->
                SetNetStreamErrorCounter(
                    nErrors);

            // wtite to format
            rWriter->
                AppendNetStreamPortion(
                    (sbuf) sPortion,
                    tickMaxFlushDelay);

            sSendingLog +=
                "\n"
                    "Sent ch[" + Str(iSubchannel) + "] "
                    "at " + 
                    moment::GFromTickToUtc(sys::GGetUtc()).ToStr("yyyy-mm-dd HH:MM:SS.TTT") + 
                    " remote errors = " +
                    Str(nErrors) + 
                    "\n"
                    "<<<---" +
                    sPortion +
                    "--->>>\n"
                    ;
        }
    }

    sbuf sbufStream = 
        rWriter->
            FlushAndGetNetSignalStream();

    bool bCorrupted = false;
    if (m_sOverlayCorruption != "")
    {
        sbufStream = 
            sbufStream.
                GetOverlay(
                    m_sOverlayCorruption,
                    0);

        m_sOverlayCorruption = "";
        bCorrupted = true;
    }

    TESTLOG(
        "",
        "\n"
            "-------- Raw resulting stream: -----------\n" + 
            "   Legend: '\\n'=data8, '#'=port, '*'=utc, '+'=dtick, '!'=errors, '?'=ctrl\n" + 
            "\n" + 
            sbufStream.
                GetReplaced(
                    CHARMAP(
                        CTA_MIDZERO_SAFE_WITHOUT_CTA "[\0-\x09][\x0b-\x1f]"), // no \n
                    '?') + 
            "\n" +
            "--------------------------------------------\n"
            "\n");

    // decoded dump
    repeat(i, sbufStream.Len())
    {
        uchar cLog = sbufStream.GetAt(i);
        str sLog = StrChar(cLog);

        // control?
        if (cLog > ' ' && cLog < ',')
        {
            while (i < sbufStream.Len() - 1)
            {
                i++;
                uchar cNext = sbufStream.GetAt(i);
                sLog += "\\x" + Hex00(cNext, 2);

                break;
                /* next one can be next control
                if (cNext > ' ' || i >= sbufStream.Len() - 1)
                {
                    break;
                }
                */
            }
        }

        TESTLOG(
            "",
            sLog);
    }

    TESTLOG(
        "",
        "\n"
            "--------------------------------------------\n"
            "\n");

    TESTLOG(
        "",
        "\n" +
            sSendingLog +
            "\n");

    //
    // Parse
    //

    TESTLOG(
        "",
        "\n"
            "-------- Parsing: -----------\n");

    ref<CNetStreamFormatReader> rReader;

    rReader->
        SetNetSignalSource(
            sbufStream,
            // no NetLite date tested!
            path(""));

    array<str> asReceivedSubchannels;
    array<int> anReceivedSubchannelError;
    asReceivedSubchannels.SetCount(aasSignals.GetCount());
    anReceivedSubchannelError.SetCount(aasSignals.GetCount());

    repeat(iRepeatParse, sbufStream.Len())
    {
        int iSubchannel = off;
        utc utcRemote = null();
        int nErrors = -1;
        str sErrors = "<undefined!!!>";

        sbuf sbufPortion = 
            rReader->
                ParseNextNetSignalPortion(
                    out iSubchannel,
                    out utcRemote,
                    out nErrors,
                    out sErrors);

        if (iSubchannel == CNetStreamFormatReader::C_nNetSignalSubchannelEmpty &&
            sErrors == "")
        {
            break;
        }

        TESTLOG(
            "",
            "\n"
                "Received ch[" + Str(iSubchannel) + "] "
                "at " + 
                moment::GFromTickToUtc(utcRemote).ToStr("yyyy-mm-dd HH:MM:SS.TTT") + 
                " remote errors = " +
                Str(nErrors) + 
                (sErrors == ""? "" : "\n  (ERRORS: " + sErrors + ")") +
                "\n"
                "<<<---" +
                sbufPortion +
                "--->>>\n");

        if (iSubchannel == CNetStreamFormatReader::C_nNetSignalSubchannelEmpty)
        {
            break;
        }

        asReceivedSubchannels.
            SetAt(
                iSubchannel, 
                asReceivedSubchannels.GetAt(iSubchannel) +
                    sbufPortion);

        anReceivedSubchannelError.
            SetAt(
                iSubchannel, 
                nErrors);
    }

    //
    // Dump signals for comparison
    //
    str sSentLog;
    str sReceivedLog;
    repeat(iSignal, aasSignals.GetCount())
    {
        str sIntro = 
            "\n"
                "------ Subchannel [" + Str(iSignal) + "] -------\n"
                ;

        sSentLog += sIntro;
        sReceivedLog += sIntro;

        sSentLog += asMergedSubchannels.GetAt(iSignal);
        sReceivedLog += asReceivedSubchannels.GetAt(iSignal);

        // match error counters
        int nReceivedErrors = 
            anReceivedSubchannelError.
                GetAt(
                    iSignal);
        int nSentErrors = 
            anErrorsInSubchannel.
                GetAt(
                    iSignal);
        if (
            nSentErrors == nReceivedErrors
            //||
            //(nSentErrors > 1 && nSentErrors == nReceivedErrors + 1)
            )
        {
            // OK
            //TRACE(("\n" + "BiiS OK: " + sSignalInfo + "\n").c());
        }
        else
        if (!bCorrupted)
        {
            rFAIL("errs sent=" + Str(nSentErrors) + ", received=" + Str(nReceivedErrors));

            str sReport = 
                "\n" +
                    "BiiS Errors: " 
                    "sent=" + Str(nSentErrors) + ", "
                    "received=" + Str(nReceivedErrors) +
                    " at " +
                    Str(m_iStressLoop) +
                    "\n" +
                    "  " + sSignalInfo +
                    "\n";

            TRACE(sReport.c());
            sys::GOpenTextInNotepad(sReport);
        }
    }

    bool bOk = 
        bCorrupted?
            true
            :
            debug::GCompareReports(
                "NetStreamFormat-match",
                sSentLog,
                sReceivedLog);

    TESTLOG(
        "",
        "\n"
            "-------- Final compare: " + 
            (bOk? "OK" : "Failed, see separate log") + 
            " ----end----.");

    return sSentLog;
}

void CNetTestStreamFormat::OnTestClass()
{
    {
        TESTLOG(
            "",
            "\n\n\n"
                "       MODERATE DELAY\n"
                "       ==============\n"
                "\n");

        RunRepack(
            1000, // tickMaxFlushDelay,
            oo, // nMaxPortions
            off, // nMaxPacketDataBytes
            unused());
    }
    {
        TESTLOG(
            "",
            "\n\n\n"
                "       MAXIMUM DELAY\n"
                "       =============\n"
                "\n");

        RunRepack(
            100000, // tickMaxFlushDelay,
            oo, // nMaxPortions
            off, // nMaxPacketDataBytes
            unused());
    }
    {
        TESTLOG(
            "",
            "\n\n\n"
                "       IMMEDIATE FLUSH\n"
                "       ===============\n"
                "\n");

        RunRepack(
            100000, // tickMaxFlushDelay,
            oo, // nMaxPortions
            off, // nMaxPacketDataBytes
            unused());
    }
    {
        TESTLOG(
            "",
            "\n\n\n"
                "       EMPTY FILE\n"
                "       ==========\n"
                "\n");

        RunRepack(
            1000, // tickMaxFlushDelay,
            0, // nMaxPortions
            off, // nMaxPacketDataBytes
            unused());
    }
    {
        TESTLOG(
            "",
            "\n\n\n"
                "       EMPTY DATA\n"
                "       ==========\n"
                "\n");

        RunRepack(
            1000, // tickMaxFlushDelay,
            oo, // nMaxPortions
            0, // nMaxPacketDataBytes
            unused());
    }
    {
        TESTLOG(
            "",
            "\n\n\n"
                "       SMALLEST FILE\n"
                "       =============\n"
                "\n");

        RunRepack(
            100, // tickMaxFlushDelay,
            1, // nMaxPortions
            off, // nMaxPacketDataBytes
            unused());
    }
    {
        TESTLOG(
            "",
            "\n\n\n"
                "       HEADER VERSION\n"
                "       ==============\n"
                "\n");

        m_sOverlayCorruption = 
            CNetStreamFormatCommon::C_sNetStreamFormatSignatureStart +
                "X";

        RunRepack(
            99, // tickMaxFlushDelay,
            2, // nMaxPortions
            off, // nMaxPacketDataBytes
            unused());
    }
    {
        TESTLOG(
            "",
            "\n\n\n"
                "       HEADER CORRUPTION\n"
                "       =================\n"
                "\n");

        m_sOverlayCorruption = 
            CNetStreamFormatCommon::C_sNetStreamFormatSignatureFull.
                GetOverlay("x", 0);

        RunRepack(
            30, // tickMaxFlushDelay,
            1, // nMaxPortions
            3, // nMaxPacketDataBytes
            unused());
    }

    /*
    {
        int i = 6361271;
        m_iStressLoop = i;
        m_rRandom->RandomizeToSeed(3000000 + i);

        bool bGenerateRandomChannelContent = 
            m_rRandom->GenerateNextRandomInt(0, 1) == 1;
        m_bGenerateRandomChannelContent = bGenerateRandomChannelContent;

        str sResult = 
            RunRepack(
                m_rRandom->GenerateNextRandomInt(0, 3000), // tickMaxFlushDelay,
                m_rRandom->GenerateNextRandomInt(0, 10), // nMaxPortions
                m_rRandom->GenerateNextRandomInt(0, 1000), // nMaxPacketDataBytes
                unused());
    }
    */

    //
    // Stress test
    //

    debug::GDisableTestLog("");

    repeat(i, off)
    {
        m_iStressLoop = i;
        m_rRandom->RandomizeToSeed(3000000 + i);

        bool bGenerateRandomChannelContent = 
            m_rRandom->GenerateNextRandomInt(0, 1) == 1;
        m_bGenerateRandomChannelContent = bGenerateRandomChannelContent;

        str sResult = 
            RunRepack(
                m_rRandom->GenerateNextRandomInt(0, 3000), // tickMaxFlushDelay,
                m_rRandom->GenerateNextRandomInt(0, 10), // nMaxPortions
                m_rRandom->GenerateNextRandomInt(0, 1000), // nMaxPacketDataBytes
                unused());

        if (i % ifdeb(30) ifrel(1000) == 0)
        {
            sResult = 
                sResult.GetReplaced(
                    CHARMAP(
                        CTA_MIDZERO_SAFE_WITHOUT_CTA "[\0-\x09][\x0b-\x1f]" "\\"), // no \n
                    '.');

            ACT(
                "Stress NetTestStreamFormat " + Str(i) + "..." +
                    "\n   (" +
                    (bGenerateRandomChannelContent? "Random" : "Predefined") +
                    ")\n\n" + 
                    CAppGp::GGet()->x_rUi->ConvertPlainTextToValidRichFragment(
                        sResult), 
                0, 
                0);
        }
    }
}
