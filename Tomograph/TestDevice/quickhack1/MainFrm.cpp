//***********************************************
// WARNING: UNSAFE PROTOTYPE CODE TO THROW AWAY!
// Intentionally Cta incompatible! You can not 
// copypaste it into production! Tied to MFC.
// Run as debug so you have buffer checks.
//***********************************************
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "Precompiled.h"
#include "Tomographics.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
    ID_SEPARATOR,           // status line indicator
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
    // TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;
    
    if (!m_wndToolBar.CreateEx(
            this, 
            TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_TOOLTIPS,
            WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | CBRS_TOOLTIPS) ||
        !m_wndToolBar.LoadToolBar(RES_MAINFRAME))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;      // fail to create
    }

    //m_wndToolBar.SetBarStyle(CBRS_ALIGN_TOP |  | CBRS_FLYBY);
    m_wndToolBar.EnableToolTips();

    if (!m_wndDlgBar.Create(this, RES_MAINFRAME, 
        CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
    {
        TRACE0("Failed to create dialogbar\n");
        return -1;        // fail to create
    }

    if (!m_wndReBar.Create(this) ||
        !m_wndReBar.AddBar(&m_wndToolBar) ||
        !m_wndReBar.AddBar(&m_wndDlgBar))
    {
        TRACE0("Failed to create rebar\n");
        return -1;      // fail to create
    }

    if (!m_wndStatusBar.Create(this) ||
        !m_wndStatusBar.SetIndicators(indicators,
          sizeof(indicators)/sizeof(UINT)))
    {
        TRACE0("Failed to create status bar\n");
        return -1;      // fail to create
    }

    // TODO: Remove this if you don't want tool tips
    m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
        CBRS_TOOLTIPS | CBRS_FLYBY);

    return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if( !CFrameWnd::PreCreateWindow(cs) )
        return FALSE;
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
    CFrameWnd::Dump(dc);
}

#endif //_DEBUG

// CMainFrame message handlers

