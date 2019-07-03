// CRecordTypeIface.h
#pragma once

class CEyeModeIface;
class CCalcRouteIndexesIface;
class CAppTypeIface;

//
// CRecordTypeIface -
//
//   Recording mode: eyes-opened/closed or neutral
//
ASSUME_IMPL_FOR_IFACE(CRecordTypeImpl, CRecordTypeIface)

class CRecordTypeIface : public typeobject
{
public:
    CRecordTypeIface();
    NEW_LOCAL(CRecordTypeImpl, CRecordTypeIface)
    //NEW_GP(MyProject, CRecordTypeImpl, CRecordTypeIface)

// Constants

// Attributes

    // UI text
    sloc GetShortRecordTypeNameForUi()
            vhook;
    sloc GetAbbreviatedRecordTypeNameForUi()
            vhook;

    // Icon
    res GetRecordTypeIcon()
            vhook;

    // Append to Patients index key (or empty)
    str GetPatientIndexKeyRecordType()
            vhook;

    // For Patients layered columns
    int GetPatientIndexLayerNumber()
            vhook;

    // Append to Patients index name (or empty)
    str GetPatientIndexShortNameRecordType()
            vhook;

    // Append to Patients index tooltip (or empty)
    str GetPatientIndexLongNameRecordType()
            vhook;

    // Route drawing
    rgb GetRecordGraphLineColor()
            vhook;

    //// Method attachment
    //type<CAppTypeIface> GetRelevantMethod()
    //        vhook;

    // Stage filter
    bool IsRecordRelevantToMethodAndEyeMode(
            type<CAppTypeIface> typeAppType,
            type<CEyeModeIface> typeEyeMode)
            vhook;

    // we need to separate results for eyes opened and closed
    ref<CCalcRouteIndexesIface> SelectIndexesForEyesOpenClosedOrNeutral(
            ref<CCalcRouteIndexesIface> rCalcRouteIndexesEyesOpened,
            ref<CCalcRouteIndexesIface> rCalcRouteIndexesEyesClosed,
            ref<CCalcRouteIndexesIface> rCalcRouteIndexesOther)
            vhook;

// Operations

// Derived Item Enumeration

    const typelist<CRecordTypeIface> GetDerivedList()
            vhook;

    type<CRecordTypeIface> AsRecordTypeEyesOpened()
            vhook;
    type<CRecordTypeIface> AsRecordTypeEyesClosed()
            vhook;
    type<CRecordTypeIface> AsRecordTypeNeutral()
            vhook;

// UI

protected:

    virtual sloc OnGetShortRecordTypeNameForUi()
            v1pure;

    virtual res OnGetRecordTypeIcon()
            v1pure;

    virtual sloc OnGetAbbreviatedRecordTypeNameForUi()
            v1pure;

    virtual str OnGetPatientIndexKeyRecordType()
            v1st
            return
                "";

    virtual int OnGetPatientIndexLayerNumber()
            v1st
            return
                0;

    virtual str OnGetPatientIndexShortNameRecordType()
            v1st
            return
                "";

    virtual str OnGetPatientIndexLongNameRecordType()
            v1st
            return
                "";

    virtual rgb OnGetRecordGraphLineColor()
            v1pure;

    //virtual type<CAppTypeIface> OnGetRelevantMethod()
    //        v1pure;

    virtual ref<CCalcRouteIndexesIface> OnSelectIndexesForEyesOpenClosedOrNeutral(
            ref<CCalcRouteIndexesIface> rCalcRouteIndexesEyesOpened,
            ref<CCalcRouteIndexesIface> rCalcRouteIndexesEyesClosed,
            ref<CCalcRouteIndexesIface> rCalcRouteIndexesOther)
            v1pure;

    virtual bool OnIsRecordRelevantToMethodAndEyeMode(
            type<CAppTypeIface> typeAppType,
            type<CEyeModeIface> typeEyeMode)
            v1pure;

    virtual const typelist<CRecordTypeIface> OnGetDerivedList()
            v1pure;
    virtual type<CRecordTypeIface> OnAsRecordTypeEyesOpened()
            v1pure;
    virtual type<CRecordTypeIface> OnAsRecordTypeEyesClosed()
            v1pure;
    virtual type<CRecordTypeIface> OnAsRecordTypeNeutral()
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

