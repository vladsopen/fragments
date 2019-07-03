// CJobQuest.h
#pragma once

class CJobQuestType;
class CNetLocatorIfaceGp;
class CNetDriverIfaceGp;
class CNetPumpOutIfaceGp;
class CJobStateImpl;

//
// CJobQuest - 
//
//   Remote request to device
//
//ASSUME_IMPL_FOR_IFACE(CJobQuestImpl, CJobQuest)

class CJobQuest : public object
{
public:
    CJobQuest();
    ~CJobQuest();
    //NEW_LOCAL(CJobQuestImpl, CJobQuest)
    //NEW_GP(Ralarm, CJobQuestImpl, CJobQuest)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CJobStateImpl> _x_pJobState
            xauto(Get, Set);
    ref<CJobStateImpl> GetJobState()
            return
                _x_pJobState.Get().Valid();

    // Status, Pairing, ...
    type<CJobQuestType> x_typeQuest
            xauto(Get, Set);

    // Quest uploader
    rp<CNetPumpOutIfaceGp> x_rpNetPumpOutQuest
            xauto(Get);

// Operations

    // One-time opener
    void StartJobQuest(
            str sJson,
            out str& out_sError);
    void TryInitExistingJobQuest();

    // Cleanup
    void CloseJobQuest();

// UI

    // JobQuestStatusRow
    ref<UiContainer> NewUiJobQuestStatusRow();
    ptrmap<CUiPanelGp> _m_ptrmapUiJobQuestStatusRow;

protected:
private:
    bool _m_bOneTimeInitJobQuestOk = false;

    bool _m_bActivated = false;

    ref<CNetDriverIfaceGp> _m_rCacheQuestUploadDriver;
    ref<CNetLocatorIfaceGp> _m_rCacheQuestUploadLocator;

    void InitJobQuest();

    void StartQuestPumpOut(
            path pathUpload);

    path GetJobQuestUploadFile(
            out str& out_sError);

    friend class CTestSendReceive;

    void _init_AttachToJobQuest();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiJobQuestStatusRow.cpp)
class UiJobQuestStatusRow : public UiContainer
{
public:
    virtual void Ui();

    ref<CJobQuest> m_rJobQuest;

private:
};
