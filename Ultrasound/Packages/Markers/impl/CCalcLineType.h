// CCalcLineType.h
#pragma once

class CUsoundAxisIfaceGp;
class CUsoundIndexTypeIfaceGp;

//
// CCalcLineType -
//
//   Defines all manual calc markers and their relation to
//     UsoundIndexes and other actions.
//
//ASSUME_IMPL_FOR_IFACE(CCalcLineTypeImpl, CCalcLineType)

class CCalcLineType : public typeobject
{
public:
    CCalcLineType();
    //NEW_LOCAL(CCalcLineTypeImpl, CCalcLineType)
    //NEW_GP(MyProject, CCalcLineTypeImpl, CCalcLineType)

// Constants

// Attributes

    // Ui ID
    str GetCalcLineName()
            vhooked;

    // Ui tip
    sloc GetCalcLineDescription()
            vhooked;

    // Different basic color for all indexes
    rgb GetCalcLineColor()
            vhook;

    // For IfaceGp
    ECalcMarkingLine GetCalcLineSharedEnum()
            vhook;

    // Related index if defined
    type<CUsoundIndexTypeIfaceGp> GetCalcLineUsoundIndex()
            vhook;

    // Axis (usually derived from the Index)
    type<CUsoundAxisIfaceGp> GetCalcLineUsoundAxis()
            vhooked;

    // Media histogram at T1 marker
    bool HasHistogramPainted()
            vhook;

// Operations

    // For IfaceGp
    type<CCalcLineType> FromCalcLineSharedEnum(
            ECalcMarkingLine eCalcMarkingLine);

// Derived Item Enumeration

    const typelist<CCalcLineType> GetDerivedList();

// UI

protected:

    virtual str OnGetCalcLineName()
            v1st
            return
                ""; // must get from Index

    virtual sloc OnGetCalcLineDescription()
            v1st
            return
                ""; // must get from Index

    virtual rgb OnGetCalcLineColor()
            v1pure;

    virtual ECalcMarkingLine OnGetCalcLineSharedEnum()
            v1st
            return
                ECalcMarkingLine(-1);

    virtual type<CUsoundIndexTypeIfaceGp> OnGetCalcLineUsoundIndex()
            v1st
            return
                null();

    virtual type<CUsoundAxisIfaceGp> OnGetCalcLineUsoundAxis()
            v1st
            return
                null();

    virtual bool OnHasHistogramPainted()
            v1st
            return
                false;

private:

    // Check all types for unique fields
    void _test_CheckCalcLineType();

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    virtual void OnTestClass();
};

    // S
    class CCalcLineTypeSystolicVelocity : public CCalcLineType
    {
    public:
        void flag_List_as_CalcLineType();

    protected:

        virtual rgb OnGetCalcLineColor()
                return
                    rgb(0, 255, 255);

        virtual ECalcMarkingLine OnGetCalcLineSharedEnum()
                return
                    E_CalcMarkingLine_SystoleVelocity;

        virtual type<CUsoundIndexTypeIfaceGp> OnGetCalcLineUsoundIndex()
                return
                    type<CUsoundIndexTypeIfaceGp>()->
                        AsScannerSystolicVelocity();
    };

    // D
    class CCalcLineTypeDiastolicVelocity : public CCalcLineType
    {
    public:
        void flag_List_as_CalcLineType();

    protected:

        virtual rgb OnGetCalcLineColor()
                return
                    rgb(255, 255, 0);

        virtual ECalcMarkingLine OnGetCalcLineSharedEnum()
                return
                    E_CalcMarkingLine_DiastoleVelocity;

        virtual type<CUsoundIndexTypeIfaceGp> OnGetCalcLineUsoundIndex()
                return
                    type<CUsoundIndexTypeIfaceGp>()->
                        AsScannerDiastolicVelocity();
    };

    // M
    class CCalcLineTypeFlowMeanVelocity : public CCalcLineType
    {
    public:
        void flag_List_as_CalcLineType();

    protected:

        virtual rgb OnGetCalcLineColor()
                return
                    rgb(200, 80, 80);
                    //rgb(0, 255, 0);

        virtual ECalcMarkingLine OnGetCalcLineSharedEnum()
                return
                    E_CalcMarkingLine_MeanVelocity;

        virtual type<CUsoundIndexTypeIfaceGp> OnGetCalcLineUsoundIndex()
                return
                    type<CUsoundIndexTypeIfaceGp>()->
                        AsScannerFlowMeanVelocity();
    };

    // AWV
    class CCalcLineTypeAverageWeightedVelocity : public CCalcLineType
    {
    public:
        void flag_List_as_CalcLineType();

    protected:

        virtual rgb OnGetCalcLineColor()
                return
                    rgb(230, 200, 255);

        virtual ECalcMarkingLine OnGetCalcLineSharedEnum()
                return
                    E_CalcMarkingLine_AverageWeightedVelocity;

        virtual type<CUsoundIndexTypeIfaceGp> OnGetCalcLineUsoundIndex()
                return
                    type<CUsoundIndexTypeIfaceGp>()->
                        AsScannerAverageWeightedVelocity();
    };

    // T1
    class CCalcLineTypeTimeAtSystole : public CCalcLineType
    {
    public:
        void flag_List_as_CalcLineType();

    protected:

        virtual rgb OnGetCalcLineColor()
                return
                    rgb(100, 255, 100);

        virtual ECalcMarkingLine OnGetCalcLineSharedEnum()
                return
                    E_CalcMarkingLine_LeftTime1;

        virtual type<CUsoundIndexTypeIfaceGp> OnGetCalcLineUsoundIndex()
                return
                    type<CUsoundIndexTypeIfaceGp>()->
                        AsScannerTimeAtSystole();

        virtual bool OnHasHistogramPainted()
                return
                    true;
    };

    // Td
    class CCalcLineTypeTimeAtDiastole : public CCalcLineType
    {
    public:
        void flag_List_as_CalcLineType();

    protected:

        virtual rgb OnGetCalcLineColor()
                return
                    rgb(255, 200, 0);

        virtual ECalcMarkingLine OnGetCalcLineSharedEnum()
                return
                    E_CalcMarkingLine_MiddleTimeDiastole;

        virtual type<CUsoundIndexTypeIfaceGp> OnGetCalcLineUsoundIndex()
                return
                    type<CUsoundIndexTypeIfaceGp>()->
                        AsScannerTimeAtDiastole();
    };

    // T2
    class CCalcLineTypeTimeAtNextSystole : public CCalcLineType
    {
    public:
        void flag_List_as_CalcLineType();

    protected:

        virtual rgb OnGetCalcLineColor()
                return
                    rgb(200, 200, 255);

        virtual ECalcMarkingLine OnGetCalcLineSharedEnum()
                return
                    E_CalcMarkingLine_RightTime2;

        virtual type<CUsoundIndexTypeIfaceGp> OnGetCalcLineUsoundIndex()
                return
                    type<CUsoundIndexTypeIfaceGp>()->
                        AsScannerTimeAtNextSystole();
    };

    /*
    // Zero Line Mover
    class CCalcLineTypeZero : public CCalcLineType
    {
    public:
        void flag_List_as_CalcLineType();

    protected:

        virtual str OnGetCalcLineName()
                return
                    "Zero";

        virtual sloc OnGetCalcLineDescription()
                return
                    TRANSLATE(
                        "Move the z[... UI-string-removed]");

        virtual rgb OnGetCalcLineColor()
                return
                    rgb(255, 0, 0);

        virtual type<CUsoundIndexTypeIfaceGp> OnGetCalcLineUsoundIndex()
                return
                    null();

        virtual type<CUsoundAxisIfaceGp> OnGetCalcLineUsoundAxis()
                return
                    type<CUsoundAxisIfaceGp>()->
                        AsUsoundAxisForVelocityScaleY();
    };
    */