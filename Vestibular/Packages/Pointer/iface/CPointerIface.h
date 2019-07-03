// CPointerIface.h
#pragma once

class CRangesIface;
class CReadingIface;
class CSessionIface;

//
// CPointerIface -
//
//   Realtime position indicator graph.
//
ASSUME_IMPL_FOR_IFACE(CPointerImpl, CPointerIface)

class CPointerIface : public object
{
public:
    CPointerIface();
    NEW_LOCAL(CPointerImpl, CPointerIface)
    //NEW_GP(Balance, CPointerImpl, CPointerIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CSessionIface> _x_pSession
            xauto(Get, Set);

// Operations

    // Data receiving
    void ReceiveReadingForPointer(
            ref<CReadingIface> rReading,
            ref<CRangesIface> rRanges)
            vhook;

// UI

    ref<UiContainer> NewUiPointerGraph()
            vhook;

protected:
    virtual void OnReceiveReadingForPointer(
            ref<CReadingIface> rReading,
            ref<CRangesIface> rRanges)
            v1pure;
    virtual ref<UiContainer> OnNewUiPointerGraph()
            v1pure;
private:
    //bool _m_bOneTimeInitPointerIfaceOk = false;

    void _init_AttachToPointerIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
