// CPatientSampleMap.h
#pragma once

class CProxyIface;
class CPatientSampleIfaceGp;
class CPatientLinkImpl;

//
// CPatientSampleMap - 
//
//   Maintains Proxy list for PatientIo-OpenSample and for PatientEmbed.
//     This map automatically forgets cloased Proxys.
//
//ASSUME_IMPL_FOR_IFACE(CPatientSampleMapImpl, CPatientSampleMap)

class CPatientSampleMap : public object
{
public:
    CPatientSampleMap();
    //NEW_LOCAL(CPatientSampleMapImpl, CPatientSampleMap)
    //NEW_GP(CustomApps, CPatientSampleMapImpl, CPatientSampleMap)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent -- used by both PatientIo and PatientEmbed wrappers
    //ptr<>

    // Counts added and not closed Proxys
    int CountOpenedProxys();

// Operations

    // Open another sample
    bool AddAndOpenNewProxy(
            ref<CPatientSampleIfaceGp> rPatientSample,
            ref<CProxyIface> rNewProxy);

    // Cleanup
    void CloseAllViewedProxys();

    // List opened Patients samples
    bool IterateViewedSessions(
            out iter& out_i, 
            out ref<CProxyIface>& out_rProxy);

    // Find a sample by its key
    ptr<CProxyIface> FindProxyByPatientSampleKey(
            str sPatientSampleKey);

// UI

protected:
private:
    //bool _m_bOneTimeInitPatientSampleMapOk = false;

    // Samples viewed in Patients
    array< ref<CProxyIface> > _m_aViewProxy;

    // auto-cleanup
    void CleanupClosedProxys();
    bool FindAndDeleteFirstClosedProxy();

    void _init_AttachToPatientSampleMap();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
