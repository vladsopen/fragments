// CDebuxPregTests.cpp

#include "CDebuxProject.h"
#include "CDebuxPregTests.h"

CDebuxPregTests::CDebuxPregTests()
{
    _init_AttachToDebuxPregTests();
}

void CDebuxPregTests::_init_AttachToDebuxPregTests()
{
    //x_r->_x_p = this;
}

void CDebuxPregTests::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToDebuxPregTests();

    rFAIL("make sure it is OK to expose CDebuxPregTests!");
}

static void GTestPregFind(
        // in
        preg pregTest,
        str sSubject,
        str sCaptureName,
        // check
        int iStartPos,
        str sMatch,
        int nMatchCount,
        str sCapture)
{
	nMatchCount;
	iStartPos;

    pregfind pregfindTest = pregTest.FindIn(sSubject);

    int iRealStartPos = pregfindTest.GetMatchPos();
    ASSERT(iRealStartPos == iStartPos);

    str sRealMatch = pregfindTest.GetMatchSubstring();
    ASSERT(sRealMatch == sMatch);

    int nRealMatchCount = pregfindTest.CountMatches();
    ASSERT(nRealMatchCount == nMatchCount);

    if (sCaptureName.Len())
    {
        int iCapturePosInSubject = -1;
        str sRealCapture = 
            pregfindTest.
                GetCaptureAndPosByName(
                    sCaptureName,
                    out iCapturePosInSubject);

        ASSERT(sRealCapture == sCapture);

        // check capture pos
        ASSERT(
            sSubject.GetMid(iCapturePosInSubject, sRealCapture.Len()) ==
                sRealCapture);
    }
}

static void GTestPregEscape(
        str sUnsafe,
        str sExpectEscaped,
        const charmap& charmapSkip)
{
    str sActual = 
        preg::GEscapeForPattern(sUnsafe, charmapSkip);

    rASSERT(sActual == sExpectEscaped);
}

/*
void preg::GLinkPregTest()
{
    STATIC_MAGIC_STR(ms__01);
    GTestPregFind(
        preg(MakeMagicStr("", &ms__01)),
        "", // sSubject
        "", // sCaptureName,
        -1, // iStartPos,
        "", // sMatch,
        0, // nMatchCount,
        ""); // sCapture
}
*/

static void GSimpleCaptureTest()
{
    //
    // Test 1
    //

    str sSubject = 
        "win 10 1234 driver released 2016- 6 - 11 provides support for USB 3.0";

    // Pattern names
    #define C_szCaptureYear "Year"
    #define C_szCaptureMonth "Month"
    #define C_szCaptureDay "Day"

    // Template for date extraction in form of year - month - day
    preg pregDate = 
        preg(                                                   preg::EVariablePattern(),
            ""
                "(?P<" C_szCaptureYear ">"
                    "[0-9]{2,4}" 
                ")"

                Preg_space "*" 
                "\\-"
                Preg_space "*" 

                "(?P<" C_szCaptureMonth ">"
                    "[0-9]{1,2}" 
                ")"

                Preg_space "*" 
                "\\-"
                Preg_space "*" 

                "(?P<" C_szCaptureDay ">"
                    "[0-9]{1,2}" 
                ")"
                
                "");

    pregfind pregfindDate = pregDate.FindIn(sSubject);

    if (pregfindDate.IsFound())
    {
        str sYear = pregfindDate.GetCaptureByName(C_szCaptureYear);
        str sMonth = pregfindDate.GetCaptureByName(C_szCaptureMonth);
        str sDay = pregfindDate.GetCaptureByName(C_szCaptureDay);

        ASSERT(sYear == "2016");
        ASSERT(sMonth == "6");
        ASSERT(sDay == "11");
    }
    else
        FAIL("");

    //
    // Patients cmd line parser
    //
        
    #define C_szCaptureRunOnly "RunOnly"
    str sRunOnly = 
        preg(                                                   preg::EVariablePattern(),
            "run_only\\="
                "(?P<" C_szCaptureRunOnly ">"
                    "[A-Za-z0-9_\\|]+" 
                ")").
            FindIn("my.exe /samplers_from_out_deb /run_only=Alpha|Beta").
                GetCaptureByName(C_szCaptureRunOnly);
    rASSERT(sRunOnly == "Alpha|Beta");
}

static void GHairyReplaceTest()
{
    // GetReplaced test
    str sSubject = "abc 123 def 555 1 xxx";
    str sReplace = "#";
    str sResult;
    int iPrevRestartPos = 0;
    pregfind pregfindScan = preg(preg::EVariablePattern(), "[0-9]+").FindIn(sSubject);
    repeat(i, sSubject.Len()) // infinity prevention
    {
        str sSkip; // mismatched part is either skipped in the middle or the remainder
        int iNextMatchPos = -1;

        // End of loop?
        bool bStop;
        if (!pregfindScan.IsFound())
        {
            bStop = true;

            // Append the remainder
            sSkip = 
                sSubject.GetTruncateLeft(iPrevRestartPos);
        }
        else
        {
            bStop = false;

            // Get next match pos
            iNextMatchPos = pregfindScan.GetMatchPos();
            if failed(iNextMatchPos >= 0)
                break;

            // Extract last mismatched fragment
            sSkip = 
                sSubject.GetMid(
                    iPrevRestartPos, 
                    iNextMatchPos - iPrevRestartPos);
        }

        // Append mismatched fragment verbatim
        sResult += sSkip;

        if (bStop)
            break;

        // Append replacement instead of the match
        sResult += sReplace;

        // Prepare to skip the match
        int nMatchLength = pregfindScan.GetMatchLength();
        if failed(nMatchLength > 0) // cant handle empty matches
            break;

        // Note initial position for the nexr skipped fragment
        ASSERT(iNextMatchPos >= 0);
        iPrevRestartPos = iNextMatchPos + nMatchLength;

        // Go to the next match
        pregfindScan = pregfindScan.FindNextMatch();
    }

    ASSERT(sResult == "abc # def # # xxx");
}

static void GStyledReplaceTest()
{
    // GetReplaced test
    str sSubject = "abc 123 def 555 1 xxx";
    str sReplace = "#";
    str sResult;

    // Stylized Loop
    preg pregPattern = preg(preg::EVariablePattern(), "[0-9]+");
    pregfind pregfindIter;
    for (iter i; pregPattern.IterateSkipMatchPairs(out i, out pregfindIter, sSubject);)
    {
        // Always append mismatched fragment verbatim
        str sSkip = pregfindIter.GetSkipSubstring();
        sResult += sSkip;

        if (pregfindIter.IsFound())
        {
            // Append replacement instead of the match
            str sMatch = pregfindIter.GetMatchSubstring();
            sResult += sReplace;
        }
    }

    ASSERT(sResult == "abc # def # # xxx");
}

static void GShortcutReplaceTest()
{
    // GetReplaced test
    str sSubject = "abc 123 def 555 11 xxx";
    str sReplace = "#";
    str sResult;

    // Stylized Loop
    sResult = 
        preg(preg::EVariablePattern(), "[0-9]+").
            ReplaceInSubjectWithText(sSubject, sReplace);

    ASSERT(sResult == "abc # def # # xxx");
}

static void GShortcutReplaceTest2()
{
    // GetReplaced test
    str sSubject = "abc 123 def 555 11 xxx";
    str sReplace = "#";
    str sResult;

    // Stylized Loop
    sResult = 
        preg(preg::EVariablePattern(), "[0-9]+").
            ReplaceInSubjectWithText(sSubject, sReplace, 2);

    ASSERT(sResult == "abc # def # 11 xxx");
}

static void VariableReplaceTest(
        preg pregTest,
        str sSubject,
        str sReplace,
        str sExpect)
{
    // Stylized Loop
    str sResult = pregTest.ReplaceInSubjectWithText(sSubject, sReplace);

    ASSERT(sResult == sExpect);
}

static void GCapturedReplaceTest()
{
    str sSubject = 
        ""
            "   int GetN();\n"
            "   str GetText();\n"
            "   void NonGetFn();\n"
            "                            \n"
            " // a comment line          \n"
            "   ref<CCccc> GetXxxx(); \n"
            "\n"
            "       friend long double GetAstronomical(P* pThis);\n"
            ;

    str sExpect = 
        ""
            "   void SetN(int value);\n"
            "   void SetText(str value);\n"
            "   void NonGetFn();\n"
            "                            \n"
            " // a comment line          \n"
            "   void SetXxxx(ref<CCccc> value); \n"
            "\n"
            "       friend void SetAstronomical(P* pThis, long double value);\n"
            ;

    // Declare all captured subpatterns as named
    #define C_szCaptureModifier "Modifier"
    #define C_szCaptureType "Type"
    #define C_szCaptureFunctionStart "FunctionStart"
    #define C_szCaptureFirstArgs "FirstArgs"

    // Patter (is constant here, despite symbolic names present)
    preg pregPattern = 
        preg(                                     preg::EVariablePattern(), // for non-Cta
            ""
                // 'friend' keyword may be present, this is not a part of type
                "(?P<" C_szCaptureModifier ">"
                    "friend" 
                    Preg_space "+" // include the following space
                ")"
                "?" // 'friend' modifier is optional

                // type spec may be anything not startng as Get function name
                "(?P<" C_szCaptureType ">"
                    "(" 
                        // type is restricted to a space separated word starting with
                        //   a lowercase letter but containing any 
                        //   non-space/non-newline chars
                        "[a-z]" "[^" Preg_space "\\n]*" "[ \\t]+" 
                    ")+" // multiple words allowed (see 'long double')
                ")"

                // function name and optional parameters
                "Get" // we only interested in Get functions, 
                      //   and dont wanna capture the 'Get' word itself
                "(?P<" C_szCaptureFunctionStart ">"
                    Preg_word "+" "\\(" // GetXxxx( ...
                    "(?P<" C_szCaptureFirstArgs ">" 
                        "[^\\)]*" // optional 1st arg
                    ")" 
                ")"

                // declaration ending
                "\\)");

    str sResult;

    // Stylized Loop
    pregfind pregfindIter;
    int nFoundCount = 0;
    for (iter i; pregPattern.IterateSkipMatchPairs(out i, out pregfindIter, sSubject);)
    {
        // Always append mismatched fragment verbatim
        str sSkip = pregfindIter.GetSkipSubstring();
        sResult += sSkip;

        if (pregfindIter.IsFound())
        {
            nFoundCount++;

            // Lookup captures (some maybe empty)
            str sModifier = pregfindIter.GetCaptureByName(C_szCaptureModifier);
            str sType = pregfindIter.GetCaptureByName(C_szCaptureType);
            str sFunctionStart = pregfindIter.GetCaptureByName(C_szCaptureFunctionStart);
            str sFirstArgs = pregfindIter.GetCaptureByName(C_szCaptureFirstArgs);

            // Get the whole match string
            str sMatch = pregfindIter.GetMatchSubstring();
            ASSERT(
                sMatch.Len() > 
                    sModifier.Len() + 
                    sType.Len() + 
                    sFunctionStart.Len());
            sMatch; // unused here

            // Compose the result

            // note slow: sResult += sModifier + "void Set" + sFunctionStart;
            sResult += sModifier;
            sResult += "void Set";
            sResult += sFunctionStart;

            if (sFirstArgs.Len() > 0)
            {
                ASSERT(sFunctionStart.FindPos(sFirstArgs) > 0);
                sResult += ", ";
            }

            sResult += sType;
            sResult += "value)";
        }
    }

    ASSERT(nFoundCount == 4);
    ASSERT(sResult == sExpect);

    //sys::GOpenTextInWordpad(
    //    sExpect + "\v\n-------------\v\n" + sResult + 
    //        "\v\n" + Str(nFoundCount) + " found");
}

static void GCapitalizeTest()
{
    str sSubject = "some Example 9text to caPITALIZE with Preg_word-check";
    str sExpect =  "Some Example 9text To CaPITALIZE With Preg_word-Check";
    str sResult;

    preg pregPattern(preg::EVariablePattern(), Preg_word "+");

    pregfind pregfindIter;
    for (iter i; pregPattern.IterateSkipMatchPairs(out i, out pregfindIter, sSubject);)
    {
        // Always append mismatched fragment verbatim
        str sSkip = pregfindIter.GetSkipSubstring();
        sResult += sSkip;

        if (pregfindIter.IsFound())
        {
            // Modify and append matching fragment
            sResult += pregfindIter.GetMatchSubstring().GetCapitalizedFirst();
        }
    }

    ASSERT(sResult == sExpect);

    //sys::GOpenTextInWordpad(
    //    sExpect + "\v\n-------------\v\n" + sResult + 
    //        "\v\n" + Str(nFoundCount) + " found");
}

static void GRepeatingLetterCatch()
{
    #define C_szCapturePrevChar "PrevChar"

    preg pregPattern = 
        preg(                                     preg::EVariablePattern(), // for non-Cta
            ""
                // capture a single char
                "(?P<" C_szCapturePrevChar ">"
                    "."
                ")"

                // the same char repeating many times
                "(?P=" C_szCapturePrevChar ")" "{2,}");

    ASSERT(!pregPattern.FindIn("").IsFound());
    ASSERT(!pregPattern.FindIn("a").IsFound());
    ASSERT(!pregPattern.FindIn("aa").IsFound());
    ASSERT(pregPattern.FindIn("aaa").IsFound());
    ASSERT(!pregPattern.FindIn("AaA").IsFound());
    ASSERT(!pregPattern.FindIn("aabaa").IsFound());
    ASSERT(pregPattern.FindIn("babaaa").IsFound());
    ASSERT(!pregPattern.FindIn("*#$^%%&##^@").IsFound());
    ASSERT(pregPattern.FindIn("*#$^%%&###^@").IsFound());
    ASSERT(pregPattern.FindIn("aaax").IsFound());
    ASSERT(pregPattern.FindIn("111111").IsFound());
    ASSERT(pregPattern.FindIn("w2aa2aaaax").IsFound());
}

LOCAL_TEST(PregFind)
{
    STATIC_MAGIC_STR(ms__def);
    GTestPregFind(
        preg(MakeMagicStr("def", &ms__def)),
        "abcdefghi", // sSubject
        "", // sCaptureName,
        3, // iStartPos,
        "def", // sMatch,
        1, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), "def(?P<MyCapture>[0-9]+)"),
        "abcdef57ghi", // sSubject
        "MyCapture", // sCaptureName,
        3, // iStartPos,
        "def57", // sMatch,
        1, // nMatchCount,
        "57"); // sCapture


    GTestPregFind(
        preg(preg::EVariablePattern(), "\x0D\x0A[ ]*"),
        "line one\r\n  second", // sSubject
        "", // sCaptureName,
        8, // iStartPos,
        "\r\n  ", // sMatch,
        1, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), "\r\n[ ]*"),
        "line one\r\n  second", // sSubject
        "", // sCaptureName,
        8, // iStartPos,
        "\r\n  ", // sMatch,
        1, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), "\r\n[ ]*"),
        "\r\n", // sSubject
        "", // sCaptureName,
        0, // iStartPos,
        "\r\n", // sMatch,
        1, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), "\r\n[ ]*"),
        "\r\n\r\n", // sSubject
        "", // sCaptureName,
        0, // iStartPos,
        "\r\n", // sMatch,
        2, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), "\r\n[ ]*"),
        "  \r\n\n", // sSubject
        "", // sCaptureName,
        2, // iStartPos,
        "\r\n", // sMatch,
        1, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), "\r\n[ ]*"),
        "\r \n \r\n ", // sSubject
        "", // sCaptureName,
        4, // iStartPos,
        "\r\n ", // sMatch,
        1, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), "\r\n[ ]*"),
        "  \r\n\r\n  ", // sSubject
        "", // sCaptureName,
        2, // iStartPos,
        "\r\n", // sMatch,
        2, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), "\r\n[ ]*"),
        "\r\n\t ", // sSubject
        "", // sCaptureName,
        0, // iStartPos,
        "\r\n", // sMatch,
        1, // nMatchCount,
        ""); // sCapture


    GTestPregFind(
        preg(preg::EVariablePattern(), "^[A-Za-z]+$"),
        "ValidName\n" "Execute!", // sSubject
        "", // sCaptureName,
        -1, // iStartPos,
        "", // sMatch,
        0, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), "^[A-Za-z]+$"),
        "ValidName", // sSubject
        "", // sCaptureName,
        0, // iStartPos,
        "ValidName", // sMatch,
        1, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), "^[A-Za-z]+$"),
        "ValidName\n", // sSubject
        "", // sCaptureName,
        -1, // iStartPos,
        "", // sMatch,
        0, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), Preg_MultilineSubject "^[A-Za-z]+$"),
        "ValidName\n" "Execute!", // sSubject
        "", // sCaptureName,
        0, // iStartPos,
        "ValidName", // sMatch,
        1, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), Preg_MultilineSubject "^[A-Za-z]+$"),
        "ValidName\n", // sSubject
        "", // sCaptureName,
        0, // iStartPos,
        "ValidName", // sMatch,
        1, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), Preg_MultilineSubject "^[A-Za-z]+$"),
        "ValidName\n" "Bad!\n" "NextOk", // sSubject
        "", // sCaptureName,
        0, // iStartPos,
        "ValidName", // sMatch,
        2, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), Preg_MultilineSubject "^[A-Za-z]+$"),
        "\n\n" "ValidName\n" "Bad!\n\n" "NextOk\n", // sSubject
        "", // sCaptureName,
        2, // iStartPos,
        "ValidName", // sMatch,
        2, // nMatchCount,
        ""); // sCapture

    {
        VariableReplaceTest(
            cta_ignore("[FAEF]", "a russian letter precedes english letter")
            cta_ignore("[FAFE]", "a russian letter follows english letter")
            preg(preg::EVariablePattern(), Preg_IgnoreCase "[Б-Г" "I]"),
[...]
                "<*>о<*>а: - М<*><*>, але <*>ик краще. ");
    }

    VariableReplaceTest(
        preg(preg::EVariablePattern(), ".*"),
        "", // sSubject
        "-", // sReplace,
        "");

    // default anchoring
    if (preg(preg::EVariablePattern(), "^[0-9A-Fa-f]+$").FindIn("0x1abcd2").IsFound()) 
        FAIL("");
    if (!preg(preg::EVariablePattern(), "[0-9A-Fa-f]+").FindIn("0x1abcd2").IsFound()) 
        FAIL("");
    if (!preg(preg::EVariablePattern(), "^[0-9A-Fa-f]+$").FindIn("1abcd2").IsFound()) 
        FAIL("");
    if (preg(preg::EVariablePattern(), "[0-9A-Fa-f]+").FindIn("Q-z").IsFound()) 
        FAIL("");
    if (preg(preg::EVariablePattern(), "[0-9A-Fa-f]+").FindIn("").IsFound()) 
        FAIL("");

    GSimpleCaptureTest();
    GHairyReplaceTest();
    GStyledReplaceTest();
    GShortcutReplaceTest();
    GShortcutReplaceTest2();
    GCapturedReplaceTest();
    GCapitalizeTest();
    GRepeatingLetterCatch();
}

LOCAL_TEST(Nothing)
{
}

LOCAL_TEST(TxPregFind)
{
    GTestPregFind(
        preg(preg::EVariablePattern(), Preg_notWord "*$"),
        "", // sSubject
        "", // sCaptureName,
        -1, // iStartPos,
        "", // sMatch,
        0, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), Preg_notWord "*$"),
        "Test", // sSubject
        "", // sCaptureName,
        -1, // iStartPos,
        "", // sMatch,
        0, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), Preg_notWord "*$"),
        "Test..))", // sSubject
        "", // sCaptureName,
        4, // iStartPos,
        "..))", // sMatch,
        1, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), Preg_notWord "*$"),
        "Test..))  test~", // sSubject
        "", // sCaptureName,
        14, // iStartPos,
        "~", // sMatch,
        1, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), Preg_notWord "*$"),
        ".T.e.s.t!! ", // sSubject
        "", // sCaptureName,
        8, // iStartPos,
        "!! ", // sMatch,
        1, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), Preg_notWord "*$"),
        ".T.e.s.t!\r\n! ", // sSubject
        "", // sCaptureName,
        8, // iStartPos,
        "!\r\n! ", // sMatch,
        1, // nMatchCount,
        ""); // sCapture

}

LOCAL_TEST(PcPreg)
{
    GTestPregFind(
        preg(preg::EVariablePattern(), "text.*?\n.*?\n"),
        "", // sSubject
        "", // sCaptureName,
        -1, // iStartPos,
        "", // sMatch,
        0, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), "text.*?\n.*?\n"),
        "text", // sSubject
        "", // sCaptureName,
        -1, // iStartPos,
        "", // sMatch,
        0, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), "text.*?\n.*?\n"),
        "text\r\n", // sSubject
        "", // sCaptureName,
        -1, // iStartPos,
        "", // sMatch,
        0, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), "text.*?\n.*?\n"),
        "text\r\n\n", // sSubject
        "", // sCaptureName,
        0, // iStartPos,
        "text\r\n\n", // sMatch,
        1, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), "text.*?\n.*?\n"),
        "text\r\ntest\ntest\n", // sSubject
        "", // sCaptureName,
        0, // iStartPos,
        "text\r\ntest\n", // sMatch,
        1, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), "text.*?\n.*?\n"),
        "text\r\ntest\ntest\n", // sSubject
        "", // sCaptureName,
        0, // iStartPos,
        "text\r\ntest\n", // sMatch,
        1, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        preg(preg::EVariablePattern(), "text.*?\n.*?\n"),
        "qwerttext\r\ntest\r\ntest\n", // sSubject
        "", // sCaptureName,
        5, // iStartPos,
        "text\r\ntest\r\n", // sMatch,
        1, // nMatchCount,
        ""); // sCapture


    preg pregCompositeType =
        preg(
            preg::EVariablePattern(), 
            "^([ \t]*("
                "long|short|signed|unsigned|"
                "int|char|double|float|"
                "__wchar_t|"
                "wchar_t|"
                "(__int[a-z0-9]+)|"
                "(__m[a-z0-9]+)|" // (MMX)
                "__w64" // (warn64)
                ")" Preg_boundary ")+");

    GTestPregFind(
        pregCompositeType,
        "long  int nVar", // sSubject
        "", // sCaptureName,
        0, // iStartPos,
        "long  int", // sMatch,
        1, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        pregCompositeType,
        "unsigned long\t long int,", // sSubject
        "", // sCaptureName,
        0, // iStartPos,
        "unsigned long\t long int", // sMatch,
        1, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        pregCompositeType,
        "long __int64 integer", // sSubject
        "", // sCaptureName,
        0, // iStartPos,
        "long __int64", // sMatch,
        1, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        pregCompositeType,
        "signed long long __w64 int,int", // sSubject
        "", // sCaptureName,
        0, // iStartPos,
        "signed long long __w64 int", // sMatch,
        1, // nMatchCount,
        ""); // sCapture

    GTestPregFind(
        pregCompositeType,
        "unsigned __wchar_t __w64 __w64id", // sSubject
        "", // sCaptureName,
        0, // iStartPos,
        "unsigned __wchar_t __w64", // sMatch,
        1, // nMatchCount,
        ""); // sCapture

    GTestPregEscape(
        "",
        "",
        charmap());

    GTestPregEscape(
        "a",
        "a",
        charmap());

    GTestPregEscape(
        "x",
        "x",
        CHARMAP("."));

    GTestPregEscape(
        "x.",
        "x.",
        CHARMAP("."));

    GTestPregEscape(
        "x.",
        "x\\.",
        CHARMAP(","));

    GTestPregEscape(
        "([^]* <{}>+$)",
        "\\(\\[\\^\\]* \\<\\{\\}>\\+\\$\\)",
        CHARMAP("*>"));

    GTestPregEscape(
        CTA_MIDZERO "zero(\0)",
        "zero\\(\\0\\)",
        charmap());

}
