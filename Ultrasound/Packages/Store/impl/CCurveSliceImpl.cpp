// CCurveSliceImpl.cpp

#include "CUsmainProject.h"
#include "CCurveSliceImpl.h"
#include "CCurveTypeIface.h"
#include "CUdbMirrorIfaceGp.h"

CCurveSliceImpl::CCurveSliceImpl()
{
}

void CCurveSliceImpl::OnSetCurvePointAmplitudeAt(
        int iCurve,
        int nAmplitude,
        int nRange)
{
    int nMaxCurveCount = GGetMaxCurveCount();
    if failed(iCurve < nMaxCurveCount)
        return;

    // grow array as needed
    if (iCurve >= _m_ashortCurveSlice.GetCount())
    {
        _m_ashortCurveSlice.SetCount(iCurve + 1);
    }

    // range
    rASSERT(nAmplitude >= 0);
    rASSERT(nAmplitude < nRange);
    rASSERT(nRange <= C_nCurveVirtualHeightRange);

    // scale to storage metrics (avoiding slow MulDiv but in a safe way!)
    int nAmplitudeScaled = nAmplitude * C_nCurveVirtualHeightRange;
    nAmplitudeScaled = 
        math::GDivNot0(
            nAmplitudeScaled, 
            nRange, 
            or_return 0);

    // save amplitude
    ASSERT((int) short(nAmplitudeScaled) == nAmplitudeScaled);
    _m_ashortCurveSlice.SetAt(iCurve, (short) nAmplitudeScaled);
}

int CCurveSliceImpl::OnGetCurvePointAmplitudeAt(
        int iCurve,
        int nRange)
{
    int nMaxCurveCount = GGetMaxCurveCount();
    rASSERT(iCurve < nMaxCurveCount);

    // do we have this curve value deined?
    if (iCurve >= _m_ashortCurveSlice.GetCount())
    {
        // undefined! and this is NOT an error.
        return 0;
    }

    // read scaled amplitude 
    int nAmplitudeScaled = _m_ashortCurveSlice.GetAt(iCurve);
    ASSERT(nAmplitudeScaled <= C_nCurveVirtualHeightRange);

    // range 
    rASSERT(nRange <= C_nCurveVirtualHeightRange);

    // adjust to required scale (avoiding slow MulDiv but in a safe way!)
    int nAmplitude = nAmplitudeScaled * nRange;
    nAmplitude /= C_nCurveVirtualHeightRange;

    return nAmplitude;
}

