// CDbFilter.h
#pragma once

class CFilterComparator;

//
// CDbFilter - 
//
//   Universal Node filter for Ubro
//
//ASSUME_IMPL_FOR_IFACE(CDbFilterImpl, CDbFilter)

class CDbFilter : public dbobject
{
public:
    CDbFilter();
    //~CDbFilter();
    //NEW_LOCAL(CDbFilterImpl, CDbFilter)
    //NEW_GP(CloudSync, CDbFilterImpl, CDbFilter)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Property name to compare to
    key x_keyFilterProperty
            xauto(Get, Set);

    // Property value to compare to
    str x_sFilterMatch
            xauto(Get, Set);

    str x_sFilterLike
            xauto(Get, Set);

    // Binary logical operator
    type<CFilterComparator> x_typeFilterComparator
            xauto(Get, Set);

    // Visible sequence, does NOT affect sort
    str x_sFilterColumns
            xauto(Get, Set);

    // ORDER BY sorting expression
    str x_sFilterSort
            xauto(Get, Set);

// Operations

// UI

protected:
private:

    void _init_AttachToDbFilter();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
