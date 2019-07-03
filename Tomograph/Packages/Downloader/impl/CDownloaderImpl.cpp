// CDownloaderImpl.cpp

#include "CProject.h"
#include "CDownloaderImpl.h"
#include "CDeviceManagerIface.h"
#include "CTimerForTomographDownloader.h"
#include "CSessionIface.h"
#include "CTomogramIface.h"
#include "CLoaderIface.h"
#include "CPatientLinkIface.h"
#include "CWizardStepType.h"
#include "CGlobalConfigIface.h"
#include "VirtOs_ask.h"
#include "CBodySchemeIface.h"
#include "VirtOs_dir.h"
#include "VirtOs_CDirList.h"
#include "CRandomIfaceGp.h"
#include "CPatientSampleIfaceGp.h"

CDownloaderImpl::CDownloaderImpl()
{
}

void CDownloaderImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);
}

bool CDownloaderImpl::GotoNextStep(
        type<CWizardStepType> typeCheckExpectedWizardStepType,
        ref<CDeviceManagerIface> rDeviceManager)
{
    // old step
    type<CWizardStepType> typeLeavingStep = x_typeWizardStepType;

    // next step
    type<CWizardStepType> typeNextStep = 
        type<CWizardStepType>()->GetDerivedList().
            GetNextTypeOr(
                typeLeavingStep,
                null());

    // we never go out of step list (staying on the last step as the dead end)
    rASSERT(typeNextStep != null());

    // did the called expexted the same?
    if (typeCheckExpectedWizardStepType != null())
        rASSERT(typeCheckExpectedWizardStepType.IsSelecting(typeNextStep));

    // save the pointer
    this->x_typeWizardStepType = typeNextStep;

    // Main step praparation
    //  (this may call this recursively forcing skip download step 
    //   at least in case when SampleCount == 0)
    bool bSwitchOk = 
        typeNextStep->
            TryEnterStep(
                this,
                rDeviceManager);
    if (!bSwitchOk)
    {
        // stay on prev step
        this->x_typeWizardStepType = typeLeavingStep;
    }

    // Update online progress indication
    _m_pUiDownloaderMain.DefaultIfNull()->UpdateContentAndLayoutNow();
    _m_pUiDownloaderMain.DefaultIfNull()->UpdatePaint();
    //if (_m_pUiDownloaderMain != null())
        //sys::GSleepInTicks(200);

    return bSwitchOk;
}

void CDownloaderImpl::OnAppendNextAmplitudePairFromTomograph(
        ref<CDeviceManagerIface> rDeviceManager,
        int nAmplitudeAt0,
        int nAmplitudeAt1)
{
    TESTLOG(
        "DownloadDetails",
        "\n"
            "Downloader: next amplitude pair "
            "(" + Str(nAmplitudeAt0) + ", " + Str(nAmplitudeAt1) + ").\n");

    if (!IsDownloadInProgress())
    {
        TESTLOG(
            "DownloadDetails",
            "===== download is done, data ignored =====\n");

        return;
    }

    // we add the amplitude pair to the last Tomogram's channels,
    //   and we must make sure THERE IS the last Tomogram in the array 
    //   and it is NOT FULL,
    //   in either case we add next Tomogram to the array and continue...

    // Initialized properly?
    if failed(m_aReceivedTomogram.GetCount() > 0)
        return;

    ref<CTomogramIface> rCurrentTomogram = m_aReceivedTomogram.GetLast();

    // current tomogram is full?
    rCurrentTomogram->
        AppendNextAmplitudePairFromTomograph(
            nAmplitudeAt0,
            nAmplitudeAt1);

    // Just repaint
    if (sys::GGetUtc() - _m_tickLastOnlineRepaint > 
            C_tickOnlineRepaintTimeout &&
        !debug::GIsTest())
    {
        _m_pUiDownloaderMain.DefaultIfNull()->UpdatePaint();

        _m_tickLastOnlineRepaint = sys::GGetUtc();
    }
}

void CDownloaderImpl::OnHandleNextTomogramComplete(
        ref<CDeviceManagerIface> rDeviceManager)
{
    if failed(m_aReceivedTomogram.GetCount() > 0)
        return;

    ref<CTomogramIface> rTomogram = m_aReceivedTomogram.GetLast();

    ASSERT_SOFT(
        rTomogram->GetTomogramAmplitudeCount() == 20000 ||
        (debug::GIsTest() && 
            rTomogram->GetTomogramAmplitudeCount() == 5));

    // Repaint final state
    _m_pUiDownloaderMain.DefaultIfNull()->UpdatePaint();

    // Send to Patients
    if not_null(
        ref<CSessionIface> rSessionTrusted = 
            _x_pSessionTrusted)
    {
        ref<CPatientSampleIfaceGp> rPatientSample;

        rSessionTrusted->
            SendSampleTomogramToPatient(
                rPatientSample,
                rTomogram,
                m_aReceivedTomogram.GetCount() == 1?
                    x_nnAddAttachmentBytes
                    :
                    0,
                x_bAddAttachmentHardRandom,
                x_bAddAttachmentRandomWave);
    }

    // have we gotten all Tomograms we expected?
    if (m_aReceivedTomogram.GetCount() >= x_nWantDownloadTomogramCount)
    {
        rASSERT(m_aReceivedTomogram.GetCount() == x_nWantDownloadTomogramCount);

        // Deselect tomogram to make help message visible
        x_iPreviewTomogram = -1;

        // Stop!
        VERIFY(
            GotoNextStep(
                type<CWizardStepTypeFinalInfo>(),
                rDeviceManager));

        return;
    }

    // Create a new sample
    AddNewTomogram();
}

void CDownloaderImpl::AddNewTomogram()
{
    // Create a new tomogram at the end
    ref<CTomogramIface> rNewTomogram;
    rNewTomogram->x_sTomogramUniqueName = Str(m_aReceivedTomogram.GetCount() + 1);
    m_aReceivedTomogram.Add(rNewTomogram);
    int iOrder = m_aReceivedTomogram.GetCount() - 1;
    x_iPreviewTomogram = iOrder;

    //
    // Assign a body point
    //

    ref<CBodySchemeIface> rBodyScheme = 
        GetGlobalConfig()->x_rBodyScheme;

    str sError;
    str sPointName = 
        rBodyScheme->QueryBodyPointNameFromCurrentProfile(
            iOrder,
            out sError);

[...]