// CDebuxTestTypeList.cpp

#include "CDebuxProject.h"
#include "CDebuxTestTypeList.h"

CDebuxTestTypeList::CDebuxTestTypeList()
{
    _init_AttachToDebuxTestTypeList();
}

void CDebuxTestTypeList::_init_AttachToDebuxTestTypeList()
{
    //x_r->_x_p = this;
}

void CDebuxTestTypeList::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToDebuxTestTypeList();

    rFAIL("make sure it is OK to expose CDebuxTestTypeList!");
}

class CTestTypeList : public typeobject
{
    typedef /* ---------- */ typeobject super; // required for GCC without Cta
public:
    CTA_STATIC_CLASS_INFO(CTestTypeList);
    CTestTypeList();
};

    class CTestTypeListDerived1 : public CTestTypeList
    {
        typedef /* ------------------ */ CTestTypeList super; // required for GCC without Cta
    public:
        CTA_STATIC_CLASS_INFO(CTestTypeListDerived1);
        CTestTypeListDerived1();
    };

    class CTestTypeListDerived2 : public CTestTypeList
    {
        typedef /* ------------------ */ CTestTypeList super; // required for GCC without Cta
    public:
        CTA_STATIC_CLASS_INFO(CTestTypeListDerived2);
        CTestTypeListDerived2();
    };

    class CTestTypeListDerived3 : public CTestTypeList
    {
       typedef /* ------------------- */ CTestTypeList super; // required for GCC without Cta
    public:
        CTA_STATIC_CLASS_INFO(CTestTypeListDerived3);
        CTestTypeListDerived3();
    };

CTestTypeList::CTestTypeList() { }
CTestTypeListDerived1::CTestTypeListDerived1() { }
CTestTypeListDerived2::CTestTypeListDerived2() { }
CTestTypeListDerived3::CTestTypeListDerived3() { }

void CDebuxTestTypeList::OnTestClass()
{
    // Test error reporting

    //TESTLOG(
    //    "",
    //    "lib__CTypeList test:\n");

    typelist<CTestTypeList> typelistTest;

    // Rotate empty list
    {
        assertoff assertoff0(
            "typelist<> must contain 2 or more type<>s for Prev/Next (count = 0)");
        assertoff assertoffArray(
            "iAt >= 0 && iAt < m_nCount");

        type<CTestTypeList> typeTestTypeListResult = 
            typelistTest.RotateToNextType(type<CTestTypeListDerived1>());

        //TESTLOG(
        //    "",
        //    typeTestTypeListResult->GetObjectClassInfo().GetClassNameInProcess() + 
        //        "\n");
    }

    // Rotate 1-element list
    {
        assertoff assertoff1(
            "typelist<> must contain 2 or more type<>s for Prev/Next (count = 1)");

        typelistTest.Add(type<CTestTypeListDerived1>());

        type<CTestTypeList> typeTestTypeListResult = 
            typelistTest.RotateToNextType(type<CTestTypeListDerived1>());
    }

    // Rotate 2-element list
    {
        typelistTest.Add(type<CTestTypeListDerived2>());

        type<CTestTypeList> typeTestTypeList = 
            typelistTest.RotateToNextType(type<CTestTypeListDerived1>());

        ASSERT(typeTestTypeList.IsSelecting(type<CTestTypeListDerived2>()));
    }

    // Rotate 2-element from unexisting type
    {
        assertoff assertoff1(
            ("RotateToNextType: type<> not found in typelist<> map "
                "(offending type<>: " + CProject::GGetProjectFixedPrivateLogo() + 
                "-CTestTypeListDerived3)").c());

        type<CTestTypeList> typeTestTypeList = 
            typelistTest.RotateToNextType(type<CTestTypeListDerived3>());

        ASSERT(typeTestTypeList.IsSelecting(type<CTestTypeListDerived1>()));
    }
}

