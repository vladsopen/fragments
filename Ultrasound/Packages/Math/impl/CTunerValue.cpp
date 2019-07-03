// CTunerValue.cpp

#include "CDucalcProject.h"
#include "CTunerValue.h"
#include "CTunerProfile.h"
#include "CTunerProfileList.h"
#include "CRoleForTunerProfile.h"

CTunerValue::CTunerValue()
{
}

ref<UiContainer> CTunerValue::NewTunerValueEditorUi(
        ref<CTunerProfile> rTunerProfile,
        ref<CTunerProfileList> rTunerProfileList)
{
    ref<UiTunerValueEditor> rUi;

    rUi->m_rTunerValue = this;
    rUi->m_rTunerProfile = rTunerProfile;
    rUi->m_rTunerProfileList = rTunerProfileList;

    return rUi;
}

ref<UiContainer> CTunerValue::NewUiTunerValueNumInput(
        ref<CTunerProfile> rTunerProfile,
        ref<CTunerProfileList> rTunerProfileList)
{
    ref<UiTunerValueNumInput> rUi;

    rUi->m_rTunerValue = this;
    rUi->m_rTunerProfile = rTunerProfile;
    rUi->m_rTunerProfileList = rTunerProfileList;

    return rUi;
}

ref<UiContainer> CTunerValue::NewUiTunerValueBoolInput(
        ref<CTunerProfile> rTunerProfile,
        ref<CTunerProfileList> rTunerProfileList)
{
    ref<UiTunerValueBoolInput> rUi;

    rUi->m_rTunerValue = this;
    rUi->m_rTunerProfile = rTunerProfile;
    rUi->m_rTunerProfileList = rTunerProfileList;

    return rUi;
}

ref<UiContainer> CTunerValue::NewTunerValueSourceProfileMenuUi(
        ref<CTunerProfile> rSelectedTunerProfile,
        ref<CTunerProfileList> rTunerProfileList)
{
    ref<UiTunerValueSourceProfileMenu> rUi;

    rUi->m_rTunerValue = this;
    rUi->m_rSelectedTunerProfile = rSelectedTunerProfile;
    rUi->m_rTunerProfileList = rTunerProfileList;

    return rUi;
}

void CTunerValue::x_linkSourceTunerProfile_Set(link<CTunerProfile, memberid> value)
{
    this->x_linkSourceTunerProfile = value;

    _m_pUiTunerValueEditor.DefaultIfNull()->InvalidateContent();
}

bool CTunerValue::IsValueMetaDataDefined()
{
    if (this->x_slocDescription.Len() > 0)
        return true;
    else
        return false;
}

void CTunerValue::DefineDefaultTuneValue(
        num numValue)
{
    if (this->x_numUntypedTuneStorage == C_numUndefinedTune)
    {
        this->x_numUntypedTuneStorage = numValue;
    }
}

void CTunerValue::ChangeInternalTuneValueDirectly(
        num numValue)
{
    this->x_numUntypedTuneStorage = numValue;
    x_bUsedRecently = true;
}

void CTunerValue::ChangeInternalTuneValueFromUi(
        num numValue)
{
    this->x_numUntypedTuneStorage = numValue;

    _m_pUiTunerValueEditor.DefaultIfNull()->InvalidateTreeState();
}

num CTunerValue::ReadCurrentInternalTuneValueAsNum()
{
    ASSERT(_x_bAddedToProfile);

    // Reset undefined to default
    //if (this->x_numUntypedTuneStorage == C_numUndefinedTune)
        //this->x_numUntypedTuneStorage = x_numDefaultTune;

    // Ensure a valid value if the default is also undefined
    if (this->x_numUntypedTuneStorage == C_numUndefinedTune)
        this->x_numUntypedTuneStorage = 0;

    return this->x_numUntypedTuneStorage;
}

int CTunerValue::ReadCurrentInternalTuneValueAsInt()
{
    num numTune = ReadCurrentInternalTuneValueAsNum();

    // convert
    int nTune = (int) numTune;

    // check
    //todo: provide some diag for the user?
    ASSERT(num(nTune) == numTune);

    return nTune;
}

bool CTunerValue::ReadCurrentInternalTuneValueAsBool()
{
    num numTune = ReadCurrentInternalTuneValueAsNum();

    // convert
    bool bTune = !!numTune;

    // check
    //todo: provide some diag for the user?
    ASSERT(num(bTune) == numTune);

    return bTune;
}

bool CTunerValue::IsVisibleInProfileRole(
        ref<CTunerProfile> rTunerProfile,
        ref<CTunerProfileList> rTunerProfileList)
{
    // First check if profile own settings override default 
    //    'Show/Hide-by-type' behavior
    if (IsProfileForceViewBypassEnabled(rTunerProfile))
        return true;

    // Lame all-demo mode
    if (!debug("Flow.Ui.FilterTunersByProfileRole?").InputBool())
        return true;

    // Then 
    return rTunerProfileList->x_typeRoleForTunerProfile->IsTunerValueVisible(this);
}

void CTunerValue::FastCopyTunerValuesFrom(
        ref<CTunerValue> rTunerValueSource)
{
    ALL_MEMBERS;
    x_iOrderedCompareValue; //rFAIL("x_iOrderedCompareValue was not copied - a bug?");
    _x_bAddedToProfile;
    x_bHideUi;

    this->x_slocDescription = rTunerValueSource->x_slocDescription;
    //this->x_numDefaultTune = rTunerValueSource->x_numDefaultTune;
    this->x_numUntypedTuneStorage = 
        rTunerValueSource->ReadCurrentInternalTuneValueAsNum();
    this->x_numMinTune = rTunerValueSource->x_numMinTune;
    this->x_numMaxTune = rTunerValueSource->x_numMaxTune;
    this->x_linkSourceTunerProfile = rTunerValueSource->x_linkSourceTunerProfile;
    this->x_bUsedRecently = rTunerValueSource->x_bUsedRecently;
}

bool CTunerValue::OnIsUiAsCheckBox()
{
    return false; // default
}

void CTunerValue::EqualizeControls(
        ref<CUiTextBasedGp> rUi)
{
    rUi->SetFontSize(18);
    rUi->SetMarginAtTop(0);
    rUi->SetMarginAtBottom(0);
}

void CTunerValue::x_bHideUi_Set(bool value)
{
    if (this->x_bHideUi != value)
    {
        this->x_bHideUi = value;

        _m_pUiTunerValueEditor.DefaultIfNull()->InvalidateContent();
    }
}

bool CTunerValue::IsHiddenInProfile(
        ref<CTunerProfile> rTunerProfile)
{
    if (x_bHideUi)
    {
        if (rTunerProfile->x_bHideRareValuesUi)
        {
            return true;
        }
    }
    return false;
}

bool CTunerValue::IsProfileDifferenceToHighlight(
        ref<CTunerProfile> rTunerProfile,
        ref<CTunerProfileList> rTunerProfileList,
        out num& out_numCompareToValue)
{
    ptr<CTunerProfile> pHighlightDifferenceTunerProfile = 
        rTunerProfile->x_linkHighlightDifferenceTunerProfile.Get().
            FindPtr(rTunerProfileList);
    if not_null(
        ref<CTunerProfile> rHighlightDifferenceTunerProfile = 
            pHighlightDifferenceTunerProfile)
    {
        // Get the pair
        ref<CTunerValue> rCompareToTunerValue = 
            rHighlightDifferenceTunerProfile->
                GetValueByOrderedCompareIndex(
                    x_iOrderedCompareValue);

        // Load values
        num numOurValue = 
            this->ReadCurrentInternalTuneValueAsNum();
        num numCompareToValue = 
            rCompareToTunerValue->ReadCurrentInternalTuneValueAsNum();

        if (numOurValue != numCompareToValue)
        {
            // do the highlight
            out_numCompareToValue = numCompareToValue;
            return true;
        }
    }

    // no compare mode or values are equal
    return false;
}

str CTunerValue::OnFormatValueForEditBox(
        num numUntypedValue)
{
    // By default just return the number, 
    //  some value types may want to override this
    return Str(numUntypedValue);
}
