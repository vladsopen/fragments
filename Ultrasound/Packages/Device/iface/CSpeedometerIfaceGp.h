// CSpeedometerIfaceGp.h
#pragma once

class CAudioSliceGp;

//
// CSpeedometerIfaceGp -
//
//   Measures audio processing speed
//
//ASSUME_IMPL_FOR_IFACE(CSpeedometerIface, CSpeedometerIfaceGp)
//ASSUME_IMPL_FOR_IFACE(CSpeedometerImpl, CSpeedometerIface)

class CSpeedometerIfaceGp : public object
{
public:
    CSpeedometerIfaceGp();
    //~CSpeedometerIfaceGp();
    //NEW_LOCAL(CSpeedometerImpl, CSpeedometerIfaceGp)
    NEW_GP(Usmain, CSpeedometerImpl, CSpeedometerIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);
    //ref<> Get ()
    //        return
    //            _x_p .Get().Valid();

    // "1.5x"
    num GetRoundedSpeedRatio()
            vhook;
    str GetFriendlySpeedRatio()
            vhook;

    // Multiline!
    str GetLongSpeedReport()
            vhook;

// Operations

    // One-time opener
    //void InitSpeedometer();

    // Cleanup
    //void CloseSpeedometer();

// UI

protected:
    SEE_ALSO(CSpeedometerImpl)

    virtual num OnGetRoundedSpeedRatio()
            v1pure;
    virtual str OnGetFriendlySpeedRatio()
            v1pure;
    virtual str OnGetLongSpeedReport()
            v1pure;

private:
    //bool _m_bInited = false;
    //bool _m_bOpened = false;

    void _init_AttachToSpeedometerIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
