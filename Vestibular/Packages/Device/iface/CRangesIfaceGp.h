// CRangesIfaceGp.h
#pragma once

//
// CRangesIfaceGp -
//
//   Balance coordinate range info for external receivers
//
ASSUME_IMPL_FOR_IFACE(CRangesIface, CRangesIfaceGp)
ASSUME_IMPL_FOR_IFACE(CRangesImpl, CRangesIface)

class CRangesIfaceGp : public object
{
public:
    CRangesIfaceGp();
    //NEW_LOCAL(CRangesImpl, CRangesIfaceGp)
    NEW_GP(Balance, CRangesImpl, CRangesIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Ranges in mm
    num GetMinXmm()
            vhook;
    num GetMaxXmm()
            vhook;
    num GetMinYmm()
            vhook;
    num GetMaxYmm()
            vhook;

// Operations

// UI

protected:
    virtual num OnGetMinXmm()
            v1pure;
    virtual num OnGetMaxXmm()
            v1pure;
    virtual num OnGetMinYmm()
            v1pure;
    virtual num OnGetMaxYmm()
            v1pure;
private:
    //bool _m_bOneTimeInitRangesIfaceGpOk = false;

    void _init_AttachToRangesIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
