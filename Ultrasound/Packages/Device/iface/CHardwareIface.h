// CHardwareIface.h
#pragma once

#include "CHardwareIfaceGp.h"

class CDocIface;
class CDetailTypeIface;
class CAudioSliceGp;
class CSendClusterIfaceGp;
class CGeneratorListIface;
class CRawSlotGp;
class CSessionIface;

//
// CHardwareIface -
//
//   Global (in a Session) controller for all devices, real and emulated.
//

class CHardwareIface : public CHardwareIfaceGp
{
public:
    CHardwareIface();
    NEW_LOCAL(CHardwareImpl, CHardwareIface)
    //NEW_GP(MyProject, CHardwareImpl, CHardwareIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Backref to parent
    ptr<CSessionIface> _x_pParentSession
            xauto(Get, Set);

    // Signal generators for all devices
    ref<CGeneratorListIface> x_rGeneratorList
            xauto(Get);

    // Doc to send playback input to
    ptr<CDocIface> _x_pSinglePlaybackDoc
            xauto(Get, Set);

    // For restoring the customization object
    void SetCustomizeHardware(
            ref<CCustomizeHardwareIfaceGp> rCustomizeHardware)
            vhook;

    // ClusterSound selection
    ref<CRawSlotGp> x_rAudibleRawSlot
            xauto(Get);

// Operations

    // Instantiates a stub for player-only session mode.
    ref<CHardwareIface> NewHardwareAsPlayerOnly()
            vhook;

    // Must be called before anything else
    void InitHardwareForNewSession()
            vhook;

    // By default Uscan relies on the client idle
    void InitHardwareForStandaloneUscanMode()
            vhook;

    // Must be called before destruction
    void CloseHardwareAtSessionEnd()
            vhook;

    // Update Doc pointers in all devices
    void HandleDeviceUpdatesAfterDocListOrRawSlotsChange()
            vhook;

    // Idle handler
    void HandleIdleIsOnlineMode(
            out CProject::EIdleNext& out_eIdleNextMayBeUpgraded)
            vhook;

// UI

    // Device controls
    ref<UiContainer> NewUiHardwareBar(
            type<CDetailTypeIface> typeDetailType)
            vhook;

    // Debug device states
    ref<UiContainer> OpenUiHardwareBarFloater()
            vhook;

protected:
    virtual void OnSetCustomizeHardware(
            ref<CCustomizeHardwareIfaceGp> rCustomizeHardware)
            v1pure;
    virtual ref<CHardwareIface> OnNewHardwareAsPlayerOnly()
            v1pure;
    virtual void OnInitHardwareForNewSession()
            v1pure;
    virtual void OnInitHardwareForStandaloneUscanMode()
            v1pure;
    virtual void OnCloseHardwareAtSessionEnd()
            v1pure;
    virtual void OnHandleDeviceUpdatesAfterDocListOrRawSlotsChange()
            v1pure;
    virtual void OnHandleIdleIsOnlineMode(
            out CProject::EIdleNext& out_eIdleNextMayBeUpgraded)
            v1pure;
    virtual ref<UiContainer> OnOpenUiHardwareBarFloater()
            v1pure;
    virtual ref<UiContainer> OnNewUiHardwareBar(
            type<CDetailTypeIface> typeDetailType)
            v1pure;

private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
