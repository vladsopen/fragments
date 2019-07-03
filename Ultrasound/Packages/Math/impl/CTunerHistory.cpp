// CTunerHistory.cpp

#include "CDucalcProject.h"
#include "CMediaDocIfaceGp.h" //(dont_uninclude)
#include "CTunerProfileList.h"
#include "CTunerValue.h"
#include "CTunerHistory.h"
#include "CRoleForTunerProfile.h"

CTunerHistory::CTunerHistory()
{
}

ref<UiContainer> CTunerHistory::NewTunerHistoryUi()
{
    ref<UiTunerHistory> rUi;

    rUi->m_rTunerHistory = this;

    return rUi;
}

void CTunerHistory::x_iViewOffset_Set(int value)
{
    ASSERT(
        (value >= -1 && value < m_aTunerProfileList.GetCount()) ||
        m_aTunerProfileList.GetCount() == 0);

    if (this->x_iViewOffset == value)
        return;

    this->x_iViewOffset = value;

    _m_pUiTunerHistory.DefaultIfNull()->InvalidateContent();
}

ptr<CTunerProfileList> CTunerHistory::LookupAt(
        int iAt)
{
    if (iAt >= 0 && iAt < m_aTunerProfileList.GetCount())
        return m_aTunerProfileList[iAt];
    else
        return null();
}

void CTunerHistory::Add(ref<CTunerProfileList> value) 
{ 
    // Change profile role
    value->x_typeRoleForTunerProfile = 
        type<CRoleForTunerProfileAsDump>(); 
    
    m_aTunerProfileList.Add(value); 
}
