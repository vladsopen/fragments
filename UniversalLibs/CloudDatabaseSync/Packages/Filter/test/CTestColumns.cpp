// CTestColumns.cpp

#include "CProject.h"
#include "CTestColumns.h"
#include "CColumnOrder.h"
#include "CFilterImpl.h"
#include "CDbFilter.h"

CTestColumns::CTestColumns()
{
    _init_AttachToTestColumns();
}

void CTestColumns::_init_AttachToTestColumns()
{
    //x_r->_x_p = this;
}

void CTestColumns::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToTestColumns();

    rFAIL("make sure it is OK to expose CTestColumns!");
}

/*
CTestColumns::~CTestColumns()
{
    rASSERT(!_m_bOpened);
}

void CTestColumns::InitTestColumns()
{
    rASSERT(!_m_bInited);
    _m_bInited = true;
    _m_bOpened = true;
}

void CTestColumns::CloseTestColumns()
{
    if (_m_bOpened)
    {
        _m_bOpened = false;
        // ...
    }
}
*/

void CTestColumns::OnTestClass()
{
    assertoff ao("make sure it is OK to expose");

    ref<CColumnOrder> rColumnOrder;

    ref<CFilterImpl> rFilter;
    rColumnOrder->_x_pFilter = rFilter;
    ref<CDbFilter> rDbFilter = rFilter->x_rDbFilter;

    rColumnOrder->_m_bDontCommitForTests = true;

    unidef unidefOrder;
    unidefOrder.DefineStr("Grow", C_sSqlAsc);
    unidefOrder.DefineStr("Neutral", "");
    unidefOrder.DefineStr("Inverse", C_sSqlDesc);

    unidef unidefShow;
    unidefShow.DefineStr("Neutral", "");
    unidefShow.DefineStr("Grow", "-");
    unidefShow.DefineStr("Inverse", "junk(visible)");

    TESTLOG(
        "",
        "\n\n"
            "-------- Input ---------\n\n"
            "===Order===\n" +
                unidefOrder.ExportAsTextWithoutHeader() +
            "\n"
            "===Show===\n" + 
                unidefShow.ExportAsTextWithoutHeader() +
            "\n"
            "\n");

    rColumnOrder->
        ReinitOrderFromUnidef(
            unidefOrder,
            unidefShow);

    TESTLOG(
        "",
        "\n\n"
            "-------- Output ---------\n\n"
            "===DbFilter===\n" + 
                rDbFilter->ExposeOut().ExportAsTextWithoutHeader() +
            "\n"
            "===Columns===\n" + 
                rColumnOrder->ExposeOut().ExportAsTextWithoutHeader().
                    GetReplaced("\\n", "\n") +
            "\n"
            "\n");

    key keyColumn1 = key::GFromStr("Neutral");
    key keyColumn2 = key::GFromStr("Grow");
    bool bShow1 = true;
    bool bShow2 = false;
    int nMoveDirection = 0;
    repeat(iChange, 4)
    {
        TESTLOG(
            "",
            "\n\n"
                "-------- Changing visibiliy ---------\n\n" +
                "  " + rDbFilter->x_sFilterColumns + "\n");

        rColumnOrder->
            ShowColumn(
                keyColumn1,
                bShow1);

        rColumnOrder->
            ShowColumn(
                keyColumn2,
                bShow2);

        rColumnOrder->
            MoveColumn(
                keyColumn1,
                nMoveDirection);

        bShow1 ^= 1;
        if (iChange >= 2)
        {
            keyColumn1 = key::GFromStr("Inverse");
            bShow2 ^= 1;
            bShow1 = true;
        }

        nMoveDirection++;
        if (nMoveDirection > 1)
            nMoveDirection = -1;

        TESTLOG(
            "",
            "\n\n"
                "===DbFilter===\n" + 
                    rDbFilter->ExposeOut().ExportAsTextWithoutHeader() +
                "\n"
                "===Columns===\n" + 
                    rColumnOrder->ExposeOut().ExportAsTextWithoutHeader().
                        GetReplaced("\\n", "\n") +
                "\n"
                "\n");
    }

    TESTLOG(
        "",
        "\n\n"
            "-------- Adding two more columns ---------\n\n");

    unidefShow.DefineStr("NewVisible", "");
    unidefShow.DefineStr("NewHidden", "-");

    rColumnOrder->
        ReinitOrderFromUnidef(
            unidefOrder,
            unidefShow);

    TESTLOG(
        "",
        "\n\n"
            "===DbFilter===\n" + 
                rDbFilter->ExposeOut().ExportAsTextWithoutHeader() +
            "\n"
            "===Columns===\n" + 
                rColumnOrder->ExposeOut().ExportAsTextWithoutHeader().
                    GetReplaced("\\n", "\n") +
            "\n"
            "\n");

    repeat(iClicks, 3)
    {
        TESTLOG(
            "",
            "\n\n"
                "-------- Clicking 'Inverse' column " + 
                Str(iClicks + 1) + "th time ---------\n\n");

        rColumnOrder->
            ChangeColumnOrder(
                key::GFromStr("Inverse"));

        TESTLOG(
            "",
            "\n\n"
                "===DbFilter===\n" + 
                    rDbFilter->ExposeOut().ExportAsTextWithoutHeader() +
                "\n"
                "===Columns===\n" + 
                    rColumnOrder->ExposeOut().ExportAsTextWithoutHeader().
                        GetReplaced("\\n", "\n") +
                "\n"
                "\n");
    }

    TESTLOG(
        "",
        "\n\n"
            "end.");
}

