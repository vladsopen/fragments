// CCurveSliceIface.h
#pragma once

//
// CCurveSliceIface -
//
//   One point value for every active curve.
//   Compact optimized 16-bit array contains 0..oo points for all curves
//     in the sequence of CCurveTypeIface.
//   All points are optional - curves at the end of array can be switched off.
//
ASSUME_IMPL_FOR_IFACE(CCurveSliceImpl, CCurveSliceIface)

class CCurveSliceIface : public object
{
public:
    CCurveSliceIface();
    NEW_LOCAL(CCurveSliceImpl, CCurveSliceIface)
    //NEW_GP(MyProject, CCurveSliceImpl, CCurveSliceIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

// Operations

    // Change a point for a curve
    void SetCurvePointAmplitudeAt(
            int iCurve,
            int nAmplitude,
            int nRange)
            vhook;

    // Read a point
    int GetCurvePointAmplitudeAt(
            int iCurve,
            int nRange)
            vhook;

// UI

protected:
    virtual void OnSetCurvePointAmplitudeAt(
            int iCurve,
            int nAmplitude,
            int nRange)
            v1pure;
    virtual int OnGetCurvePointAmplitudeAt(
            int iCurve,
            int nRange)
            v1pure;

private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
