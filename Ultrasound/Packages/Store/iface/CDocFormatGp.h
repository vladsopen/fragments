// CDocFormatGp.h
#pragma once

class CRawSlotGp;
class CMatrixFormatGp;

//
// CDocFormatGp - 
//
//   All variable audio-media store-view parameters.
//   Most of them only used to init a new CDoc object 
//     and cannot be changed afterwards.
//
//ASSUME_IMPL_FOR_IFACE(CDocFormatGpImpl, CDocFormatGp)

class CDocFormatGp : public object
{
public:
    CDocFormatGp();
    //NEW_LOCAL(CDocFormatGpImpl, CDocFormatGp)
    //NEW_GP(MyProject, CDocFormatGpImpl, CDocFormatGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // Default if x_nDefaultViewWidthSlices is not set
    static const int C_nDefaultDefaultViewWidthSlices = 500;

// Attributes

    // Device RawSlot mapping to Doc
    ref<CRawSlotGp> x_rRawSlot
            xauto(Get);

    // FFT metrics for storage init
    ref<CMatrixFormatGp> x_rMatrixFormat
            xauto(Get);

    // Initial view width 
    //   see CViewState::SetViewWidthSlices()
    int x_nDefaultViewWidthSlices = C_nDefaultDefaultViewWidthSlices
            xauto(Get, Set)
            xassert(value > 0);

// Operations

// UI

protected:
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
