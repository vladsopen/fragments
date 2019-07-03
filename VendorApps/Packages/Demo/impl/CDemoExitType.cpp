// CDemoExitType.cpp

#include "CProject.h"
#include "CDemoExitType.h"
#include "CProtocolIface.h"
#include "CDemoImpl.h"

CDemoExitType::CDemoExitType()
{
}

const typelist<CDemoExitType> CDemoExitType::GetDerivedList()
{
    typelist<CDemoExitType> typelistDerived;
    if (typelistDerived.IsNeedInit(__super::GetDerivedListStorage()))
    {
        typelistDerived.AddDerivedClassesWithFlag("flag_List_as_DemoExitType");
    }

    return typelistDerived;
}

void CDemoExitTypeIsCreateNew::OnMakeDemoExit(
        ref<CDemoImpl> rDemo,
        out str& out_sError)
{
    ref<CProtocolIface> rProtocol =
        rDemo->x_rExternalProtocol;

    if (rProtocol->x_sIniNewFilePrefix == "")
    {
        out_sError = 
            "Creating a new sample requires "
                "a valid NewFilePrefix.";
        return;
    }

    path pathIni = rDemo->GetCommandLineIni();

    if ok(pathIni != "")
    {
        str sError;
        rDemo->SaveDemoDataFile(out sError);
        rASSTR(sError); 

        if (sError == "")
        {
            rProtocol->
                WriteIniResultsAsExternalApp(
                    pathIni);
        }
        else
        {
            out_sError = sError;
        }
    }
    else
    {
        out_sError = "Missing .ini";
    }
}

void CDemoExitTypeIsUpdatedVersion::OnMakeDemoExit(
        ref<CDemoImpl> rDemo,
        out str& out_sError)
{
    ref<CProtocolIface> rProtocol =
        rDemo->x_rExternalProtocol;

    if (rProtocol->x_sIniUpdatedFileRevision == "")
    {
        out_sError = 
            "Updating an existing sample requires "
                "a valid UpdatedFileRevision.";
        return;
    }

    path pathIni = rDemo->GetCommandLineIni();

    if ok(pathIni != "")
    {
        str sError;
        rDemo->SaveDemoDataFile(out sError);
        rASSTR(sError); 

        if (sError == "")
        {
            rProtocol->
                WriteIniResultsAsExternalApp(
                    pathIni);
        }
        else
        {
            out_sError = sError;
        }
    }
    else
    {
        out_sError = "Missing .ini";
    }
}

void CDemoExitTypeIsNothing::OnMakeDemoExit(
        ref<CDemoImpl> rDemo,
        out str& out_sError)
{
    rDemo;
    out_sError;

    // nop, OK
}

void CDemoExitTypeIsFail::OnMakeDemoExit(
        ref<CDemoImpl> rDemo,
        out str& out_sError)
{
    ref<CProtocolIface> rProtocol =
        rDemo->x_rExternalProtocol;

    if (rProtocol->x_sIniResultFailed == "")
    {
        out_sError = 
            "Please enter some error text to exit with "
                "a Failed result.";
        return;
    }

    path pathIni = rDemo->GetCommandLineIni();

    if ok(pathIni != "")
    {
        rProtocol->
            WriteIniResultsAsExternalApp(
                pathIni);
    }
    else
    {
        out_sError = "Missing .ini";
    }
}

