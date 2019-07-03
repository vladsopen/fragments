// CRawAudioImpl.cpp

#include "CUsmainProject.h"
#include "CRawAudioImpl.h"
#include "CRawWaveImpl.h"
#include "CAudioSliceGp.h"
#include "CAppEventFloaterIfaceGp.h"
#include "CStoreImpl.h"
#include "CDocIface.h"
#include "CImageIface.h"
#include "CFftIface.h"
#include "CMatrixListImpl.h"
#include "VirtOs_file.h"
#include "VirtOs_ask.h"
#include "CRawSlotGp.h"
#include "CDocMetricsIfaceGp.h"
#include "CUsoundFileHeapIfaceGp.h"
#include "CUsoundFileHeapSubheaderIfaceGp.h"

CRawAudioImpl::CRawAudioImpl()
{
    // Left/Right or Sin/Cos channels are fixed and identical
    m_aRawWave.Add(ref<CRawWaveImpl>()); // Sin
    m_aRawWave.Add(ref<CRawWaveImpl>()); // Cos
    ASSERT(m_aRawWave.GetCount() == C_nAudioChannelCount);

    // autoinit
    repeat(i, m_aRawWave.GetCount())
    {
        m_aRawWave.GetAt(i)->x_iRawWaveChannel = i;
    }

    // optimize Add() a little
    //_m_rAudioSlice->SetGrowByShortCount(1 * 1000 * 1000);

    _init_AttachProperties();
}

CRawAudioImpl::~CRawAudioImpl()
{
    HAIRY; // searching for slow destructors

    ClearAudioSliceAndTempFile();
}

void CRawAudioImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachProperties();

    //rFAIL("make sure it is OK to expose CRawAudioImpl!");
}

void CRawAudioImpl::_init_AttachProperties()
{
    repeat(i, m_aRawWave.GetCount())
    {
        m_aRawWave.GetAt(i)->_x_pParentRawAudio = this;
    }
}

void CRawAudioImpl::PreallocAudioBeforeStart()
{
    // THIS IS CALLED FOR EVERY DEVICE STATUS EVENT!
    //ClearAudioSliceAndTempFile();

    EnsureAudioBufferPreallocated();
}

int CRawAudioImpl::GetAudioSliceShortCount()
{
    // eiter in-memory count or what we have saved to a temp file

    int nCount = _m_nAudioSlicesFlushedToTempFile;
    if (nCount <= 0)
    {
        nCount = _m_rAudioSlice->GetCount();
    }

    return nCount;
}

void CRawAudioImpl::EnsureAudioBufferPreallocated()
{
    if failed(_m_nAudioSlicesFlushedToTempFile <= 0)
    {
        ClearAudioSliceAndTempFile();
        rASSERT(_m_nAudioSlicesFlushedToTempFile <= 0);
    }

    // Preallocate
    int nChannels = m_aRawWave.GetCount();
    int nReserveTotal = CalcMaxStoredAudioComposites() * nChannels;
    if (nReserveTotal != _m_rAudioSlice->GetCount())
    {
        TESTLOG(
            "",
            "RawAudio circular buffer resize [" + 
                Str(_m_rAudioSlice->GetCount()) + "] --> [" + 
                Str(nReserveTotal) + "] quadratures."
                "\n");

        ref<CStoreImpl> rStore =
            _x_pParentStore.Get().Valid();
        ref<CDocIface> rDoc =
            rStore->_x_pParentDoc.Get().Valid();

        // busy...
        rDoc->
            StartDocBusyState(
                CDocIface::E_DocBusyState_AllocatingAudio);

        rASSERTM(
            rDoc->x_rRawSlot->x_iScannerChannel >= 0 &&
            rDoc->x_rRawSlot->x_iScannerCluster >= 0,
            "audio buffer should only be allocated for "
                "Doc linked to a valid RawSlot");

        // Alloc
        _m_rAudioSlice->SetCount(nReserveTotal);

        // not busy.
        rDoc->
            StartDocBusyState(
                CDocIface::E_DocBusyState_AudioAllocated);

        if (m_iNextFreeCircularAudioWord >= nReserveTotal)
        {
            NOTEST; //todo!

            TESTLOG(
                "",
                "Resetting circular buffer pointer = " + 
                    Str(m_iNextFreeCircularAudioWord) + " after resize."
                    "\n");

            // wrong anyway, but causes no array failures later
            m_iNextFreeCircularAudioWord = 0;
        }
    }
}

void CRawAudioImpl::AddNextOnlineRawComposites(
        ref<CAudioSliceGp> rAddAudioSlice)
{
    HAIRY; // profiling

    //int nChannels = m_aRawWave.GetCount();

    // Prealloc
    EnsureAudioBufferPreallocated();

    rASSERT(_m_nAudioSlicesFlushedToTempFile <= 0);

    // max we can save
    int nCircularBufferCount = _m_rAudioSlice->GetCount();

    // incoming count
    int nAddAudioWords = rAddAudioSlice->GetVirtualizableAudioWordCount();

    // removing requirement for double-short alignment for RASTER with a header

    short shortRe = 0;
    short shortIm = 0;
    repeat(iSample, nAddAudioWords)
    {
        short shortSample = 
            rAddAudioSlice->
                GetAt(
                    iSample);

        _m_rAudioSlice->
            SetAt(
                m_iNextFreeCircularAudioWord, 
                shortSample);

        shortRe = shortIm;
        shortIm = shortSample;

        // Next Circular
        m_iNextFreeCircularAudioWord += 1;
        if (m_iNextFreeCircularAudioWord >= nCircularBufferCount)
        {
            TESTLOG(
                "",
                "RawAudio restarts Circular buffer [" + 
                    Str(nCircularBufferCount) + 
                    "] at rotation #" + 
                    Str(m_nLoopsAround) +
                    "\n");

            m_iNextFreeCircularAudioWord = 0;
            m_nLoopsAround++;
        }
    }

    TESTLOG(
        "",
        "RawAudio has circualrly added " + 
            Str(nAddComposites) + " quadratures <..., " +
            Str((int) shortRe) + ", " + 
            Str((int) shortIm) + ">"
            "\n");

    // update debug sometimes
    if (sys::GGetUtc() - _m_tickLastDebugGraphAutoUpdate > 300 &&
        !debug::GIsTest())
    {
        NOTEST; 

        InvalidateAudioDebugGraphPaint();

        _m_tickLastDebugGraphAutoUpdate = sys::GGetUtc();
    }
}

int CRawAudioImpl::TranslatePlainIndexToCircularBuffer(
        int iAt)
{
    NONHAIRY;

    int nTotal = GetAudioSliceShortCount();

    // saturate on illegal out of bounds

    if failed(iAt >= 0)
    {
        NOTEST; //todo!

        iAt = 0;
    }

    if failed(iAt < nTotal)
    {
        NOTEST; //todo!

        iAt = nTotal - 1;
    }

    // usual curcular logic

    int iCircular = iAt - (nTotal - m_iNextFreeCircularAudioWord);

    if (iCircular < 0)
    {
        iCircular = nTotal - (-iCircular);
    }

    ASSERT(iCircular >= 0 && iCircular < nTotal);
    return iCircular;
}

void CRawAudioImpl::SetAudioAt(
        int iChannel,
        int iAt,
        int nAmplitude)
{
    NOTEST;

    ASSERT(_m_rAudioSlice->GetCount() > 0);

    // multiplication optimization
    static const int C_nConstChannelCount = 2;
    ASSERT(m_aRawWave.GetCount() == C_nConstChannelCount);

    int iCircular = 
        TranslatePlainIndexToCircularBuffer(
            iAt * C_nConstChannelCount + iChannel);

    ASSERT(int(short(nAmplitude)) == nAmplitude);

    _m_rAudioSlice->
        SetAt(
            iCircular,
            (short) nAmplitude);
}

void CRawAudioImpl::ConvertFromSliceToCompositeRange(
        int iScrollAtMatrix,
        int nViewMatrixWidth,
        out int& out_iFirstVisibleVirtualOffset,
        out int& out_nVisibleCompositeCount)
{
    // Map audio to the visible area
    ref<CStoreImpl> rStore =
        _x_pParentStore.Get().Valid();
    ref<CMatrixListImpl> rMatrixList = 
        rStore->x_rMatrixList;
    ref<CDocIface> rDoc =
        rStore->_x_pParentDoc.Get().Valid();
    ref<CFftIface> rFft = 
        rDoc->x_rFft;

    int nTotalMatrixVirtual = 
        rMatrixList->GetVirtualSliceCount();
    int nCompositesPerMatrix = 
        rFft->x_nCountCompositesBeforeRecalc;

    int nTotalAudioVirtual = 
        nTotalMatrixVirtual * nCompositesPerMatrix;
    int iScrollAtComposite = 
        iScrollAtMatrix * nCompositesPerMatrix;
    int nViewComposites = 
        nViewMatrixWidth * nCompositesPerMatrix;

    // now we have two cases
    //  1. we have whole raw audio recording (256 s?) 
    //     which is always 'right-aligned' to the last virtual index
    //  2. we have an imported raw audio cut to the view width
    //     it is supposedly aligned to the right edge of the (fixed!) media view
    if (math::GAbs(nViewComposites - GetAudioSliceShortCount() / 2) > 1) // +/-1 tolerance
    {
        // case 1: calculate from the rightmost virtual position
        //
        //       |<--------------- TotalVirtual -------------------->|
        //                             |<--------- 256 s. ---------->|
        //       .........lost.........:::::::cutoff:::::[MTXVIEW]:::|  // matrix
        //                             [AAAAAAAAAAAAAAAAAAAAAAAAAAAAA|  // audio (complete)
        //                                         [AAAAAAAAAAAAAAAAA|  // audio (incomplete)
        //                                               i <-- first visible
        //                                               <--nIndent-->
        //
        int nIndent = nTotalAudioVirtual - iScrollAtComposite;
        rASSERT(nIndent >= 0);
        rASSERT(nIndent <= GetAudioSliceShortCount() / 2);
        out_iFirstVisibleVirtualOffset = GetAudioSliceShortCount() / 2 - nIndent;
        out_nVisibleCompositeCount = nViewComposites;
    }
    else
    {
        // case 2: calculate within the matrix window
        //
        //       |<--------------- TotalVirtual -------------------->|
        //                             |<--------- 256 s. ---------->|
        //       .........lost.........:::::::cutoff:::::[MTXVIEW]:::|  // matrix
        //                                               [AAAAAAA]      // audio
        //                                                   |
        //                                                   v
        //                                               see assert:
        ASSERT(nViewComposites == GetAudioSliceShortCount() / 2);

[...]