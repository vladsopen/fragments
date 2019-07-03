//***********************************************
// WARNING: UNSAFE PROTOTYPE CODE TO THROW AWAY!
// Intentionally Cta incompatible! You can not 
// copypaste it into production! Tied to MFC.
// Run as debug so you have buffer checks.
//***********************************************
// CTomoSample.cpp

#include "Precompiled.h"
#include "Tomographics.h"
#include "CTomoSample.h"
#include "TomographicsDoc.h"
#include "TomographicsView.h"

//const int CTomoSample::s_nMaxAmplitude = 1024;

CTomoSample::CTomoSample(
        int iChannel,
        CTomographicsDoc* pDoc)
{
    xx_pDoc = pDoc;
    xx_iChannel = iChannel;
    xx_anCurve.SetCount(C_nTomoAllocatedAmplitudesPerSample);
    m_nAdded = 0;
    //s_nMaxAmplitude = 0;
}

//void CTomoSample::OnExposeContent(ref<CExpose> rExpose)
//{
//    __super::OnExposeContent(rExpose);
//    OnExposeContentGenerated(rExpose);
//
//    rFAIL("make sure it is OK to expose CTomoSample!");
//}

bool CTomoSample::TryAppendAmplitudeIsOverflow(
        int nAmplitude)
{
    if (m_nAdded >= xx_anCurve.GetCount())
        return true;

    if (nAmplitude == 0xffff)
        nAmplitude = 0;
    else
        strlen("");

    xx_anCurve.SetAt(m_nAdded, nAmplitude);
    m_nAdded++;

    if (nAmplitude > s_nMaxAmplitude)
    {
        if (nAmplitude < 0xdfff)
        {
            //s_nMaxAmplitude = nAmplitude;
        }
    }

    return false;
}

void CTomoSample::DrawCurve(
        CDC* pDc,
        CRect rectClient,
        int iSample,
        int iDrawn,
        int nScrollScaledSignalOffset)
{
    m_apointCurveCache.SetCount(rectClient.Width());

    int nAddedPoints = 0;
    repeat(iPosX, rectClient.Width())
    {
        POINT& pointIter = m_apointCurveCache._unsafe_ElementAt(iPosX);
        pointIter.x = rectClient.left + iPosX + xx_iChannel;

        int iAmplitude = 
            MulDiv(
                iPosX,
                xx_anCurve.GetCount(),
                rectClient.Width());

        iAmplitude += nScrollScaledSignalOffset;

        if (iAmplitude >= m_nAdded)
            break;

        if (iAmplitude < 0)
            continue;

        nAddedPoints++;

        int nAmplitude = 
            xx_anCurve.GetAt(iAmplitude);

        pointIter.y = 
            rectClient.top + 
                rectClient.Height() - C_nTomoChannels - 
                    MulDiv(
                        nAmplitude,
                        rectClient.Height() - C_nTomoChannels,
                        s_nMaxAmplitude? s_nMaxAmplitude : 1);

        pointIter.y += xx_iChannel;
    }

    COLORREF colorrefLine = RGB(255, 0, 255);
    if (xx_iChannel == 0)
        colorrefLine = RGB(0, 0, 255);
    else
    if (xx_iChannel == 1)
        colorrefLine = RGB(0, 255, 0);
    else
        ASSERT(0);

    CPen oPen(PS_SOLID, 1, colorrefLine);
    CPen* pOldPen = pDc->SelectObject(&oPen);

    pDc->Polyline(
        m_apointCurveCache._unsafe_GetPointer(), 
        min(m_apointCurveCache.GetCount(), nAddedPoints));

    // Ellipse knots
    /*
    if (rectClient.Width() > 10000)
    {
        repeat(i, m_apointCurveCache.GetCount())
        {
            POINT pt = m_apointCurveCache.GetAt(i);
            int nRadius = 3;
            pDc->Ellipse(
                pt.x - nRadius, 
                pt.y - nRadius, 
                pt.x + nRadius, 
                pt.y + nRadius);
        }
    }
    */

    static HFONT my_font = 0;
    if (!my_font)
        my_font = CreateFont(
              -10, // logical height of font
              0,  // logical average character width
              0,  // angle of escapement
              0,  // base-line orientation angle
              FW_NORMAL, // font weight
              0, // italic attribute flag
              0, // underline attribute flag
              0, // strikeout attribute flag
              RUSSIAN_CHARSET, // character set identifier
              0, // output precision
              0, // DWORD fdwClipPrecision,  // clipping precision
              0, // DWORD fdwQuality,        // output quality
              0, // DWORD fdwPitchAndFamily,  // pitch and family
              "MS Sans Serif" // pointer to typeface name string
            );
    ::SelectObject(pDc->GetSafeHdc(), my_font);

    pDc->SetBkMode(OPAQUE);
    pDc->SetTextColor(colorrefLine);
    CString sInfo;
    sInfo.Format(
        " %i [%i] ", //  max: %i / 0x%04x ", 
        (iSample / C_nTomoChannels) + 1, 
        xx_iChannel
        //s_nMaxAmplitude, s_nMaxAmplitude
        );
    pDc->TextOut(
        rectClient.left + 5, 
        rectClient.top + 5 + 16 * xx_iChannel, 
        sInfo);

    pDc->SetTextColor(RGB(100, 100, 0));

    if (iDrawn == 0 &&
        !xx_pDoc->xx_bSomeDataReceived)
    {
        sInfo.Format(
            " max fixed: %i / 0x%04x ", 
            s_nMaxAmplitude, s_nMaxAmplitude
            );
        pDc->TextOut(
            rectClient.left + 5, 
            rectClient.top + 40, 
            sInfo);
    }

    CPen oPenBox(PS_SOLID, 1, RGB(200, 200, 0));
    pDc->SelectObject(&oPenBox);
    pDc->SelectStockObject(NULL_BRUSH);
    pDc->Rectangle(rectClient);

    pDc->SelectObject(pOldPen);

    pDc->SelectStockObject(SYSTEM_FONT);
}

int CTomoSample::ExportToStereoInterleaveWaveCountBytes(
        short* pshortBufferOrNull)
{
    repeat(i, xx_anCurve.GetCount())
    {
        if (pshortBufferOrNull)
        {
            pshortBufferOrNull[i * 2 + xx_iChannel] = (short) xx_anCurve.GetAt(i);

            //pshortBufferOrNull += 2;
        }
    }

    return xx_anCurve.GetCount() * sizeof(xx_anCurve.GetAt(0));
}

