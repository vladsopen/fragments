// CDbStepConfigIface.h
#pragma once

class CDemandHeadTypeIface;
class CDemandFeetTypeIface;
class CDemandEyesTypeIface;
class CDemandPostureTypeIface;
class CDizzyPaintTypeIface;

//
// CDbStepConfigIface - 
//
//   Parameters defining a single checkup stage.
//
//ASSUME_IMPL_FOR_IFACE(CDbStepConfigImpl, CDbStepConfigIface)

class CDbStepConfigIface : public dbobject
{
public:
    CDbStepConfigIface();
    //NEW_LOCAL(CDbStepConfigImpl, CDbStepConfigIface)
    //NEW_GP(Balance, CDbStepConfigImpl, CDbStepConfigIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Use?
    bool x_bStepInUse = false // important default for null() step configs
            xauto(Get, Set);

    // Stage length
    num x_numStepSeconds = 0
            xauto(Get, Set)
            xminmax(0, oo);

    // Auto/manual stage initiation
    bool x_bAutoStartStep = true
            xauto(Get, Set);

    // Auto/manual stage initiation
    num x_numPauseSecondsBeforeStep = 1
            xauto(Set)
            xminmax(0, oo);
    num x_numPauseSecondsBeforeStep_Get()
            return
                math::GMax(0., this->x_numPauseSecondsBeforeStep);

    // Postural directions
    type<CDemandPostureTypeIface> x_typeDemandPostureType
            xauto(Get, Set);

    // Head direction
    type<CDemandHeadTypeIface> x_typeDemandHeadType
            xauto(Get, Set);

    // Feet position directions
    type<CDemandFeetTypeIface> x_typeDemandFeetType
            xauto(Get, Set);

    // Eyse state directions
    type<CDemandEyesTypeIface> x_typeDemandEyesType
            xauto(Get, Set);

    // What to paint
    type<CDizzyPaintTypeIface> x_typeDizzyPaintType
            xauto(Get, Set);

    // Refresh period = how much time it takes a stripe to cover the screen
    num x_numStripeSpeedInSeconds = 1
            xauto(Get, Set)
            xminmax(0, oo);

    // Stripes per screen
    num x_numStripeCount = 1
            xauto(Get, Set)
            xminmax(0, oo);

// Operations

// UI

protected:
private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToDbStepConfigIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
