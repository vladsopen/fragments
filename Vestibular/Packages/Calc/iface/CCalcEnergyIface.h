// CCalcEnergyIface.h
#pragma once

class CCalcupIface;

//
// CCalcEnergyIface -
//
//   Calculates energy graph and the final value.
//
ASSUME_IMPL_FOR_IFACE(CCalcEnergyImpl, CCalcEnergyIface)

class CCalcEnergyIface : public object
{
public:
    CCalcEnergyIface();
    NEW_LOCAL(CCalcEnergyImpl, CCalcEnergyIface)
    //NEW_GP(Balance, CCalcEnergyImpl, CCalcEnergyIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    //
    // Input
    //

    // Cylinder radius.
    num x_numPatientClinicalBaseMm = 0
            xauto(Get, Set)
            xassert(value >= 0);

    //
    // Intermediate Result
    //

    // Instant energy change
    num x_numNextWorkEnergyIndex = 0
            xauto(Get, Set)
            xassert(value >= 0);

    //
    // Final Result
    //

    // The sum
    num x_numWorkEnergyIndexSum = 0
            xauto(Get, Set)
            xassert(value >= 0);

// Operations

    // Start
    void InitCalcEnergy()
            vhook;

    // Next
    void CalcNextEnergyPoint(
            num numXmm,
            num numYmm,
            num numKilo,
            num numDeltaMm,
            ref<CCalcupIface> rCalcup)
            vhook;

    // Finish
    void FinalizeResults()
            vhook;

// UI

protected:
    virtual void OnInitCalcEnergy()
            v1pure;
    virtual void OnCalcNextEnergyPoint(
            num numXmm,
            num numYmm,
            num numKilo,
            num numDeltaMm,
            ref<CCalcupIface> rCalcup)
            v1pure;
    virtual void OnFinalizeResults()
            v1pure;
private:
    //bool _m_bOneTimeInitCalcEnergyIfaceOk = false;

    void _init_AttachToCalcEnergyIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
