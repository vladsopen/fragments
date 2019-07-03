// CDebuxTestExpressionCalc.cpp

#include "CDebuxProject.h"
#include "VirtOs_CUnidefCalcGp.h"
#include "VirtOs_ask.h"

CDebuxTestExpressionCalc::CDebuxTestExpressionCalc()
{
    _init_AttachToDebuxTestExpressionCalc();
}

void CDebuxTestExpressionCalc::_init_AttachToDebuxTestExpressionCalc()
{
    //x_r->_x_p = this;
}

void CDebuxTestExpressionCalc::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToDebuxTestExpressionCalc();

    rFAIL("make sure it is OK to expose CDebuxTestExpressionCalc!");
}

CExpressionCalcTest::CExpressionCalcTest()
{
}

CMyStruct::CMyStruct()
{
}

void CMyStruct::SetIntAt(const str& sKey, int nValue)
{
    m_sId = sKey;
    m_i = nValue;
}

static big g_nnInterpreted64 = -1;
void CMyStruct::SetBigAt(const str& sKey, big nnValue)
{
    m_sId = sKey;
    m_i64 = nnValue;
    g_nnInterpreted64 = nnValue;
}

static num g_numInterpretedNum = -1;
void CMyStruct::SetNumAt(const str& sKey, num numValue)
{
    m_sId = sKey;
    m_i64 = (big) numValue;
    g_numInterpretedNum = numValue;
}

str CExpressionCalcTest::GSelfTestExpressionCalc()
{
    str sExpression32;
    str sExpression64;
    str sExpressionIntToNum;
    str sExpressionIntTo64;
    str sExpressionOver;

    // int32
    ref<CExpressionCalcTest> rMyCalc32; // = CExpressionCalcTest();
    {
        sExpression32 = "a = -1946084015";

        str sError;
        rMyCalc32->InterpretExpressionCalc(sExpression32.c(), out sError);
        rASSTR(sError);

        str s = "-1946084015";
        int n = s.ParseDecimal();
        num numValue = n;
        int nBackValue = (int) numValue;
        str sPrint = Str(numValue);
        nBackValue;
    }

    // int64
    ref<CExpressionCalcTest> rMyCalc64; // = CExpressionCalcTest();
    {
        sExpression64 = "a = -50522659200123LL";
        g_nnInterpreted64 = 0;

        str sError;
        rMyCalc64->InterpretExpressionCalc(sExpression64.c(), out sError);
        rASSTR(sError);

        rASSERT(rMyCalc64->m_nnResult == -50522659200123LL);
        rASSERT(g_nnInterpreted64 == -50522659200123LL);
    }

    // int-->num
    ref<CExpressionCalcTest> rMyCalcIntToNum; // = CExpressionCalcTest();
    {
        sExpressionIntToNum = "a = 5000111777";
        g_nnInterpreted64 = 0;
        g_numInterpretedNum = 0;

        str sError;
        rMyCalcIntToNum->InterpretExpressionCalc(sExpressionIntToNum.c(), out sError);
        rASSTR(sError);

        rASSERT(rMyCalcIntToNum->m_nnResult == 5000111777LL);
        rASSERT(g_numInterpretedNum == 5000111777.);
        rASSERT(g_nnInterpreted64 == 0);
    }

    // int-->big
    ref<CExpressionCalcTest> rMyCalcIntTo64; // = CExpressionCalcTest();
    {
        sExpressionIntTo64 = "a = 9223372036854775807";
        g_nnInterpreted64 = 0;
        g_numInterpretedNum = 0;

        str sError;
        rMyCalcIntTo64->InterpretExpressionCalc(sExpressionIntTo64.c(), out sError);
        rASSTR(sError);

        rASSERT(rMyCalcIntTo64->m_nnResult == 9223372036854775807LL);
        rASSERT(g_numInterpretedNum == 0);
        rASSERT(g_nnInterpreted64 == 9223372036854775807LL);
    }

    // int-->overflow
    ref<CExpressionCalcTest> rMyCalcOver; // = CExpressionCalcTest();
    {
        sExpressionOver = "a = 9223372036854775807111";
        g_nnInterpreted64 = 0;
        g_numInterpretedNum = 0;

        str sError;
        //try
        //{
            rMyCalcOver->InterpretExpressionCalc(sExpressionOver.c(), out sError);
        //}
        //catch (CErrorExpressionCalc e)
        //{
        //    sError = e.GetErrorText();
        //}

        rASSERT(sError.FindPos("Integer constant overflow") != -1);

        rASSERT(rMyCalcOver->m_nnResult == 0);
        rASSERT(g_numInterpretedNum == 0);
        rASSERT(g_nnInterpreted64 == 0);
    }

    // div/0
    {
        ref<CExpressionCalcTest> rMyCalc;
        sExpressionOver = "$'-' = 12.3/0";
        g_nnInterpreted64 = 0;
        g_numInterpretedNum = 0;

        str sError;
        rMyCalc->InterpretExpressionCalc(sExpressionOver.c(), out sError);

        rASSERT(sError.FindPos("Floating point divide by zero") != -1);

        rASSERT(rMyCalc->m_rMyStructRoot->m_sId == "");
        rASSERT(rMyCalc->m_nnResult == 0);
        rASSERT(g_numInterpretedNum == 0);
        rASSERT(g_nnInterpreted64 == 0);
    }

    // id
    {
        ref<CExpressionCalcTest> rMyCalc;
        sExpressionOver = "$\"c:\\\\x!\" = -(1 + 2) / 1000 * 100";
        g_nnInterpreted64 = 0;
        g_numInterpretedNum = 0;

        str sError;
        rMyCalc->InterpretExpressionCalc(sExpressionOver.c(), out sError);

        rASSERT(sError == "");

        rASSERT(rMyCalc->m_rMyStructRoot->m_sId == "c:\\x!");
        rASSERT(math::GIsNumNearlyZero(-g_numInterpretedNum - 0.3));
        rASSERT(g_numInterpretedNum < 0);
        rASSERT(g_nnInterpreted64 == 0);
    }

    // strange case of BB
    {
        ref<CExpressionCalcTest> rMyCalc;
        sExpressionOver = "formula = ***";
        g_nnInterpreted64 = 0;
        g_numInterpretedNum = 0;

        str sError;
        rMyCalc->InterpretExpressionCalc(sExpressionOver.c(), out sError);

        rASSERT(sError.FindPos("Syntax error") != -1);

        rASSERT(rMyCalc->m_rMyStructRoot->m_sId == "");
        //rASSERT(g_numInterpretedNum < 0);
        rASSERT(g_nnInterpreted64 == 0);
    }

    big bigValue = 91870000000000003;
    num numBig = (num) bigValue;
    str sBig = Str(numBig);
    big bigBack = (big) numBig;
    big bigBack2 = sBig.ParseBig();
    bigBack2;
    bigBack;

    int nLarge = 1234567890;
    str sLarge = StrFormat(num(nLarge), "%g");
    int nRecovered = sLarge.ParseDecimal();
    nRecovered;

    num numLong = 29. / 14.;
    str sLong = Str(numLong);

    //ask(sExpression + " -> " + RichBold(Str(rMyCalc.m_nnResult)));
    return 
        ""
            "32 bit: " + sExpression32 + 
                " -> " + 
                RichBold(Str(rMyCalc32->m_nnResult)) +
            "\n"
            "64 bit: " + sExpression64 + 
                " -> " + 
                RichBold(Str(rMyCalc64->m_nnResult)) +
            "\n"
            "int->num: " + sExpressionIntToNum + 
                " -> " + 
                RichBold(Str(rMyCalcIntToNum->m_nnResult)) +
            "\n"
            "int->big: " + sExpressionIntTo64 + 
                " -> " + 
                RichBold(Str(rMyCalcIntTo64->m_nnResult)) +
            "\n"
            "int->overflow: " + sExpressionOver + 
                " -> " + 
                RichBold(Str(rMyCalcOver->m_nnResult)) +
[...]