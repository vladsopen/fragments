// CFeetStandardIface.h
#pragma once

class CRangesIface;
class CPatientLinkIface;

//
// CFeetStandardIface -
//
//   Euro or US positioning.
//
ASSUME_IMPL_FOR_IFACE(CFeetStandardImpl, CFeetStandardIface)

class CFeetStandardIface : public typeobject
{
public:
    CFeetStandardIface();
    NEW_LOCAL(CFeetStandardImpl, CFeetStandardIface)
    //NEW_GP(Balance, CFeetStandardImpl, CFeetStandardIface)

// Constants

// Attributes

    // Name
    sloc GetFeetStandardNameForUi()
            vhook;

// Operations

    // Axis init
    num CalculateAddToY(
            num numFootLengthMm,
            num numAnkleJointMm)
            vhook;

    // Feet image scaled to patient biometrics
    void DrawFeet(
            draw dc,
            point pointoffset,
            size sizeUi,
            ref<CPatientLinkIface> rPatientLink,
            ref<CRangesIface> rRanges,
            bool bSessionForPatientPicture)
            vhook;

// Derived Item Enumeration

    const typelist<CFeetStandardIface> GetDerivedList()
            vhook;

    type<CFeetStandardIface> AsFeetStandardForEurope()
            vhook;
    type<CFeetStandardIface> AsFeetStandardForAmerica()
            vhook;

// UI

protected:
    virtual sloc OnGetFeetStandardNameForUi()
            v1pure;
    virtual num OnCalculateAddToY(
            num numFootLengthMm,
            num numAnkleJointMm)
            v1pure;
    virtual void OnDrawFeet(
            draw dc,
            point pointoffset,
            size sizeUi,
            ref<CPatientLinkIface> rPatientLink,
            ref<CRangesIface> rRanges,
            bool bSessionForPatientPicture)
            v1pure;
    virtual const typelist<CFeetStandardIface> OnGetDerivedList()
            v1pure;
    virtual type<CFeetStandardIface> OnAsFeetStandardForEurope()
            v1pure;
    virtual type<CFeetStandardIface> OnAsFeetStandardForAmerica()
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
