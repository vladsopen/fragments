// CUmoveAxis.cpp

#include "CGuiPlusProject.h"
#include "CUmoveAxis.h"

CUmoveAxis::CUmoveAxis()
{
}

const typelist<CUmoveAxis> CUmoveAxis::GetDerivedList()
{
    typelist<CUmoveAxis> typelistDerived;
    if (typelistDerived.IsNeedInit(__super::GetDerivedListStorage()))
    {
        typelistDerived.AddDerivedClassesWithFlag("flag_List_as_UmoveAxis");
    }

    return typelistDerived;
}

