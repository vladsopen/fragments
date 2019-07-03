// CDebuxTestRingShared.cpp

#include "CDebuxProject.h"
#include "CDebuxTestRingShared.h"

CDebuxTestRingShared::CDebuxTestRingShared()
{
    _init_AttachToDebuxTestRingShared();
}

void CDebuxTestRingShared::_init_AttachToDebuxTestRingShared()
{
    //x_r->_x_p = this;
}

void CDebuxTestRingShared::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToDebuxTestRingShared();

    //rFAIL("make sure it is OK to expose CDebuxTestRingShared!");
}

struct DItem
{
    DItem(str s = "")
    {
        lowlevel::memmove(data, s.c(), math::GMin(int(s.Len() + 1), int(sizeof(data) - 1)));
        data[sizeof(data) - 1] = 0;
    }

    operator str() const
    {
        return data;
    }

    char data[1000];
};

#define ITEM DItem

str GDumpStrRing(
        CInterRing<ITEM>& ringTest)
{
    str sDump;
    sDump.ReserveAlloc(1000);

    sDump += "  virtual: ";
    repeat(i, ringTest.GetCurrentSavedCount())
    {
        big iiAt = i + ringTest.GetLostCount();
        sDump += "<";
        sDump += ringTest.GetItemAtVirtualPosOr(iiAt, "?");
        sDump += ">";
    }

    sDump += " [v.total " + Str(ringTest.GetAddedVirtualCount()) + "]";

    sDump += "\n";
    sDump += "  _planar: ";
    repeat(i, ringTest.GetMaxSavedCount())
    {
        sDump += "<";
        sDump += ringTest._internal_GetAtInternalArray(i);
        sDump += ">";
    }

    sDump += " (circular at " + Str(ringTest._internal_GetRingArrayNextFreeIndex()) + ")\n";

    return sDump;
}

void GTestPop(
        out CInterRing<ITEM>& out_ringTest)
{
    ITEM item = ITEM();
    bool bOverflow = false;
    if (out_ringTest.PopItem(out item, out bOverflow))
    {
        TESTLOG(
            "",
            "\n" + 
                "Popped: " + Str(item) + ""
                "\n");
    }
    else
    {
        TESTLOG(
            "",
            "\n" + 
                "Nothing to pop!"
                "\n");
    }

    if (bOverflow)
    {
        TESTLOG(
            "",
                "OVERFLOW DETECTED!"
                "\n");
    }
}

void CDebuxTestRingShared::OnTestClass()
{
    CInterRing<ITEM> ringTest;

    ringTest.InitInterRing("TestCircularStrings", 3);

    TESTLOG(
        "",
        "Init:\n" + 
            GDumpStrRing(ringTest) + 
            "\n");

    GTestPop(
        out ringTest);
    GTestPop(
        out ringTest);
    GTestPop(
        out ringTest);

    int nItems = 8;
    int iAdd = 0;
    repeat(i, nItems)
    {
        ringTest.AddItem(Str(iAdd));
        iAdd++;

        TESTLOG(
            "",
            GDumpStrRing(ringTest) + 
                "\n");
    }

    repeat(i, nItems * 10)
    {
        GTestPop(
            out ringTest);

        int nAddMore = i % 10 < 3? 0 : i % (3 + i / 10);
        repeat(iMore, nAddMore)
        {
            str sAdded = Str(iAdd);
            ringTest.AddItem(sAdded);
            iAdd++;

            TESTLOG(
                "",
                "\n" + 
                    "Added: " + sAdded + ""
                    "\n");
        }
    }

[...]