// CRecorderIface.h
#pragma once

class CReadingIface;
class CDbReadingIface;
class CRangesIface;
class CSessionIface;

//
// CRecorderIface -
//
//   Controls route recording and graph representations.
//
ASSUME_IMPL_FOR_IFACE(CRecorderImpl, CRecorderIface)

class CRecorderIface : public object
{
public:
    CRecorderIface();
    NEW_LOCAL(CRecorderImpl, CRecorderIface)
    //NEW_GP(Balance, CRecorderIface, CRecorderIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CSessionIface> _x_pSession
            xauto(Get, Set);

    // Query the recording
    rowset<CDbReadingIface> GetReadingRecording()
            vhook;

    // Recorded device metrics
    ref<CRangesIface> GetRanges()
            vhook;

// Operations

    // Data receiving
    void ReceiveReadingForRecorder(
            ref<CReadingIface> rReading,
            ref<CRangesIface> rRanges)
            vhook;

    // Clear
    void HandleStartNewRecording()
            vhook;

    // Finalize
    void HandleEndCompleteRecording()
            vhook;

    // Extra updates after calcup
    void HandleCalcupDone()
            vhook;

    // Set temporary internal modes for Patients image
    void HandleInsideSaveToPatient(
            bool bInside)
            vhook;

    // Switch to all-graph view
    void SelectViewForPatientPicture()
            vhook;

    // Recalc scaling
    void ResetAllScaling()
            vhook;

// UI

    // Graphs
    ref<UiContainer> NewUiRecorderGraphs()
            vhook;

    // RecorderToolbar
    ref<UiContainer> NewUiRecorderToolbar()
            vhook;

    // RecorderGraphSelector
    ref<UiContainer> NewUiRecorderGraphSelector()
            vhook;

    // RecorderMain
    ref<UiContainer> NewUiRecorderMain()
            vhook;

    // Indexes
    ref<UiContainer> NewUiDrawTableMainGraph()
            vhook;

    // All-Graphs view
    ref<UiContainer> NewUiDrawAll()
            vhook;

protected:
    virtual rowset<CDbReadingIface> OnGetReadingRecording()
            v1pure;
    virtual ref<CRangesIface> OnGetRanges()
            v1pure;
    virtual void OnReceiveReadingForRecorder(
            ref<CReadingIface> rReading,
            ref<CRangesIface> rRanges)
            v1pure;
    virtual void OnHandleStartNewRecording()
            v1pure;
    virtual void OnHandleEndCompleteRecording()
            v1pure;
    virtual void OnHandleCalcupDone()
            v1pure;
    virtual void OnHandleInsideSaveToPatient(
            bool bInside)
            v1pure;
    virtual void OnSelectViewForPatientPicture()
            v1pure;
    virtual void OnResetAllScaling()
            v1pure;
    virtual ref<UiContainer> OnNewUiRecorderGraphs()
            v1pure;
    virtual ref<UiContainer> OnNewUiRecorderToolbar()
            v1pure;
    virtual ref<UiContainer> OnNewUiRecorderGraphSelector()
            v1pure;
    virtual ref<UiContainer> OnNewUiRecorderMain()
            v1pure;
    virtual ref<UiContainer> OnNewUiDrawTableMainGraph()
            v1pure;
    virtual ref<UiContainer> OnNewUiDrawAll()
            v1pure;
private:
    //bool _m_bOneTimeInitRecorderIfaceOk = false;

    void _init_AttachToRecorderIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
