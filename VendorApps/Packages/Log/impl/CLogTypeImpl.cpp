// CLogTypeImpl.cpp

#include "CProject.h"
#include "CLogTypeImpl.h"
#include "CLogMapImpl.h"
#include "CAppEventFloaterIfaceGp.h"

CLogTypeImpl::CLogTypeImpl()
{
}

const typelist<CLogTypeIface> CLogTypeImpl::OnGetDerivedList()
{
    return GetDerivedListImpl();
}

const typelist<CLogTypeImpl> CLogTypeImpl::GetDerivedListImpl()
{
    typelist<CLogTypeImpl> typelistDerived;
    if (typelistDerived.IsNeedInit(__super::GetDerivedListStorage()))
    {
        typelistDerived.AddDerivedClassesWithFlag("flag_List_as_LogTypeImpl");
    }

    return typelistDerived;
}

void CLogTypeImpl::OnLogEvent(
        sloc slocFriendlyMessage,
        str sTechnicalDetails)
{
    // all floaters are global
    ref<CLogMapImpl> rLogMap = 
        CProject::GGetIt().
            x_rLogMap;

    // get the floater corresponding to our type
    //   (this also does auto-init first time)
    ref<CAppEventFloaterIfaceGp> rAppEventFloater = 
        rLogMap->
            GetEventFloaterForLogType(
                this);

    // send message to the floater 
    rAppEventFloater->
        LogNewEvent(
            slocFriendlyMessage,
            sTechnicalDetails);

    // you can add your logging functionality 
    //   common to all types
    //TRACE(sTechnicalDetails + "\n");

    // NOTE: you can override this function for any log type<>
}
