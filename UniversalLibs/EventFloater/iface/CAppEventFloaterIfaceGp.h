// CAppEventFloaterIfaceGp.h
#pragma once

#if defined(__Cta__) || !defined(REAL_COMPILER)
#include "AppEventCommonLib.h"

#endif //__Cta__

class CAppEventCallbackIfaceGp;
class CAppEventIfaceGp;

//
// CAppEventFloaterIfaceGp - error logging floater window,
//   must be fed with CAppEventIfaceGp
//
ASSUME_IMPL_FOR_IFACE(CAppEventFloaterImpl, CAppEventFloaterIfaceGp);

class CAppEventFloaterIfaceGp : public object
{
    typedef /* -------------------- */ object super; // required for GCC without Cta
public:
#if defined(__Cta__) || !defined(REAL_COMPILER)

    CAppEventFloaterIfaceGp();

    virtual void OnExposeContent(
            ref<CExpose> rExpose);
    //NEW_LOCAL(CAppEventFloaterImpl, CAppEventFloaterIfaceGp)
    NEW_GP(GuiPlus, CAppEventFloaterImpl, CAppEventFloaterIfaceGp)

// Constants

// Attributes

    // Current selection
    //link<CAppEventIfaceGp> x_linkSelection
    //        xauto(Get);
    //void x_linkSelection_Set(link<CAppEventIfaceGp> value); // UI update

    // Window caption
    sloc x_slocFloaterWindowCaption = ""
            xauto(Set)
            xassert(true);
    rich x_slocFloaterWindowCaption_Get()
            vhook;

    // Optional Fixed main text
    rich x_richFixedMainText = ""
            xauto(Get)
            xassert(true);
    void x_richFixedMainText_Set(
            rich richSet)
            vhook;

    // Optional Fixed main text
    rich x_richNoEventsPlaceholder = "No events."
            xauto(Get)
            xassert(true);
    void x_richNoEventsPlaceholder_Set(
            rich richSet)
            vhook;

    // Main floater icon (large)
    res _x_resMainLargeFloaterIcon = RES_STD_WARNING
            xauto(Get, Set)
            xassert(true);

    // Close [x] option may not be available for critical errors
    bool x_bEnableEndUserCloseButton = true
            xauto(Get, Set);
    bool x_bEnableEndUserCollapseButton = true
            xauto(Get, Set);

    // For long progresses
    bool x_bExclusiveModalEventFloater = false
            xauto(Get)
            xassert(true);
    void x_bExclusiveModalEventFloater_Set(
            bool value)
            vhook;

    // Total count (list get cleaned after a while)
    int GetSavedTotalAppEventCount()
            vhook;

    // Skin
    rgb x_rgbDominantFloaterColor =
            C_rgbSysNoUiFill
            //rgb(255, 0, 0)
            xauto(Get, Set)
            xassert(true);

    // Allows app to process calls to its event logs.
    ptr<CAppEventCallbackIfaceGp> _x_pAppEventCallback
            xauto(Get, Set);

    // Minimize window content
    bool x_bMinimizeContent = false
            xauto(Get);
    void x_bMinimizeContent_Set(
            bool value)
            vhook;

// Operations

    // Constructor-safe attribute init
    static ref<CAppEventFloaterIfaceGp> _init_GNewAppEventFloater(
            sloc slocCaption,
            res resMainLargeIcon);
                // = RES_STD_WARNING: app can continue after a feature failed
                // = RES_STD_ERROR: core app failure or data loss

    // Add error data
    void LogNewEvent(
            sloc slocFriendlyMessage,
            str sTechnicalDetails)
            vhook;

    // Add error object
    void LogNewEventObject(
            ref<CAppEventIfaceGp> rAppEvent)
            vhook;

    // Export report
    void ShowExportedEventReport()
            vhook;

    // Report to text
    rich ReportAllEvents()
            vhook;

    // Hide exit errors
    void EnableFloaterShow(
            bool bEnable)
            vhook;

    // Hide exit errors
    void ClearLog()
            vhook;

    //
    // Progress (Busy) support
    //

    // For an embedded floater the app may
    //   force stop from its own [x] button
    bool x_bUserStoppedEventProgress = false
            xauto(Get, Set);

    // Does not go to the log
    void SetProgressText(
            str sText)
            vhook;

    // [||||.....]
    void SetProgressTotal(
            big nnTotal)
            vhook;
    void SetProgressDone(
            big nnDone)
            vhook;
    void SetProgressComplete()
            vhook;

    // Allows abort after confirmation
    void EnableStopPrompt(
            rich richPrompt)
            vhook;
    bool IsUserStopped()
            vhook;

    // for legacy packers
    tick x_tickAllowLeakMessagesCheckingIsUserStopped = 0
            xauto(Get, Set)
            xassert(true)
            SEMIPUBLIC(.,
                AllowPath "/Installer/Packages/Generator/",
                AllowPath "/GuiPlus/Packages/AppEventFloater/impl/",
                AllowPath "/Packages/PatientLegacy/Backup/impl/",
                );

#ifdef UV_IMPL
    void Text(str sText)
        { SetProgressText(sText); }
    void Total(big nnTotal)
        { SetProgressTotal(nnTotal); }
    void Done(big nnDone)
        { SetProgressDone(nnDone); }
    void Complete()
        { SetProgressComplete(); }
#endif

// UI

    // Show error log main window
    void OpenEventLogFloater()
            vhook;

    // Deinvalidate all
    void UpdateFloaterNow()
            vhook;

    // Alternate floater inside an app window
    ref<UiContainer> NewUiEventLogEmbedded()
            vhook;

    // Direct UI access
    ptr<CUiPanelGp> GetFloaterPanel()
            vhook;

    // Hide
    void CloseEventLogFloater()
            vhook;
    void _template_CloseScope(
            unused unusedCookie)
            vhook;

protected:
    // (see CAppEventFloaterImpl)

    virtual void Onx_richFixedMainText_Set(
            rich richSet)
            v1pure;
    virtual void Onx_richNoEventsPlaceholder_Set(
            rich richSet)
            v1pure;
    virtual void Onx_bExclusiveModalEventFloater_Set(
            bool value)
            v1pure;
    virtual int OnGetSavedTotalAppEventCount()
            v1pure;
    virtual void Onx_bMinimizeContent_Set(
            bool value)
            v1pure;
    virtual void OnLogNewEvent(
            sloc slocFriendlyMessage,
            str sTechnicalDetails)
            v1pure;
    virtual void OnLogNewEventObject(
            ref<CAppEventIfaceGp> rAppEvent)
            v1pure;
    virtual rich OnReportAllEvents()
            v1pure;
    virtual void OnShowExportedEventReport()
            v1pure;
    virtual void OnEnableFloaterShow(
            bool bEnable)
            v1pure;
    virtual rich Onx_slocFloaterWindowCaption_Get()
            v1pure;
    virtual void OnClearLog()
            v1pure;
    virtual void OnSetProgressText(
            str sText)
            v1pure;
    virtual void OnSetProgressTotal(
            big nnTotal)
            v1pure;
    virtual void OnSetProgressDone(
            big nnDone)
            v1pure;
    virtual void OnSetProgressComplete()
            v1pure;
    virtual void OnEnableStopPrompt(
            rich richPrompt)
            v1pure;
    virtual bool OnIsUserStopped()
            v1pure;
    virtual void OnOpenEventLogFloater()
            v1pure;
    virtual void OnUpdateFloaterNow()
            v1pure;
    virtual ref<UiContainer> OnNewUiEventLogEmbedded()
            v1pure;
    virtual ptr<CUiPanelGp> OnGetFloaterPanel()
            v1pure;
    virtual void OnCloseEventLogFloater()
            v1pure;
    virtual void On_template_CloseScope(
            unused unusedCookie)
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);

#else
// a stub for member ref<>s visible without Cta
public:
    CAppEventFloaterIfaceGp() {}
    CTA_STATIC_CLASS_INFO(CAppEventFloaterIfaceGp);
#endif //__Cta__
};

