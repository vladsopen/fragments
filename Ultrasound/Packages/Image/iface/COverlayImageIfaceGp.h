// COverlayImageIfaceGp.h
#pragma once

//
// COverlayImageIfaceGp -
//
//   Overlays media with event data
//
//ASSUME_IMPL_FOR_IFACE(COverlayImageImpl, COverlayImageIfaceGp)

class COverlayImageIfaceGp : public object
{
public:
    COverlayImageIfaceGp();
    //~COverlayImageIfaceGp();
    //NEW_LOCAL(COverlayImageImpl, COverlayImageIfaceGp)
    //NEW_GP(Usmain, COverlayImageImpl, COverlayImageIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);
    //ref<> Get ()
    //        return
    //            _x_p .Get().Valid();

// Operations

    // One-time opener
    //void InitOverlayImage();

    // Cleanup
    //void CloseOverlayImage();

// UI

    // Main overlay
    ref<UiContainer> NewUiOverlayImageView()
            vhook;

protected:
    virtual ref<UiContainer> OnNewUiOverlayImageView()
            v1pure;
private:
    //bool _m_bInited = false;
    //bool _m_bOpened = false;

    void _init_AttachToOverlayImageIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
