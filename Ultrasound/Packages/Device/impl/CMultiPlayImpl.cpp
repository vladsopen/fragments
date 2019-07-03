// CMultiPlayImpl.cpp

#include "CUsmainProject.h"
#include "CMultiPlayImpl.h"
#include "CDocIface.h"
#include "CIoControlIfaceGp.h"
#include "CHardwareImpl.h"
#include "CSessionIface.h"
#include "CDocListIface.h"
#include "CDeviceCommon.h"
#include "CCustomizeDocIfaceGp.h"
#include "CCallDealIfaceGp.h"
#include "CDealDocCommandsIfaceGp.h"
#include "CRawSlotGp.h"

CMultiPlayImpl::CMultiPlayImpl()
{
}

void CMultiPlayImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CMultiPlayImpl!");
}

bool CMultiPlayImpl::OnIsMultiPlaying()
{
    return m_bMultiPlaying;
}

void CMultiPlayImpl::OnInitiateDocIntoPlayback(
        ref<CDocIfaceGp> rDoc,
        EUsoundPlaybackRange eUsoundPlaybackRange,
        percent percentVolume)
{
    rASSERT(!m_bMultiPlaying);

    ref<CDocIface> rDocIface = rDoc;

    rDocIface->
        PrepareDocForMultiPlay(
            eUsoundPlaybackRange,
            percentVolume);

    // trace single doc with undefined audible cluster selection
    m_nDocsInPlay++;
    if (m_nDocsInPlay > 1)
    {
        m_rRawSlotSingleDocHasAudibleClusterButtonHidden = null();
    }
    else
    {
        // Check if [Cluster-Mute] button visible
        ref<CCallDealIfaceGp> rCallDeal;
            rDoc->
                _x_rDealDocCommands->
                    DealUsoundClusterSound(
                        rCallDeal);

        // save to tell Parallelizer later
        if (!rCallDeal->IsCommandImplemented())
        {
            m_rRawSlotSingleDocHasAudibleClusterButtonHidden.
                ExposeIn(
                    rDocIface->x_rRawSlot->ExposeOut());

            if (!rDocIface->IsDocInitializedForRecording())
            {
                //sync:_single_saved_forced_play_as_0_0_ Unstable solution!
                m_rRawSlotSingleDocHasAudibleClusterButtonHidden->x_iScannerChannel = 0;
                m_rRawSlotSingleDocHasAudibleClusterButtonHidden->x_iScannerCluster = 0;
            }
        }
    }

    // first always wins...
    if (_m_pFirstStartedDocToControlVolume == null())
    {
        _m_pFirstStartedDocToControlVolume = rDocIface;
    }
}

void CMultiPlayImpl::OnStartMultiPlay()
{
    m_bMultiPlaying = true;

    // if single doc forced audible was not defined ch/cluster will be -1/-1
    int iChannel = m_rRawSlotSingleDocHasAudibleClusterButtonHidden->x_iScannerChannel;
    int iCluster = m_rRawSlotSingleDocHasAudibleClusterButtonHidden->x_iScannerCluster;

    _m_rIoControl->
        SetPlayback(
            true, // bool bOn,
            iChannel, // int iChannel,
            iCluster, // int iCluster,
            path("")); // path pathAudio)
}

void CMultiPlayImpl::OnInitiateAllClustersIntoPlayback(
        percent percentVolume)
{
    ref<CHardwareImpl> rHardware = 
        _x_pParentHardware.Get().Valid();
    ref<CSessionIface> rSession = 
        rHardware->_x_pParentSession.Get().Valid();

    // all docs do their cleanup
    ref<CDocIface> rIterDoc;
    for (
        iter i;
        rSession->x_rDocList->
            IterateRegisteredDocs(
                out i,
                out rIterDoc);)
    {
        if (rIterDoc->IsDocInitializedForRecording() &&
            !rIterDoc->_x_rCustomizeDoc->x_bHideAboveTimeBar)
        {
            InitiateDocIntoPlayback(
                rIterDoc,
                sys::GIsAccess(...)?
                    E_UsoundPlaybackRange_VisibleView
                    :
                    E_UsoundPlaybackRange_WholeRecording,
                percentVolume);
        }
    }
}

[...]