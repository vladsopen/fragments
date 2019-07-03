// CCalcMarkingConfigIface.h
#pragma once

//
// CCalcMarkingConfigIface -
//
//   CalcMarking (and Formula Editor) configuration.
//
ASSUME_IMPL_FOR_IFACE(CCalcMarkingConfigImpl, CCalcMarkingConfigIface)

class CCalcMarkingConfigIface : public object
{
public:
    CCalcMarkingConfigIface();
    NEW_LOCAL(CCalcMarkingConfigImpl, CCalcMarkingConfigIface)
    //NEW_GP(MyProject, CCalcMarkingConfigImpl, CCalcMarkingConfigIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

// Operations

// UI

    // For CCES config compact
    ref<UiContainer> NewUiCalcMarkingConfigDialogEmbedded()
            vhook;

protected:
    virtual ref<UiContainer> OnNewUiCalcMarkingConfigDialogEmbedded()
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
