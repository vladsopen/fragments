// CNetStatsImpl.cpp

#include "CNetProject.h"
#include "CNetStatsImpl.h"

CNetStatsImpl::CNetStatsImpl()
{
    _init_AttachToNetStatsImpl();
}

void CNetStatsImpl::_init_AttachToNetStatsImpl()
{
    //x_r->_x_p = this;
}

void CNetStatsImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToNetStatsImpl();

    //rFAIL("make sure it is OK to expose CNetStatsImpl!");
}

void CNetStatsImpl::OnInitNetStats(
        str sPurpose)
{
    x_sStatsForPurpose = sPurpose;
}

void CNetStatsImpl::OnHandlePollingEvent()
{
    x_nTransferredPollingCount = x_nTransferredPollingCount + 1;
}

void CNetStatsImpl::OnResetNetStats()
{
    str sCaption = x_sStatsForPurpose;

    this->ExposeObjectContentInWithoutRenew(unidef());

    x_sStatsForPurpose = sCaption;
}

void CNetStatsImpl::OnHandleTransferEvent(
        int nBytes)
{
    // comchan!
    /*
    if (nBytes <= 0)
    {
        rASSERT(nBytes == 0);
        return;
    }
    */

    x_nTransferredBoxCount = x_nTransferredBoxCount + 1;

    utc utcNow = sys::GGetUtc();
    x_utcLastTransfer = utcNow;

    if (x_utcFirstTransfer == null())
    {
        x_utcFirstTransfer = utcNow;
    }

    x_nnTransferredBytes = 
        x_nnTransferredBytes 
            +
            nBytes;
}

utc CNetStatsImpl::Onx_utcLastTransferOrProgressOrInit_Get()
{
    if (m_utcInit == null())
    {
        m_utcInit = sys::GGetUtc();
    }

    if (x_utcLastTransfer < m_utcInit)
    {
        x_utcLastTransfer = null();
        x_utcFirstTransfer = null();
    }

    utc utcResult;
    if (x_utcLastTransfer != null())
    {
        utcResult = x_utcLastTransfer;
    }
    else
    {
        utcResult = m_utcInit;
    }

    if (x_utcLastProgress > utcResult)
    {
        utcResult = x_utcLastProgress;
    }

    return utcResult;
}

void CNetStatsImpl::OnHandleStartArchiving()
{
    m_nnTscStartCompress = GReadTsc();
}

void CNetStatsImpl::OnHandleEndArchiving(
        int nInBytes,
        int nOutBytes)
{
    rASSERT(m_nnTscStartCompress != 0);

    big nnTscEndCompress = GReadTsc();

    big nnTscOutside = 
        m_nnTscStartCompress - m_nnTscLastAfterCompress;
    big nnTscCompress = 
        nnTscEndCompress - m_nnTscStartCompress;

    if (m_nnTscLastAfterCompress != 0)
    {
        num numPercentTimeCompress = 
            math::GCalcPercentOf(
                num(nnTscOutside + nnTscCompress),
                num(nnTscCompress));

        m_numAverageCompressTimePercent = 
            CNetStatsIfaceGp::GCalcNextAverage(
                m_numAverageCompressTimePercent,
                numPercentTimeCompress,
                2);
    }

    num numRatioPercent = 
        math::GCalcPercentOf(
            nInBytes,
            nOutBytes);

    m_numAverageCompressRatioPercent = 
        CNetStatsIfaceGp::GCalcNextAverage(
            m_numAverageCompressRatioPercent,
            numRatioPercent,
            2);

    m_nnTscLastAfterCompress = GReadTsc();

    m_nnTscStartCompress = 0;
}

str CNetStatsImpl::OnGetCompressionReport()
{
    if (x_sCompressionError != "")
    {
        return x_sCompressionError;
    }

    if (m_nnTscLastAfterCompress == 0)
    {
        return "";
    }

    return 
        "zip " +
            (!GIsDebugUiCompact()? "time: " : "") +
            Str(math::GEnsurePrecision(m_numAverageCompressTimePercent, 3)) + 
            "%, " +
            (!GIsDebugUiCompact()? "ratio to: " : "") +
            RichBold(Str(math::GEnsurePrecision(m_numAverageCompressRatioPercent, 3))) + 
            "%";
}

ref<UiContainer> CNetStatsImpl::OnNewUiNetStatsIndicator()
{
    ref<UiNetStatsIndicator> rUi;

    rUi->m_rNetStats = this;

    return rUi;
}

