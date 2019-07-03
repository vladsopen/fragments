// CConstIface.h
#pragma once

//
// CConstIface - 
//
//   Shared constants local to Ralarm project.
//
//ASSUME_IMPL_FOR_IFACE(CConstImpl, CConstIface)

class CConstIface : public object
{
public:
    CConstIface();
    //NEW_LOCAL(CConstImpl, CConstIface)
    //NEW_GP(Ralarm, CConstImpl, CConstIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    //
    // Here we put global project constants which don't belong to any local class
    //   and some enums used by multiple classes must also be declared separately
    //

    // Comment ...
    //enum ESharedXxxxx { ... }

    // _|__|__|__|_
    static const rgb C_rgbFakeEcg = rgb(0, 0, 255);

    // Comment ...
    //#define C_szXxxx "xxxx"

// Attributes

    // Extra debug info
    static bool GIsTestPublic();

// Operations

// UI

    // Inversed captions
    static void GEqualizeCabinetTerminalCaption(
            ref<CUiActionGp> rUi);
    static rgb GGetCabinetTerminalCaptionBackColor(
            ref<CUiPanelGp> rUi);

protected:
private:
    //bool _m_bOneTimeInitConstIfaceOk = false;

    void _init_AttachToConstIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
