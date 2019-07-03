// CDebuxTestWriteBinaryBits.cpp

#include "CDebuxProject.h"
#include "CDebuxTestWriteBinaryBits.h"

CDebuxTestWriteBinaryBits::CDebuxTestWriteBinaryBits()
{
    _init_AttachToDebuxTestWriteBinaryBits();
}

void CDebuxTestWriteBinaryBits::_init_AttachToDebuxTestWriteBinaryBits()
{
    //x_r->_x_p = this;
}

void CDebuxTestWriteBinaryBits::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToDebuxTestWriteBinaryBits();

    rFAIL("make sure it is OK to expose CDebuxTestWriteBinaryBits!");
}

void GTestWriteBinaryBits(
        str sDigit,
        big nnDigit,
        str sExpect,
        int nUseBits,
        int nRemoveMaxInsignificantZeros,
        str sZero,
        str sOne,
        str sSeparator)
{
    if (sDigit != "")
    {
        rASSERT(nnDigit == 0);

        int nEndPos = -1;
        nnDigit = 
            sDigit.
                ParseArbitraryBasedNumber(
                    out nEndPos,
                    2);
        rASSERT(nEndPos == sDigit.Len());
    }

    str sResult = 
        GWriteBinaryBits(
            nnDigit,
            nUseBits,
            nRemoveMaxInsignificantZeros,
            sZero,
            sOne,
            sSeparator);

    rASSERTM(
        sExpect == sResult,
        "GWriteBinaryBits() returned\n"
            "<" + sResult + "> instaed of \n"
            "<" + sExpect + ">");
}

void GRunTestWriteBinaryBits()
{
    GTestWriteBinaryBits(
        // Test source 
        "01010011", 0,

        // Expected result 
        "1010011",

        // Options 
        8, // nUseBits,
        7, // nRemoveMaxInsignificantZeros,
        "0", // sZero,
        "1", // sOne,
        ""); // sSeparator

    GTestWriteBinaryBits(
        // Test source 
        "101", 0,

        // Expected result 
        "x, o, x",

        // Options 
        8, // nUseBits,
        7, // nRemoveMaxInsignificantZeros,
        "o", // sZero,
        "x", // sOne,
        ", "); // sSeparator

    GTestWriteBinaryBits(
        // Test source 
        "1001", 0,

        // Expected result 
        "OneOne",

        // Options 
        40, // nUseBits,
        40, // nRemoveMaxInsignificantZeros,
        "", // sZero,
        "One", // sOne,
        ""); // sSeparator

    GTestWriteBinaryBits(
        // Test source 
        "00001111", 0,

        // Expected result 
        "0 0 0 0 1 1 1 1",

        // Options 
        8, // nUseBits,
        0, // nRemoveMaxInsignificantZeros,
        "0", // sZero,
        "1", // sOne,
        " "); // sSeparator

    GTestWriteBinaryBits(
        // Test source 
        "100001111", 0,

        // Expected result 
        "01111",

        // Options 
        8, // nUseBits,
        3, // nRemoveMaxInsignificantZeros,
        "0", // sZero,
        "1", // sOne,
        ""); // sSeparator

    GTestWriteBinaryBits(
        // Test source 
        "0", 0,

        // Expected result 
        "",

        // Options 
        10, // nUseBits,
        10, // nRemoveMaxInsignificantZeros,
        "0", // sZero,
        "1", // sOne,
        ","); // sSeparator

    GTestWriteBinaryBits(
        // Test source 
        "", 0x8000000000000000LL,

        // Expected result 
        "1" + StrChar('0', 63),

        // Options 
        64, // nUseBits,
        64, // nRemoveMaxInsignificantZeros,
        "0", // sZero,
        "1", // sOne,
        ""); // sSeparator

    GTestWriteBinaryBits(
        // Test source 
        "0", 0,

        // Expected result 
        "0",

        // Options 
        8, // nUseBits,
        7, // nRemoveMaxInsignificantZeros,
        "0", // sZero,
        "1", // sOne,
        ""); // sSeparator

    GTestWriteBinaryBits(
        // Test source 
        "0", 0,

        // Expected result 
        "ZERO",

        // Options 
        8, // nUseBits,
        7, // nRemoveMaxInsignificantZeros,
        "ZERO", // sZero,
        "ONE", // sOne,
        "--"); // sSeparator

    GTestWriteBinaryBits(
        // Test source 
        "0", 0,

        // Expected result 
        "o-o-o-o-o-o-o-o",

        // Options 
        8, // nUseBits,
        0, // nRemoveMaxInsignificantZeros,
        "o", // sZero,
        "ONE", // sOne,
        "-"); // sSeparator

    GTestWriteBinaryBits(
        // Test source 
        "10000", 0,

        // Expected result 
        "(1)",

        // Options 
        9, // nUseBits,
        0, // nRemoveMaxInsignificantZeros,
        "", // sZero,
        "(1)", // sOne,
        "-"); // sSeparator

    GTestWriteBinaryBits(
        // Test source 
        "10010", 0,

        // Expected result 
        "(1)-(1)",

        // Options 
        9, // nUseBits,
        0, // nRemoveMaxInsignificantZeros,
        "", // sZero,
        "(1)", // sOne,
        "-"); // sSeparator
}

void CDebuxTestWriteBinaryBits::OnTestClass()
{
    GRunTestWriteBinaryBits();
}