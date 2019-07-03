// CSkinizerCommonIface.cpp

#include "CProject.h"
#include "CSkinizerCommonIface.h"
#include "CSkinizerCommonImpl.h"

CSkinizerCommonIface::CSkinizerCommonIface()
{
}

void CSkinizerCommonIface::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CSkinizerCommonIface!");
}

str CSkinizerCommonIface::GGetSkinReport(
        ref<CSkinizerCommonIfaceGp> rSkinizer)
{
    str sReport = 
        rSkinizer->GetObjectClassInfo().GetClassNameInProcess() + ": ";
    
    if not_null(
        ref<CSkinizerCommonIface> rSkinizerCommon = 
            Derive<CSkinizerCommonIface>(rSkinizer))
    {
        sReport += " Tomo"; // + rSkinizerCommon->GetSkinTestLabel() + ">";
    }
    else
    {
        sReport += " Non-Tomo";
    }

    return sReport;
}

/*
CSkinizerCommonIfaceGp* CSkinizerCommonIface::OnGetSkinizerCommonImpl()
{
    // override implementation redirector so that subderived classes would use
    //   our impl instead of parent's
    
    // note: this is not what it may seem!
    //   this will allocate default *impl* using AUTO_NEW_OBJECT_GP()
    return CSkinizerCommonIface::G New Object();

    // derived classes may re-hook this function again and force this pointer to 
    //   their own extended impls.
}
*/