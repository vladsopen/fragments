// CTotalTestImpl.h
#pragma once

#include "CTotalTestIface.h"

class CPatientBaseConnectionIfaceGp;
class CMatrixFormatGp;
class CGlobalPaletteConfigIface;
class CSessionImpl;

//
// CTotalTestImpl - auto test for the whole app
//
//ASSUME_IMPL_FOR_IFACE(CTotalTestImplImpl, CTotalTestImpl)

class CTotalTestImpl : public CTotalTestIface
{
public:
    CTotalTestImpl();
    //NEW_LOCAL(CTotalTestImplImpl, CTotalTestImpl)
    //NEW_GP(MyProject, CTotalTestImplImpl, CTotalTestImpl)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    //bool x_bTestDeviceOffline = false
            //xauto(Get, Set);

    ref<CPatientBaseConnectionIfaceGp> x_rPatientBaseConnection
            xauto(Get);

// Operations

    void DelayTest(
            tick tickSleep);

// UI

protected:
private:

    ref<CMatrixFormatGp> m_rMatrixFormat;

    void InitTestPalette(
            ref<CGlobalPaletteConfigIface> rGlobalPaletteConfig);

    void _test_RunTotalTestVariation();
    void _test_RunTotalTestVariationForSession(
            ref<CSessionImpl> rSession);

    void TestLoadFromPatient(
            path pathFrom);

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    virtual void OnTestClass();
};
