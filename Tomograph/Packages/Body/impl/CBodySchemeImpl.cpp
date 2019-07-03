// CBodySchemeImpl.cpp

#include "CProject.h"
#include "CBodySchemeImpl.h"
#include "CBodyProfileImpl.h"
#include "CBodyPointImpl.h"
#include "VirtOs_ask.h"
#include "CDownloaderIface.h"

CBodySchemeImpl::CBodySchemeImpl()
{
}

void CBodySchemeImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);
}

str CBodySchemeImpl::OnQueryBodyPointNameFromCurrentProfile(
        int iTomogram,
        out str& out_sError)
{
    ASSERT(
        iTomogram >= 0 && 
        iTomogram < CBodyProfileImpl::C_nBodyProfilePointCount);

    if not_null(
        ref<CBodyProfileImpl> rBodyProfile = 
            x_linkSelectedBodyProfile.Get().
                FindPtr(
                    this))
    {
        if (iTomogram < rBodyProfile->GetBodyPointCount())
        {
            return rBodyProfile->GetBodyPointAt(iTomogram)->x_sBodyPointName;
        }
        else
        {
            out_sError = "Out of body point list";
        }
    }
    else
    {
        out_sError = "No selected body point profile";
    }

    return "";
}

void CBodySchemeImpl::x_linkSelectedBodyProfile_Set(
        link<CBodyProfileImpl> linkBodyProfile)
{
    if (this->x_linkSelectedBodyProfile == linkBodyProfile)
        return;

    this->x_linkSelectedBodyProfile = linkBodyProfile;

    _m_pUiBodySchemeSetupSelectedProfile.DefaultIfNull()->InvalidateContent();
}

ref<UiContainer> CBodySchemeImpl::OnNewUiBodySchemeSetup()
{
    ref<UiBodySchemeSetup> rUi;

    rUi->m_rBodyScheme = this;

    return rUi;
}

ref<UiContainer> CBodySchemeImpl::OnNewUiBodySchemeProfileSelector(
        ref<CDownloaderIface> rDownloader)
{
    ref<UiBodySchemeProfileSelector> rUi;

    rUi->m_rBodyScheme = this;
    rUi->m_rDownloader = rDownloader;

    return rUi;
}

ref<UiContainer> CBodySchemeImpl::NewUiBodySchemeSetupProfileList(
        EProfileListUi eProfileListUi)
{
    ref<UiBodySchemeSetupProfileList> rUi;

    rUi->m_rBodyScheme = this;
    rUi->_m_eProfileListUi = eProfileListUi;

    return rUi;
}

[...]