// UiCalcMarkingConfig.cpp

#include "CUsmainProject.h"
#include "CCalcMarkingConfigImpl.h"

void UiCalcMarkingConfig::Ui()
{
    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rCalcMarkingConfig->_m_ptrmapUiCalcMarkingConfig += rUi;
    }

    //
    // Calculator Config
    //
    {
        ref<CUiButtonGp> rUi;

        rUi->SetText(
            TRANSLATE(
                "Calculator[... UI-string-removed]");

        rUi->SetIcon(RES__FORMULA_EDITOR_S());

        rUi->SetFontBold(true);
        
        rUi->SetMaxX(oo);
        rUi->SetAlignContentToCenterX();

        rUi->OnClick = [=]
        {
            m_rCalcMarkingConfig->
                OpenFormulaEditorConfig();
        }
    }
}
