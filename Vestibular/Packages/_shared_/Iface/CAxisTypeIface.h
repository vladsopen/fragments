// CAxisTypeIface.h
#pragma once

#include "CAxisTypeIfaceGp.h"

//
// CAxisTypeIface -
//
//   X or Y
//

class CAxisTypeIface : public CAxisTypeIfaceGp
{
public:
    CAxisTypeIface();
    NEW_LOCAL(CAxisTypeImpl, CAxisTypeIface)
    //NEW_GP(MyProject, CAxisTypeImpl, CAxisTypeIface)

// Constants

// Attributes

    // Added to Patients indexes dependent on axis (or empty)
    str GetPatientIndexKeyAxis()
            vhook;

    // Added to Patients index name dependent on axis (or empty)
    sloc GetPatientIndexShortNameAxis()
            vhook;

    // Added to Patients index tooltip dependent on axis (or empty)
    sloc GetPatientIndexLongNameAxis()
            vhook;

// Operations

// Derived Item Enumeration

    const typelist<CAxisTypeIface> GetDerivedListIface()
            vhook;

// UI

protected:

    virtual str OnGetPatientIndexKeyAxis()
            v1st
            return
                "";

    virtual sloc OnGetPatientIndexShortNameAxis()
            v1st
            return
                "";

    virtual sloc OnGetPatientIndexLongNameAxis()
            v1st
            return
                "";

    virtual const typelist<CAxisTypeIface> OnGetDerivedListIface()
            v1pure;

private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
