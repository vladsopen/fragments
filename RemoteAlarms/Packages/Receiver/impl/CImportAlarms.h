// CImportAlarms.h
#pragma once

class CJobStateIface;
class CPiSampleIfaceGp;
class CPatientLinkIface;
class CTaskIface;

//
// CImportAlarms - 
//
//   Creates a Patients checkup for a received alarm
//
//ASSUME_IMPL_FOR_IFACE(CImportAlarmImpl, CImportAlarms)

class CImportAlarms : public object
{
public:
    CImportAlarms();
    //~CImportAlarms();
    //NEW_LOCAL(CImportAlarmImpl, CImportAlarms)
    //NEW_GP(Ralarm, CImportAlarmImpl, CImportAlarms)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);
    //ref<> Get ()
    //        return
    //            _x_p .Get().Valid();

    // Stats
    str GetMissingAlarmReport();

    // Check attachments ready
    void GetCurrentPiSampleStatus(
            ref<CPiSampleIfaceGp> rPiSample,
            out str& out_aText,
            out str& out_aTip);

// Operations

    // One-time opener
    //void InitImportAlarm();

    // Cleanup
    //void CloseImportAlarm();

    // Do import from meta
    path ImportAlarmAsCheckupInitAttachPath(
            ref<CTaskIface> rTask,
            ref<CPatientLinkIface> rPatientLink,
            ref<CJobStateIface> rJob);

    // Complete with the binary attachment
    void ImportAlarmBulk(
            path pathReceived,
            path pathRegged,
            bool bOffsetedPart);

// UI

    ptrmap<CUiActionGp> _m_ptrmapUiMissingAlarms;

protected:
private:
    //bool _m_bOneTimeInitImportAlarmOk = false;
    //bool _m_bImportAlarmOpened = false;

    array< ref<CTaskIface> > m_arTaskMissingPatient;

    void _init_AttachToImportAlarm();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
