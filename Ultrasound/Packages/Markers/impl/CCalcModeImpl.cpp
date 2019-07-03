// CCalcModeImpl.cpp

#include "CUsmainProject.h"
#include "CCalcModeImpl.h"
#include "CUsoundCalcModeIfaceGp.h"

CCalcModeImpl::CCalcModeImpl()
{
}

const typelist<CCalcModeIface> CCalcModeImpl::OnGetDerivedList()
{
    typelist<CCalcModeImpl> typelistDerived;
    if (typelistDerived.IsNeedInit(__super::GetDerivedListStorage()))
    {
        typelistDerived.AddDerivedClassesWithFlag("flag_List_as_CalcModeImpl");
    }

    return typelistDerived;
}

type<CCalcModeIface> CCalcModeImpl::OnFromUsoundCalcMode(
        type<CUsoundCalcModeIfaceGp> typeUsoundCalcMode)
{
    // this is an inefficient method - we could create a 
    //   fast mapping cache to convert types by fast lookup
    //   if the typelist is long and we do need optimization here.

    // find a matching back-convert
    type<CCalcModeIface> typeIterCalcMode;
    for (
        iter i;
        this->
            GetDerivedList().
                Iterate(
                    out i,
                    out typeIterCalcMode);)
    {
        if (typeIterCalcMode->ConvertToUsoundCalcMode() == 
                typeUsoundCalcMode)
        {
            return typeIterCalcMode;
        }
    }

    rFAIL(
        "Cannot find a suitable conversion to CalcMode from " + 
            typeUsoundCalcMode->GetObjectClassInfo().GetClassNameInProcess());

    return null();
}

void CCalcModeImpl::OnTestClass()
{
    // Make sure all conversions are unique
    array< type<CCalcModeIface> > atypeCalcModeAlreadyConvertedTo;

    // Test the converter for all shared types
    type<CUsoundCalcModeIfaceGp> typeIterUsoundCalcMode;
    for (
        iter i;
        type<CUsoundCalcModeIfaceGp>()->
            GetDerivedList().
                Iterate(
                    out i,
                    out typeIterUsoundCalcMode);)
    {
        // Known conversion?
        type<CCalcModeIface> typeCalcModeConverted = 
            FromUsoundCalcMode(typeIterUsoundCalcMode);
        rASSERT(typeCalcModeConverted != null());

        // Is it unique?
        rASSERT(
            atypeCalcModeAlreadyConvertedTo.
                Find(typeCalcModeConverted)
                ==
                -1);
        atypeCalcModeAlreadyConvertedTo.
            Add(
                typeCalcModeConverted);
    }
}

