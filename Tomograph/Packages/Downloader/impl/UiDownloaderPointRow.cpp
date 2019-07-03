// UiDownloaderPointRow.cpp

#include "CProject.h"
#include "CDownloaderImpl.h"
#include "CBodySchemeIface.h"
#include "CGlobalConfigIface.h"
#include "CTomogramIface.h"
#include "CSkinizerCommonIface.h"

void UiDownloaderPointRow::Ui()
{
    ref<CBodySchemeIface> rBodyScheme = 
        m_rDownloader->GetGlobalConfig()->x_rBodyScheme;

    int nTomogramCount = m_rDownloader->GetTomogramCount();
    //int nBodyPointCount = rBodyScheme->GetCurrentBodyProfilePointCount();
    int iCurrentSelection = m_rDownloader->x_iPreviewTomogram;

    /*
    idle update may cause this to fail
    rASSERT(
        m_iPointOrTomogram >= 0 && 
        m_iPointOrTomogram < math::GMax(nTomogramCount, nBodyPointCount));
        */

    static const int C_nLeftMargin = 4;

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        rUi->SetMinX(0);
        rUi->SetPreferredX(oo); //scale(90));

        m_rDownloader->_m_pUiDownloaderPointRow = rUi;
    }

    //
    // Order #
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetText(Str(m_iPointOrTomogram + 1));

        rUi->SetPreferredX(scale(25));

        rUi->SetAlignContentToCenterX();
    }

    if (m_iPointOrTomogram < nTomogramCount)
    {
        ref<CTomogramIface> rTomogram = 
            m_rDownloader->GetStoredTomogramAt(m_iPointOrTomogram).
                Valid();

        //
        // Show Tomogram Selector
        //
        {
            ref<CUiButtonGp> rUi;

            rUi->SetText(
                str() +
                    RichTextColor(
                        rgb(0, 130, 0),
                        RichFontMarlett(C_RichMarlett_Check)) + 
                    " " +
                    rTomogram->x_sTomogramBodyPointName);

            rUi->SetTip(
                TRANSLATE(
                    "Tomogram b[... UI-string-removed]");

            rUi->SetLookToFlat();

            rUi->SetSelect(iCurrentSelection == m_iPointOrTomogram);

            rUi->SetMaxX(oo);

            rUi->SetMarginAtLeft(C_nLeftMargin);

            rUi->SetAlignContentToLeft();

            //if (debug("Body.DemoCheckOnProfile?").InputBool())
            //{
            //    rUi->SetLookToCheck();
            //    rUi->SetCheck(!!rRandom->GenerateNextRandomInt(0, 1));
            //}

            rUi->OnClick = [=]
            {
                m_rDownloader->x_iPreviewTomogram = m_iPointOrTomogram;
            }
        }
    }
    else
    {
        rASSERT(m_iPointOrTomogram >= nTomogramCount);
        
        // No tomogram for this point (yet?)

        //
        // Show Just a point label
        //
        {
            ref<CUiStaticGp> rUi;
            
            str sError;
            str sName = 
                rBodyScheme->
                    QueryBodyPointNameFromCurrentProfile(
                        m_iPointOrTomogram,
                        out sError);

            rUi->SetText(sName);

            rUi->SetTip(
                TRANSLATE(
                    "Body point[... UI-string-removed]");

            /*
            rUi->SetDisableInfo(
                TRANSLATE(
                    "No tomogra[... UI-string-removed]");
                    */

            rUi->SetMaxX(oo);

            rUi->SetMarginAtLeft(C_nLeftMargin);

            rUi->SetTextColor(
                CSkinizerCommonIface::C_rgbPointNotDownloadedText);

            //if (debug("Body.DemoCheckOnProfile?").InputBool())
            //{
            //    rUi->SetLookToCheck();
            //    rUi->SetCheck(!!rRandom->GenerateNextRandomInt(0, 1));
            //}

            rUi->OnClick = [=]
            {
                m_rDownloader->x_iPreviewTomogram = m_iPointOrTomogram;
            }
        }
    }
}
