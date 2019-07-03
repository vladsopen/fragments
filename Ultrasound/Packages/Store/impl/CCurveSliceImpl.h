// CCurveSliceImpl.h
#pragma once

#include "CCurveSliceIface.h"

//
// CCurveSliceImpl - see iface
//

class CCurveSliceImpl : public CCurveSliceIface
{
public:
    CCurveSliceImpl();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);
    bool MirrorDbCurveSliceAsBlob(
            ref<CUdbMirrorIfaceGp> rUdbMirror,
            out sbuf& out_sbufBlob);

// Constants

    // We store amplitudes in fixed point extended scale
    //static const int C_nScaleCurvePointForStorage = 16384; // up to 16-bit
        // see C_nCurveVirtualHeightRange

// Attributes

    // Optimized curve count
    static int GGetMaxCurveCount();

// Operations

// UI

protected:
    virtual void OnSetCurvePointAmplitudeAt(
            int iCurve,
            int nAmplitude,
            int nRange)
            ;
    virtual int OnGetCurvePointAmplitudeAt(
            int iCurve,
            int nRange)
            ;

private:

    // compact curve values in CCurveTypeIface order 
    //   (all optional, count = 0...oo)
    array<short> _m_ashortCurveSlice;

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
