// CDocListIface.h
#pragma once

#include "CDocListIfaceGp.h"

class CVelocityUnitsIfaceGp;
class CDocIface;
class CCalcupAsymmetryIfaceGp;
class CSessionIface;

//
// CDocListIface - see ifacegp
//

class CDocListIface : public CDocListIfaceGp
{
public:
    CDocListIface();
    NEW_LOCAL(CDocListImpl, CDocListIface)
    //NEW_GP(MyProject, CDocListImpl, CDocListIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Backref to parent
    ptr<CSessionIface> _x_pParentSession
            xauto(Get, Set);

    // Global inter-Media asymmetry index calculations
    ref<CCalcupAsymmetryIfaceGp> x_rCalcupAsymmetry
            xauto(Get);

    // Setup the number of media Columns on the printout
    int x_nPrintMediaColumns = 1
            xauto(Get)
            xassert(value >= 1 && value < oo);
    void x_nPrintMediaColumns_Set(
            int value)
            vhook;

    // store refs to Docs (for Duu only)
    bool x_bOwnDocs = false
            xauto(Get);
    void x_bOwnDocs_Set(
            bool value)
            vhook;

// Operations

    // Iface-level iterator
    bool IterateRegisteredDocs(
            out iter& out_i,
            out ref<CDocIface>& out_rDoc)
            vhook;

    // Added for PatientEmbed
    void CloseAllDocs()
            vhook;

    // Update main and extra doc lists
    void UpdateAllAfterDocListOrRawSlotsChange()
            vhook;

    // Support for PatientEmbed _x_bDocExcludedFromList update
    void UpdateDocListAfterDocHideShow()
            vhook;

    // Repaint media images for Gamma
    void UpdateAllAfterPaletteChange()
            vhook;

    // Global velocify units change update
    void SyncVelocityUnitsToAllRecorderDocs(
            type<CVelocityUnitsIfaceGp> typeNewVelocityUnits)
            vhook;

    // For PatientEmbed matching sSampleKey
    ptr<CDocIface> FindDocByPatientSampleKey(
            str sSampleKey)
            vhook;

    // For mem check
    struct DRecordingStats
    {
        DRecordingStats(null = null()) {}
        num _m_numTotalSeconds;
        int _m_nRecordings;
    };
    CDocListIface::DRecordingStats CalcRecordedStats()
            vhook;

// UI

    // Created for test
    ref<UiContainer> NewUiDocListMain()
            vhook;

    // Debug floater content for extra docs not visible anywhere
    ref<UiContainer> NewUiDocListExtra()
            vhook;
    ref<UiContainer> OpenUiDocListExtraFloater()
            vhook;

protected:
    virtual void Onx_nPrintMediaColumns_Set(
            int value)
            v1pure;
    virtual void Onx_bOwnDocs_Set(
            bool value)
            v1pure;
    virtual bool OnIterateRegisteredDocs(
            out iter& out_i,
            out ref<CDocIface>& out_rDoc)
            v1pure;
    virtual void OnCloseAllDocs()
            v1pure;
    virtual void OnUpdateAllAfterDocListOrRawSlotsChange()
            v1pure;
    virtual void OnUpdateDocListAfterDocHideShow()
            v1pure;
    virtual void OnUpdateAllAfterPaletteChange()
            v1pure;
    virtual void OnSyncVelocityUnitsToAllRecorderDocs(
            type<CVelocityUnitsIfaceGp> typeNewVelocityUnits)
            v1pure;
    virtual ptr<CDocIface> OnFindDocByPatientSampleKey(
            str sSampleKey)
            v1pure;
    virtual CDocListIface::DRecordingStats OnCalcRecordedStats()
            v1pure;
    virtual ref<UiContainer> OnNewUiDocListMain()
            v1pure;
    virtual ref<UiContainer> OnNewUiDocListExtra()
            v1pure;
    virtual ref<UiContainer> OnOpenUiDocListExtraFloater()
            v1pure;

private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
