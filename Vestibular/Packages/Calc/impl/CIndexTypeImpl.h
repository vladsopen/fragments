// CIndexTypeImpl.h
#pragma once

#include "CIndexTypeIface.h"

//
// CIndexTypeImpl -
//
//   Balance indexes sent to Patients.
//
//ASSUME_IMPL_FOR_IFACE(CIndexTypeImpl, CIndexTypeImpl)

class CIndexTypeImpl : public CIndexTypeIface
{
public:
    CIndexTypeImpl();
    //NEW_LOCAL(CIndexTypeImpl, CIndexTypeImpl)
    //NEW_GP(MyProject, CIndexTypeImpl, CIndexTypeImpl)

// Constants

// Attributes

// Operations

    // For PatientEmbed
    //type<CIndexTypeImpl> TryConvertFromPatientIndicatorKey(
            //str sKey);

// Derived Item Enumeration

    const typelist<CIndexTypeImpl> GetDerivedListImpl();

// UI

protected:

    virtual const typelist<CIndexTypeIface> OnGetDerivedListIface()
            ;
    virtual const typelist<CIndexTypeIfaceGp> OnGetDerivedList()
            ;

    virtual type<CIndexTypeIfaceGp> OnAsIndexInstantPos()
            vnull;
    virtual type<CIndexTypeIfaceGp> OnAsIndexInstantKilo()
            vnull;
    virtual type<CIndexTypeIfaceGp> OnAsIndexRecordTypeCaption()
            vnull;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeAverageCenter()
            vnull;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeMax()
            vnull;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeDispersion()
            vnull;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeDeviation()
            vnull;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeCovariation()
            vnull;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeFreq()
            vnull;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeFreq60()
            vnull;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeLength()
            vnull;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeVelocity()
            vnull;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeAverageAngle()
            vnull;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeArea()
            vnull;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeMass()
            vnull;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeLengthForSquareArea95()
            vnull;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeEllipseLengthByWidth95()
            vnull;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeClassic()
            vnull;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeStabilityEstimation()
            vnull;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeStabilityIndex()
            vnull;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeWorkEnergyIndex()
            vnull;
    virtual type<CIndexTypeIfaceGp> OnAsIndexTypeGameScore()
            vnull;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

    // Instant X/Y
    class CIndexInstantPos : public CIndexTypeImpl
    {
    public:
        void flag_List_as_IndexType();

    protected:

        virtual str OnGetInternalIndexKeyBase()
                return
                    "InstantPos";

        virtual sloc OnGetShortIndexName()
                return
                    "";
                    //TRANSLATE(
                    //   [... UI-string-removed]");

        virtual sloc OnGetLongIndexName()
                return
                    TRANSLATE(
                        "Current co[... UI-string-removed]");

        virtual bool OnIsAxisDependentIndex()
                return
                    true;

        virtual bool OnIsRecordTypeDependentIndex()
                return
                    true;

        virtual bool OnIsPatientIndexHiddenByFactoryDefault()
                return
                    false;
    };

    // Instant Weight
    class CIndexInstantKilo : public CIndexTypeImpl
    {
    public:
        void flag_List_as_IndexType();

    protected:

        virtual str OnGetInternalIndexKeyBase()
                return
                    "InstantKilo";

        virtual sloc OnGetShortIndexName()
                return
                    "";
                    //TRANSLATE(
                    //   [... UI-string-removed]");

        virtual sloc OnGetLongIndexName()
                return
                    TRANSLATE(
                        "Current we[... UI-string-removed]");

        virtual bool OnIsAxisDependentIndex()
                return
                    false;

        virtual bool OnIsRecordTypeDependentIndex()
                return
                    true;

        virtual bool OnIsPatientIndexHiddenByFactoryDefault()
                return
                    false;
    };

    // The Caption
    class CIndexRecordTypeCaption : public CIndexTypeImpl
    {
    public:
        void flag_List_as_IndexType();

    protected:

        virtual str OnGetInternalIndexKeyBase()
                return
                    "RecordCaption";

        virtual sloc OnGetShortIndexName()
                return
                    TRANSLATE(
                        "Mode[... UI-string-removed]");

        virtual sloc OnGetLongIndexName()
                return
                    TRANSLATE(
                        "Recording [... UI-string-removed]");

        virtual bool OnIsAxisDependentIndex()
                return
                    false;

        virtual bool OnIsRecordTypeDependentIndex()
                return
                    true;

        virtual bool OnIsPatientIndexHiddenByFactoryDefault()
                return
                    false;
    };

    // Average X/Y
    class CIndexTypeAverageCenter : public CIndexTypeImpl
    {
    public:
        void flag_List_as_IndexType();

    protected:

        virtual str OnGetInternalIndexKeyBase()
                return
                    "AverageCenter";

        virtual sloc OnGetShortIndexName()
                return
                    "~";
                    //TRANSLATE(
                    //   [... UI-string-removed]");

        virtual sloc OnGetLongIndexName()
                return
                    TRANSLATE(
                        "Pressure c[... UI-string-removed]");

        virtual bool OnIsAxisDependentIndex()
                return
                    true;

        virtual bool OnIsRecordTypeDependentIndex()
                return
                    true;

        virtual bool OnIsPatientIndexHiddenByFactoryDefault()
                return
                    false;
    };

    // Max X/Y
    class CIndexTypeMax : public CIndexTypeImpl
    {
    public:
        void flag_List_as_IndexType();

    protected:

        virtual str OnGetInternalIndexKeyBase()
                return
                    "Max";

        virtual sloc OnGetShortIndexName()
                return
                    "Max";
                    //TRANSLATE(
                    //   [... UI-string-removed]");

        virtual sloc OnGetLongIndexName()
                return
                    TRANSLATE(
                        "Pressure c[... UI-string-removed]");

        virtual bool OnIsAxisDependentIndex()
                return
                    true;

        virtual bool OnIsRecordTypeDependentIndex()
                return
                    true;

        virtual bool OnIsPatientIndexHiddenByFactoryDefault()
                return
                    false;
    };

    // Dispersion X/Y
    class CIndexTypeDispersion : public CIndexTypeImpl
    {
    public:
        void flag_List_as_IndexType();

    protected:

        virtual str OnGetInternalIndexKeyBase()
                return
                    "Dispersion";

        virtual sloc OnGetShortIndexName()
                return
                    "D";
                    //TRANSLATE(
                    //   [... UI-string-removed]");

        virtual sloc OnGetLongIndexName()
                return
                    TRANSLATE(
                        "Pressure c[... UI-string-removed]");

        virtual bool OnIsAxisDependentIndex()
                return
                    true;

        virtual bool OnIsRecordTypeDependentIndex()
                return
                    true;

        virtual bool OnIsPatientIndexHiddenByFactoryDefault()
                return
                    true;
    };

    // Deviation X/Y
    class CIndexTypeDeviation : public CIndexTypeImpl
    {
    public:
        void flag_List_as_IndexType();

    protected:

        virtual str OnGetInternalIndexKeyBase()
                return
                    "Deviation";

        virtual sloc OnGetShortIndexName()
                return
                    "SqD";
                    //TRANSLATE(
                    //   [... UI-string-removed]");

        virtual sloc OnGetLongIndexName()
                return
                    TRANSLATE(
                        "Pressure c[... UI-string-removed]");

        virtual bool OnIsAxisDependentIndex()
                return
                    true;

        virtual bool OnIsRecordTypeDependentIndex()
                return
                    true;

        virtual bool OnIsPatientIndexHiddenByFactoryDefault()
                return
                    true;
    };

    // Covariation
    class CIndexTypeCovariation : public CIndexTypeImpl
    {
    public:
        void flag_List_as_IndexType();

    protected:

        virtual str OnGetInternalIndexKeyBase()
                return
                    "Covariation";

        virtual sloc OnGetShortIndexName()
                return
                    "CovXY";
                    //TRANSLATE(
                    //   [... UI-string-removed]");

        virtual sloc OnGetLongIndexName()
                return
                    TRANSLATE(
                        "Covariatio[... UI-string-removed]");

        virtual bool OnIsAxisDependentIndex()
                return
                    false;

        virtual bool OnIsRecordTypeDependentIndex()
                return
                    true;

        virtual bool OnIsPatientIndexHiddenByFactoryDefault()
                return
                    true;
    };

    // Freq X/Y
    class CIndexTypeFreq : public CIndexTypeImpl
    {
    public:
        void flag_List_as_IndexType();

    protected:

        virtual str OnGetInternalIndexKeyBase()
                return
                    "Freq";

        virtual sloc OnGetShortIndexName()
                return
                    "F";
                    //TRANSLATE(
                    //   [... UI-string-removed]");

        virtual sloc OnGetLongIndexName()
                return
                    TRANSLATE(
                        "Pressure c[... UI-string-removed]");

        virtual bool OnIsAxisDependentIndex()
                return
                    true;

        virtual bool OnIsRecordTypeDependentIndex()
                return
                    true;

        virtual bool OnIsPatientIndexHiddenByFactoryDefault()
                return
                    false;
    };

    // Freq X/Y 60% power
    class CIndexTypeFreq60 : public CIndexTypeImpl
    {
    public:
        void flag_List_as_IndexType();

    protected:

        virtual str OnGetInternalIndexKeyBase()
                return
                    "Freq60";

        virtual sloc OnGetShortIndexName()
                return
                    "F60";
                    //TRANSLATE(
                    //   [... UI-string-removed]");

        virtual sloc OnGetLongIndexName()
                return
                    TRANSLATE(
                        "60%[... UI-string-removed]");

        virtual bool OnIsAxisDependentIndex()
                return
                    true;

        virtual bool OnIsRecordTypeDependentIndex()
                return
                    true;

        virtual bool OnIsPatientIndexHiddenByFactoryDefault()
                return
                    false;
    };

    // Length
    class CIndexTypeLength : public CIndexTypeImpl
    {
    public:
        void flag_List_as_IndexType();

    protected:

        virtual str OnGetInternalIndexKeyBase()
                return
                    "RouteLength";

        virtual sloc OnGetShortIndexName()
                return
                    "L";
                    //TRANSLATE(
                    //   [... UI-string-removed]");

        virtual sloc OnGetLongIndexName()
                return
                    TRANSLATE(
                        "Stabilogra[... UI-string-removed]");

        virtual bool OnIsAxisDependentIndex()
                return
                    false;

        virtual bool OnIsRecordTypeDependentIndex()
                return
                    true;

        virtual bool OnIsPatientIndexHiddenByFactoryDefault()
                return
                    false;
    };

    // Velocity
    class CIndexTypeVelocity : public CIndexTypeImpl
    {
    public:
        void flag_List_as_IndexType();

    protected:

        virtual str OnGetInternalIndexKeyBase()
                return
                    "Velocity";

        virtual sloc OnGetShortIndexName()
                return
                    "V";
                    //TRANSLATE(
                    //   [... UI-string-removed]");

        virtual sloc OnGetLongIndexName()
                return
                    TRANSLATE(
                        "Pressure c[... UI-string-removed]");

        virtual bool OnIsAxisDependentIndex()
                return
                    false;

        virtual bool OnIsRecordTypeDependentIndex()
                return
                    true;

        virtual bool OnIsPatientIndexHiddenByFactoryDefault()
                return
                    false;
    };

    // Angle
    class CIndexTypeAverageAngle : public CIndexTypeImpl
    {
    public:
        void flag_List_as_IndexType();

    protected:

        virtual str OnGetInternalIndexKeyBase()
                return
                    "AverageAngle";

        virtual sloc OnGetShortIndexName()
                return
                    //RichFontSymbol(C_RichSymbol_Angle);
                    "A";
                    //TRANSLATE(
                    //   [... UI-string-removed]");

        virtual sloc OnGetLongIndexName()
                return
                    TRANSLATE(
                        "Average fl[... UI-string-removed]");

        virtual bool OnIsAxisDependentIndex()
                return
                    false;

        virtual bool OnIsRecordTypeDependentIndex()
                return
                    true;

        virtual bool OnIsPatientIndexHiddenByFactoryDefault()
                return
                    false;
    };

    // Area
    class CIndexTypeArea : public CIndexTypeImpl
    {
    public:
        void flag_List_as_IndexType();

    protected:

        virtual str OnGetInternalIndexKeyBase()
                return
                    "Area";

        virtual sloc OnGetShortIndexName()
                return
                    "S";
                    //TRANSLATE(
                    //   [... UI-string-removed]");

        virtual sloc OnGetLongIndexName()
                return
                    TRANSLATE(
                        "Stabilogra[... UI-string-removed]");

        virtual bool OnIsAxisDependentIndex()
                return
                    false;

        virtual bool OnIsRecordTypeDependentIndex()
                return
                    true;

        virtual bool OnIsPatientIndexHiddenByFactoryDefault()
                return
                    false;
    };

    // Mass
    class CIndexTypeMass : public CIndexTypeImpl
    {
    public:
        void flag_List_as_IndexType();

    protected:

        virtual str OnGetInternalIndexKeyBase()
                return
                    "Mass";

        virtual sloc OnGetShortIndexName()
                return
                    "W";
                    //TRANSLATE(
                    //   [... UI-string-removed]");

        virtual sloc OnGetLongIndexName()
                return
                    TRANSLATE(
                        "Average ma[... UI-string-removed]");

        virtual bool OnIsAxisDependentIndex()
                return
                    false;

        virtual bool OnIsRecordTypeDependentIndex()
                return
                    true;

        virtual bool OnIsPatientIndexHiddenByFactoryDefault()
                return
                    false;
    };

    // Ellipse Length/Width 95
    class CIndexTypeEllipseLengthByWidth95 : public CIndexTypeImpl
    {
    public:
        void flag_List_as_IndexType();

    protected:

        virtual str OnGetInternalIndexKeyBase()
                return
                    "EllipseLengthByWidth95";

        virtual sloc OnGetShortIndexName()
                return
                    "Le/We";
                    //TRANSLATE(
                    //   [... UI-string-removed]");

        virtual sloc OnGetLongIndexName()
                return
                    TRANSLATE(
                        "Ellipse le[... UI-string-removed]");

        virtual bool OnIsAxisDependentIndex()
                return
                    false;

        virtual bool OnIsRecordTypeDependentIndex()
                return
                    true;

        virtual bool OnIsPatientIndexHiddenByFactoryDefault()
                return
                    true;
    };

    // LFS
    class CIndexTypeLengthForSquareArea95 : public CIndexTypeImpl
    {
    public:
        void flag_List_as_IndexType();

    protected:

        virtual str OnGetInternalIndexKeyBase()
                return
                    "LFS95";

        virtual sloc OnGetShortIndexName()
                return
                    "LFS";
                    //TRANSLATE(
                    //   [... UI-string-removed]");

        virtual sloc OnGetLongIndexName()
                return
                    TRANSLATE(
                        "Length to [... UI-string-removed]");

        virtual bool OnIsAxisDependentIndex()
                return
                    false;

        virtual bool OnIsRecordTypeDependentIndex()
                return
                    true;

        virtual bool OnIsPatientIndexHiddenByFactoryDefault()
                return
                    true;
    };

    // Classic
    class CIndexTypeClassic : public CIndexTypeImpl
    {
    public:
        void flag_List_as_IndexType();

    protected:

        virtual str OnGetInternalIndexKeyBase()
                return
                    "Classic";

        virtual sloc OnGetShortIndexName()
                return
                    "QR";
                    //"R";
                    //TRANSLATE(
                    //   [... UI-string-removed]");

        virtual sloc OnGetLongIndexName()
                return
                    TRANSLATE(
                        "Classic co[... UI-string-removed]");

        virtual bool OnIsAxisDependentIndex()
                return
                    false;

        virtual bool OnIsRecordTypeDependentIndex()
                return
                    false;

        virtual bool OnIsPatientIndexHiddenByFactoryDefault()
                return
                    false;
    };

    // "Stab" 
    class CIndexTypeStabilityEstimation : public CIndexTypeImpl
    {
    public:
        void flag_List_as_IndexType();

    protected:

        virtual str OnGetInternalIndexKeyBase()
                return
                    "Stab";

        virtual sloc OnGetShortIndexName()
                return
                    "Stab";
                    //TRANSLATE(
                    //   [... UI-string-removed]");

        virtual sloc OnGetLongIndexName()
                return
                    TRANSLATE(
                        "Stability [... UI-string-removed]");

        virtual bool OnIsAxisDependentIndex()
                return
                    false;

        virtual bool OnIsRecordTypeDependentIndex()
                return
                    true;
                    //false;

        virtual bool OnIsPatientIndexHiddenByFactoryDefault()
                return
                    true;
    };

    // "Si" 
    class CIndexTypeStabilityIndex : public CIndexTypeImpl
    {
    public:
        void flag_List_as_IndexType();

    protected:

        virtual str OnGetInternalIndexKeyBase()
                return
                    "Si";

        virtual sloc OnGetShortIndexName()
                return
                    "Si";
                    //TRANSLATE(
                    //   [... UI-string-removed]");

        virtual sloc OnGetLongIndexName()
                return
                    TRANSLATE(
                        "Stability [... UI-string-removed]");

        virtual bool OnIsAxisDependentIndex()
                return
                    false;

        virtual bool OnIsRecordTypeDependentIndex()
                return
                    true;

        virtual bool OnIsPatientIndexHiddenByFactoryDefault()
                return
                    true;
    };

    // "A (E/Work)"
    class CIndexTypeWorkEnergyIndex : public CIndexTypeImpl
    {
    public:
        void flag_List_as_IndexType();

    protected:

        virtual str OnGetInternalIndexKeyBase()
                return
                    "Ei";

        virtual sloc OnGetShortIndexName()
                return
                    //"EiK";
                    "Ei";
                    //TRANSLATE(
                    //   [... UI-string-removed]");

        virtual sloc OnGetLongIndexName()
                return
                    TRANSLATE(
                        "Energoinde[... UI-string-removed]");

        virtual bool OnIsAxisDependentIndex()
                return
                    false;

        virtual bool OnIsRecordTypeDependentIndex()
                return
                    true;

        // Game compatible!
        virtual bool OnIsGameIndexSendToPatient()
                return
                    true;

        virtual bool OnIsPatientIndexHiddenByFactoryDefault()
                return
                    false;
    };

    // to be sent to HomeMon
    class CIndexTypeGameScore : public CIndexTypeImpl
    {
    public:
        void flag_List_as_IndexType();

    protected:

        virtual str OnGetInternalIndexKeyBase()
                return
                    "Score";

        virtual sloc OnGetShortIndexName()
                return
                    "Score";

        virtual sloc OnGetLongIndexName()
                return
                    TRANSLATE(
                        "Scor[... UI-string-removed]");

        virtual bool OnIsAxisDependentIndex()
                return
                    false;

        virtual bool OnIsRecordTypeDependentIndex()
                return
                    false;

        // Game compatible!
        virtual bool OnIsGameIndexSendToPatient()
                return
                    true;

        virtual bool OnIsPatientIndexHiddenByFactoryDefault()
                return
                    false;
    };

