// CImportCheckups.h
#pragma once

class CPiSampleIfaceGp;
class CPiCheckupIfaceGp;
class CPatInfoStoreIfaceGp;
class CJobStateIface;
class CPatInfoSessionIfaceGp;
class CSessionIface;

//
// CImportCheckups - 
//
//   Processes downloaded ExpertMail checkups
//
//ASSUME_IMPL_FOR_IFACE(CImporterImpl, CImportCheckups)

class CImportCheckups : public object
{
public:
    CImportCheckups();
    //~CImportCheckups();
    //NEW_LOCAL(CImporterImpl, CImportCheckups)
    //NEW_GP(Ralarm, CImporterImpl, CImportCheckups)
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

    // Reentry preventer
    bool IsImportCheckupsInProgress();

// Operations

    // One-time opener
    //void InitImporter();

    // Cleanup
    //void CloseImporter();

    // Scan inbox dir for completions
    void PollImportNewCheckups(
            path pathPollExtType,
            path pathReggedExtType,
            path pathTempMergedIn,
            ref<CSessionIface> rSession,
            ptr<CPatInfoSessionIfaceGp> pPatInfoSession);

    // Short textual part
    str ImportExpertMailCheckupInitial(
            path pathDownload,
            out str& out_sError,
            ref<CSessionIface> rSession,
            ref<CPatInfoSessionIfaceGp> rPatInfoSession,
            ref<CJobStateIface> rJob);

    // Unpack attach archive
    bool ImportCheckupAttachments(
            path pathDownload,
            out str& out_sError,
            ref<CSessionIface> rSession,
            ptr<CPatInfoSessionIfaceGp> pPatInfoSession);

// UI

protected:
private:
    //bool _m_bOneTimeInitImporterOk = false;
    //bool _m_bImporterOpened = false;
    bool m_bImportCheckupsInProgress = false;

    friend class CTestSendReceive;

    void _init_AttachToImporter();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
