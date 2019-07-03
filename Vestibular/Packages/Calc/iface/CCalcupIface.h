// CCalcupIface.h
#pragma once

class CRecordTypeIface;
class CSessionIface;
class CPatientSampleIfaceGp;
class CCalcRouteIndexesIface;

//
// CCalcupIface -
//
//   Calculates indexes from the route arrays.
//
ASSUME_IMPL_FOR_IFACE(CCalcupImpl, CCalcupIface)

class CCalcupIface : public object
{
public:
    CCalcupIface();
    NEW_LOCAL(CCalcupImpl, CCalcupIface)
    //NEW_GP(Balance, CCalcupImpl, CCalcupIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CSessionIface> _x_pSession
            xauto(Get, Set);

    // Result dump
    rich GetCalcupReport()
            vhook;

    // Result in Patients format
    ref<CPatientSampleIfaceGp> x_rHackResultAsPatientSample
            xauto(Get);

    // Result for a specific record type
    ptr<CCalcRouteIndexesIface> GetLastResultsForRecordType(
            type<CRecordTypeIface> typeRecordType)
            vhook;

// Operations

    // Do recalc
    void UpdateCalcup()
            vhook;

    // Patients indicators
    void ExportIndexesToPatientIndicatorList(
            ref<CPatientSampleIfaceGp> rPatientSample)
            vhook;

    // export index value received from outside of Balance
    // as Balance-like index
    void ExportExternalPatientIndicatorTextual(
            rich richValue,
            str sInternalIndexKeyBase,
            sloc slocLongIndexName,
            sloc slocShortIndexName,
            bool bHiddenByFactoryDefault,
            type<CRecordTypeIface> typeRecordType,
            ref<CPatientSampleIfaceGp> rPatientSample)
            vhook;

    // Nice and round
    num RoundIndexForIndication(
            num numRaw)
            vhook;

// Operations

// UI

    // Game insertion
    ref<UiContainer> NewUiCalcupIndicatorsForGames()
            vhook;

protected:
    virtual rich OnGetCalcupReport()
            v1pure;
    virtual ptr<CCalcRouteIndexesIface> OnGetLastResultsForRecordType(
            type<CRecordTypeIface> typeRecordType)
            v1pure;
    virtual void OnUpdateCalcup()
            v1pure;
    virtual void OnExportIndexesToPatientIndicatorList(
            ref<CPatientSampleIfaceGp> rPatientSample)
            v1pure;
    virtual num OnRoundIndexForIndication(
            num numRaw)
            v1pure;
    virtual ref<UiContainer> OnNewUiCalcupIndicatorsForGames()
            v1pure;
    virtual void OnExportExternalPatientIndicatorTextual(
            rich richValue,
            str sInternalIndexKeyBase,
            sloc slocLongIndexName,
            sloc slocShortIndexName,
            bool bHiddenByFactoryDefault,
            type<CRecordTypeIface> typeRecordType,
            ref<CPatientSampleIfaceGp> rPatientSample)
            v1pure;
private:
    //bool _m_bOneTimeInitCalcupIfaceOk = false;

    void _init_AttachToCalcupIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
