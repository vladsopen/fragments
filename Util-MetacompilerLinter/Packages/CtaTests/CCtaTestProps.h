// CCtaTestProps.h
#pragma once

//
// CCtaTestProps - permanent Unit-tests for Cta
//

class CCtaTestProps : public object
{
public:
    CCtaTestProps()
        SEMIPUBLIC(.,
            AllowPath "aaaaaaa",
            AllowPath "/CGlobalTest.",
            AllowPath "/CMyCalc.",
            AllowPath "/z/z/z");

    //NEW_LOCAL(CCtaTestImpl, CCtaTestProps)
    //NEW_GP(MyProject, CCtaTestImpl, CCtaTestProps)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    int x_iIntMember = 1
            xauto(Get, Set)
            xassert(true)
            SEMIPUBLIC(
                .Get,
                AllowPath "/xsss");

// Operations

    // Test pointer return
    void* GetVoidPointer()
            vhook;
    SEMIPUBLIC(
        GetVoidPointer,
        AllowPath "aaaaaaa",
        AllowPath "/CGlobalTest.",
        AllowPath "/z/z/z");

    void DoNothingLimited(
            int nUnused)
            SEMIPUBLIC(.,
                AllowPath "aaaaaaa",
                AllowPath "/CGlobalTest.",
                AllowPath "/z/z/z")
        do
            nothing();
    

// UI

protected:
    virtual void* OnGetVoidPointer()
            v1st;

private:

    void DoNothingJustMakeCtaCheckSomeCode();

    str m_sStrMember = "";
    str s_sStaticMember = "";

    void TestBadChars();
    void ComposeSomeWrongTranslate();
    void TestCtaAllMembers();
    void TestCtaAllMembersLike();

    void TestMacros2(
            int a,
            out ref<CCtaTestProps>& out_rTest)
            { a; out_rTest }

    void TestMacros(
            int a,
            out ref<CCtaTestProps>& out_rTest)
        return
            cta_macro(
                Write "$(ThisFunctionRootName)Number($(ThisFunctionArgList))",
                Find "Number",
                Replace "2");

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
