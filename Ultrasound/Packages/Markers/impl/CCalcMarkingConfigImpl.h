// CCalcMarkingConfigImpl.h
#pragma once

#include "CCalcMarkingConfigIface.h"

class CCalcMachineIfaceGp;

//
// CCalcMarkingConfigImpl - see iface
//

class CCalcMarkingConfigImpl : public CCalcMarkingConfigIface
{
public:
    CCalcMarkingConfigImpl();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Calculator
    ref<CCalcMachineIfaceGp> GetLoadedCalcMachine();

// Operations

    // Open formula editor in config mode
    void OpenFormulaEditorConfig();

// UI

    // CalcMarkingConfig
    ref<UiContainer> NewUiCalcMarkingConfig();
    ptrmap<CUiPanelGp> _m_ptrmapUiCalcMarkingConfig;

protected:
    virtual ref<UiContainer> OnNewUiCalcMarkingConfigDialogEmbedded()
            ;
private:

    ref<CCalcMachineIfaceGp> _m_rCalcMachine;
    unidef m_unidefCalcMachine;
    bool _m_bCalcMachineLoaded = false;

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiCalcMarkingConfig.cpp)
class UiCalcMarkingConfig : public UiContainer
{
public:
    virtual void Ui();

    ref<CCalcMarkingConfigImpl> m_rCalcMarkingConfig;
};
