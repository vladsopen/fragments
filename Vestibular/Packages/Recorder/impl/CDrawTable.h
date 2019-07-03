// CDrawTable.h
#pragma once

#include "CDrawAnythingRecorded.h"

class CPatientIndicatorIfaceGp;

//
// CDrawTable - 
//
//   Result table in Graph area.
//
//ASSUME_IMPL_FOR_IFACE(CDrawTableImpl, CDrawTable)

class CDrawTable : public CDrawAnythingRecorded
{
public:
    CDrawTable();
    //NEW_LOCAL(CDrawTableImpl, CDrawTable)
    //NEW_GP(MyProject, CDrawTableImpl, CDrawTable)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent - see base

// Operations

// UI

    // DrawTableItem
    ref<UiContainer> NewUiDrawTableItem(
            int iColumn,
            int iValue,
            ref<CPatientIndicatorIfaceGp> rIterPatientIndicator);
    ptrmap<CUiPanelGp> _m_ptrmapUiDrawTableItem;
    alikex _m_alikexItemCaption;
    alikex _m_alikexItemName;
    alikex _m_alikexItemValue;

    // DrawTable
    ref<UiContainer> NewUiDrawTable();
    ptrmap<CUiPanelGp> _m_ptrmapUiDrawTable;

protected:
    virtual void OnHandleRecordingChange()
            ;
    virtual void OnHandleCalcupDone()
            ;
    virtual void OnResetScalling()
            vnull;
    virtual ref<UiContainer> OnNewUiDrawMainGraph()
            ;
private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToDrawTable();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiDrawTable.cpp)
class UiDrawTable : public UiContainer
{
public:
    virtual void Ui();

    ref<CDrawTable> m_rDrawTable;
};

// (UiDrawTableItem.cpp)
class UiDrawTableItem : public UiContainer
{
public:
    virtual void Ui();

    ref<CDrawTable> m_rDrawTable;
    int m_iColumn = -1;
    int m_iValue = -1;
    ref<CPatientIndicatorIfaceGp> m_rIterPatientIndicator;
};
