// CDebuxTestUnidef.cpp

#include "CDebuxProject.h"
#include "CDebuxTestUnidef.h"
#include "CAppEventIfaceGp.h"

CDebuxTestUnidef::CDebuxTestUnidef()
{
    _init_AttachToDebuxTestUnidef();
}

void CDebuxTestUnidef::_init_AttachToDebuxTestUnidef()
{
    //x_r->_x_p = this;
}

void CDebuxTestUnidef::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToDebuxTestUnidef();

    rFAIL("make sure it is OK to expose CDebuxTestUnidef!");
}

void CDebuxTestUnidef::TestBinaryExport()
{
    assertoff ao("varFUTURE!");

    unidef unidefTest;

    unidefTest.
        DefineStr(
            "NormalString", 
            "some text");

    unidefTest.
        DefineInt(
            "NormalInterger", 
            12345);

    unidefTest.
        DefineStr(
            "tst___future___ParseIgnoreString", 
            "invisible");

    unidefTest.
        DefineUnidef(
            "tst___future___ParseIgnoreStruct", 
            ref<CAppEventIfaceGp>()->ExposeOut());

    unidefTest.
        DefineStr(
            "TheEnd", 
            "last text");

    TESTLOG("", "\n\n\n----- varFUTURE test -----------\n\n");

    TESTLOG(
        "", 
        "\n" +
            "Normal ExportAsText: " +
            unidefTest.ExportAsText() +
            "\n\n");

    sbuf sbufBin = unidefTest.ExportAsBinary();
    unidef unidefImported;
    str sError;
    rASSERT(unidefImported.Import(sbufBin, out sError));

    TESTLOG(
        "", 
        "\n" +
            "Imported from binary: " +
            unidefImported.ExportAsText() +
            "\n\n");
   
}

void CDebuxTestUnidef::LogUnidefImport(
        str sScript)
{
    str sLog = 
        "\n<" + sScript + ">\n";

    unidef unidefImported;
    str sError;
    if (!unidefImported.Import((sbuf) sScript, out sError))
    {
        sLog += "***** Failed: " + sError;
    }
    else
    if (unidefImported.GetBranchCount() == 0)
    {
        sLog += "[[Empty]]";
    }
    else
    {
        str sExported = 
            unidefImported.
                ExportAsTextWithoutHeader().
                    GetStripBoth(CHARMAP(" \r\n"));

        sLog += sExported;

        unidef unidefReImport;
        if (!unidefReImport.Import((sbuf) sExported, out sError))
        {
            rFAIL(
                "unidef failed to import what we just exported!\n"
                    "\n" +
                    sScript + 
                    "\n\n" +
                    sExported);
        }
        else
        {
            str sExported2 = 
                unidefReImport.
                    ExportAsTextWithoutHeader().
                        GetStripBoth(CHARMAP(" \r\n"));

            rASSERT(sExported == sExported2);
        }
    }

    sLog += "\n\n";

    TESTLOG("", sLog);
}

void CDebuxTestUnidef::OnTestClass()
{
    LogUnidefImport(
        "");

    LogUnidefImport(
        " ");

    LogUnidefImport(
        "a");

    LogUnidefImport(
        "1");

    LogUnidefImport(
        "_a1");

    LogUnidefImport(
        "_1_");

    LogUnidefImport(
        "_");

    LogUnidefImport(
        "1a");

    LogUnidefImport(
        "a=1");

    LogUnidefImport(
        "a . 1");

    LogUnidefImport(
        "a . { b = 1 }");

    LogUnidefImport(
        "{ b = 1 }");

    LogUnidefImport(
        "{ a . { b = 1 } }");

    LogUnidefImport(
        "$\"\" = -1");

    LogUnidefImport(
        "\"\" = -1");

    LogUnidefImport(
        "$\"!\" = 111111111112222222222");

    LogUnidefImport(
        "___ = 111111111112222222222LL,;,,,,;");

    LogUnidefImport(
        "Max_64 = 9223372036854775806LL");
    
    LogUnidefImport(
        "NoSemi = { val = 1 }");

    LogUnidefImport(
        "{}"); // OK in JSON!

    LogUnidefImport(
        "Empty = { }");

    LogUnidefImport(
        "SuperEmpty : { {{{{ }}}} }");

    LogUnidefImport(
        "y7 = { xxx = 2+3; }");

    LogUnidefImport(
        "b . { x = 2 }");

    LogUnidefImport(
        "b = { x = 2; }");

    LogUnidefImport(
        "$\"@\" = \"Line\\n\";");

    LogUnidefImport(
        "\"foo\" = \"bar\"");

    LogUnidefImport(
        "xxx : 1");

    LogUnidefImport(
        "\"\" : 1");

    LogUnidefImport(
        "\"xxx!\" : -0");

    LogUnidefImport(
        "x = { y = 1; z = 2; }");

    LogUnidefImport(
        "x : { y : 1; z : 2; }");

    LogUnidefImport(
        "x : { y : 1, z : 2, }");

    LogUnidefImport(
        "top : { " 
            "nest = " 
                "{inner:\"x\", inside1:{x}, inside2:{y:{z}} }" 
        "}");

    LogUnidefImport(
        "// asdasdasd\n \"wow!\" : { \"1\" : -0.5e20, \"\" : -0 }");

    LogUnidefImport(
        "Foo = \"123\" \"456\", \"Bar\" : -1--1");

    TestBinaryExport();
}

