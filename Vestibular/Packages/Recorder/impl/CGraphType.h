// CGraphType.h
#pragma once

class CDrawAnythingRecorded;
class CRecorderImpl;

//
// CGraphType -
//
//   Info selection we indicate in the graph area.
//
//ASSUME_IMPL_FOR_IFACE(CGraphTypeImpl, CGraphType)

class CGraphType : public typeobject
{
public:
    CGraphType();
    //NEW_LOCAL(CGraphTypeImpl, CGraphType)
    //NEW_GP(MyProject, CGraphTypeImpl, CGraphType)

// Constants

// Attributes

    // Selection button text
    sloc GetGraphNameForButton()
            vhook;

    // Selection button icon
    res GetGraphIconForButton()
            vhook;

    // Select rendering object
    ref<CDrawAnythingRecorded> SelectDrawAnythingRecorded(
            ref<CRecorderImpl> rRecorder)
            vhook;

    // An option to hide in toolbar as a separate independent view
    bool IsPresentAsToolbarButton()
            vhook;

    // Experimental only
    bool IsPresentAsToolbarButtonOnlyInExperimentalMode()
            vhook;

    // New section
    bool HasPadBeforeToolbarButton()
            vhook;

// Operations

// Derived Item Enumeration

    type<CGraphType> GetDefaultGraphType();

    const typelist<CGraphType> GetDerivedList();

    type<CGraphType> AsGraphTypeRoute()
            vhook;
    type<CGraphType> AsGraphTypeVelocity()
            vhook;
    type<CGraphType> AsGraphTypeFreq()
            vhook;
    type<CGraphType> AsGraphTypeTable()
            vhook;
    type<CGraphType> AsGraphTypeAll()
            vhook;

// UI

protected:
    virtual sloc OnGetGraphNameForButton()
            v1pure;
    virtual res OnGetGraphIconForButton()
            v1pure;
    virtual ref<CDrawAnythingRecorded> OnSelectDrawAnythingRecorded(
            ref<CRecorderImpl> rRecorder)
            v1pure;
    virtual bool OnIsPresentAsToolbarButton()
            v1st
            return
                true;
    virtual bool OnIsPresentAsToolbarButtonOnlyInExperimentalMode()
            v1st
            return
                false;
    virtual bool OnHasPadBeforeToolbarButton()
            v1st
            return
                false;

    virtual type<CGraphType> OnAsGraphTypeRoute()
            v1null;
    virtual type<CGraphType> OnAsGraphTypeVelocity()
            v1null;
    virtual type<CGraphType> OnAsGraphTypeFreq()
            v1null;
    virtual type<CGraphType> OnAsGraphTypeTable()
            v1null;
    virtual type<CGraphType> OnAsGraphTypeAll()
            v1null;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

    // Disposition
    class CGraphTypeDisposition : public CGraphType
    {
    public:
    protected:

        void flag_List_as_GraphType();

        virtual sloc OnGetGraphNameForButton()
                return
                    TRANSLATE(
                        "Platform[... UI-string-removed]");

        virtual res OnGetGraphIconForButton()
                return
                    RES__MONITOR_SIGNAL_L();
                    //RES__AXIS_MARKER_DARK_L();

        virtual bool OnIsPresentAsToolbarButton()
                return
                    false;

        virtual ref<CDrawAnythingRecorded> OnSelectDrawAnythingRecorded(
                ref<CRecorderImpl> rRecorder)
                return
                    rRecorder->x_rDrawDisposition;

    };

    // [L] - Route
    class CGraphTypeRoute : public CGraphType
    {
    public:
    protected:

        void flag_List_as_GraphType();

        virtual sloc OnGetGraphNameForButton()
                return
                    TRANSLATE(
                        "Trajectory[... UI-string-removed]");

        virtual res OnGetGraphIconForButton()
                return
                    RES__GRAPH_TRAJECTORY_L();
                    //RES__MONITOR_SIGNAL_L();
                    //RES__AXIS_MARKER_DARK_L();

        virtual ref<CDrawAnythingRecorded> OnSelectDrawAnythingRecorded(
                ref<CRecorderImpl> rRecorder)
                return
                    rRecorder->x_rDrawRoute;

    };

    // [X][Y] - Coords
    class CGraphTypeCoords : public CGraphType
    {
    public:
        void flag_List_as_GraphType();

    protected:

        virtual sloc OnGetGraphNameForButton()
                return
                    TRANSLATE(
                        "Coordinate[... UI-string-removed]");

        virtual res OnGetGraphIconForButton()
                return
                    RES__MON_MULTI_GRAPH_L();
                    //RES__MONITOR_SIGNAL_L();
                    //RES__AXIS_MARKER_DARK_L();

        virtual ref<CDrawAnythingRecorded> OnSelectDrawAnythingRecorded(
                ref<CRecorderImpl> rRecorder)
                return
                    rRecorder->x_rDrawCoords;
    };

    // [V] - Velocity
    class CGraphTypeVelocity : public CGraphType
    {
    public:
        void flag_List_as_GraphType();

    protected:

        virtual sloc OnGetGraphNameForButton()
                return
                    TRANSLATE(
                        "Velocity[... UI-string-removed]");

        virtual res OnGetGraphIconForButton()
                return
                    RES__HISTOGRAM_L();

        virtual ref<CDrawAnythingRecorded> OnSelectDrawAnythingRecorded(
                ref<CRecorderImpl> rRecorder)
                return
                    rRecorder->x_rDrawVelocity;
    };

    // [F] - Freq
    class CGraphTypeFreq : public CGraphType
    {
    public:
        void flag_List_as_GraphType();

    protected:

        virtual sloc OnGetGraphNameForButton()
                return
                    TRANSLATE(
                        "Frequenc[... UI-string-removed]");

        virtual res OnGetGraphIconForButton()
                return
                    (res) RES_SAMPLER_DPX;

        virtual bool OnIsPresentAsToolbarButton()
                return
                    false;

        virtual ref<CDrawAnythingRecorded> OnSelectDrawAnythingRecorded(
                ref<CRecorderImpl> rRecorder)
                return
                    rRecorder->x_rDrawFreq;
    };

    // [A] - Energy
    class CGraphTypeEnergy : public CGraphType
    {
    public:
        void flag_List_as_GraphType();

    protected:

        virtual sloc OnGetGraphNameForButton()
                return
                    TRANSLATE(
                        "Energy[... UI-string-removed]");

        virtual res OnGetGraphIconForButton()
                return
                    RES__OPTIONS_INTERNAL_L();

        virtual bool OnIsPresentAsToolbarButton()
                return
                    false;

        virtual bool OnIsPresentAsToolbarButtonOnlyInExperimentalMode()
                return
                    true;

        virtual ref<CDrawAnythingRecorded> OnSelectDrawAnythingRecorded(
                ref<CRecorderImpl> rRecorder)
                return
                    rRecorder->x_rDrawEnergy;
    };

    // Table
    class CGraphTypeTable : public CGraphType
    {
    public:
        void flag_List_as_GraphType();

    protected:

        virtual sloc OnGetGraphNameForButton()
                return
                    TRANSLATE(
                        "Math[... UI-string-removed]");
                    //TRANSLATE(
                    //   [... UI-string-removed]");

        virtual res OnGetGraphIconForButton()
                return
                    RES__FORMULA_EDITOR_L();

        virtual ref<CDrawAnythingRecorded> OnSelectDrawAnythingRecorded(
                ref<CRecorderImpl> rRecorder)
                return
                    rRecorder->x_rDrawTable;

    };

    // All
    class CGraphTypeAll : public CGraphType
    {
    public:
        void flag_List_as_GraphType();

    protected:

        virtual sloc OnGetGraphNameForButton()
                return
                    TRANSLATE(
                        "All[... UI-string-removed]");

        virtual res OnGetGraphIconForButton()
                return
                    RES__WINDOWS_ARRANGE_L();
                    //RES__WINDOWS_ARRANGE_L40();

        virtual ref<CDrawAnythingRecorded> OnSelectDrawAnythingRecorded(
                ref<CRecorderImpl> rRecorder)
                return
                    rRecorder->x_rDrawAll;

        virtual bool OnHasPadBeforeToolbarButton()
                return
                    true;
    };
