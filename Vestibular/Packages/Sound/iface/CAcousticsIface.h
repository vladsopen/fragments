// CAcousticsIface.h
#pragma once

class CRangesIface;
class CReadingIface;
class CSessionIface;

//
// CAcousticsIface -
//
//   Sound services.
//
ASSUME_IMPL_FOR_IFACE(CAcousticsImpl, CAcousticsIface)

class CAcousticsIface : public object
{
public:
    CAcousticsIface();
    NEW_LOCAL(CAcousticsImpl, CAcousticsIface)
    //NEW_GP(Balance, CAcousticsImpl, CAcousticsIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CSessionIface> _x_pSession
            xauto(Get, Set);

// Operations

    // Online data receiving
    void ReceiveReadingForAcoustics(
            ref<CReadingIface> rReading,
            ref<CRangesIface> rRanges)
            vhook;

    // Offset sound
    void PlayPointerPlayOffset(
            num numOffset)
            vhook;

// UI

protected:
    virtual void OnReceiveReadingForAcoustics(
            ref<CReadingIface> rReading,
            ref<CRangesIface> rRanges)
            v1pure;
    virtual void OnPlayPointerPlayOffset(
            num numOffset)
            v1pure;
private:
    //bool _m_bOneTimeInitAcousticsIfaceOk = false;

    void _init_AttachToAcousticsIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
