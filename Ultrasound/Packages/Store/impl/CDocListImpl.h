// CDocListImpl.h
#pragma once

#include "CDocListIface.h"

class CDocRegistryAbstract;
class CDocImpl;

//
// CDocListImpl - see iface
//

class CDocListImpl : public CDocListIface
{
public:
    CDocListImpl();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Current selection
    //link<CDocImpl> x_linkSelection
    //        xauto(Get);
    //void x_linkSelection_Set(link<CDocImpl> value); // UI update

// array<> Publishing

    int GetDocCount();
    // void SetCount(int nNewSize)
    //     { return m_aDoc.SetCount(nNewSize); }
    // ref<CDocImpl> GetAt(int iAt)
    //     { return m_aDoc.GetAt(iAt); }
    // void SetAt(int iAt,
    //     { m_aDoc.SetAt(iAt,
    void AddDoc(ref<CDocImpl> value);
    // void InsertAt(int iAt,
    //     { m_aDoc.InsertAt(iAt,
    // void RemoveAt(int iAt,
    //     { m_aDoc.RemoveAt(iAt,
    // void RemoveAll()
    //     { m_aDoc.RemoveAll(); }
    bool IterateDocs(out iter& out_i, out ref<CDocImpl>& out_value);
        SEE_ALSO(IterateRegisteredDocs())
    // see also IterateBackwards()

// Operations

    // First doc registration phase - only link pointers
    void StartUninitializedDocRegistration(
            ref<CDocImpl> rDoc);

    // Second doc registration phase - can do global updates
    void CompleteInitializedDocRegistration(
            ref<CDocImpl> rDoc);

    // Close an existing doc
    void UnregisterDoc(
            ref<CDocImpl> rDoc);

// UI

    // DocListMain
    //ref<UiContainer> NewUiDocListMain();
    ptr<CUiPanelGp> _m_pUiDocListMain;

    // DocListExtra
    //ref<UiContainer> NewUiDocListExtra();
    ptr<CUiPanelGp> _m_pUiDocListExtra;
    bool _x_bFindAndShowInvisibleExtraDocOnIdle = true
            xauto(Get, Set);

protected:

    // from IfaceGp
    virtual ref<CCalcupAsymmetryIfaceGp> OnGetCalcupAsymmetryRelatedToThisList()
            ;
    virtual void OnRecalcIndexesForAllMediaDocs(
            str sDebugContextInfo)
            ;
    virtual void OnHandleIndexSetupChanged()
            ;
    virtual bool OnIterateMediaDocs(
            out iter& out_i,
            out ref<CMediaDocIfaceGp>& out_value)
            ;

    // from Iface
    virtual void Onx_nPrintMediaColumns_Set(
            int value)
            ;
    virtual void Onx_bOwnDocs_Set(
            bool value)
            ;
    virtual bool OnIterateRegisteredDocs(
            out iter& out_i,
            out ref<CDocIface>& out_rDoc)
            ;
    virtual void OnCloseAllDocs()
            ;
    virtual void OnUpdateAllAfterDocListOrRawSlotsChange()
            ;
    virtual void OnUpdateDocListAfterDocHideShow()
            ;
    virtual void OnUpdateAllAfterPaletteChange()
            ;
    virtual void OnSyncVelocityUnitsToAllRecorderDocs(
            type<CVelocityUnitsIfaceGp> typeNewVelocityUnits)
            ;
    virtual ptr<CDocIface> OnFindDocByPatientSampleKey(
            str sSampleKey)
            ;
    virtual CDocListIface::DRecordingStats  OnCalcRecordedStats()
            ;
    virtual ref<UiContainer> OnNewUiDocListMain()
            ;
    virtual ref<UiContainer> OnNewUiDocListExtra()
            ;
    virtual ref<UiContainer> OnOpenUiDocListExtraFloater()
            ;

private:
    //array< ref<CDocImpl> > m_aDoc;

    //rp?!
    rp<CDocRegistryAbstract> m_rpDocRegistryAbstract;
    //ref<CDocRegistryAbstract> m_rDocRegistryAbstract;
    //bool m_bRegistryInitOk = false;

    // We cannot close all in one loop
    void CloseNextDoc();

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiDocListMain.cpp)
class UiDocListMain : public UiContainer
{
public:
    virtual void Ui();

    ref<CDocListImpl> m_rDocList;
};

// (UiDocListExtra.cpp)
class UiDocListExtra : public UiContainer
{
public:
    virtual void Ui();

    ref<CDocListImpl> m_rDocList;

private:
    bool _m_bFindAndShowInvisibleExtraDocOnIdle = false;

};
