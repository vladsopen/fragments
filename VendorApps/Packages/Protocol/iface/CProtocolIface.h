// CProtocolIface.h
#pragma once

//
// CProtocolIface -
//
//   External interface definitions
//
//ASSUME_IMPL_FOR_IFACE(CProtocolImpl, CProtocolIface)

class CProtocolIface : public object
{
public:
    CProtocolIface();
    //~CProtocolIface();
    NEW_LOCAL(CProtocolImpl, CProtocolIface)
    //NEW_GP(CustomApps, CProtocolImpl, CProtocolIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    //spec: External application always starts with a command
    //  line key followed with full .ini file path
    static const str C_sCommandLineProtocolKey = "--commini";
    static const int C_iCommandLineIniPathAt = 2; // 0:[exe] 1:[--...] 2:[.../.ini]
        // they hardcode it!

    //
    //spec: .Ini format
    //

    //***************************************************
    // External app demo does not use the named constants
    //sync:_Protocol_const_
    //***************************************************

    // [OUT]
    static const str C_sIniValueOutTrue = "1";
    static const str C_sIniValueOutFalse = "0";
    static const str C_sIniSectionOutContext = "Context";
        static const str C_sIniKeyOutSignature = "Signature";
            static const str C_sIniValueOutSignature = "Protocol";
        static const str C_sIniKeyOutModality = "Modality";
        static const str C_sIniKeyOutDirectory = "Directory";
        static const str C_sIniKeyOutOperation = "Operation";
            static const str C_sIniValueOutCreate = "Create";
            static const str C_sIniValueOutOpen = "Open";
        static const str C_sIniKeyOutExistingFilePrefix = "ExistingFilePrefix";
        static const str C_sIniKeyOutExistingFileExtension = "FileExtension";
        static const str C_sIniKeyOutUseFileRevision = "UseFileRevision";
        static const str C_sIniKeyOutReadOnly = "ReadOnly";
    static const str C_sIniSectionOutPatient = "Patient";

    // [IN]
    static const str C_sIniSectionInSaveResult = "SaveResult";
        static const str C_sIniKeyInNewFilePrefix = "NewFilePrefix";
        static const str C_sIniKeyInNewFileExtension = "FileExtension";
        static const str C_sIniKeyInUpdatedFileRevision = "UpdatedFileRevision";
        static const str C_sIniKeyInFailed = "Failed";
    static const str C_sIniSectionInSaveIndicatorValues = "SaveIndicatorValues";

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);
    //ref<> Get ()
    //        return
    //            _x_p .Get().Valid();

    //
    // [OUT]
    //

    // Modality
    str x_sIniModality = ""
            xauto(Get, Set)
            xassert(value == "CustomApps" || value == "Ecg" || value == "Press");

    // Working dir
    path x_pathIniSampleFolder = path()
            xauto(Get, Set)
            xassert(true);

    // File prefix supplied externally
    str x_sIniOpenPrefix = path()
            xauto(Get, Set)
            xassert(true);

    // Optional file extension for icar
    str x_sIniOpenFileExtension = ""
            xauto(Get, Set)
            xassert(true);

    // Updated file version
    str x_sIniUseFileRevision = ""
            xauto(Get, Set)
            xassert(true);

    // Mode
    bool x_bIniReadOnly = false
            xauto(Get, Set);

    //
    // [IN]
    //

    // New charckup: resulting file prefix
    str x_sIniNewFilePrefix = ""
            xauto(Get, Set)
            xassert(true);

    // Ecg requires this, we don't
    str x_sIniNewFileExtension = ""
            xauto(Get, Set)
            xassert(true);

    // Modify checkup: file appendix
    str x_sIniUpdatedFileRevision = ""
            xauto(Get, Set)
            xassert(true);

    // Modify checkup: file appendix
    str x_sIniResultFailed = ""
            xauto(Get, Set)
            xassert(true);

    // The table
    str x_sIniIndicatorRecordingStart = ""
            xauto(Get, Set)
            xassert(true);
    str x_sIniIndicatorRecordingDuration = ""
            xauto(Get, Set)
            xassert(true);
    str x_sIniIndicatorEcgFragmentCount = ""
            xauto(Get, Set)
            xassert(true);

// Operations

    // One-time opener
    //void InitProtocol();

    // Cleanup
    //void CloseProtocol();

    // Prepare out args
    void WriteIniContextSection(
            path pathIni)
            vhook;

    // Prepare patient data
    void WriteIniPatientField(
            path pathIni,
            str sFieldKey,
            str sProtocolName,
            str sFieldValue)
            vhook;

    // For demo mode
    void ReadIniContextSectionAsExternalApp(
            path pathIni)
            vhook;

    // From demo mode
    void WriteIniResultsAsExternalApp(
            path pathIni)
            vhook;

    // Get results
    void ReadIniResults(
            path pathIni)
            vhook;

// UI

protected:
    SEE_ALSO(CProtocolImpl)

    virtual void OnWriteIniContextSection(
            path pathIni)
            v1pure;
    virtual void OnWriteIniPatientField(
            path pathIni,
            str sFieldKey,
            str sProtocolName,
            str sFieldValue)
            v1pure;
    virtual void OnReadIniContextSectionAsExternalApp(
            path pathIni)
            v1pure;
    virtual void OnWriteIniResultsAsExternalApp(
            path pathIni)
            v1pure;
    virtual void OnReadIniResults(
            path pathIni)
            v1pure;

private:
    //bool _m_bOneTimeInitProtocolIfaceOk = false;
    //bool _m_bProtocolIfaceOpened = false;

    void _init_AttachToProtocolIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
