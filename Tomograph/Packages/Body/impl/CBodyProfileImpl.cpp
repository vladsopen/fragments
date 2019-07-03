// CBodyProfileImpl.cpp

#include "CProject.h"
#include "CBodyProfileImpl.h"
#include "CBodyPointImpl.h"
#include "VirtOs_ask.h"
#include "CBodySchemeImpl.h"
#include "CDownloaderIface.h"

CBodyProfileImpl::CBodyProfileImpl()
{
}

void CBodyProfileImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);
}

rich CBodyProfileImpl::GetBodyProfileNameForIndication()
{
    rich richShowName = x_sBodyProfileName;
    if (richShowName.GetStripBoth() == "")
    {
        richShowName = 
            TRANSLATE(
                "<No name>"[... UI-string-removed]");

        richShowName = RichTextColor(rgb(180, 130, 130), richShowName);
    }

    return richShowName;
}

ref<UiContainer> CBodyProfileImpl::NewUiBodyProfileSetup()
{
    ref<UiBodyProfileSetup> rUi;

    rUi->m_rBodyProfile = this;

    return rUi;
}

void CBodyProfileImpl::UpdateBodyProfileSetupWhenListChanged()
{
    _m_pUiBodyProfileSetup.DefaultIfNull()->InvalidateContent();
}

ref<CBodyPointImpl> CBodyProfileImpl::AppendNewPoint()
{
    ref<CBodyPointImpl> rNewBodyPoint;

    if (GetBodyPointCount() >= C_nBodyProfilePointCount)
    {
        str sMax = Str(C_nBodyProfilePointCount);
        sloc slocError = 
            TRANSLATE(
                "Cannot add[... UI-string-removed]");

        ask(slocError).DeferToIdle("");

        return rNewBodyPoint;
    }

    AddBodyPoint(rNewBodyPoint);
    x_iSelectedBodyPoint = GetBodyPointCount() - 1;

    UpdateBodyProfileSetupWhenListChanged();

    return rNewBodyPoint;
}

void CBodyProfileImpl::DeleteSelectedPoint()
{
    if failed(x_iSelectedBodyPoint >= 0)
        return;

    m_aBodyPoint.RemoveAt(x_iSelectedBodyPoint);
    //x_iSelectedBodyPoint = x_iSelectedBodyPoint - 1;
    if (x_iSelectedBodyPoint >= GetBodyPointCount())
        x_iSelectedBodyPoint = GetBodyPointCount() - 1;

    UpdateBodyProfileSetupWhenListChanged();
}

ref<UiContainer> CBodyProfileImpl::NewUiBodyProfileListRow(
        ref<CBodySchemeImpl> rBodyScheme)
{
    ref<UiBodyProfileListRow> rUi;

    rUi->m_rBodyProfile = this;
    rUi->m_rBodyScheme = rBodyScheme;

    return rUi;
}

ref<UiContainer> CBodyProfileImpl::NewUiBodyProfileProgress(
        ref<CDownloaderIface> rDownloader)
{
    ref<UiBodyProfileProgress> rUi;

    rUi->m_rBodyProfile = this;
    rUi->m_rDownloader = rDownloader;

    return rUi;
}
