// CTestPatientLink.cpp

#include "CProject.h"
#include "CTestPatientLink.h"
#include "CLoaderIface.h"
#include "CPatientLinkImpl.h"
#include "CProxyIface.h"
#include "CProxyImpl.h"
#include "CPatientBaseConnectionIfaceGp.h"
#include "CGlobalConfigIface.h"
#include "CPatientSampleIfaceGp.h"
#include "CPatientSampleMap.h"
#include "CPatientEmbedLinkIface.h"

CTestPatientLink::CTestPatientLink()
{
    _init_AttachToTestPatientLink();
}

void CTestPatientLink::_init_AttachToTestPatientLink()
{
    //x_r->_x_p = this;
}

void CTestPatientLink::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToTestPatientLink();

    rFAIL("make sure it is OK to expose CTestPatientLink!");
}

void CTestPatientLink::OnTestClass()
{
    ref<CLoaderIface> rLoader = 
        CProject::GGetIt().
            x_rLoader;

    ref<CGlobalConfigIface> rGlobalConfig =
        CProject::GGetIt().
            x_rGlobalConfig;

    ref<CPatientEmbedLinkIface> rPatientEmbedLink = 
        CProject::GGetIt().
            x_rPatientEmbedLink;

    //
    // PatientLink - prepare to emulate Patients
    //

    TESTLOG(
        "",
        "\n"
            "=== Init fake Patients connection parameters ===\n");

    // Access to Patients Link (for result log)
    ref<CPatientLinkImpl> rPatientLink = rLoader->x_rPatientLink;

    // Access to lower level PatientIo API (for result log)
    ref<CPatientBaseConnectionIfaceGp> rPatientBaseConnection =
        rPatientLink->x_rPatientBaseConnection;

    // Test: Lets log Patients into the global log
    //rPatientBaseConnection->x_sPatientLogName = "";

    // Test: Work as if we are successfully connected to Patients
    rPatientBaseConnection->_x_bEmulatePatientConnectionOkForClassTest = true;
        // no separate standalone logic in app yet, otherwise it got to be
        //   tested for unconnected mode

    // App: Init Patients Connecton
    //rPatientLink->InitPatientConnection();
        // has simple internal once-init check, linear otherwise

    //
    // Start Application 
    //

    TESTLOG(
        "",
        "\n"
            "=== Actual application starts here ===\n\n");

    // This will use fake Patients link to make important decisions
    rLoader->
        StartApplication();

    //
    // For Patients-connected mode we emulate activation call as if it came from Patients itself
    //

    TESTLOG(
        "",
        "\n"
            "=== Now real app would stay hidden and wait for a Patients call ===\n"
            "=== But we won't wait: lets emulate an activation call from Patients ==="
            "\n\n");

    // Test: Handle connection mode
    ASSERT(rPatientLink->IsConnectedToPatient());

    // This should create a session and show the Main Frame
    rPatientBaseConnection->
        EmulateCallbackActivateSamplerApp();

    TESTLOG(
        "",
        "\n"
            "=== by now the app is visible and ready to receive user input ==="
            "\n");

    //
    // First Test Doc Context
    //

    ref<CProxyImpl> rProxy = 
        rLoader->x_rProxy;

    path pathDoc = 
        rProxy->GetSampleFolder().
            GetAppendedPath("123456AsExtPrefix");
    rASSERT(pathDoc != "");

    TESTLOG(
        "",
        "\n"
            "=== Lets emulate the user clicking Save button ==="
            "\n\n");

    // once
    rProxy->
        FinalizeStudy(
            "12345external",
            "r0123",
            ".testext",
            "11-22",
            "33-44",
            "12345");

    /*
    // twice
    rProxy->
        FinalizeStudy();
        */

    // Test normal doc close
    //aTestDoc.GetAt(1)->x_rNewDoc->CloseDoc();

    //
    // Finish
    //

    TESTLOG(
        "",
        "\n"
            "======= Test is complete. Dumping states... =======\n");

    // expose final object states:
    {
        assertoff ao1("make sure it is OK to expose");
        assertoff aoGuiPlus("it may not be safe to expose this GuiPlus class!");

        // skip a line
        TESTLOG(
            "",
            "\n");

        // session dump
        TESTLOG(
            "FinalSession",
            "rProxy: " + 
                rProxy->ExposeOut().ExportAsText() + "\n\n");

        // config dump
        TESTLOG(
            "FinalConfig",
            "GlobalConfig: " + 
                rGlobalConfig->ExposeOut().ExportAsText() + "\n\n");
    }

    // App: Close session
    //rProxy->TerminateProxy();

    //
    // Close Application 
    //

    TESTLOG(
        "",
        ""
            "======= Closing =======\n\n");

    // This will use fake Patients link to make important decisions
    //   (will not really exit in TEST mode I hope...)
    rLoader->CloseProxyAndMainWindow();

    TESTLOG(
        "",
        "\n"
            "--------------- final dumps complete ------------------\n\n");

    //
    // Load media from Patients
    //

    TESTLOG(
        "",
        "\n"
            "=== Lets emulate a request to open a media from Patients ==="
            "\n\n");

    ref<CPatientSampleIfaceGp> rPatientSample;
    rPatientSample->x_pathAttachmentPrefix = 
        pathDoc;

    TESTLOG(
        "",
        "(reusing saved temp file " + 
            rPatientSample->x_pathAttachmentPrefix + 
            " to load from it now)\n");

    // Open
    {
        //assertoff aoViewFailure7(
        //    "value > 0 || (debug::GIsTest() && value == -111555)");

        // Go!
        rPatientBaseConnection->
            EmulateCallbackOpenSample(
                rPatientSample);
    }

    /*
    TESTLOG(
        "",
        ""
            "======= Closing opened sample windows =======\n\n");
    rPatientLink->
        x_rOpenedPatientSampleMap->
            CloseAllViewedProxys();
    */

    TESTLOG(
        "",
        ""
            "======= Closing reopened main window =======\n\n");
[...]