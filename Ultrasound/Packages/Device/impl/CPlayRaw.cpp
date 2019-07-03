// CPlayRaw.cpp

#include "CProject.h"
#include "CPlayRaw.h"
#include "CUsoundFileHeapIfaceGp.h"
#include "CAppEventFloaterIfaceGp.h"
#include "CUsbIface.h"
#include "CUsoundFileHeapSubheaderIfaceGp.h"

CPlayRaw::CPlayRaw()
{
}

void CPlayRaw::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CPlayRaw!");
}

void CPlayRaw::SetFilterChannel(
        int iChannel)
{
    rASSERT(m_rPlayUsoundFileHeap->IsUsoundFileHeapOpen());

    m_rPlayUsoundFileHeap->x_iFilterChannel = iChannel;
}

void CPlayRaw::SetFilterCluster(
        int iCluster)
{
    rASSERT(m_rPlayUsoundFileHeap->IsUsoundFileHeapOpen());

    m_rPlayUsoundFileHeap->x_iFilterCluster = iCluster;
}

void CPlayRaw::InitPlayClusterHeapFile(
        path pathAudio)
{
    ClosePlayClusterHeap();

    ref<CUsoundFileHeapSubheaderIfaceGp> rUsoundFileHeapSubheader;
        // all zeros - we only read

    str sError;
    m_rPlayUsoundFileHeap->
        OpenUsoundFileHeap(
            CFileHeapIfaceGp::E_FileHeapAccess_ExistingReadOnly,
            pathAudio,
            rUsoundFileHeapSubheader,
            out sError);

    if (sError != "")
    {
        CProject::GGetProjectCriticalErrorFloater()->
            LogNewEvent(
                "" + 
                    TRANSLATE(
                        "Cannot ope[... UI-string-removed]");
        return;
    }
}

void CPlayRaw::ClosePlayClusterHeap()
{
    m_rPlayUsoundFileHeap->
        CloseUsoundFileHeap();

    m_rPlayUsoundFileHeap = null();
}

int CPlayRaw::GetLoopAroundCount()
{
    return m_rPlayUsoundFileHeap->x_nLoopAroundCount;
}

sbuf CPlayRaw::GetNextPlayBuffer()
{
    // some cluster players may be uninitialized playing only 2nd 
    if (!m_rPlayUsoundFileHeap->IsUsoundFileHeapOpen())
    {
        return sbuf("");
    }

    str sError;
    sbuf sbufCluster = 
        m_rPlayUsoundFileHeap->
            ReadNextPlaybackBlock(
                CUsbIface::C_nHsBytesPerCluster,
                out sError);

    if (sError != "")
    {
        rFAIL(sError);
    }

    return sbufCluster;
}

