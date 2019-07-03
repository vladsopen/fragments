// CCalcMarkingImpl.h
#pragma once

#include "CCalcMarkingIface.h"

class CCalcMachineIfaceGp;
class CUsoundIndexTypeIfaceGp;
class CCalcMarkingConfigImpl;
class CCalcLineList;

//
// CCalcMarkingImpl - see iface (Manual calculation mode for USB Doc)
//

class CCalcMarkingImpl : public CCalcMarkingIface
{
public:
    CCalcMarkingImpl();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // All markers
    ref<CCalcLineList> _x_rCalcLineList
            xauto(Get);

    // Config
    ref<CCalcMarkingConfigImpl> GetGlobalCalcMarkingConfig();

    // For media button
    res GetFormulaEditorLargeIcon();

// Operations

    void OpenFormulaEditor();

    // Called after manual line move (except for M-line!)
    void UpdateMeanVelocity();

// UI

    // CalcMarkingMain
    //ref<UiContainer> NewUiCalcMarkingMain();
    ptr<CUiPanelGp> _m_pUiCalcMarkingMain;

protected:

    virtual num OnGetCalcMakerPos(
            ECalcMarkingLine eCalcMarkingLine,
            type<CUsoundAxisIfaceGp> rUsoundAxis)
            ;

    virtual num OnGetOriginalCalcMakerIndexValueToRestoreOrOo(
            ECalcMarkingLine eCalcMarkingLine,
            type<CUsoundAxisIfaceGp> rUsoundAxis)
            ;

    virtual void OnPrepareCalcMarkingForCalcModeWuu(
            type<CUsoundCalcModeIfaceGp> typeUsoundCalcMode)
            ;

    virtual ref<UiContainer> OnNewUiCalcMarkingMainOverlay()
            ;

private:

    // init parent back pointers in our members
    void _init_AttachProperties();

    // Calc init helper
    void DefineCalcVariableFromUsoundIndex(
            type<CUsoundIndexTypeIfaceGp> typeUsoundIndexType,
            ref<CCalcMachineIfaceGp> rCalcMachine);
    void DefineCalcVariable(
            str sName,
            str sFormula,
            ref<CCalcMachineIfaceGp> rCalcMachine);


    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiCalcMarkingMain.cpp)
class UiCalcMarkingMain : public UiContainer
{
public:
    virtual void Ui();

    ref<CCalcMarkingImpl> m_rCalcMarking;
};

