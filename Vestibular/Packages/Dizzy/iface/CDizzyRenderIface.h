// CDizzyRenderIface.h
#pragma once

class CDbStepConfigIface;

//
// CDizzyRenderIface -
//
//   Renders dizzyfying images for the chekup.
//     The root object fot the dizzy service.
//
//ASSUME_IMPL_FOR_IFACE(CDizzyRenderImpl, CDizzyRenderIface)

class CDizzyRenderIface : public object
{
public:
    CDizzyRenderIface();
    NEW_LOCAL(CDizzyRenderImpl, CDizzyRenderIface)
    //NEW_GP(Balance, CDizzyRenderImpl, CDizzyRenderIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // WARNING: this object is used in a parallel process for Dizzy ParaService
    //          and in CANNOT HAVE ANY EXTERNAL POINTERS not available outside
    //          of the primaty application process.
    // parent
    //ptr<CSessionIface> _x_pSession
            //xauto(Get, Set);

// Operations

    // Rendering control
    void RenderByStepConfig(
            ref<CDbStepConfigIface> rDbStepConfig,
            bool bAllowSecondMonitor,
            bool bForceSecondMonitor)
            vhook;
    void StopRendering()
            vhook;
    void ClearPrimaryMonitor()
            vhook;

// UI

protected:
    virtual void OnRenderByStepConfig(
            ref<CDbStepConfigIface> rDbStepConfig,
            bool bAllowSecondMonitor,
            bool bForceSecondMonitor)
            v1pure;
    virtual void OnStopRendering()
            v1pure;
    virtual void OnClearPrimaryMonitor()
            v1pure;
private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToDizzyRenderIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
