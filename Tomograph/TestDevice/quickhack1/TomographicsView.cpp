//***********************************************
// WARNING: UNSAFE PROTOTYPE CODE TO THROW AWAY!
// Intentionally Cta incompatible! You can not 
// copypaste it into production! Tied to MFC.
// Run as debug so you have buffer checks.
//***********************************************
// TomographicsView.cpp : implementation of the CTomographicsView class
//

#include "Precompiled.h"
#include "Tomographics.h"
#include "TomographicsDoc.h"
#include "TomographicsView.h"
#include "CTomoDevice.h"
#include "CTomoSample.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTomographicsView

IMPLEMENT_DYNCREATE(CTomographicsView, CView)

BEGIN_MESSAGE_MAP(CTomographicsView, CView)
    // Standard printing commands
    ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
    ON_COMMAND(ID_START, &CTomographicsView::OnStart)
    ON_WM_TIMER()
    ON_UPDATE_COMMAND_UI(ID_START, &CTomographicsView::OnUpdateStart)
    ON_COMMAND(ID_EDIT_CLEARMEMORY, &CTomographicsView::OnEditClearmemory)
    ON_COMMAND(ID_DEVICE_EMULATESIGNAL, &CTomographicsView::OnDeviceEmulatesignal)
    ON_UPDATE_COMMAND_UI(ID_DEVICE_EMULATESIGNAL, &CTomographicsView::OnUpdateDeviceEmulatesignal)
    ON_COMMAND(ID_VIEW_SCROLLLEFT, &CTomographicsView::OnViewScrollleft)
    ON_COMMAND(ID_VIEW_SCROLLRIGHT, &CTomographicsView::OnViewScrollright)
    ON_COMMAND(ID_VIEW_SHOWMORESAMPLES, &CTomographicsView::OnViewShowmoresamples)
    ON_COMMAND(ID_VIEW_SHOWFEWERSAMPLES, &CTomographicsView::OnViewShowfewersamples)
    ON_COMMAND(ID_DEVICE_GETSAMPLECOUNT, &CTomographicsView::OnDeviceGetsamplecount)
    ON_COMMAND(ID_DEVICE_RESETCONNECTION, &CTomographicsView::OnDeviceResetconnection)
    ON_COMMAND(ID_DEVICE_SHOWFULLDATADUMP, &CTomographicsView::OnDeviceShowfulldatadump)
    ON_UPDATE_COMMAND_UI(ID_DEVICE_SHOWFULLDATADUMP, &CTomographicsView::OnUpdateDeviceShowfulldatadump)
    ON_COMMAND(ID_DEVICE_GETSAMPLECOUNT32797, &CTomographicsView::OnDeviceGetsamplecount32797)
    ON_COMMAND(ID_DEVICE_FLUSHBUFFERS, &CTomographicsView::OnDeviceFlushbuffers)
    ON_COMMAND(ID_VIEW_SHOW1STCHANNEL, &CTomographicsView::OnViewShow1stchannel)
    ON_COMMAND(ID_VIEW_SHOW2NDCHANNEL, &CTomographicsView::OnViewShow2ndchannel)
    ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW1STCHANNEL, &CTomographicsView::OnUpdateViewShow1stchannel)
    ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW2NDCHANNEL, &CTomographicsView::OnUpdateViewShow2ndchannel)
    ON_COMMAND(ID_DEVICE_GETVERSIONS, &CTomographicsView::OnDeviceGetversions)
    ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CTomographicsView construction/destruction

CTomographicsView::CTomographicsView()
{
    // TODO: add construction code here
    xx_iStartDrawSample = oo; // last
    xx_nViewSamples = 2;
    xx_abShowChannel.SetCount(C_nTomoChannels);
    xx_abShowChannel.Fill(true);
    xx_numWidthScale = 1.0;
    xx_nScrollScaledSignalOffset = 0;
}

CTomographicsView::~CTomographicsView()
{
}

BOOL CTomographicsView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    bool bOk = !!CView::PreCreateWindow(cs);

    return bOk;
}

// CTomographicsView drawing

void CTomographicsView::OnDraw(CDC* pDc)
{
    CTomographicsDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    CRect rectClient;
    GetClientRect(rectClient);
    CRect rectOriginalClient = rectClient;

    rectClient.right = int(rectClient.right * xx_numWidthScale);

    // Upon load view first, not last
    if (GetDocument()->xx_bLoadedFromFile &&
        xx_iStartDrawSample == oo)
    {
        xx_iStartDrawSample = 0;
    }

    int iStartSample = xx_iStartDrawSample;
    if (iStartSample >= GetDocument()->xx_apTomoSample.GetCount())
        iStartSample = GetDocument()->xx_apTomoSample.GetCount() - 1;

    // start from even sample for channel 1
    iStartSample -= iStartSample % C_nTomoChannels;

    int nSplitScreen = xx_nViewSamples / C_nTomoChannels;

    int nWidthPerSamplePair = 
        nSplitScreen? rectClient.Width() / nSplitScreen : 10;

    int nChannelsDrawn = 0;
    repeat(iPair, nSplitScreen)
    {
        CRect rectPair;
        rectPair.left = iPair * nWidthPerSamplePair;
        rectPair.top = rectClient.top;
        rectPair.right = (iPair + 1) * nWidthPerSamplePair;
        rectPair.bottom = rectClient.bottom;

        repeat(iChannel, C_nTomoChannels)
        {
            int iDraw = iStartSample + iChannel + iPair * C_nTomoChannels;
            if (iDraw >= GetDocument()->xx_apTomoSample.GetCount())
                break;

            if (!xx_abShowChannel[iChannel])
                continue;

            nChannelsDrawn++;

            CTomoSample* pSample = 
                GetDocument()->
                    xx_apTomoSample.GetAt(
                        iDraw);
        
            pSample->
                DrawCurve(
                    pDc,
                    rectPair,
                    iDraw,
                    iPair,
                    xx_nScrollScaledSignalOffset);
        }
    }

    if (!nChannelsDrawn)
    {
        pDc->TextOut(10, 10, "all channels are hidden");
    }
    
    rectClient = rectOriginalClient;

    //else 
    //if (!GetDocument()->xx_bSomeDataReceived)
    {
        //pDc->SetTextAlign(TA_RIGHT | TA_BOTTOM);
        CRect rectInfo = rectClient;
        str sInfo = GetDocument()->GetStatusText();

        if (xx_numWidthScale != 1.0 || xx_nScrollScaledSignalOffset != 0)
        {
            sInfo += 
                str() +
                    "\n\n\n" + 
                    "[Scale " + Str(xx_numWidthScale) + 
                    " +" + Str(xx_nScrollScaledSignalOffset) + "]";
        }

        pDc->DrawText(
            sInfo,
            rectInfo,
            DT_CALCRECT);

        //int nIndent = 10;
        CRect rectDraw;
        /*
        rectDraw.left = rectClient.right - nIndent - rectInfo.Width();
        rectDraw.right = rectClient.right - nIndent;
        rectDraw.top = rectClient.bottom - nIndent - rectInfo.Height();
        rectDraw.bottom = rectClient.bottom - nIndent;
        */
        rectDraw.left = (rectClient.Width() - rectInfo.Width()) / 2;
        rectDraw.right = rectDraw.left + rectInfo.Width();
        rectDraw.top = (rectClient.Height() - rectInfo.Height()) / 2;
        rectDraw.bottom = rectClient.top + rectInfo.Height();

        //pDc->SetTextColor(RGB(0, 0, 150));
        pDc->DrawText(
            sInfo,
            rectDraw,
            DT_WORDBREAK | DT_NOCLIP);
    }
}

// CTomographicsView printing

BOOL CTomographicsView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // default preparation
    return DoPreparePrinting(pInfo);
}

void CTomographicsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: add extra initialization before printing
}

void CTomographicsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: add cleanup after printing
}

// CTomographicsView diagnostics

#ifdef _DEBUG
void CTomographicsView::AssertValid() const
{
    CView::AssertValid();
}

void CTomographicsView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CTomographicsDoc* CTomographicsView::GetDocument() const // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTomographicsDoc)));
    return (CTomographicsDoc*) m_pDocument;
}
#endif //_DEBUG

// CTomographicsView message handlers

void CTomographicsView::OnStart()
{
    theApp.xx_pTomoDevice->Init();

    SetTimer(101, 10, NULL);

    theApp.xx_pTomoDevice->xx_bEnablePolling = 
        !theApp.xx_pTomoDevice->xx_bEnablePolling;

    GetDocument()->SetStatusText("");
}

void CTomographicsView::OnUpdateStart(CCmdUI *pCmdUI)
{
    // TODO: Add your command update UI handler code here
    pCmdUI->SetCheck(theApp.xx_pTomoDevice->xx_bEnablePolling);
}

void CTomographicsView::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    CView::OnTimer(nIDEvent);

    theApp.xx_pTomoDevice->PollNextBlock(this);
}

void CTomographicsView::OnEditClearmemory()
{
    theApp.xx_pTomoDevice->ClearAll();
}

void CTomographicsView::OnDeviceEmulatesignal()
{
    theApp.xx_pTomoDevice->xx_bEmulateSignal = 
        !theApp.xx_pTomoDevice->xx_bEmulateSignal;
}

void CTomographicsView::OnUpdateDeviceEmulatesignal(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(theApp.xx_pTomoDevice->xx_bEmulateSignal);
}

void CTomographicsView::OnViewScrollleft()
{
    if (xx_iStartDrawSample < C_nTomoChannels)
    {
        Beep(200, 50);
    }
    else
    {
        if (xx_iStartDrawSample > GetDocument()->xx_apTomoSample.GetCount())
        {
            xx_iStartDrawSample = 
                GetDocument()->xx_apTomoSample.GetCount() -
                    GetDocument()->xx_apTomoSample.GetCount() % C_nTomoChannels;

            xx_iStartDrawSample -= C_nTomoChannels;
        }

        xx_iStartDrawSample -= C_nTomoChannels;
        if (xx_iStartDrawSample < 0)
            xx_iStartDrawSample = 0;

        Invalidate();
    }
}

void CTomographicsView::OnViewScrollright()
{
    if (xx_iStartDrawSample >= GetDocument()->xx_apTomoSample.GetCount() - C_nTomoChannels)
    {
        Beep(200, 50);
    }
    else
    {
        xx_iStartDrawSample += C_nTomoChannels;
        Invalidate();
    }
}

void CTomographicsView::OnViewShowfewersamples()
{
    if (xx_nViewSamples <= C_nTomoChannels)
    {
        Beep(100, 50);
    }
    else
    {
        xx_nViewSamples -= C_nTomoChannels;
        Invalidate();
    }
}

void CTomographicsView::OnViewShowmoresamples()
{
    if (xx_nViewSamples >= 
            max(10, GetDocument()->xx_apTomoSample.GetCount() + 10))
    {
        Beep(100, 50);
    }
    else
    {
        xx_nViewSamples += C_nTomoChannels;

        if (xx_iStartDrawSample > 0)
        {
            OnViewScrollleft();
        }

        int nEmptyPairs = 
            GetDocument()->xx_apTomoSample.GetCount() - 
                xx_iStartDrawSample;

        xx_iStartDrawSample -= nEmptyPairs;
        if (xx_iStartDrawSample < 0)
            xx_iStartDrawSample = 0;

        Invalidate();
    }
}

void CTomographicsView::OnDeviceGetsamplecount()
{
    GetDocument()->SetStatusText(theApp.xx_pTomoDevice->RunGetSampleCount());
}

void CTomographicsView::OnDeviceGetsamplecount32797()
{
    GetDocument()->SetStatusText(theApp.xx_pTomoDevice->RunGetSampleCount(2048));
}

void CTomographicsView::OnDeviceGetversions()
{
    GetDocument()->SetStatusText(theApp.xx_pTomoDevice->RunGetVersions());
}

void CTomographicsView::OnDeviceResetconnection()
{
    theApp.xx_pTomoDevice->ReInitLibrary();
}

void CTomographicsView::OnDeviceFlushbuffers()
{
    theApp.xx_pTomoDevice->FlushBuffers();
}

void CTomographicsView::OnDeviceShowfulldatadump()
{
    if (theApp.xx_pTomoDevice->xx_nMaxRows <= 1)
        theApp.xx_pTomoDevice->xx_nMaxRows = oo;
    else
        theApp.xx_pTomoDevice->xx_nMaxRows = 1;
}

void CTomographicsView::OnUpdateDeviceShowfulldatadump(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(theApp.xx_pTomoDevice->xx_nMaxRows > 1);
}

void CTomographicsView::OnViewShow1stchannel()
{
    xx_abShowChannel.SetAt(0, !xx_abShowChannel.GetAt(0));
    Invalidate();
}

void CTomographicsView::OnViewShow2ndchannel()
{
    xx_abShowChannel.SetAt(1, !xx_abShowChannel.GetAt(1));
    Invalidate();
}

void CTomographicsView::OnUpdateViewShow1stchannel(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(xx_abShowChannel.GetAt(0));
}

void CTomographicsView::OnUpdateViewShow2ndchannel(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(xx_abShowChannel.GetAt(1));
}

void CTomographicsView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    // TODO: Add your specialized code here and/or call the base class
    __super::OnUpdate(pSender, lHint, pHint);

    UpdateWindow();
}

void CTomographicsView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CView::OnKeyDown(nChar, nRepCnt, nFlags);

    bool bUpdate = false;
    num numScaleStep = 1 + int(xx_numWidthScale / 2);
    if (nChar == VK_DOWN) //LEFT && GetAsyncKeyState(VK_CONTROL) < 0)
    {
        if (xx_numWidthScale == 1)
        {
            OnViewShowmoresamples();
            xx_nScrollScaledSignalOffset = 0;
        }
        else
        {
            xx_numWidthScale -= numScaleStep;
            if (xx_numWidthScale < 1)
            {
                xx_numWidthScale = 1;
                xx_nScrollScaledSignalOffset = 0;
            }
        }

        bUpdate = true;
    }
    else
    if (nChar == VK_UP) // && GetAsyncKeyState(VK_CONTROL) < 0)
    {
        if (xx_nViewSamples > 2)
        {
            OnViewShowfewersamples();
            xx_nScrollScaledSignalOffset = 0;
        }
        else
        {
            xx_numWidthScale += numScaleStep;
        }

        bUpdate = true;
    }
    else
    if (nChar == VK_LEFT) // && GetAsyncKeyState(VK_SHIFT) < 0)
    {
        xx_nScrollScaledSignalOffset -= 1;
        bUpdate = true;
    }
    else
    if (nChar == VK_RIGHT) // && GetAsyncKeyState(VK_SHIFT) < 0)
    {
        xx_nScrollScaledSignalOffset += 1;
        bUpdate = true;
    }
    else
    {
        bUpdate = false;
    }

    if (bUpdate)
    {
        GetDocument()->SetStatusText("");

        Invalidate();
    }
}
