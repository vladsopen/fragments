// CCtaTestProps.cpp

#include "CProject.h"
#include "CCtaTestProps.h"

CCtaTestProps::CCtaTestProps()
{
}

void CCtaTestProps::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);
}

struct xxx
{
    void GETSTUFF() {}
    void GET_UNDERSCORE() {}
    void C_PLUSPLUS() {}
};

void CCtaTestProps::DoNothingJustMakeCtaCheckSomeCode()
{
    xxx* x = new xxx();
    x->GETSTUFF();
    x->GET_UNDERSCORE();
    x->C_PLUSPLUS();

    ComposeSomeWrongTranslate();
    TestBadChars();
}

@define CTA_COUNTER_TEST(sId, iCounter, anLines) \
    GCtaCounterTest(sId, iCounter, sizeof(anLines) / sizeof(anLines[0]))
    ;

str GCtaCounterTest(
        str sId, 
        int iCounter, 
        int nMax)
{
    rASSERT(sId == "Test"); 
    rASSERT(iCounter >= 0);
    rASSERT(iCounter < nMax);

@define CONCAT(x, y) x##y
    CONCAT(DeclaredProject_, cta_::SourceProjectName) test;
    DeclaredProject_ ideal = test;
    ideal;

    return "";
}

void CCtaTestProps::ComposeSomeWrongTranslate()
{
    str sCrap;

    CTA_COUNTER_TEST(cta_::CounterInFile(Test));
    {
        str sJunk =
            cta_ignore("[FEE3]", "wrong translation count")
            TRANSLATE(
                "on[... UI-string-removed]");
    }

    {
        CTA_COUNTER_TEST(cta_::CounterGlobal(Test));

        str sJunk =
            cta_ignore("[FEE3]", "wrong translation count")
            TRANSLATE(
                "on[... UI-string-removed]");
    }

    {
        str sJunk =
            //cta_ignore("[FEE3]", "wrong translation count")
            TRANSLATE(
                "on[... UI-string-removed]");
    }

    {
        str sJunk =
            cta_ignore("[FEE7]", "pattern mismatch: <<<S+sCrap+S>>> vs <<<S+S>>>")
            TRANSLATE(
                "a[... UI-string-removed]");

        CTA_COUNTER_TEST(cta_::CounterGlobal(Test));
    }

    CTA_COUNTER_TEST(cta_::CounterInFile(Test));
    {
        str sJunk =
            cta_ignore("[FED1]", "you cannot use double quotes")
            cta_ignore("[FEE7]", "pattern mismatch: <<<S!>>> vs <<<S>>>")
            TRANSLATE(
                "No double [... UI-string-removed]");
    }

    CTA_COUNTER_TEST(cta_::CounterGlobal(Test));
    {
        str sJunk =
            cta_ignore("[FED3]", "unexpected slash escape found")
            cta_ignore("[FED8]", "forbidden slash found")
            TRANSLATE(
                "Bad \\b RT[... UI-string-removed]");
    }

    CTA_COUNTER_TEST(cta_::CounterInFile(Test));
    {
        str sJunk =
            cta_ignore("[FEC1]", "complex expression characters detected in translation")
            //cta_ignore("[FEBA]", "forbidden complex expressions detected")
            TRANSLATE(
                "complex ex[... UI-string-removed]");
    }

    CTA_COUNTER_TEST(cta_::CounterInFile(Test));
    {
        str sJunk =
            cta_ignore("[FED7]", "russian symbols detected in non-russian translation")
            TRANSLATE(
                "this is [... UI-string-removed]");
    }

    CTA_COUNTER_TEST(cta_::CounterInFile(Test2));
    CTA_COUNTER_TEST(cta_::CounterInFile(Test2));
    CTA_COUNTER_TEST(cta_::CounterInFile(Test2));
    {
        str sJunk =
            cta_ignore("[FED8]", "forbidden slash found")
            TRANSLATE(
                "Bad \a cha[... UI-string-removed]");
    }

    {
        str sJunk =
            cta_ignore("[FED8]", "forbidden slash found")
            TRANSLATE(
                "Bad \x81 c[... UI-string-removed]");
    }

    {
        str sJunk =
            cta_ignore("[FED2]", "unsupported character")
            cta_ignore("[FED9]", "interpreter-unsafe character")
            cta_ignore("[FAFF]", "unexpected extended char detected")
            TRANSLATE(
                "Bad BELL(•[... UI-string-removed]");
    }

    {
        str sJunk =
            cta_ignore("[FED2]", "unsupported character")
            cta_ignore("[FED9]", "interpreter-unsafe character")
            cta_ignore("[FAFF]", "unexpected extended char detected")
            TRANSLATE(
                "Bad EXTEND[... UI-string-removed]");
    }

    {
        str sJunk =
            cta_ignore("[FED6]", "double spaces detected")
            TRANSLATE(
                "Double  sp[... UI-string-removed]");
    }

    {
        str sJunk =
            //cta_ignore("[FEBA]", "forbidden complex expressions detected")
            cta_ignore("[FEDF]", "possible printf-like %-formatting detected")
            TRANSLATE(
                "Cant allow[... UI-string-removed]");
    }

    {
        str sJunk =
            cta_ignore("[FEC6]", "does not contain any visible string constants")
            TRANSLATE(
                "[... UI-string-removed]");
    }

    {
        str sJunk =
            TRANSLATE(
                "Blah asdff[... UI-string-removed]");
    }

    {
        int nStuff = 0;
        str sJunk =
            //cta_ignore("[FEC6]", "does not contain any visible string constants")
            //cta_ignore("[FEBA]", "forbidden complex expressions detected")
            TRANSLATE(
                "Blah asdff[... UI-string-removed]");
    }

    {
        //int nStuff = 0;
        str sJunk = 
            cta_ignore("[FEE7]", "translation #1 pattern mismatch")
            TRANSLATE("Att[... UI-string-removed]");
    }

    {
        //int nStuff = 0;
        str sJunk =
            cta_ignore("[FEE7]", "translation #1 pattern mismatch")
            TRANSLATE("Att[... UI-string-removed]");
    }

    {
        //int nStuff = 0;
        str sJunk =
            cta_ignore("[FEE7]", "translation #1 pattern mismatch")
            TRANSLATE("Att[... UI-string-removed]");
    }

    {
        //int nStuff = 0;
        str sJunk =
            //cta_ignore("[FEE7]", "translation #1 pattern mismatch")
            TRANSLATE("per[... UI-string-removed]");
    }

    {
        //int nStuff = 0;
        str sFile;
        str sLogo;
        str sDoubledVn;
        str sJunk =
            //cta_ignore("[FEE7]", "translation #1 pattern mismatch")
            TRANSLATE(
                "WARNING: You are about to update [...]");
    }

    {
        str sName;
        str sJunk =
            cta_ignore(
                "[FEE7]", 
                "<<<S!\v\nS+sName+S!>>> vs <<<S!\v\n+sName+S!>>>")
    }
}

void CCtaTestProps::TestBadChars()
{
    {
        str sBadRussianWithEnglish = 
            cta_ignore("[FAFE]", "a russian letter follows english letter")
            "U¸";
    }

    {
        str sBadRussianWithEnglish = 
            cta_ignore("[FAEF]", "a russian letter precedes english letter")
            "ÛSDF";
    }

    {
        str s = 
            cta_ignore("[FAFF]", "unexpected extended char detected")
            "Bad EXTENDED(«) char";
    }

    {
        str tab_here = 
            //cta_ignore("[FAB9]", "tab character detected")
            //cta_ignore("[D03D]", "tab character in a quoted constant")
            "    ";
    }

    {
            str tab_here    ; // undetected
    }
}

void* CCtaTestProps::OnGetVoidPointer()
{
    void* (CCtaTestProps::*pFn)() = 
        cta_macro(
            Write "&$(ThisFunctionQualifiedName)");
    pFn;

    // must be empty
    const int C_3 = 
        1 
        cta_macro(
            Write "$(ThisFunctionArgDecl)+$(ThisFunctionArgList)",
            MustMatch "\\+",
            ErrorIfMatch "-")
        2;
    CASSERT(C_3 == 3)

    return NULL;
}

void CCtaTestProps::TestCtaAllMembers()
{
    ALL_MEMBERS;

    x_iIntMember = 1;
    m_sStrMember = "";
}
cta_ignore("[CA11]", "ALL_MEMBERS detected member 's_sStaticMember'");

void CCtaTestProps::TestCtaAllMembersLike()
{
    ALL_MEMBERS_LIKE(
        CCtaTestProps
        functions 
        sans super
        matching preg CTA_SZ "TestCta");

    &CCtaTestProps::TestCtaAllMembersLike;
    &CCtaTestProps::TestCtaAllMembers;

    //x_iIntMember = 1;
    //m_sStrMember = "";
}
