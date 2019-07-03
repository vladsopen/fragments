// CCalcFreqIface.h
#pragma once

class CFreqFft;

//
// CCalcFreqIface -
//
//   FFT calculations.
//
ASSUME_IMPL_FOR_IFACE(CCalcFreqImpl, CCalcFreqIface)

class CCalcFreqIface : public object
{
public:
    CCalcFreqIface();
    NEW_LOCAL(CCalcFreqImpl, CCalcFreqIface)
    //NEW_GP(Balance, CCalcFreqImpl, CCalcFreqIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Freq found
    num x_numMaxFreq = 0
            xauto(Get, Set)
            xassert(true);

    // Freq found
    num x_numMaxAmplitude = 0
            xauto(Get, Set)
            xassert(true);

    // For constant level removal
    num x_numAverageAmplitude = 0
            xauto(Get, Set)
            xassert(true);

    // 60% power
    num x_numFreq60 = 0
            xauto(Get, Set)
            xassert(true);

    // For the graph
    ref<CFreqFft> GetLastFreqFft()
            vhook;

// Operations

// UI

protected:
    virtual ref<CFreqFft> OnGetLastFreqFft()
            v1pure;
private:
    //bool _m_bOneTimeInitCalcFreqIfaceOk = false;

    void _init_AttachToCalcFreqIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
