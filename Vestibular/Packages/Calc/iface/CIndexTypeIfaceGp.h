// CIndexTypeIfaceGp.h
#pragma once

//
// CIndexTypeIfaceGp -
//
//   Balance corrdinate and calculated indexes.
//
ASSUME_IMPL_FOR_IFACE(CIndexTypeIface, CIndexTypeIfaceGp)
ASSUME_IMPL_FOR_IFACE(CIndexTypeImpl, CIndexTypeIface)

class CIndexTypeIfaceGp : public typeobject
{
public:
    CIndexTypeIfaceGp();
    //NEW_LOCAL(CIndexTypeImpl, CIndexTypeIfaceGp)
    NEW_GP(Balance, CIndexTypeImpl, CIndexTypeIfaceGp)

// Constants

// Attributes

    // Main key part as sent to Patients
    str GetInternalIndexKeyBase()
            vhook;

    // Shortest possible for Patients table
    sloc GetShortIndexName()
            vhook;

    // Patients tooltip
    sloc GetLongIndexName()
            vhook;

    // X/Y?
    bool IsAxisDependentIndex()
            vhook;

    // Eyes O/C?
    bool IsRecordTypeDependentIndex()
            vhook;

    // Send with games as weill?
    bool IsGameIndexSendToPatient()
            vhook;

    // Hide in Patients factory install?
    bool IsPatientIndexHiddenByFactoryDefault()
            vhook;

// Operations

// Derived Item Enumeration

    const typelist<CIndexTypeIfaceGp> GetDerivedList()
            vhook;

    type<CIndexTypeIfaceGp> AsIndexInstantPos()
            vhook;

    type<CIndexTypeIfaceGp> AsIndexInstantKilo()
            vhook;

    type<CIndexTypeIfaceGp> AsIndexRecordTypeCaption()
            vhook;

    type<CIndexTypeIfaceGp> AsIndexTypeAverageCenter()
            vhook;

    type<CIndexTypeIfaceGp> AsIndexTypeMax()
            vhook;

    type<CIndexTypeIfaceGp> AsIndexTypeDispersion()
            vhook;

    type<CIndexTypeIfaceGp> AsIndexTypeDeviation()
            vhook;

    type<CIndexTypeIfaceGp> AsIndexTypeCovariation()
            vhook;

    type<CIndexTypeIfaceGp> AsIndexTypeFreq()
            vhook;

    type<CIndexTypeIfaceGp> AsIndexTypeFreq60()
            vhook;

    type<CIndexTypeIfaceGp> AsIndexTypeLength()
            vhook;

    type<CIndexTypeIfaceGp> AsIndexTypeVelocity()
            vhook;

    type<CIndexTypeIfaceGp> AsIndexTypeAverageAngle()
            vhook;

    type<CIndexTypeIfaceGp> AsIndexTypeArea()
            vhook;

    type<CIndexTypeIfaceGp> AsIndexTypeMass()
            vhook;

    type<CIndexTypeIfaceGp> AsIndexTypeLengthForSquareArea95()
            vhook;

    type<CIndexTypeIfaceGp> AsIndexTypeEllipseLengthByWidth95()
            vhook;

    type<CIndexTypeIfaceGp> AsIndexTypeClassic()
            vhook;

    type<CIndexTypeIfaceGp> AsIndexTypeStabilityEstimation()
            vhook;

    type<CIndexTypeIfaceGp> AsIndexTypeStabilityIndex()
            vhook;

    type<CIndexTypeIfaceGp> AsIndexTypeWorkEnergyIndex()
            vhook;

    // to be sent to HomeMon
    type<CIndexTypeIfaceGp> AsIndexTypeGameScore()
            vhook;

// UI

protected:
    virtual str OnGetInternalIndexKeyBase()
            v1pure;
    virtual sloc OnGetShortIndexName()
            v1pure;
    virtual sloc OnGetLongIndexName()
            v1pure;
    virtual bool OnIsAxisDependentIndex()
            v1pure;
    virtual bool OnIsRecordTypeDependentIndex()
            v1pure;
    virtual bool OnIsGameIndexSendToPatient()
            v1st
            return
                false;
    virtual bool OnIsPatientIndexHiddenByFactoryDefault()
            v1pure;

    virtual const typelist<CIndexTypeIfaceGp> OnGetDerivedList()
            v1pure;

    virtual type<CIndexTypeIfaceGp> OnAsIndexInstantPos()
            v1pure;
    virtual type<CIndexTypeIfaceGp> OnAsIndexInstantKilo()
            v1pure;
    virtual type<CIndexTypeIfaceGp> OnAsIndexRecordTypeCaption()
            v1pure;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeAverageCenter()
            v1pure;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeMax()
            v1pure;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeDispersion()
            v1pure;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeDeviation()
            v1pure;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeCovariation()
            v1pure;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeFreq()
            v1pure;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeFreq60()
            v1pure;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeLength()
            v1pure;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeVelocity()
            v1pure;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeAverageAngle()
            v1pure;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeArea()
            v1pure;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeMass()
            v1pure;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeLengthForSquareArea95()
            v1pure;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeEllipseLengthByWidth95()
            v1pure;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeClassic()
            v1pure;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeStabilityEstimation()
            v1pure;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeStabilityIndex()
            v1pure;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeWorkEnergyIndex()
            v1pure;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeGameScore()
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
