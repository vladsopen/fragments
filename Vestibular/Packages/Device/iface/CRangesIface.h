// CRangesIface.h
#pragma once

#include "CRangesIfaceGp.h"

class CDbDocIface;

//
// CRangesIface - see ifacegp
//

class CRangesIface : public CRangesIfaceGp
{
public:
    CRangesIface();
    NEW_LOCAL(CRangesImpl, CRangesIface)
    //NEW_GP(Balance, CRangesImpl, CRangesIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);
    bool MirrorDbDocRanges(
            ref<CUdbMirrorIfaceGp> rUdbMirror,
            ref<CDbDocIface> rDbDoc)
            vhook;

// Constants

    static const int C_nDefaultRangeXmm = 420 / 2;
    static const int C_nDefaultRangeYmm = 150 / 2;
    static const int C_nFeetOffsetFromBottomMm = 20;

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

    // Position ranges
    num x_numPhysicalMinXmm = -C_nDefaultRangeXmm
            xauto(Get, Set)
            xassert(true);
    num x_numMinXmm_Get()
            vhook;

    num x_numPhysicalMaxXmm = C_nDefaultRangeXmm
            xauto(Get, Set)
            xassert(true);
    num x_numMaxXmm_Get()
            vhook;

    num x_numPhysicalMinYmm = -C_nDefaultRangeYmm
            xauto(Get, Set)
            xassert(true);
    num x_numMinYmm_Get()
            vhook;

    num x_numPhysicalMaxYmm = C_nDefaultRangeYmm
            xauto(Get, Set)
            xassert(true);
    num x_numMaxYmm_Get()
            vhook;

    // Offset
    num x_numAddedToXmm = 0
            xauto(Get, Set)
            xassert(true);
    num x_numAddedToYmm = 0
            xauto(Get, Set)
            xassert(true);

    // Comparer
    bool IsRangesEqual(
            ref<CRangesIface> rMatch)
            vhook;

// UI

protected:
    virtual bool OnMirrorDbDocRanges(
            ref<CUdbMirrorIfaceGp> rUdbMirror,
            ref<CDbDocIface> rDbDoc)
            v1pure;
    virtual num Onx_numMinXmm_Get()
            v1pure;
    virtual num Onx_numMaxXmm_Get()
            v1pure;
    virtual num Onx_numMinYmm_Get()
            v1pure;
    virtual num Onx_numMaxYmm_Get()
            v1pure;
    virtual bool OnIsRangesEqual(
            ref<CRangesIface> rMatch)
            v1pure;
private:
    //bool _m_bOneTimeInitRangesIfaceOk = false;

    void _init_AttachToRangesIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
