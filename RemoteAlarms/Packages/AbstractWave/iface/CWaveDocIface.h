// CWaveDocIface.h
#pragma once

class CPatientLinkSampleIface;
class CSessionIface;

//
// CWaveDocIface -
//
//   Main built-in simple ECG viewer
//
//ASSUME_IMPL_FOR_IFACE(CWaveDocImpl, CWaveDocIface)

class CWaveDocIface : public object
{
public:
    CWaveDocIface();
    //~CWaveDocIface();
    NEW_LOCAL(CWaveDocImpl, CWaveDocIface)
    //NEW_GP(Ralarm, CWaveDocImpl, CWaveDocIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<CSessionIface> _x_pSession
    //        xauto(Get, Set);
    //ref<CSessionIface> GetSession()
    //        return
    //            _x_pSession.Get().Valid();

    // Opened?
    bool IsWaveDocActive()
            vhook;

    // Patients sample context
    ref<CPatientLinkSampleIface> _x_rPatientLinkSample
            xauto(Get);

// Operations

    // One-time opener
    void OpenWaveDoc(
            path pathWave)
            vhook;

    // Cleanup
    void CloseWaveDoc()
            vhook;

// UI

    // Main
    ref<UiContainer> NewUiWaveDocMain()
            vhook;

    // Options menu
    ref<UiContainer> NewUiWaveDocMenu()
            vhook;

protected:
    SEE_ALSO(CWaveDocImpl)

    virtual bool OnIsWaveDocActive()
            v1pure;
    virtual void OnOpenWaveDoc(
            path pathWave)
            v1pure;
    virtual void OnCloseWaveDoc()
            v1pure;
    virtual ref<UiContainer> OnNewUiWaveDocMain()
            v1pure;
    virtual ref<UiContainer> OnNewUiWaveDocMenu()
            v1pure;

private:
    //bool _m_bOneTimeInitWaveDocIfaceOk = false;
    //bool _m_bWaveDocIfaceOpened = false;

    void _init_AttachToWaveDocIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
