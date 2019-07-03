// type_information.cpp
// ("type_information.h")

#include "Precompiled.h"
#include "CCommon.h"

//
// CClassId
//

CClassId::CClassId(EUniqueAddress* peUniqueAddress)
{
    m_peUniqueAddress = peUniqueAddress;
}

bool CClassId::operator == (const CClassId& id) const
{
    return m_peUniqueAddress == id.m_peUniqueAddress;
}

CCommon* TryCastImpl(CClassId id, CCommon* p)
{
    if (!p)
        return 0;
    return const_cast<CCommon*>(p)->TryCastToClass(id);
}

CCommon* TryCast(CClassId id, CCommon* p)
{
    return TryCastImpl(id, p);
}

CCommon* TryCast(CClassId id, const nullable_base& p)
{
    return TryCastImpl(id, p.GetPointerOrNull());
}

