//***********************************************
// WARNING: UNSAFE PROTOTYPE CODE TO THROW AWAY!
// Intentionally Cta incompatible! You can not 
// copypaste it into production! Tied to MFC.
// Run as debug so you have buffer checks.
//***********************************************
// Tomographics.cpp : Defines the class behaviors for the application.
//

#include "Precompiled.h"
#include "Tomographics.h"
#include "MainFrm.h"

#include "TomographicsDoc.h"
#include "TomographicsView.h"
#include "CTomoDevice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTomographicsApp

BEGIN_MESSAGE_MAP(CTomographicsApp, CWinApp)
    ON_COMMAND(ID_APP_ABOUT, &CTomographicsApp::OnAppAbout)
    // Standard file based document commands
    ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
    // Standard print setup command
    ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

// CTomographicsApp construction

CTomographicsApp::CTomographicsApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance

    xx_pTomoDevice = new CTomoDevice;
}

// The one and only CTomographicsApp object

CTomographicsApp theApp;

// CTomographicsApp initialization

BOOL CTomographicsApp::InitInstance()
{
    // InitCommonControlsEx() is required on Windows XP if an application
    // manifest specifies use of ComCtl32.dll version 6 or later to enable
    // visual styles.  Otherwise, any window creation will fail.
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // Set this to include all the common control classes you want to use
    // in your application.
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    // of your final executable, you should remove from the following
    // the specific initialization routines you do not need
    // Change the registry key under which our settings are stored
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization
    SetRegistryKey(_T("Local AppWizard-Generated Applications"));
    LoadStdProfileSettings(16);  // Load standard INI file options (including MRU)
    // Register the application's document templates.  Document templates
    //  serve as the connection between documents, frame windows and views
    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        RES_MAINFRAME,
        RUNTIME_CLASS(CTomographicsDoc),
        RUNTIME_CLASS(CMainFrame),       // main SDI frame window
        RUNTIME_CLASS(CTomographicsView));
    if (!pDocTemplate)
        return FALSE;
    AddDocTemplate(pDocTemplate);

    // Enable DDE Execute open
    EnableShellOpen();
    RegisterShellFileTypes(TRUE);

    // Parse command line for standard shell commands, DDE, file open
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // Dispatch commands specified on the command line.  Will return FALSE if
    // app was launched with /RegServer, /Register, /Unregserver or /Unregister.
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    m_pMainWnd->SetWindowPos(NULL, 4, 4, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

    // The one and only window has been initialized, so show and update it
    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();
    // call DragAcceptFiles only if there's a suffix
    //  In an SDI app, this should occur after ProcessShellCommand
    // Enable drag/drop open
    m_pMainWnd->DragAcceptFiles();
    return TRUE;
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// Dialog Data
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnStnClickedMyversion();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    ON_STN_CLICKED(IDC_MYVERSION, &CAboutDlg::OnStnClickedMyversion)
END_MESSAGE_MAP()

class CGlobalFunctions  : public CObject
{
public:
    CGlobalFunctions();
    virtual ~CGlobalFunctions();
public:
    static CString GetFileVersionX();
    static CString GetProductVersionX();
    static CString GetVersionInfo(HMODULE hLib, CString csEntry);
    static CString FormatVersion(CString cs);

private:
    static CString m_csFileVersion;
    static CString m_csProductVersion;
};

CString CGlobalFunctions::m_csFileVersion;
CString CGlobalFunctions::m_csProductVersion;

CGlobalFunctions::CGlobalFunctions()
{

}

CGlobalFunctions::~CGlobalFunctions()
{

}
// This is the key method
CString CGlobalFunctions::GetVersionInfo(HMODULE hLib, CString csEntry)
{
  CString csRet;

  if (hLib == NULL)
    hLib = AfxGetResourceHandle();
  
  HRSRC hVersion = FindResource( hLib, 
    MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION );
  if (hVersion != NULL)
  {
    HGLOBAL hGlobal = LoadResource( hLib, hVersion ); 
    if ( hGlobal != NULL)  
    {  
  
      LPVOID versionInfo  = LockResource(hGlobal);  
      if (versionInfo != NULL)
      {
        DWORD vLen,langD;
        BOOL retVal;    
    
        LPVOID retbuf=NULL;
    
        static char fileEntry[256];

        sprintf(fileEntry,"\\VarFileInfo\\Translation");
        retVal = VerQueryValue(versionInfo,fileEntry,&retbuf,(UINT *)&vLen);
        if (retVal && vLen==4) 
        {
          memcpy(&langD,retbuf,4);            
          sprintf(fileEntry, "\\StringFileInfo\\%02X%02X%02X%02X\\%s",
                  (langD & 0xff00)>>8,langD & 0xff,(langD & 0xff000000)>>24, 
                  (langD & 0xff0000)>>16, csEntry);            
        }
        else 
          sprintf(fileEntry, "\\StringFileInfo\\%04X04B0\\%s", 
            GetUserDefaultLangID(), csEntry);

        if (VerQueryValue(versionInfo,fileEntry,&retbuf,(UINT *)&vLen)) 
          csRet = (char*) retbuf;
      }
    }

    UnlockResource( hGlobal );  
    FreeResource( hGlobal );  
  }

  return csRet;
}

// Re-formats a string formatted as "m,n,o,p" to format as "m.nop"
CString CGlobalFunctions::FormatVersion(CString cs)
{
  CString csRet;
  if (!cs.IsEmpty())
  {
    cs.TrimRight();
    int iPos = cs.Find(',');
    if (iPos == -1)
      return "";
    cs.TrimLeft();
    cs.TrimRight();
    csRet.Format("%s.", cs.GetLeft(iPos));

    while (1)
    {
      cs = cs.GetMid(iPos + 1);
      cs.TrimLeft();
      iPos = cs.Find(',');
      if (iPos == -1)
      {
        csRet +=cs;
        break;
      }
      csRet += cs.GetLeft(iPos);
    }
  }

  return csRet;
}

// Loads "FileVersion" from resource formats it and keeps it in mind
CString CGlobalFunctions::GetFileVersionX()
{
  if (m_csFileVersion.IsEmpty())
  {
    CString csVersion = FormatVersion(GetVersionInfo(NULL, "FileVersion"));
    m_csFileVersion.Format("Version %s (Build %s)", 
      csVersion, GetVersionInfo(NULL, "SpecialBuild"));
  }

  return m_csFileVersion;
}

// Loads "ProductVersion" from resource formats it and keeps it in mind
CString CGlobalFunctions::GetProductVersionX()
{
  if (m_csProductVersion.IsEmpty())
    m_csProductVersion = FormatVersion(GetVersionInfo(NULL, "ProductVersion"));

  return m_csProductVersion;
}


// App command to run the dialog
void CTomographicsApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}

// CTomographicsApp message handlers


BOOL CAboutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here

    /*
    CString cs;
    cs.Format("FileVersion: %s\nProductVersion: %s\nMyPrivateInfo: %s", 
             CGlobalFunctions::GetFileVersionX(), 
             CGlobalFunctions::GetProductVersionX(),
             CGlobalFunctions::GetVersionInfo(NULL, "MyPrivateInfo"));
    GetDlgItem(IDC_MYVERSION)->SetWindowText(cs);
    */

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnStnClickedMyversion()
{
    // TODO: Add your control notification handler code here
}

BOOL CTomographicsApp::PumpMessage()
{
    // TODO: Add your specialized code here and/or call the base class

    BOOL bQuit = !CWinApp::PumpMessage();

	_AFX_THREAD_STATE *pState = AfxGetThreadState();

    UINT nMessage = pState->m_msgCur.message;
	if ((nMessage >= WM_MOUSEFIRST && nMessage <= WM_MOUSELAST)
        ||
        (nMessage >= WM_KEYFIRST && nMessage <= WM_KEYLAST))
    {
        theApp.xx_pTomoDevice->SkipSomeTimers();
    }

    return !bQuit;
}

void CTomographicsApp::EnsureConsoleReady()
{
    if (AllocConsole())
    {
        CRect rectDesktop;
        ::OS_SystemParametersInfo(SPI_GETWORKAREA, 0, &rectDesktop, 0);

        CRect rectConsole;
        ::GetWindowRect(
            GetConsoleWindow(),
            rectConsole);

        ::SetWindowPos(
            GetConsoleWindow(), 
            NULL, 
            rectDesktop.right - rectConsole.Width() - 4, 
            rectDesktop.bottom - rectConsole.Height() - 4, 
            0, 
            0, 
            SWP_NOZORDER | SWP_NOSIZE);

        freopen("CONOUT$", "wt", stdout);

        SetConsoleTitle("Tomograph USB device log console");

        SetConsoleTextAttribute(
            GetStdHandle(STD_OUTPUT_HANDLE), 
            FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

        SetForegroundWindow(GetConsoleWindow());
    }
}