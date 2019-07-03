// CTunerValue.h
#pragma once // base class

// ***** DEPRECATED - TEMPORARY - DO NOT COPY ***************
#include "lib__memberid.h"
#include "lib__idobject.h"
// **********************************************************

class CTunerProfile;
class CTunerProfileList;

//
// CTunerValue - a single adaptive calculation parameter value
//

class CTunerValue : public object
{
public:
    CTunerValue();

// Constants

    static const num C_numUndefinedTune = OO;
    static const sloc C_slocNoSourceProfile = "<from this profile>";
    static const rgb C_rgbDifferenceHighlightBack = rgb(rgb(255, 240, 150));

// Attributes

    // Init is determined by x_slocDescription length
    bool IsValueMetaDataDefined();

    // Description
    sloc x_slocDescription = ""
            xauto(Get, Set)
            xassert(value.Len() > 0);

    // Initial value to reset current value to
    //num x_numDefaultTune = C_numUndefinedTune, -- may be numStartupReset, but 
            //xauto(Get, Set),                    --  the rare cases can be hardcoded
            //xassert(true);

    // Value Compare Index (see CTunerProfile::GetValueByOrderedCompareIndex())
    int x_iOrderedCompareValue = -1
            xauto(Get, Set)
            xassert(value >= 0);

    // Current value
    num x_numUntypedTuneStorage = C_numUndefinedTune;
    void DefineDefaultTuneValue(
            num numValue);
    void ChangeInternalTuneValueDirectly(
            num numValue);
    void ChangeInternalTuneValueFromUi(
            num numValue);
    num ReadCurrentInternalTuneValueAsNum(); // may reset into default
    int ReadCurrentInternalTuneValueAsInt(); // safely converts num
    bool ReadCurrentInternalTuneValueAsBool(); // safely converts num

    // Min value
    num x_numMinTune = -OO
            xauto(Get, Set)
            xassert(true);

    // Max value
    num x_numMaxTune = OO
            xauto(Get, Set)
            xassert(true);

    // A value can be 'inherited' from another profile
    link<CTunerProfile, memberid> x_linkSourceTunerProfile
            xauto(Get);
    void x_linkSourceTunerProfile_Set(link<CTunerProfile, memberid> value);

    // Usage tracking
    bool x_bUsedRecently = false
            xauto(Get, Set);

    // Debug check
    bool _x_bAddedToProfile = false
            xauto(Get, Set);

// Operations

    // Optimized copy for Tuner log
    void FastCopyTunerValuesFrom(
            ref<CTunerValue> rTunerValueSource);

// UI

    // Hide rare values
    bool x_bHideUi = false
            xauto(Get);
    void x_bHideUi_Set(bool value);

    // Can be shown according to own+profile 'Hide' settings
    bool IsHiddenInProfile(
            ref<CTunerProfile> rTunerProfile);

    // Value type visibility in different profile list roles (Visitor pattern)
    bool IsVisibleInProfileRole(
            ref<CTunerProfile> rTunerProfile,
            ref<CTunerProfileList> rTunerProfileList);
    bool IsVisibleInProfileRoleForSetup()
            vhook;
    bool IsVisibleInProfileRoleForDump()
            vhook;

    // Pretty input formatting
    str FormatValueForEditBox(
            num numUntypedValue)
            vhook;

    // Icon by input/output role
    res GetTunerIcon()
            vhook;

    // Profile Comparison support
    bool IsProfileDifferenceToHighlight(
            ref<CTunerProfile> rTunerProfile,
            ref<CTunerProfileList> rTunerProfileList,
            out num& out_numCompareToValue);

    ref<UiContainer> NewTunerValueEditorUi(
            ref<CTunerProfile> rTunerProfile,
            ref<CTunerProfileList> rTunerProfileList);
    ptr<CUiPanelGp> _m_pUiTunerValueEditor;

    ref<UiContainer> NewTunerValueSourceProfileMenuUi(
            ref<CTunerProfile> rSelectedTunerProfile,
            ref<CTunerProfileList> rTunerProfileList);
    ptr<CUiPanelGp> _m_pUiTunerValueSourceProfileMenu;

    // Special CTunerValueSettingAsBool input
    bool IsUiAsCheckBox()
            vhook;

    ref<UiContainer> NewUiTunerValueNumInput(
            ref<CTunerProfile> rTunerProfile,
            ref<CTunerProfileList> rTunerProfileList);
    ptr<CUiPanelGp> _m_pUiTunerValueNumInput;

    ref<UiContainer> NewUiTunerValueBoolInput(
            ref<CTunerProfile> rTunerProfile,
            ref<CTunerProfileList> rTunerProfileList);
    ptr<CUiPanelGp> _m_pUiTunerValueBoolInput;

    void EqualizeControls(
            ref<CUiTextBasedGp> rUi);

protected:
    virtual bool OnIsVisibleInProfileRoleForSetup()
            v1pure;
    virtual bool OnIsVisibleInProfileRoleForDump()
            v1pure;
    virtual bool OnIsProfileForceViewBypassEnabled(
            ref<CTunerProfile> rTunerProfile)
            v1pure;
    virtual bool OnIsUiAsCheckBox()
            v1st;
    virtual str OnFormatValueForEditBox(
            num numUntypedValue)
            v1st;
    virtual res OnGetTunerIcon()
            v1pure;

private:
    bool IsProfileForceViewBypassEnabled(
            ref<CTunerProfile> rTunerProfile)
            vhook;
};

// (UiTunerValueEditor.cpp)
class UiTunerValueEditor : public UiContainer
{
public:
    virtual void Ui();

    ref<CTunerValue> m_rTunerValue;
    ref<CTunerProfile> m_rTunerProfile;
    ref<CTunerProfileList> m_rTunerProfileList;
};

// (UiTunerValueSourceProfileMenu.cpp)
class UiTunerValueSourceProfileMenu : public UiContainer
{
public:
    virtual void Ui();

    ref<CTunerValue> m_rTunerValue;
    ref<CTunerProfile> m_rSelectedTunerProfile;
    ref<CTunerProfileList> m_rTunerProfileList;
};

// (UiTunerValueNumInput.cpp)
class UiTunerValueNumInput : public UiContainer
{
public:
    virtual void Ui();

    ref<CTunerValue> m_rTunerValue;
    ref<CTunerProfile> m_rTunerProfile;
    ref<CTunerProfileList> m_rTunerProfileList;
};

// (UiTunerValueBoolInput.cpp)
class UiTunerValueBoolInput : public UiContainer
{
public:
    virtual void Ui();

    ref<CTunerValue> m_rTunerValue;
    ref<CTunerProfile> m_rTunerProfile;
    ref<CTunerProfileList> m_rTunerProfileList;
};
