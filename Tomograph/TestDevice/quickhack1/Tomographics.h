//***********************************************
// WARNING: UNSAFE PROTOTYPE CODE TO THROW AWAY!
// Intentionally Cta incompatible! You can not 
// copypaste it into production! Tied to MFC.
// Run as debug so you have buffer checks.
//***********************************************
// Tomographics.h : main header file for the Tomographics application
//
#pragma once

#ifndef __AFXWIN_H__
    #error "include 'Precompiled.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "AboutTomographicsVersion.h"

class CTomoDevice;

// CTomographicsApp:
// See Tomographics.cpp for the implementation of this class
//

class CTomographicsApp : public CWinApp
{
public:
    CTomographicsApp();

// Device controller

    CTomoDevice* xx_pTomoDevice;

    void EnsureConsoleReady();

// Overrides
public:
    virtual BOOL InitInstance();

// Implementation
    afx_msg void OnAppAbout();
    DECLARE_MESSAGE_MAP()
    virtual BOOL PumpMessage();
};

extern CTomographicsApp theApp;