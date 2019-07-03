// CCalcMarkingConfigImpl.cpp

#include "CUsmainProject.h"
#include "CCalcMarkingConfigImpl.h"
#include "CCalcMachineIfaceGp.h"
#include "CCalcVariableIfaceGp.h"
#include "CCalcVariableListIfaceGp.h"
#include "CGlobalConfigIface.h"
#include "CGlobalPaletteConfigIface.h"
#include "CLocalPaletteConfigIface.h"

CCalcMarkingConfigImpl::CCalcMarkingConfigImpl()
{
}

void CCalcMarkingConfigImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    if (_m_bCalcMachineLoaded)
    {
        // Import the exposed unidef
        m_unidefCalcMachine = _m_rCalcMachine->ExposeOut();
        _m_bCalcMachineLoaded = false;
    }

    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

}

ref<CCalcMachineIfaceGp> CCalcMarkingConfigImpl::GetLoadedCalcMachine()
{
    if (!_m_bCalcMachineLoaded)
    {
        // Delayed load
        PRELOAD_PROJECT(Calc);

        // Import the exposed unidef
        _m_rCalcMachine.ExposeIn(m_unidefCalcMachine);
        _m_bCalcMachineLoaded = true;

        // use config and Calc state
        ref<CCalcMachineIfaceGp> rCalcMachine = _m_rCalcMachine;
        ref<CCalcVariableIfaceGp> rTempCalcVariable = 
            rCalcMachine->GetCalcVariableList()->GetCalculatorCalcVariable();
    }

    return _m_rCalcMachine;
}

ref<UiContainer> CCalcMarkingConfigImpl::NewUiCalcMarkingConfig()
{
    ref<UiCalcMarkingConfig> rUi;

    rUi->m_rCalcMarkingConfig = this;

    return rUi;
}

ref<UiContainer> CCalcMarkingConfigImpl::OnNewUiCalcMarkingConfigDialogEmbedded()
{
    return NewUiCalcMarkingConfig();
}

void CCalcMarkingConfigImpl::OpenFormulaEditorConfig()
{
    // Delayed load
    PRELOAD_PROJECT(Calc);

    ref<CCalcMachineIfaceGp> rCalcMachine = 
        GetLoadedCalcMachine();

    // Open UI
    rCalcMachine->OpenCalcDialog();
}

