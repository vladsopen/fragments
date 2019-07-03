// CWaveLaunch.h
#pragma once

class CPatInfoContactToSamplerIfaceGp;
class CWaveDocImpl;

//
// CWaveLaunch - 
//
//   Runs external processor via PatInfoContact
//
//ASSUME_IMPL_FOR_IFACE(CWaveLaunchImpl, CWaveLaunch)

class CWaveLaunch : public object
{
public:
    CWaveLaunch();
    ~CWaveLaunch();
    //NEW_LOCAL(CWaveLaunchImpl, CWaveLaunch)
    //NEW_GP(Ralarm, CWaveLaunchImpl, CWaveLaunch)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CWaveDocImpl> _x_pWaveDoc
            xauto(Get, Set);
    ref<CWaveDocImpl> GetWaveDoc()
            return
                _x_pWaveDoc.Get().Valid();

    // On?
    bool IsProcessorLaunched();

// Operations

    // One-time opener
    void InitWaveLaunch();

    // Cleanup
    void CloseWaveLaunch();

    // Open/ActivateSampler
    void LaunchOrReactivateExternalProcessor();

// UI

protected:
private:
    bool _m_bInited = false;
    bool _m_bOpened = false;

    rp<CPatInfoContactToSamplerIfaceGp> m_rpPatInfoContactToSampler;

    void HandleDeadToSampler();

    void _init_AttachToWaveLaunch();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
