// CDrawAnythingRouted.h
#pragma once

#include "CDrawAnythingRecorded.h"

class CRectIface;
class CRangesIface;
class CRecordTypeIface;
class CEllipse;
class CRangesIface;
class CRulersForRoutes;

//
// CDrawAnythingRouted -
//
//   Abstract extension for graphs related to the trajectory diagram.
//
//ASSUME_IMPL_FOR_IFACE(CDrawAnythingRoutedImpl, CDrawAnythingRouted)

class CDrawAnythingRouted : public CDrawAnythingRecorded
{
public:
    CDrawAnythingRouted();
    //NEW_LOCAL(CDrawAnythingRoutedImpl, CDrawAnythingRouted)
    //NEW_GP(Balance, CDrawAnythingRoutedImpl, CDrawAnythingRouted)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Rulers
    ref<CRulersForRoutes> x_rRulersForRoutes
            xauto(Get);

    // Draws calculated ellipse
    ref<CEllipse> x_rEllipse
            xauto(Get);

    // Optional scaling
    ref<CRectIface> GetRouteGraphVisibleRect()
            vhook;

    // Offset?
    bool IsCoordinateOffsetDrawnInGrid()
            vhook;

    // Grid/ruler step (larger one)
    num GetRouteGridStepMm()
            vhook;

    // Proportional UI size
    size x_sizeLastSquareBack = size()
            xauto(Get, Set)
            xassert(true);
    point x_pointLastSquareBack = point()
            xauto(Get, Set)
            xassert(true);

// Operations

    // Coord conversions
    int ConvertMmToPixel(
            num numPos,
            num numRange,
            num numAddedToRange,
            int nSizeUi,
            ref<CRectIface> rRect);
    int ConvertMmToPixelX(
            num numPosX,
            ref<CRangesIface> rRanges,
            size sizeUi,
            ref<CRectIface> rRect);
    int ConvertMmToPixelY(
            num numPosY,
            ref<CRangesIface> rRanges,
            size sizeUi,
            ref<CRectIface> rRect);

// UI

    // Ensures square
    void HandleSquareAreaOnModifyShape(
            ref<CUiPanelGp> rUi,
            num numReserveForFeet);

    // Draws an ellipse for a record type
    void DrawCalculatedEllipses(
            draw dc,
            type<CRecordTypeIface> typeRecordType,
            ref<CRangesIface> rRanges,
            size sizeUi,
            ref<CRectIface> rRect);

    // Unified grid
    void DrawRouteGrid(
            draw dc,
            ref<CUiPanelGp> rUi);

protected:

    virtual void OnHandleRecordingChange()
            vcontinue;

    virtual void OnUpdateGraphNow()
            vcontinue;

    virtual ref<CRectIface> OnGetRouteGraphVisibleRect()
            v1st
            return
                ref<CRectIface>();

    virtual bool OnIsCoordinateOffsetDrawnInGrid()
            v1pure;

    virtual num OnGetRouteGridStepMm()
            v1pure;

private:
    //bool _m_bOneTimeInitOk = false;

    ref<CRangesIface> m_rLastRanges;
    ref<CRectIface> m_rLastVisibleRect;

    // Determines if scale is changed and rulers need be reset
    void CheckRulerUpdate();

    void _init_AttachToDrawAnythingRouted();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
