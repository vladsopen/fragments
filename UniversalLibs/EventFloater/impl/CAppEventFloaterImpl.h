// CAppEventFloaterImpl.h
#pragma once

#include "CAppEventFloaterIfaceGp.h"

class CAppEventImpl;
class CAppEventFloaterTimer;

//
// CAppEventFloaterImpl - error logging
//

class CAppEventFloaterImpl : public CAppEventFloaterIfaceGp
{
public:
    CAppEventFloaterImpl();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Current selection
    //link<CAppEventImpl> x_linkSelection
    //        xauto(Get);
    //void x_linkSelection_Set(link<CAppEventImpl> value); // UI update

    // Total count (list get cleaned after a while)
    int x_nTotalAppEventCount = 0
            xauto(Get)
            xassert(true);

    big x_nnProgressTotal = 0
            xauto(Get)
            xassert(value >= 0);
    big x_nnProgressDone = 0
            xauto(Get)
            xassert(value >= 0);

    // [x] control (legacy?)
    rich x_richStopPrompt = ""
            xauto(Get, Set)
            xassert(true);
    //bool x_bUserStoppedEventProgress = false
    //        xauto(Get, Set);

// array<> Publishing

    // void SetCount(int nNewSize)
    //     { return m_aAppEvent.SetCount(nNewSize); }
    // ref<CAppEventImpl> GetAt(int iAt)
    //     { return m_aAppEvent.GetAt(iAt); }
    // ref<CAppEventImpl> GetLast(int nCountMinus = -1)
    //     { return m_aAppEvent.GetLast(nCountMinus); }
    // void SetAt(int iAt, ref<CAppEventImpl> value)
    //     { m_aAppEvent.SetAt(iAt, value); }
    // void InsertAt(int iAt, ref<CAppEventImpl> value)
    //     { m_aAppEvent.InsertAt(iAt, value); }
    // void RemoveAt(int iAt, int nCount = 1)
    //     { m_aAppEvent.RemoveAt(iAt, nCount); }
    // void RemoveAll()
    //     { m_aAppEvent.RemoveAll(); }
    int GetListedEventCount();
    bool IterateEvents(out iter& out_i, out ref<CAppEventImpl>& out_value);
    // bool IterateBackwards(out iter& out_i, out ref<CAppEventImpl>& out_value)
    //     { return m_aAppEvent.IterateBackwards(out_i, out_value); }

    bool IsLastEvent(
            ref<CAppEventImpl> rAppEvent);

// Operations

    // Timer handler
    void HandleTimerUpdate();

// UI

    // Instant update
    void UpdateEventListAndScrollToBottom();

    // Last error text or something else
    rich GetMainDescriptionText();

    // [x]
    bool HandleAllowStop();

    ref<UiContainer> NewUiAppEventFloater(
            bool bEmbedded);
    ptrmap<CUiPanelGp> _m_ptrmapUiMainFloater;
    ptrmap<CUiPanelGp> _m_ptrmapUiMainEmbedded;
    ptrmap<CUiPanelGp> _m_ptrmapUiAppEventFloaterClient;
    ptrmap<CUiActionGp> _m_ptrmapUiAppEventFloaterCaptionText;
    ptrmap<CUiPanelGp> _m_ptrmapUiAppEventList;
    ptrmap<CUiPanelGp> _m_ptrmapUiAppEventListEnd;
    ptrmap<CUiActionGp> _m_ptrmapUiMainAppEventText;
    ptrmap<CUiProgressGp> _m_ptrmapUiMainAppEventProgress;

    rgb GetFloaterBackColor();
    //rgb GetFloaterTextColor();

protected:
    // (see CAppEventFloaterIfaceGp)

    virtual void Onx_richFixedMainText_Set(
            rich richSet)
            ;
    virtual void Onx_richNoEventsPlaceholder_Set(
            rich richSet)
            ;
    virtual void Onx_bExclusiveModalEventFloater_Set(
            bool value)
            ;
    virtual int OnGetSavedTotalAppEventCount()
            ;
    virtual void Onx_bMinimizeContent_Set(
            bool value)
            ;
    virtual void OnLogNewEventObject(
            ref<CAppEventIfaceGp> rAppEvent)
            ;
    virtual void OnLogNewEvent(
            sloc slocFriendlyMessage,
            str sTechnicalDetails)
            ;
    virtual void OnShowExportedEventReport()
            ;
    virtual rich OnReportAllEvents()
            ;
    virtual void OnEnableFloaterShow(
            bool bEnable)
            ;
    virtual rich Onx_slocFloaterWindowCaption_Get()
            ;
    virtual void OnClearLog()
            ;
    virtual void OnSetProgressText(
            str sText)
            ;
    virtual void OnSetProgressTotal(
            big nnTotal)
            ;
    virtual void OnSetProgressDone(
            big nnDone)
            ;
    virtual void OnSetProgressComplete()
            ;
    virtual void OnEnableStopPrompt(
            rich richPrompt)
            ;
    virtual bool OnIsUserStopped()
            ;
    virtual void OnOpenEventLogFloater()
            ;
    virtual void OnUpdateFloaterNow()
            ;
    virtual ref<UiContainer> OnNewUiEventLogEmbedded()
            ;
    virtual ptr<CUiPanelGp> OnGetFloaterPanel()
            ;
    virtual void OnCloseEventLogFloater()
            ;
    virtual void On_template_CloseScope(
            unused unusedCookie)
            ;

private:
    //array< ref<CAppEventImpl> > m_aAppEvent;
    map<str, ref<CAppEventImpl> > _m_mapAppEvent;
    // fix: map.Tail() won't work as we skip duplicates
    ref<CAppEventImpl> _m_rLastAppEvent;

    bool m_bHideFloaterWindow = false;

    // DoS prevention
    // (// errors are removed from the middle of the list)
    static const int C_nMaxLogEntries = 
            30;
            //100; 
    static const tick C_tickUpdateTimeout = 
            // flood prevention should have made it faster on long lists
            1000;
            //3000;
    static const tick C_tickReactivationTimeout = 5000;

    tick m_tickLastUpdate = 0;
    tick m_tickLastReactivation = 0;

    int m_nSavedToUlog = 0;

    // hook flag to avoid infinite recursion
    bool m_bInsideImplementLogNewEventObject = false;

    // Lazy update
    ref<CAppEventFloaterTimer> m_rAppEventFloaterTimer;
    bool m_bUpdateOnIdleTimer = false;

    void UpdateUiAfterNewLogEvent();
    void EnsureVisibleIfNotHidden();

    void KillExtra();

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
};

// (UiAppEventFloater.cpp)
class UiAppEventFloater : public UiContainer
{
public:
    virtual void Ui();

    ref<CAppEventFloaterImpl> m_rAppEventFloater;
    bool m_bEmbedded = false;
};
