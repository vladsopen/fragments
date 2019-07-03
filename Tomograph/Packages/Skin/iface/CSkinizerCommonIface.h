// CSkinizerCommonIface.h
#pragma once

#include "CSkinizerCommonIfaceGp.h"
#include "CSkinizerDividoIfaceGp.h"

//
// CSkinizerCommonIface -
//
//   CSkinizerCommonIfaceGp derivation test.
//   This class redirects CSkinizerCommonIfaceGp to its own CSkinizerCommonImpl
//     overriding OnGetSkinizerCommonImpl.
//   Actual customization is located in CSkinizerCommonImpl class.
//
ASSUME_IMPL_FOR_IFACE(CSkinizerCommonImpl, CSkinizerCommonIface)

class CSkinizerCommonIface : public CSkinizerDividoIfaceGp //CSkinizerCommonIfaceGp
{
public:
    CSkinizerCommonIface();
    NEW_LOCAL(CSkinizerCommonImpl, CSkinizerCommonIface)
    //NEW_GP(MyProject, CSkinizerCommonImpl, CSkinizerCommonIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // Main Window
    static const rgb C_rgbMainWindowBack = rgb(255, 255, 255);

    // Indicator area
    static const rgb C_rgbLargeText = rgb(240, 250, 255);
    static const rgb C_rgbBehindIndicators = rgb(250, 253, 255);
    static const rgb C_rgbBorder = rgb(10, 10, 100);
    static const rgb C_rgbTextBack = rgb(240, 245, 255);
    static const rgb C_rgbTextInfo = rgb(20, 40, 70);
    static const rgb C_rgbPointNotDownloadedText = rgb(100, 100, 120);
    static const alpha C_alphaTextBack = alpha(60);
    static const alpha C_alphaListBack = alpha(170);
    static const alpha C_alphaSeparatorBack = alpha(100);
    static const scale C_scaleInfoSectionLeftMargin = prescale(20);

    // Wizard controls
    static const rgb C_rgbPrevNextText = rgb(50, 10, 20);
    static const alpha C_alphaPrevNextBack = alpha(100);

    // Signal
    static const alpha C_alphaSignalPreviewWindow = alpha(170);

    // Options Window
    static const rgb C_rgbOptionsWindowBack = rgb(230, 225, 220);

// Attributes

// Operations

    // Debug
    static str GGetSkinReport(
            ref<CSkinizerCommonIfaceGp> rSkinizer);

// UI

protected:

    /*
    // redefined base virtuals:
    //   (this provides the correct call route for other ifaces derived from this)

    virtual int OnGetDefaultFontSize()
            vnullcontinue;

    virtual rgb OnGetDefaultTextColor()
            vnullcontinue;

    // 1st virtuals:

    virtual str OnGetSkinTestLabel()
            v1pure;
    */

private:

//    // experimental!
//
//    // This is the real implementation pointer for virtual calls.
//    // Cta recognizes "_m_gluedimpl" and generates special support 
//    //   code in new virtuals. This is meaningless without Cta!
//    // Every derived Iface should have another _m_gluedimplXxxx declared like this.
//    // **** This facility I only support for CommonSkin-derived classes now ****
//    gluedimpl<CSkinizerCommonIface> _m_gluedimplCommon;

    // WARNING: No member data is allowed with gluedimpl!

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
