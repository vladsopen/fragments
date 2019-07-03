//***********************************************
// WARNING: UNSAFE PROTOTYPE CODE TO THROW AWAY!
// Intentionally Cta incompatible! You can not 
// copypaste it into production! Tied to MFC.
// Run as debug so you have buffer checks.
//***********************************************
// TomographicsView.h : interface of the CTomographicsView class
//

#pragma once

class CTomographicsView : public CView
{
protected: // create from serialization only
    CTomographicsView();
    DECLARE_DYNCREATE(CTomographicsView)

// Attributes
public:
    CTomographicsDoc* GetDocument() const;

    int xx_iStartDrawSample;
    int xx_nViewSamples;
    double xx_numWidthScale;
    int xx_nScrollScaledSignalOffset;
    array<bool> xx_abShowChannel;

// Operations
public:

// Overrides
public:
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
    virtual ~CTomographicsView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnStart();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnUpdateStart(CCmdUI *pCmdUI);
    afx_msg void OnEditClearmemory();
    afx_msg void OnDeviceEmulatesignal();
    afx_msg void OnUpdateDeviceEmulatesignal(CCmdUI *pCmdUI);
    afx_msg void OnViewScrollleft();
    afx_msg void OnViewScrollright();
    afx_msg void OnViewShowmoresamples();
    afx_msg void OnViewShowfewersamples();
    afx_msg void OnDeviceGetsamplecount();
    afx_msg void OnDeviceResetconnection();
    afx_msg void OnDeviceShowfulldatadump();
    afx_msg void OnUpdateDeviceShowfulldatadump(CCmdUI *pCmdUI);
    afx_msg void OnDeviceGetsamplecount32797();
    afx_msg void OnDeviceFlushbuffers();
    afx_msg void OnViewShow1stchannel();
    afx_msg void OnViewShow2ndchannel();
    afx_msg void OnUpdateViewShow1stchannel(CCmdUI *pCmdUI);
    afx_msg void OnUpdateViewShow2ndchannel(CCmdUI *pCmdUI);
    afx_msg void OnDeviceGetversions();
protected:
    virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in TomographicsView.cpp
inline CTomographicsDoc* CTomographicsView::GetDocument() const
   { return reinterpret_cast<CTomographicsDoc*>(m_pDocument); }
#endif

