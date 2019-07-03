// CViewState.h
#pragma once

class CDbDocIface;
class CUdbMirrorIfaceGp;
class CImageImpl;
class CViewCache;
class CStoreIface;

//
// CViewState - 
//
//   Slice count, offline scroll and online pointers.
//   Everything we need to know to render media image in current UI context,
//     but indepenently of Ui-panel size (that size is out of our scope here!).
//
//ASSUME_IMPL_FOR_IFACE(CViewStateImpl, CViewState)

class CViewState : public object
{
public:
    CViewState();
    //NEW_LOCAL(CViewStateImpl, CViewState)
    //NEW_GP(MyProject, CViewStateImpl, CViewState)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);
    bool MirrorDbViewState(
            ref<CUdbMirrorIfaceGp> rUdbMirror,
            ref<CDbDocIface> rDbDoc);

// Constants

// Attributes

    // I like very rigid sync between State and Image.
    ptr<CImageImpl> _x_pParentImage
            xauto(Get, Set);

    // Scroll pos is the offset of the first visible Slice
    int GetViewScrollSlice();
    void SetViewScrollSliceInternalPos(
            int iScrollAtSlice);
    void SetViewScrollSliceFromUserAction(
            int iScrollAtSlice);
    void SetViewScrollSliceWithSliderUpdate(
            int iScrollAtSlice);

    // View width is internally measured in Slices 
    //   (and can be converted to/from seconds)
    int GetViewWidthSlices();
    void SetViewWidthSlices(
            int nWidthSlices);

    // We need to support online screen splitting 
    //   moving from the left to the right
    //   (must be less than ViewWidthSlices)
    int GetViewPassedSlices();
    void AdvanceViewPassed();

    // Debug 
    bool IsTurboViewState();

// Operations

    // Starting offline mode
    void ScrollToEndScreenReset();

    // see CImageIface::ForceBroadcastMyTimebarSyncToEveryone()
    void ForceViewStateBroadcastMyTimebarSyncToEveryone();

    // make scroll sync forget old scroll pos when all is clared
    void ResetScrollForRestart();

// UI

    // ViewStateInternals
    ref<UiContainer> NewUiViewStateInternals();
    ptr<CUiPanelGp> _m_pUiViewStateInternals;

    // Public should only call AdvanceViewPassed()
    void SetViewPassedSlicesInternalAndForDebugUi(
            int nPassedSlices);

    // ViewScroll
    ref<UiContainer> NewUiViewScroll();
    ptr<CUiPanelGp> _m_pUiViewScroll;
    alikey _m_alikeyViewScroll;
    ptr<CUiSliderGp> _m_pUiSlider;

    // ViewScrollAlikeUnderScale
    ref<UiContainer> NewUiViewScrollAlikeUnderScale();
    ptr<CUiPanelGp> _m_pUiViewScrollAlikeUnderScale;

protected:
private:

    // metrics
    int m_iScrollSlice = 0;
    int m_nWidthSlices = 1; // absurd default - MUST be initialized!
    int m_nPassedSlices = 0;

    // Internal validator
    void CheckViewState();

    // Helpers for ForceViewStateBroadcastMyTimebarSyncToEveryone();
    num PrepareOrSendTimeWidthSync(
            bool bSend);
    num PrepareOrSendTimeScrollSync(
            bool bSend);

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiViewStateInternals.cpp)
class UiViewStateInternals : public UiContainer
{
public:
    virtual void Ui();

    ref<CViewState> m_rViewState;
};

// (UiViewScroll.cpp)
class UiViewScroll : public UiContainer
{
public:
    virtual void Ui();

    ref<CViewState> m_rViewState;
};

// (UiViewScrollAlikeUnderScale.cpp)
class UiViewScrollAlikeUnderScale : public UiContainer
{
public:
    virtual void Ui();

    ref<CViewState> m_rViewState;
};
