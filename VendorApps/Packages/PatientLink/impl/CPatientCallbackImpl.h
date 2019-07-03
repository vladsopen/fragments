// CPatientCallbackImpl.h
#pragma once

#include "CPatientPatientCallbackIfaceGp.h"

class CPatientLinkImpl;
class CLoaderIface;

//
// CPatientCallbackImpl - 
//
//  Implements Patient callback maessage handling for this app.
//

class CPatientCallbackImpl : public CPatientPatientCallbackIfaceGp
{
public:
    CPatientCallbackImpl();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CPatientLinkImpl> _x_pPatientLink
            xauto(Get, Set);

// Operations

// UI

protected:
    virtual void OnActivateSamplerApp()
            ;
    virtual void OnHandlePatientExit()
            ;
    virtual void OnOpenSample(
            ref<CPatientSampleIfaceGp> rPatientSample)
            ;
    virtual void OnPaintSample(
            ref<CPatientSampleIfaceGp> rPatientSample,
            ref<CPatientPaintContextIfaceGp> rPatientPaintContext)
            ;
    virtual void OnHandleNewDbSelectionReport()
            ;
    virtual void OnHandlePatientSearchResult(
            str sUniqueSearchOperationId,
            int nMatchingPatientsFound,
            bool bPatientSelectionChanged,
            str sSearchError)
            ;
    virtual ref<UiContainer> OnNewUiCurrentCheckupContent()
            ;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
