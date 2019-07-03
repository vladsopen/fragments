// CNoteLineTypeImpl.h
#pragma once

#include "CNoteLineTypeIface.h"

class CCustomizeDocIfaceGp;
class CNoteLineState;
class CUsoundAxisIfaceGp;

//
// CNoteLineTypeImpl -
//
//   Defines all manual markers.
//
//ASSUME_IMPL_FOR_IFACE(CNoteLineTypeImplImpl, CNoteLineTypeImpl)

class CNoteLineTypeImpl : public CNoteLineTypeIface
{
public:
    CNoteLineTypeImpl();
    //NEW_LOCAL(CNoteLineTypeImplImpl, CNoteLineTypeImpl)
    //NEW_GP(MyProject, CNoteLineTypeImplImpl, CNoteLineTypeImpl)

// Constants

// Attributes

    // Ui ID
    str GetNoteLineName(
            ptr<CNoteLineState> pNoteLineState)
            vhooked;

    // Ui ID
    sloc GetNoteLineDescription()
            vhooked;

    // Different basic color for all indexes
    rgb GetNoteLineColor()
            vhook;

    // We can add multiple Comments but Start/End markers are singular
    bool IsSingularNoteLine()
            vhook;

    // User enters comment text
    bool HasUserEnteredText()
            vhook;

    // Axis (usually derived from the Index)
    type<CUsoundAxisIfaceGp> GetNoteLineUsoundAxis()
            vhooked;

    // Media histogram at T1 marker
    bool HasHistogramPainted()
            vhook;

    // We play media at some markers
    bool HasPlaybackAssociated()
            vhook;

    // Show only with debug
    bool IsInternalDebugMarker()
            vhook;

    // Hide some makers for some view modes
    bool IsMarkerVisibleForCustomizeDoc(
            ref<CCustomizeDocIfaceGp> rCustomizeDoc)
            vhook;

// Operations

// Derived Item Enumeration

    const typelist<CNoteLineTypeImpl> GetDerivedList();

// UI

protected:

    virtual const typelist<CNoteLineTypeIface> OnGetDerivedListIface()
            return
                GetDerivedList();

    virtual str OnGetNoteLineName(
            ptr<CNoteLineState> pNoteLineState)
            v1st
            return
                ""; // must get from Index

    virtual sloc OnGetNoteLineDescription()
            v1st
            return
                ""; // must get from Index

    virtual bool OnIsSingularNoteLine()
            v1pure;

    virtual bool OnHasUserEnteredText()
            v1pure;

    virtual rgb OnGetNoteLineColor()
            v1pure;

    virtual type<CUsoundAxisIfaceGp> OnGetNoteLineUsoundAxis()
            v1st
            return
                null();

    virtual bool OnHasHistogramPainted()
            v1st
            return
                false;

    virtual bool OnHasPlaybackAssociated()
            v1st
            return
                false;

    virtual bool OnIsInternalDebugMarker()
            v1st
            return
                false;

    virtual bool OnIsMarkerVisibleForCustomizeDoc(
            ref<CCustomizeDocIfaceGp> rCustomizeDoc)
            v1st
            return
                true;

private:

    // Check all types for unique fields
    void _test_CheckNoteLineType();

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    virtual void OnTestClass();
};

    // Generic Vertical Marker
    class CNoteLineTypeImplInTimeDomain : public CNoteLineTypeImpl
    {
    public:
    protected:

        virtual type<CUsoundAxisIfaceGp> OnGetNoteLineUsoundAxis()
                return
                    type<CUsoundAxisIfaceGp>()->AsUsoundAxisForTimeX();

    };

        // Generic Audio Range limit
        class CNoteLineTypeImplAudioRange : public CNoteLineTypeImplInTimeDomain
        {
        public:
        protected:

            virtual bool OnIsSingularNoteLine()
                    return
                        true;

            virtual bool OnHasUserEnteredText()
                    return
                        false;

            virtual bool OnIsMarkerVisibleForCustomizeDoc(
                    ref<CCustomizeDocIfaceGp> rCustomizeDoc)
                    return
                        !rCustomizeDoc->x_bRestrictToNoteMarkerRange;

        };

            // From
            class CNoteLineTypeImplAudioFrom : public CNoteLineTypeImplAudioRange
            {
            public:
                void flag_List_as_NoteLineType();

            protected:

                virtual bool OnHasPlaybackAssociated()
                        return
                            true;

                virtual rgb OnGetNoteLineColor()
                        return
                            rgb(0, 255, 0);

                virtual str OnGetNoteLineName(
                        ptr<CNoteLineState> pNoteLineState)
                        return
                            TRANSLATE(
                                "Star[... UI-string-removed]");

                virtual sloc OnGetNoteLineDescription()
                        return
                            TRANSLATE(
                                "Marks inte[... UI-string-removed]");
            };

            // To
            class CNoteLineTypeImplAudioTo : public CNoteLineTypeImplAudioRange
            {
            public:
                void flag_List_as_NoteLineType();

            protected:

                virtual rgb OnGetNoteLineColor()
                        return
                            rgb(255, 0, 0);

                virtual str OnGetNoteLineName(
                        ptr<CNoteLineState> pNoteLineState)
                        return
                            TRANSLATE(
                                "End[... UI-string-removed]");

                virtual sloc OnGetNoteLineDescription()
                        return
                            TRANSLATE(
                                "Marks inte[... UI-string-removed]");
            };

        // Generic Comment Marker
        class CNoteLineTypeImplComment : public CNoteLineTypeImplInTimeDomain
        {
        public:

        protected:

            virtual bool OnIsSingularNoteLine()
                    return
                        false;

            virtual bool OnHasUserEnteredText()
                    return
                        true;

            virtual rgb OnGetNoteLineColor()
                    return
                        rgb(150, 190, 190);

            virtual sloc OnGetNoteLineDescription()
                    return
                        TRANSLATE(
                            "Marks inte[... UI-string-removed]");

        };

            // Genetal Comment
            class CNoteLineTypeImplCommentSimple : public CNoteLineTypeImplComment
            {
            public:
                void flag_List_as_NoteLineType();

            protected:

                virtual str OnGetNoteLineName(
                        ptr<CNoteLineState> pNoteLineState)
                        return
                            TRANSLATE(
                                "Commen[... UI-string-removed]");
            };

            // Histogram
            class CNoteLineTypeImplHistogram : public CNoteLineTypeImplComment
            {
            public:
                void flag_List_as_NoteLineType();

            protected:

                virtual str OnGetNoteLineName(
                        ptr<CNoteLineState> pNoteLineState)
                        return
                            TRANSLATE(
                                "Histogra[... UI-string-removed]");

                virtual bool OnHasHistogramPainted()
                        return
                            true;
            };

    // Generic Horizontal Marker
    class CNoteLineTypeImplInScaleDomain : public CNoteLineTypeImpl
    {
    public:
    protected:

        virtual type<CUsoundAxisIfaceGp> OnGetNoteLineUsoundAxis()
                return
                    type<CUsoundAxisIfaceGp>()->AsUsoundAxisForVelocityScaleY();

    };

        // Horizontal Demo Comment
        class CNoteLineTypeImplVelocityLevel : public CNoteLineTypeImplInScaleDomain
        {
        public:
            void flag_List_as_NoteLineType();

        protected:

            virtual bool OnIsSingularNoteLine()
                    return
                        false;

            virtual bool OnHasUserEnteredText()
                    return
                        true;

            virtual rgb OnGetNoteLineColor()
                    return
                        rgb(255, 0, 255);

            virtual str OnGetNoteLineName(
                    ptr<CNoteLineState> pNoteLineState)
                    return
                        TRANSLATE(
                            "Leve[... UI-string-removed]");

            virtual sloc OnGetNoteLineDescription()
                    return
                        TRANSLATE(
                            "Marks inte[... UI-string-removed]");

            virtual bool OnIsInternalDebugMarker()
                    return
                        true;

        };

