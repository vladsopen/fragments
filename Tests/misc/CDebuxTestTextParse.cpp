// CDebuxTestTextParse.cpp

#include "CDebuxProject.h"
#include "CDebuxTestTextParse.h"
#include "VirtOs_text.h"

CDebuxTestTextParse::CDebuxTestTextParse()
{
    _init_AttachToDebuxTestTextParse();
}

void CDebuxTestTextParse::_init_AttachToDebuxTestTextParse()
{
    //x_r->_x_p = this;
}

void CDebuxTestTextParse::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToDebuxTestTextParse();

    rFAIL("make sure it is OK to expose CDebuxTestTextParse!");
}

void CDebuxTestTextParse::TestTextParseStringSeparatedOption(
        str sSource,
        str sSeparator,
        str sExpect,
        text::EMergeSeparators eMergeSeparators)
{
    text textTest;
    textTest.
        ParseStringSeparated(
            sSource,
            sSeparator,
            eMergeSeparators);

    str sConcatenated = 
        textTest.CatenateTextSeparatingWith(":");

    str sReport =
        "text source: " + sSource + "\n"
            "  merge=" + Str(eMergeSeparators) + "\n"
            "  expect: " + sExpect + "\n"
            "  result: " + sConcatenated + "\n"
            ;

    rASSERTM(
        sConcatenated == sExpect,
        sReport);

    TESTLOG(
        "",
        "\n" +
            sReport);
}

void CDebuxTestTextParse::TestTextParseStringSeparated(
        str sSource,
        str sSeparator,
        str sExpectWithMergedSeparators,
        str sExpectWithAllSeparators)
{
    TestTextParseStringSeparatedOption(
        sSource,
        sSeparator,
        sExpectWithMergedSeparators,
        text::E_MergeSeparators_On);

    TestTextParseStringSeparatedOption(
        sSource,
        sSeparator,
        sExpectWithAllSeparators,
        text::E_MergeSeparators_Off);
}

void CDebuxTestTextParse::OnTestClass()
{
    TestTextParseStringSeparated(
        "a<!>b",
        "<!>",
        "a:b",
        "a:b");

    TestTextParseStringSeparated(
        "",
        "<!>",
        "",
        "");

    TestTextParseStringSeparated(
        "a",
        "<!>",
        "a",
        "a");

    TestTextParseStringSeparated(
        "a<!><!>b",
        "<!>",
        "a:b",
        "a::b");

    TestTextParseStringSeparated(
        "a<!><!>b<!>",
        "<!>",
        "a:b",
        "a::b");

    TestTextParseStringSeparated(
        "<!>a<!><!>b<!>",
        "<!>",
        "a:b",
        ":a::b");

    TestTextParseStringSeparated(
        "<!><!>a<!><!>b<!>",
        "<!>",
        "a:b",
        "::a::b");

    TestTextParseStringSeparated(
        "<!><!>abcd<!>ef<!><!><!>",
        "<!>",
        "abcd:ef",
        "::abcd:ef::");

    TestTextParseStringSeparated(
        "abcd<!><!>ef",
        "<!>",
        "abcd:ef",
        "abcd::ef");

    TestTextParseStringSeparated(
        "<!>",
        "<!>",
        "",
        "");

    TestTextParseStringSeparated(
        "<!><!>",
        "<!>",
        "",
        ":");

    TestTextParseStringSeparated(
        "<!><!><!>",
        "<!>",
        "",
        "::");

    TestTextParseStringSeparated(
        "a<!>",
        "<!>",
        "a",
        "a");

    TestTextParseStringSeparated(
        "aa<!><!>",
        "<!>",
        "aa",
        "aa:");

    TestTextParseStringSeparated(
        "<!>a",
        "<!>",
        "a",
        ":a");

    TestTextParseStringSeparated(
        "<!><!>aa",
        "<!>",
        "aa",
        "::aa");

[...]