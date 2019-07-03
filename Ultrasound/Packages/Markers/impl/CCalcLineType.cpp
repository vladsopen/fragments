// CCalcLineType.cpp

#include "CUsmainProject.h"
#include "CCalcLineType.h"
#include "CUsoundIndexTypeIfaceGp.h"
#include "CUsoundAxisIfaceGp.h"

CCalcLineType::CCalcLineType()
{
}

const typelist<CCalcLineType> CCalcLineType::GetDerivedList()
{
    typelist<CCalcLineType> typelistDerived;
    if (typelistDerived.IsNeedInit(__super::GetDerivedListStorage()))
    {
        typelistDerived.AddDerivedClassesWithFlag("flag_List_as_CalcLineType");
    }

    return typelistDerived;
}

str CCalcLineType::GetCalcLineName()
{
    type<CUsoundIndexTypeIfaceGp> typeUsoundIndexType = 
        GetCalcLineUsoundIndex();

    str sCustomName = OnGetCalcLineName();

    if (typeUsoundIndexType != null())
    {
        rASSERT(sCustomName == "");
        return
            typeUsoundIndexType->
                GetFixedIndexName();
    }
    else
    {
        rASSERT(sCustomName != "");
        return
            sCustomName;
    }
}

str CCalcLineType::GetCalcLineDescription()
{
    type<CUsoundIndexTypeIfaceGp> typeUsoundIndexType = 
        GetCalcLineUsoundIndex();

    str sCustomDescription = OnGetCalcLineDescription();

    if (typeUsoundIndexType != null())
    {
        rASSERT(sCustomDescription == "");
        return
            typeUsoundIndexType->
                GetFixedIndexDescription();
    }
    else
    {
        rASSERT(sCustomDescription != "");
        return
            sCustomDescription;
    }
}

type<CUsoundAxisIfaceGp> CCalcLineType::GetCalcLineUsoundAxis()
{
    type<CUsoundIndexTypeIfaceGp> typeUsoundIndexType = 
        GetCalcLineUsoundIndex();

    type<CUsoundAxisIfaceGp> typeCustomUsoundAxis = 
        OnGetCalcLineUsoundAxis();

    if (typeCustomUsoundAxis != null())
    {
        rASSERT(typeUsoundIndexType == null());

        return typeCustomUsoundAxis;
    }
    else
    {
        rASSERT(typeUsoundIndexType != null());

        type<CUsoundAxisIfaceGp> typeDefaultUsoundAxis = 
            typeUsoundIndexType->GetUsoundAxisForBasicIndex();

        rASSERT(typeDefaultUsoundAxis != null());

        return
            typeDefaultUsoundAxis;
    }
}

type<CCalcLineType> CCalcLineType::FromCalcLineSharedEnum(
        ECalcMarkingLine eCalcMarkingLine)
{
    // this is an inefficient method - we could create a
    //   fast mapping cache to convert types by fast lookup
    //   if the typelist is long and we do need optimization here.

    // find a matching back-convert
    type<CCalcLineType> typeIterCalcLineType;
    for (
        iter i;
        this->
            GetDerivedList().
                Iterate(
                    out i,
                    out typeIterCalcLineType);)
    {
        if (typeIterCalcLineType->GetCalcLineSharedEnum() ==
                eCalcMarkingLine)
        {
            return typeIterCalcLineType;
        }
    }

    rFAIL(
        "Cannot find a suitable conversion to CalcLineType from " +
            "enum = " + Str((int) eCalcMarkingLine));

    return null();
}

void CCalcLineType::OnTestClass()
{
    // Make sure all unique attributes are unique
    type<CCalcLineType> typeIterCalcLineType;
    for (
        iter i;
        this->
            GetDerivedList().
                Iterate(
                    out i,
                    out typeIterCalcLineType);)
    {
        typeIterCalcLineType->_test_CheckCalcLineType();
    }
}

void CCalcLineType::_test_CheckCalcLineType()
{
    // Test a type
    rASSERT(
        this->GetCalcLineName() != 
            this->GetCalcLineDescription()); // (example)

    // Test the converter for all shared types
    type<CCalcLineType> typeIterCalcLineType;
    for (
        iter i;
        this->
            GetDerivedList().
                Iterate(
                    out i,
                    out typeIterCalcLineType);)
    {
        // Don't compate to itself
        if (this == typeIterCalcLineType)
        {
            continue;
        }

        // enum converters
        rASSERT(
            this->GetCalcLineSharedEnum() 
                != 
            typeIterCalcLineType->GetCalcLineSharedEnum());

        // index mathcing
        rASSERT(
            this->GetCalcLineUsoundIndex() 
                != 
            typeIterCalcLineType->GetCalcLineUsoundIndex());
    }
}

