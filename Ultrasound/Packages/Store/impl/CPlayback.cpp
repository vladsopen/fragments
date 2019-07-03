// CPlayback.cpp

#include "CUsmainProject.h"
#include "CPlayback.h"
#include "CAudioSliceGp.h"
#include "CUsoundFileHeapIfaceGp.h"
#include "CDocImpl.h"
#include "CIoControlIfaceGp.h"
#include "CStoreIface.h"
#include "CAppEventFloaterIfaceGp.h"
#include "VirtOs_file.h"
#include "CHardwareIface.h"
#include "CSessionIface.h"
#include "CSendClusterIfaceGp.h"
#include "CImageIface.h"
#include "CRawSlotGp.h"

CPlayback::CPlayback()
{
}

void CPlayback::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CPlayback!");
}

bool CPlayback::IsPlaying()
{
    return m_bPlayingBack;
}

void CPlayback::PrepareToStartPlayback(
        ref<CAudioSliceGp> rAudioSlice,
        percent percentVolume)
{
    ref<CDocImpl> rDoc = 
        _x_pParentDoc.Get().Valid();

    // stop prev
    StopPlayback();

    //
    // Start
    //

    m_bPlayingBack = true;

    //
    // Save temp file for Parallelizer to read
    //

    path pathPlayTemp = 
        SaveTempPlaybackFileHeap(
            rAudioSlice);

    if (pathPlayTemp == "")
    {
        return;
    }

    m_pathTempFileHeap = pathPlayTemp;

    //
    // Register our doc to receive data
    //

    ref<CSessionIface> rSession = 
        rDoc->GetSessionForInitedDoc();
    ref<CHardwareIface> rHardware = 
        rSession->x_rHardware;

    // we need the pointer for device to send the data to saved doc in viewer
    rASSERT(rHardware->_x_pSinglePlaybackDoc == null()); // only 1 doc allowed!
    if (!rDoc->IsDocInitializedForRecording())
    {
        rHardware->_x_pSinglePlaybackDoc = rDoc;
    }

    //
    // Start Parallelizer
    //

    int iChannel = rDoc->x_rRawSlot->x_iScannerChannel;
    int iCluster = rDoc->x_rRawSlot->x_iScannerCluster;

    // we can only play a single saved doc for which ch/cluster is meaningless
    if (!rDoc->IsDocInitializedForRecording())
    {
        // force zero cluster buffer always audible
        //sync:_single_saved_forced_play_as_0_0_ Unstable solution!
        iChannel = 0;
        iCluster = 0;
        _m_rIoControl->
            SelectAudioChannelAndCluster(
                iChannel,
                iCluster);
    }

    // moved out of if() above because we receive valid percentVolume from the client

    SetPlaybackVolume(
        percentVolume);

    _m_rIoControl->
        SetPlayback(
            false, // bool bOn,
            iChannel, // int iChannel,
            iCluster, // int iCluster,
            m_pathTempFileHeap); // path pathAudio)
}

void CPlayback::SetPlaybackVolume(
        percent percentVolume)
{
    ref<CDocImpl> rDoc = 
        _x_pParentDoc.Get().Valid();

    // viewer has no device to initialize this:
    _m_rIoControl->
        SetAudioVolumeScannerScaleAndSampling(
            percentVolume,
            //100, // nVolumeInPercents,
            rDoc->x_rLastSendCluster->x_nCurrentScannerScaleInHz,
            rDoc->x_rLastSendCluster->x_nCurrentScannerSamplingFreqInHz);
}

path CPlayback::SaveTempPlaybackFileHeap(
        ref<CAudioSliceGp> rAudioSlice)
{
    ref<CDocImpl> rDoc = 
        _x_pParentDoc.Get().Valid();

    str sTempName =         ;

    path pathTemp = 
        CStoreIface::GGetStoreTempFolder().
            GetAppendedPath(sTempName)
            //GetAppendedPath(CStoreIface::C_pathStoreTempFileExt)
            // store will add the ext
            ;

    str sError;
    path pathResult = 
        rDoc->
            x_rStore->
                SaveAudioForDoc(
                    rAudioSlice,
                    pathTemp,
                    out sError);

    if (sError != "")
    {
        CProject::GGetProjectCriticalErrorFloater()->
            LogNewEvent(
                "" + 
                    TRANSLATE(
                        "Cannot wri[... UI-string-removed]");
        return path("");
    }

    return pathResult;

}

void CPlayback::StopPlayback()
{
    ref<CDocImpl> rDoc = 
        _x_pParentDoc.Get().Valid();

    if (m_bPlayingBack)
    {
        rASSERT(m_pathTempFileHeap != "");

        //
        // Stop
        //

        m_bPlayingBack = false;

        _m_rIoControl->
            SetPlayback(
                false, // bool bOn,
                -1, // int iChannel,
                -1, // int iCluster,
                path("")); // path pathAudio)

        //
        // Unregister our doc to receive data
        //

        ref<CSessionIface> rSession = 
            rDoc->GetSessionForInitedDoc();
        ref<CHardwareIface> rHardware = 
            rSession->x_rHardware;

        // yes, clear always!
        rHardware->_x_pSinglePlaybackDoc = null();

        //
        // OFF - Notify doc
        //

        ref<CSendClusterIfaceGp> rSendCluster;

        rSendCluster->
            AssignSendClusterFrom(
                rDoc->x_rLastSendCluster);
        //rSendCluster.ExposeIn(rDoc->x_rLastSendCluster.Get()->ExposeOut());

        rSendCluster->x_bPlayingScannerAudio = false;
        //rSendCluster->x_bScannerOnline = rSendCluster->x_bPlayingScannerAudio;
        rDoc->
            SendNewSendCluster(
                E_ScannerDevice_Emulator,
                rSendCluster);
    }
    else
    {
        rASSERT(m_pathTempFileHeap == "");
    }
}

ref<UiContainer> CPlayback::NewUiPlaybackStop()
{
    ref<UiPlaybackStop> rUi;

    rUi->m_rPlayback = this;

    return rUi;
}
