// CDrawAnythingRecorded.h
#pragma once

#include "VirtOs_ontimer.h"

class CRecorderImpl;

//
// CDrawAnythingRecorded -
//
//   A base class for all recording drawers.
//
//ASSUME_IMPL_FOR_IFACE(CDrawAnythingRecordedImpl, CDrawAnythingRecorded)

class CDrawAnythingRecorded : public object
{
public:
    CDrawAnythingRecorded();
    //NEW_LOCAL(CDrawAnythingRecordedImpl, CDrawAnythingRecorded)
    //NEW_GP(MyProject, CDrawAnythingRecordedImpl, CDrawAnythingRecorded)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // Idle update
    static const tick C_tickPostUpdateGraph = 30;

// Attributes

    // parent
    ptr<CRecorderImpl> _x_pRecorder
            xauto(Get);
    void _x_pRecorder_Set(
            ptr<CRecorderImpl> pRecorder)
            vhooked;

// Operations

    // Update online
    void HandleRecordingChange()
            vhooked;

    // Calcup changed
    void HandleCalcupDone()
            vhook;

    // Idle update (for auto rescale)
    void DeferGraphUpdate();

    // Call updates
    void UpdateGraphNow()
            vhook;

    // Reinit after deleting data
    void ResetScalling()
            vhook;

    // Handle end of recording
    void FinalizeAtRecordingStop()
            vhook;

// UI

    // Relevant main UI
    ref<UiContainer> NewUiDrawMainGraph()
            vhook;

protected:
    virtual void On_x_pRecorder_Set(
            ptr<CRecorderImpl> pRecorder)
            v1null;
    virtual void OnHandleRecordingChange()
            v1pure;
    virtual void OnUpdateGraphNow()
            v1null;
    virtual void OnHandleCalcupDone()
            v1null;
    virtual void OnResetScalling()
            v1pure;
    virtual void OnFinalizeAtRecordingStop()
            v1null;
    virtual ref<UiContainer> OnNewUiDrawMainGraph()
            v1pure;
private:

    // Timer handler
    void HandlePostUpdateGraphTimer();
    ontimer<HandlePostUpdateGraphTimer> _m_ontimerPostUpdateGraphTimer;

    void _init_AttachPropertiesForDrawAnythingRecorded();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
