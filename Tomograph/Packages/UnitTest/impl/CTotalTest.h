// CTotalTest.h
#pragma once

class CPatInfoStoreIfaceGp;
class CPatientLinkImpl;
class CDownloaderImpl;

//
// CTotalTest - tests all interfaces without going into math
//
//ASSUME_IMPL_FOR_IFACE(CTotalTestImpl, CTotalTest)

class CTotalTest : public object
{
public:
    CTotalTest();
    //NEW_LOCAL(CTotalTestImpl, CTotalTest)
    //NEW_GP(MyProject, CTotalTestImpl, CTotalTest)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    bool x_bTestDeviceOffline = false
            xauto(Get, Set);

// Operations

    void TestWithStore(
            rp<CPatInfoStoreIfaceGp> rpPatInfoStore);

// UI

protected:
private:

    // log depth control
    bool m_bDetailedLog = false;

    // error generators
    bool m_bObsoleteHeaderSignature = false;

    // UI branching
    bool m_bEraseAllUponCompletion = true;

    // For smaller signals
    int m_nForceTomographSamplePointCount = -1;
    int m_nForceTomographFrameDataBytes = -1;
    array<int> m_anPresetAmplitudes;

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
    void _test_RunTotalTestVariation();
    void _test_RunTotalTestVariationForDownloader(
            ref<CDownloaderImpl> rDownloader);
};
