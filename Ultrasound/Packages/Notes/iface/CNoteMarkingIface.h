// CNoteMarkingIface.h
#pragma once

class CDbDocIface;
class CUdbMirrorIfaceGp;
class CDocIface;

//
// CNoteMarkingIface -
//
//   On-media markers mode for Doc.
//
ASSUME_IMPL_FOR_IFACE(CNoteMarkingImpl, CNoteMarkingIface)

class CNoteMarkingIface : public object
{
public:
    CNoteMarkingIface();
    NEW_LOCAL(CNoteMarkingImpl, CNoteMarkingIface)
    //NEW_GP(Usmain, CNoteMarkingImpl, CNoteMarkingIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);
    bool MirrorDbNoteMarking(
            ref<CUdbMirrorIfaceGp> rUdbMirror,
            ref<CDbDocIface> rDbDoc)
            vhook;

// Constants

// Attributes

    // Backref to parent
    ptr<CDocIface> _x_pParentDoc
            xauto(Get, Set);

// Operations

    // Initial positions
    void InitNoteMarkingForNewRecording()
            vhook;

    // Update
    void UpdateNoteMarking()
            vhook;

    // see x_bRestrictToNoteMarkerRange
    void ModifyMediaScrollRanges(
            out int& out_nRange,
            out int& out_nLostInitialSlices,
            int nTotalVirtualSlices)
            vhook;

    // For cutting playback audio
    int GetRangeStartMarkerPosOr(
            int iReturnIfNotDefined)
            vhook;
    int GetRangeEndMarkerPosOr(
            int iReturnIfNotDefined)
            vhook;

// UI

    // Overlay over media image
    ref<UiContainer> NewUiNoteMarkingMainOverlay()
            vhook;

    // Inserted in Doc context menu
    ref<UiContainer> NewUiNoteMarkingMenu(
            point pointAtScreen)
            vhook;

protected:

    virtual bool OnMirrorDbNoteMarking(
            ref<CUdbMirrorIfaceGp> rUdbMirror,
            ref<CDbDocIface> rDbDoc)
            v1pure;
    virtual ref<UiContainer> OnNewUiNoteMarkingMainOverlay()
            v1pure;
    virtual void OnUpdateNoteMarking()
            v1pure;
    virtual void OnModifyMediaScrollRanges(
            out int& out_nRange,
            out int& out_nLostInitialSlices,
            int nTotalVirtualSlices)
            v1pure;
    virtual int OnGetRangeStartMarkerPosOr(
            int iReturnIfNotDefined)
            v1pure;
    virtual int OnGetRangeEndMarkerPosOr(
            int iReturnIfNotDefined)
            v1pure;
    virtual void OnInitNoteMarkingForNewRecording()
            v1pure;
    virtual ref<UiContainer> OnNewUiNoteMarkingMenu(
            point pointAtScreen)
            v1pure;

private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
