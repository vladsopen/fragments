// CAppEventFloaterImpl.cpp

#include "CGuiPlusProject.h"
#include "CAppEventFloaterImpl.h"
#include "CAppEventImpl.h"
#include "CAppEventFloaterTimer.h"
#include "VirtOs_CAppGp.h"
#include "VirtOs_CGlobalUi.h"
#include "CAppEventCallbackIfaceGp.h"
#include "VirtOs_ask.h"
#include "CUvGlobalsIfaceGp.h"

CAppEventFloaterImpl::CAppEventFloaterImpl()
{
}

void CAppEventFloaterImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("do not expose event floaters!");
}

void CAppEventFloaterImpl::OnOpenEventLogFloater()
{
    if (_m_ptrmapUiMainFloater.GetCountClearingNulls())
    {
        // already open
    }
    else
    {
        // Create the window
        ref<UiContainer> rUiFrame = 
            NewUiAppEventFloater(
                false); // not embedded

        if (x_bExclusiveModalEventFloater)
        {
            rUiFrame->
                CreateExclusiveModalNotTopmost();
        }
        else
        {
            rUiFrame->
                CreateFloater();
        }

        // Update scrolling
        UpdateEventListAndScrollToBottom();
    }
}

void CAppEventFloaterImpl::OnUpdateFloaterNow()
{
    ref<CUiPanelGp> rUiWindowPanel;
    for (
        iter i;
        _m_ptrmapUiAppEventFloaterClient.
            Iterate(
                out i,
                out rUiWindowPanel);)
    {
        rUiWindowPanel->UpdateContentAndLayoutNow();
        rUiWindowPanel->UpdatePaint();
    }
}

void CAppEventFloaterImpl::OnCloseEventLogFloater()
{
    /*
    if not_null(
        ref<CUiPanelGp> rUiFrame = 
            _m_ptrmapUiMainFloater)
    {
        rUiFrame->CloseFrame(); 
        _m_ptrmapUiMainFloater = null();
    }
    */
    _m_ptrmapUiMainFloater.CloseAllFrames();
    _m_ptrmapUiMainFloater.RemoveAll();
}

void CAppEventFloaterImpl::On_template_CloseScope(
        unused unusedCookie)
{
    unusedCookie;

    CloseEventLogFloater()
}

void CAppEventFloaterImpl::OnLogNewEvent(
        sloc slocFriendlyMessage,
        str sTechnicalDetails)
{
    ref<CAppEventIfaceGp> rAppEvent;
    
    rAppEvent->x_slocFriendlyMessage = slocFriendlyMessage;
    rAppEvent->x_sTechnicalDetails = sTechnicalDetails;

    LogNewEventObject(rAppEvent);
}

void CAppEventFloaterImpl::OnLogNewEventObject(
        ref<CAppEventIfaceGp> rAppEvent)
{ 
    if not_null(
        ref<CAppEventCallbackIfaceGp> rAppEventCallback = 
            _x_pAppEventCallback)
    {
        if (!m_bInsideImplementLogNewEventObject)
        {
            cleaner<bool, false> cleanerInsideHook(&m_bInsideImplementLogNewEventObject);
            m_bInsideImplementLogNewEventObject = true;

            rAppEventCallback->
                ImplementLogNewEventObject(
                    rAppEvent,
                    this);

            return;
        }
    }

    // Skip UI and ulog in unit-tests
    if (debug::GIsTest())
    {
        //todo: save stats info for tests

        return;
    }
    // CAppEventIfaceGp never exists
    ref<CAppEventImpl> rAppEventImpl = rAppEvent;

    //bool bFirstTime = (GetSavedTotalAppEventCount() == 0);

    // Increase count
    this->x_nTotalAppEventCount = x_nTotalAppEventCount + 1;
    rAppEventImpl->x_iEventIssueNumber = x_nTotalAppEventCount;
    rAppEventImpl->x_momentEventLogged = moment::GGetLocal();

    str sKey = 
        rAppEventImpl->x_slocFriendlyMessage + 
            "\n" +
            rAppEventImpl->x_sTechnicalDetails;

    // a duplication?
    bool bDuplication = false;
    {
        ref<CAppEventImpl> rExistingEvent;
        if (_m_mapAppEvent.
                LookupExisting(
                    sKey,
                    out rExistingEvent))
        {
            bDuplication = true;
            rExistingEvent->x_nEventTimesIssued += 1;

            _m_rLastAppEvent = rExistingEvent;
        }
        else
        {
            rAppEventImpl->x_nEventTimesIssued += 1;

            // Store
            //_m_mapAppEvent.Add(rAppEventImpl); 
            _m_mapAppEvent[sKey] = rAppEventImpl; 
            _m_rLastAppEvent = rAppEventImpl;
        }
    }

    KillExtra();

    UpdateUiAfterNewLogEvent();
}

void CAppEventFloaterImpl::KillExtra()
{
    // Kill some old errors
    if (_m_mapAppEvent.GetCount() > C_nMaxLogEntries)
    //if (GetSavedTotalAppEventCount() > C_nMaxLogEntries)
    {
        //int iRemoveAt = C_nMaxLogEntries / 2;

        str sIterKey;
        ref<CAppEventImpl> rIterEvent;
        for (
            iter iEvent;
            _m_mapAppEvent.
                IterateKeys(
                    out iEvent,
                    out sIterKey,
                    out rIterEvent);)
        {
            if (iEvent == _m_mapAppEvent.GetCount() / 2)
            {
                _m_mapAppEvent.RemoveAtKey(sIterKey);
                break;
            }
        }

        /*
        if (iRemoveAt <= _m_mapAppEvent.GetCount())
        {
            _m_mapAppEvent.RemoveAt(iRemoveAt);
        }
        */
    }
}

void CAppEventFloaterImpl::EnsureVisibleIfNotHidden()
{
    if (//bFirstTime &&
        !m_bHideFloaterWindow)
    {
        OpenEventLogFloater();
    }
}

void CAppEventFloaterImpl::UpdateUiAfterNewLogEvent()
{
    EnsureVisibleIfNotHidden();

    // Update and show everything instantly
    tick tickNow = sys::GGetUtc();
    if (tickNow - m_tickLastUpdate > C_tickUpdateTimeout)
    {
        m_tickLastUpdate = tickNow;

        // Instantly
        UpdateEventListAndScrollToBottom();
    }
    else
    {
        // Deferred

        // update list
        _m_ptrmapUiAppEventList.InvalidateContent();

        // update the counter in caption
        _m_ptrmapUiAppEventFloaterCaptionText.InvalidateState();

        // call everything later
        m_rAppEventFloaterTimer->InitAppEventFloaterTimer(this);
        m_bUpdateOnIdleTimer = true;
    }
}

void CAppEventFloaterImpl::HandleTimerUpdate()
{
    if (m_bUpdateOnIdleTimer)
    {
        m_bUpdateOnIdleTimer = false;

        UpdateEventListAndScrollToBottom();

        m_bUpdateOnIdleTimer = false;
    }
}

void CAppEventFloaterImpl::Onx_richFixedMainText_Set(
        rich richSet)
{
    SET_IF_NEW;

    _m_ptrmapUiMainAppEventText.InvalidateTreeState();
}

void CAppEventFloaterImpl::Onx_richNoEventsPlaceholder_Set(
        rich richSet)
{
    SET_IF_NEW;

    _m_ptrmapUiMainAppEventText.InvalidateTreeState();
}

void CAppEventFloaterImpl::Onx_bExclusiveModalEventFloater_Set(
        bool value)
{
    SET_IF_NEW;

    // need to reopen?
    if (_m_ptrmapUiMainFloater.GetCountClearingNulls())
    {
        CloseEventLogFloater();
        OpenEventLogFloater();
    }
}

void CAppEventFloaterImpl::UpdateEventListAndScrollToBottom()
{
    //if not_null(ref<CUiPanelGp> rUiWindowPanel = _m_ptrmapUiAppEventFloaterClient)
    ref<CUiPanelGp> rUiWindowPanel;
    for (
        iter i;
        _m_ptrmapUiAppEventFloaterClient.
            Iterate(
                out i,
                out rUiWindowPanel);)
    {
        // Debug skip?
        if (sys::GIsAccess(...) &&
            sys::GIsAccess(...))
        {
            // skip update
        }
        else
        {
            rUiWindowPanel->UpdateContentAndLayoutNow();
            rUiWindowPanel->UpdatePaint();

            _m_ptrmapUiAppEventListEnd.ScrollToShow();

            _m_ptrmapUiMainFloater.UpdateTreeState();
            _m_ptrmapUiMainFloater.UpdateTreeLayout();

            _m_ptrmapUiMainEmbedded.UpdateTreeState();
            _m_ptrmapUiMainEmbedded.UpdateTreeLayout();

            // never reactivate floaters too often obscuring working windows
            if (sys::GGetUtc() - 
                    m_tickLastReactivation > C_tickReactivationTimeout)
            {
                _m_ptrmapUiMainFloater.MakeActiveApplicationFrame();    
                m_tickLastReactivation = sys::GGetUtc();
            }
        }
    }
}

void CAppEventFloaterImpl::Onx_bMinimizeContent_Set(bool value)
{
    this->x_bMinimizeContent = value;

    if (_m_ptrmapUiMainFloater.GetCountClearingNulls())
    {
        _m_ptrmapUiMainFloater.CloseAllFrames();
        _m_ptrmapUiMainFloater.RemoveAll();
        OpenEventLogFloater();
    }

    _m_ptrmapUiMainEmbedded.InvalidateContent();

    /*
    if not_null(ref<CUiPanelGp> rUiWindowPanel = _m_ptrmapUiMainFloater)
    {
        //rUiWindowPanel->InvalidateContent();
        // close and reopen to reset the size
        rUiWindowPanel->CloseFrame();
        _m_ptrmapUiMainFloater = null();
        OpenEventLogFloater();
    }
    */
}

void CAppEventFloaterImpl::OnShowExportedEventReport()
{
    str sLog = 
        CAppGp::GGet()->
            x_rUvGlobals->
                GetSystemReport() +
        "\n";

    str sIterKey;
    ref<CAppEventImpl> rIterEvent;
    for (
        iter iEvent;
        _m_mapAppEvent.
            IterateKeys(
                out iEvent,
                out sIterKey,
                out rIterEvent);)
    {
        int nTimes = rIterEvent->x_nEventTimesIssued;

        sLog +=
            "\n" +
                "#" + Str(rIterEvent->x_iEventIssueNumber) + 
                (nTimes != 1?
                    " x" + Str(nTimes)
                    :
                    "") +
                ": " +
                rIterEvent->
                    x_momentEventLogged.Get().
                        ToStr("[yy-][mm-][dd, ]HH:MM:SS") +
                "\n" +
                rIterEvent->x_slocFriendlyMessage + 
                "\n" +
                rIterEvent->x_sTechnicalDetails + 
                "\n";
    }

    sys::GOpenTextInNotepad(
        sLog.GetReplaced("\n", nline));
        //this->ExposeOut().ExportAsText());
}

rich CAppEventFloaterImpl::OnReportAllEvents()
{
    return this->ExposeOut().ExportAsText();
}

void CAppEventFloaterImpl::OnEnableFloaterShow(
        bool bEnable)
{
    m_bHideFloaterWindow = !bEnable;
}

rich CAppEventFloaterImpl::Onx_slocFloaterWindowCaption_Get()
{
    // Always return a valid caption
    rich richCaption = this->x_slocFloaterWindowCaption;

    if (richCaption.Len() == 0)
    {
        //FAIL("provide a caption: CAppEventFloater::x_slocFloaterWindowCaption");

        richCaption = 
            //CProject::GGetProjectFixedPrivateLogo() + " Event Log (UNNAMED)";
            CAppGp::GGet()->x_rUi->x_sAppLogoForUi;
    }

    return richCaption;
}

rich CAppEventFloaterImpl::GetMainDescriptionText()
{
    str sFriendlyMessage;
    str sTechnicalMessage;

    //if (_m_mapAppEvent.GetCount())
    if (_m_rLastAppEvent->IsAppEventDefined())
    {
        //_m_mapAppEvent.GetTailValueOr(null())
        sFriendlyMessage = _m_rLastAppEvent->x_slocFriendlyMessage;
        sTechnicalMessage = _m_rLastAppEvent->x_sTechnicalDetails;
    }

    if (x_richFixedMainText.Get().Len())
    {
        sFriendlyMessage = x_richFixedMainText;
    }
    else
    if (sTechnicalMessage == "" &&
        sFriendlyMessage == "")
    {
        sTechnicalMessage = 
            x_richNoEventsPlaceholder;
            //"No events.";
    }

    return 
        RichFontArial(
            RichSize(
                32, 
                sFriendlyMessage)) + 
            "\v\n\v\n" +
            RichSize(18, sTechnicalMessage);
}

int CAppEventFloaterImpl::OnGetSavedTotalAppEventCount()
{
    return x_nTotalAppEventCount;
}

rgb CAppEventFloaterImpl::GetFloaterBackColor()
{
    rgb rgbBack = x_rgbDominantFloaterColor;

    if (!rgbBack.IsSysRgb())
    {
        // Lighten
        rgbBack = rgbBack.Minimums(242, 242, 242);
    }

    return rgbBack;
}

//rgb CAppEventFloaterImpl::GetFloaterTextColor()
//{
//    rgb rgbBack = x_rgbDominantFloaterColor;
//
//    // Darken
//    rgbBack = rgbBack.Maximums(20, 20, 20);
//
//    return rgbBack;
//}

bool CAppEventFloaterImpl::IsLastEvent(
        ref<CAppEventImpl> rAppEvent)
{
    return
        rAppEvent == _m_rLastAppEvent;

    /*
    if (_m_mapAppEvent.GetCount() > 0)
    {
        if (_m_mapAppEvent.GetTailValueOr(null()) == rAppEvent)
            return true;
    }

    return false;
    */
}

void CAppEventFloaterImpl::OnClearLog()
{
    _m_mapAppEvent.RemoveAll();
    UpdateEventListAndScrollToBottom();
}

bool CAppEventFloaterImpl::OnIsUserStopped()
{
    tick tickWait = 
        x_tickAllowLeakMessagesCheckingIsUserStopped;
    if (tickWait > 0)
    {
        CProject::GGetIt().
            PumpMessagesWithinTimeout(
                tickWait);
    }

    return x_bUserStoppedEventProgress;
}

ref<UiContainer> CAppEventFloaterImpl::OnNewUiEventLogEmbedded()
{
    return 
        NewUiAppEventFloater(
            true); // embedded
}

ptr<CUiPanelGp> CAppEventFloaterImpl::OnGetFloaterPanel()
{
    iter i;
    ref<CUiPanelGp> rUi;
    if (_m_ptrmapUiMainFloater.Iterate(out i, out rUi))
        return rUi;
    else
        return ptr<CUiPanelGp>();
}

ref<UiContainer> CAppEventFloaterImpl::NewUiAppEventFloater(
        bool bEmbedded)
{
    ref<UiAppEventFloater> rUi;

[...]