// CStepSaveResult.cpp

#include "CProject.h"
#include "CStepSaveResult.h"
#include "CProtocolIface.h"
#include "CProxyImpl.h"

CStepSaveResult::CStepSaveResult()
{
    _init_AttachToStepSaveResult();
}

void CStepSaveResult::_init_AttachToStepSaveResult()
{
    //x_r->_x_p = this;
}

void CStepSaveResult::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToStepSaveResult();

    rFAIL("make sure it is OK to expose CStepSaveResult!");
}

str CStepSaveResult::OnGetFixedStepTitle()
{
    return "Save results.";
}

void CStepSaveResult::OnHandleActiveStepPolling()
{
    rASSERT(!x_bStepComplete);
    x_bStepComplete = true;

    //
    // Load results ini
    //

    ref<CProtocolIface> rProtocol = 
        GetProxy()->x_rProtocol;

    rProtocol->
        ReadIniResults(
            GetProxy()->x_pathProtocolIni);

    str sSendPrefix = 
        GetProxy()->
            GetSampleExternalFilePrefix();

    str sSendExtension = 
        GetProxy()->
            GetSampleFileExtension();

    bool bSendSampleInfo = false;
    bool bReportErrorStepTitle = false;

    if (rProtocol->x_sIniResultFailed != "")
    {
        bSendSampleInfo = false;

        x_sReplaceStepTitle = 
            str() +
                RichError(
                    "A failure reported by the modality: <" +
                        RichBold(rProtocol->x_sIniResultFailed) +
                        ">");
                //saveerror?
    }
    else
    if (rProtocol->x_sIniNewFilePrefix != "")
    {
        // they want Create a new one...

        if (rProtocol->x_sIniOpenPrefix != "")
        {
            bSendSampleInfo = false;

            bReportErrorStepTitle = true;
            x_sReplaceStepTitle = 
                str() +
                    RichErrorBold(
                        "Cannot create a new sample when "
                            "executing an Open command!");
        }
        else
        if (rProtocol->x_sIniUpdatedFileRevision != "")
        {
            bSendSampleInfo = false;

            bReportErrorStepTitle = true;
            x_sReplaceStepTitle = 
                str() +
                    RichErrorBold(
                        "Cannot update revision while "
                            "creating a new sample!");
        }
        else
        {
            // Load vendor dependent hacks as specified in topic B610
[...]
        bSendSampleInfo = false;
    }

    if (bReportErrorStepTitle)
    {
        GetProxy()->
            ReportProxyError(
                x_sReplaceStepTitle,
                "/ERR:INE/");
    }

    //
    // Save to Patients
    //

    if (bSendSampleInfo)
    {
        GetProxy()->
            FinalizeStudy(
                sSendPrefix,
                rProtocol->x_sIniUpdatedFileRevision,
                sSendExtension,
                rProtocol->x_sIniIndicatorRecordingStart,
                rProtocol->x_sIniIndicatorRecordingDuration,
                rProtocol->x_sIniIndicatorEcgFragmentCount);

        x_sStepDetails = 
            RichSuccess("Sample data recorded to the database.");
    }
    else
    {
        x_sStepDetails = 
            RichItalic("Nothing recorded to the database.");
    }
}
