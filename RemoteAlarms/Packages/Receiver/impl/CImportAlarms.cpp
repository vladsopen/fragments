// CImportAlarms.cpp

#include "CProject.h"
#include "CImportAlarms.h"
#include "CPatientSampleIfaceGp.h"
#include "CPatientIndicatorIfaceGp.h"
#include "CPatientIndicatorListIfaceGp.h"
#include "CPatientLinkIface.h"
#include "CPatientAlarmIface.h"
#include "CPatientEmbedLinkIface.h"
#include "CGlobalConfigIface.h"
#include "CTaskIface.h"
#include "CIoCommonIface.h"
#include "VirtOs_file.h"
#include "CLogTypeIface.h"
#include "CPiSampleIfaceGp.h"
#include "CConstIface.h"
#include "CJobStateIface.h"
#include "CFileMultipartMergeIfaceGp.h"
#include "CParseEcgFile.h"

CImportAlarms::CImportAlarms()
{
    _init_AttachToImportAlarm();
}

void CImportAlarms::_init_AttachToImportAlarm()
{
    //x_r->_x_p = this;
}

void CImportAlarms::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToImportAlarm();

    rFAIL("make sure it is OK to expose CImportAlarms!");
}

path CImportAlarms::ImportAlarmAsCheckupInitAttachPath(
        ref<CTaskIface> rTask,
        ref<CPatientLinkIface> rPatientLink,
        ref<CJobStateIface> rJob)
{
    //rASSERT(_m_bOneTimeInitReceiverSessionImplOk);

    //
    // Select the patient
    //
    if (!CProject::GGetIt().
            x_rGlobalConfig->
                _x_bImportInCurrentPatient)
    {
        ref<CPatientAlarmIface> rPatientAlarm =
            CProject::GGetIt().
                x_rPatientEmbedLink->
                    x_rPatientAlarm;

        if (!rPatientAlarm->
                SelectPatientByAuthDevice(
                    rTask->x_sAuthDevice))
        {
            TESTLOG(
                "",
                "\n"
                    "Alarm missing a patient!\n");

            m_arTaskMissingPatient.Add(rTask);
            _m_ptrmapUiMissingAlarms.InvalidateState();

            rJob->x_slocJobErrorVeryShort =
                TRANSLATE(
                    "Misstarget[... UI-string-removed]");

            return path();
        }
    }

    //
    // Send task to Patients
    //

    ref<CPatientSampleIfaceGp> rPatientSample;

    //
    // Alarm Type
    //
    {
        ref<CPatientIndicatorIfaceGp> rPatientIndicator;

        rPatientIndicator->x_sIndicatorKey = 
            "Alarm"; // internal ID

        rPatientIndicator->x_richIndicatorShortName = 
            TRANSLATE(
                "Alar[... UI-string-removed]");

        rPatientIndicator->x_richIndicatorDescription = 
            TRANSLATE(
                "Alarm type[... UI-string-removed]");

        rPatientIndicator->x_bIndicatorIsPartOfSampleTitle = 
            true; // only for important desciptive indicators

        rPatientIndicator->x_bIndicatorNameOptional = 
            true; // only if value meaning is obvious

        rPatientIndicator->x_bIndicatorHiddenByFactoryDefault = 
            false; // hide in factory setup?

        rPatientIndicator->x_richAssignTextIndicator = 
            rTask->GetNiceAlarmForUi().GetStripBoth().
                GetStub(
                    TRANSLATE(
                        "Undefined"[... UI-string-removed]");

        rPatientSample->
            x_rIndicatorList->
                AddIndicator(
                    rPatientIndicator);
    }

    //
    // Alarm Class
    //
    {
        ref<CPatientIndicatorIfaceGp> rPatientIndicator;

        rPatientIndicator->x_sIndicatorKey = 
            "AlarmClass"; // internal ID

        rPatientIndicator->x_richIndicatorShortName = 
            TRANSLATE(
                "Type[... UI-string-removed]");

        rPatientIndicator->x_richIndicatorDescription = 
            TRANSLATE(
                "Alarm clas[... UI-string-removed]");

        rPatientIndicator->x_bIndicatorIsPartOfSampleTitle = 
            true; // only for important desciptive indicators

        rPatientIndicator->x_bIndicatorNameOptional = 
            true; // only if value meaning is obvious

        rPatientIndicator->x_bIndicatorHiddenByFactoryDefault = 
            false; // hide in factory setup?

        rPatientIndicator->x_richAssignTextIndicator = 
            rTask->GetNiceAlarmClassForUi().GetStripBoth().
                GetStub(
                    "-"
                    //TRANSLATE(
                    //    "Unde[... UI-string-removed]");

        rPatientSample->
            x_rIndicatorList->
                AddIndicator(
                    rPatientIndicator);
    }

    //
    // Battery %
    //
    {
        ref<CPatientIndicatorIfaceGp> rPatientIndicator;

        rPatientIndicator->x_sIndicatorKey = 
            "BatteryPercent"; // internal ID

        rPatientIndicator->x_richIndicatorShortName = 
            TRANSLATE(
                "Charge %",[... UI-string-removed]");

        rPatientIndicator->x_richIndicatorDescription = 
            TRANSLATE(
                "Device bat[... UI-string-removed]");

        rPatientIndicator->x_bIndicatorIsPartOfSampleTitle = 
            false; // only for important desciptive indicators

        rPatientIndicator->x_bIndicatorNameOptional = 
            false; // only if value meaning is obvious

        rPatientIndicator->x_bIndicatorHiddenByFactoryDefault = 
            false; // hide in factory setup?

        rPatientIndicator->x_richAssignTextIndicator = 
            Str(rTask->x_nBatteryPercent) + "%";

        rPatientSample->
            x_rIndicatorList->
                AddIndicator(
                    rPatientIndicator);
    }

    //
    // Task ID
    //
    {
        ref<CPatientIndicatorIfaceGp> rPatientIndicator;

        rPatientIndicator->x_sIndicatorKey = 
            "TaskId"; // internal ID

        rPatientIndicator->x_richIndicatorShortName = 
            "ID";

        rPatientIndicator->x_richIndicatorDescription = 
            TRANSLATE(
                "Debug Info[... UI-string-removed]");

        rPatientIndicator->x_bIndicatorIsPartOfSampleTitle = 
            true; // only for important desciptive indicators

        rPatientIndicator->x_bIndicatorNameOptional = 
            true; // only if value meaning is obvious
                // "123 KHz" -- don't need to prefix with "Frequency"

        rPatientIndicator->x_bIndicatorHiddenByFactoryDefault = 
            false; // hide in factory setup?

        rPatientIndicator->x_richAssignTextIndicator = 
            rTask->x_sTaskId;

        rPatientSample->
            x_rIndicatorList->
                AddIndicator(
                    rPatientIndicator);
    }

    //
    // Save attached data storage
    //

    // Save an empty "promise" file 
    //   to be updated when reception is complete
    path pathAttach;
    {
        pathAttach = 
            rPatientLink->
                GetNewFullAttachmentFilePath().
                GetFolder().
                GetAppendedPath(rTask->x_sTaskId + 
                    CIoCommonIface::C_pathTeleAlarmExt);

        str sSaveError;
        if failed(file(pathAttach).Save("", out sSaveError))
        {
            type<CLogTypeIface>()->
                AsLogTypeForReceiverErrors()->
                    LogJob(
                        "Failed to create a task/alarm attachment file.",
                        "/ERR:RSA/ " + sSaveError + "\n" +
                            rTask->x_sTaskId);
        }

        // Attach the file to the sample
        rPatientSample->x_pathAttachmentPrefix = 
            pathAttach;
    }

    //
    // Send to Patients
    //

    rPatientLink->
        HandleStartNewCheckup();

    rPatientLink->
        SendSampleToPatient(
            rPatientSample);

    rJob->
        SaveJobSampleKey(
            rPatientSample->x_sSampleKey);

    return pathAttach;
}

[...]