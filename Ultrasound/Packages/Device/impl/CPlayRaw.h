// CPlayRaw.h
#pragma once

class CUsoundFileHeapIfaceGp;

//
// CPlayRaw - 
//
//   Controls data source for playback for a single cluster buffer.
//
//ASSUME_IMPL_FOR_IFACE(CPlayRawImpl, CPlayRaw)

class CPlayRaw : public object
{
public:
    CPlayRaw();
    //NEW_LOCAL(CPlayRawImpl, CPlayRaw)
    //NEW_GP(MyProject, CPlayRawImpl, CPlayRaw)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Loop count
    int GetLoopAroundCount();

    // For multichannel media and RASTER files selects a gete to filter by
    void SetFilterChannel(
            int iChannel);

    // For multichannel media files selects a gete to filter by
    void SetFilterCluster(
            int iCluster);

// Operations

    // Init
    void InitPlayClusterHeapFile(
            path pathAudio);

    // Cleanup
    void ClosePlayClusterHeap();

    // Play step (Wavetrain raw block)
    sbuf GetNextPlayBuffer();

// UI

protected:
private:

    ref<CUsoundFileHeapIfaceGp> m_rPlayUsoundFileHeap;

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
