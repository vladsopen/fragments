// CAdjusterTypeIfaceGp.h
#pragma once

class CDeviceIfaceGp;

//
// CAdjusterTypeIfaceGp -
//
//   Abstract UsoundUsb Control Parameter
//
ASSUME_IMPL_FOR_IFACE(CAdjusterTypeImpl, CAdjusterTypeIfaceGp)

class CAdjusterTypeIfaceGp : public typeobject
{
public:
    CAdjusterTypeIfaceGp();
    //NEW_LOCAL(CAdjusterTypeImpl, CAdjusterTypeIfaceGp)
    NEW_GP(Usmain, CAdjusterTypeImpl, CAdjusterTypeIfaceGp)

// Constants

// Attributes

    // Name
    sloc GetAdjusterNameForUi()
            vhook;

    // Show only UI controls for visible adjustres
    bool IsAdjusterUiVisible(
            ref<CDeviceIfaceGp> rDevice,
            int iChannel)
            vhook;

    // Filter for cluster-specific controls
    //   How to use it: when you are showing only controls for cluster N:
    //      if (rAdjuster->GetClusterNumberAdjusterSpecificToOr(N) != N)
    //          { Hide-Adjuster-Ui }
    //   (currently returns 0 for Depth1, 1 for Depth2 and iOrReturn otherwise)
    int GetClusterNumberAdjusterSpecificToOr(
            int iOrReturnForMultiplexControls)
            vhook;

// Operations

// Derived Item Enumeration

    const typelist<CAdjusterTypeIfaceGp> GetDerivedList()
            vhook;

    type<CAdjusterTypeIfaceGp> AsAdjusterForProbe()
            vhook;
    type<CAdjusterTypeIfaceGp> AsAdjusterForScale()
            vhook;
    type<CAdjusterTypeIfaceGp> AsAdjusterForGain()
            vhook;
    type<CAdjusterTypeIfaceGp> AsAdjusterForPower()
            vhook;
    type<CAdjusterTypeIfaceGp> AsAdjusterForAgc()
            vhook;

// UI

protected:

    virtual sloc OnGetAdjusterNameForUi()
            v1pure;
    virtual bool OnIsAdjusterUiVisible(
            ref<CDeviceIfaceGp> rDevice,
            int iChannel)
            v1pure;
    virtual int OnGetClusterNumberAdjusterSpecificToOr(
            int iOrReturnForMultiplexControls)
            v1pure;

    virtual const typelist<CAdjusterTypeIfaceGp> OnGetDerivedList()
            v1pure;

    virtual type<CAdjusterTypeIfaceGp> OnAsAdjusterForProbe()
            v1pure;
    virtual type<CAdjusterTypeIfaceGp> OnAsAdjusterForScale()
            v1pure;
    virtual type<CAdjusterTypeIfaceGp> OnAsAdjusterForGain()
            v1pure;
    virtual type<CAdjusterTypeIfaceGp> OnAsAdjusterForPower()
            v1pure;
    virtual type<CAdjusterTypeIfaceGp> OnAsAdjusterForAgc()
            v1pure;

private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
