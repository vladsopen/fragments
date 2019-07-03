// CImageTest.cpp

#include "CUsmainProject.h"
#include "CImageTest.h"
#include "CImageImpl.h"
#include "CDocIface.h"
#include "CDocFormatGp.h"
#include "CMatrixFormatGp.h"
#include "CMatrixSliceIface.h"
#include "CSessionIfaceGp.h"
#include "CViewCache.h"
#include "CLocalPaletteConfigIface.h"
#include "CGlobalConfigIface.h"
#include "CGlobalPaletteConfigIface.h"
#include "CColoring.h"
#include "CAmplitudeToColorIndexGp.h"
#include "CColorListGp.h"
#include "CPaletteForSessionIface.h"
#include "CPaletteLimitTypeIface.h"
#include "CTotalTestIface.h"
#include "CStoreIface.h"
#include "CBaselineIface.h"
#include "CRawSlotGp.h"
#include "CAudioSliceGp.h"
#include "CDocImpl.h"
#include "CSendClusterIfaceGp.h"

CImageTest::CImageTest()
{
}

void CImageTest::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CImageTest!");
}

void CImageTest::OnTestClass()
{
    //
    // General Init
    //

    TESTLOG(
        "",
        "\n"
            "=== Starting Image test ===\n\n");

    debug::GMirrorAssertFailuresToLog("");
    assertoff aoPtrNull("!IsNull()");
    assertoff aoNoRawSlot("CRawSlotGp::x_i");

    // doc we need to store input data
    ref<CDocIface> rDoc;

    // to avoid asserts
    //rDoc->x_rRawSlot->x_iScannerChannel = 10;
    //rDoc->x_rRawSlot->x_iScannerCluster = 20;

    // global config (temporary for the test of course!)
    ref<CGlobalConfigIface> rGlobalConfig = 
        CUsmainProject::GGetIt().
            x_rUsmainGlobalConfig;
    ref<CGlobalPaletteConfigIface> rGlobalPaletteConfig = 
        rGlobalConfig->_x_rGlobalPaletteConfig;

    // The Image We Test
    ref<CImageImpl> rImage = rDoc->x_rImage;
    ref<CViewCache> rViewCache = rImage->x_rViewCache;
    ref<CColoring> rColoring = rImage->x_rColoring;
    ref<CLocalPaletteConfigIface> rLocalPaletteConfig = 
        rColoring->x_rLocalPaletteConfig;

    // Store settings
    ref<CStoreIface> rStore = rDoc->x_rStore;
    ref<CBaselineIface> rBaseline = rStore->x_rBaseline;

    // prepare to deep testing
    rImage->_x_bTestLogPalettePaintOperations = true;

    // remove on-media info we dont init
    rViewCache->x_bOnImageControls = false;

    // Fill in init parameters
    ref<CDocFormatGp> rDocFormat;
    ref<CMatrixFormatGp> rMatrixFormat = rDocFormat->x_rMatrixFormat;
    rMatrixFormat->x_nMatrixSliceAmplitudeRange = 11;
    rMatrixFormat->x_nMatrixLength = 6;
    rDocFormat->x_nDefaultViewWidthSlices = 3;

    // init doc
    {
        TESTLOG(
            "",
            "\n"
                "--- Init Doc ---\n");

        assertoff aoIsSessionActive("IsSessionActive()");
        assertoff aoIsHardwareInitOk("IsHardwareInitOk()");

        rDoc->
            InitDocAsRecorder(
                rDocFormat,
                ref<CSessionIfaceGp>()); // no session required for this test
    }

    //rStore->InitStoreRecordingSeconds(-111555);
    ref<CDocImpl> rDocImpl = rDoc;
    rDocImpl->x_rLastSendCluster->x_nCurrentScannerSamplingFreqInHz = 44100;

    // no delays!
    rStore->x_nDelayAheadSlices = 0;

    // baseline
    rBaseline->SetBaselinePos(0, 1);

    //
    // Generate Some Media
    //

    TESTLOG(
        "",
        "\n"
            "--- Generate Test Media ---\n");

    // fill a simple slice
    ref<CMatrixSliceIface> rNewMatrixSlice = 
        rStore->GetNextOnlineMatrixSlice();
    rNewMatrixSlice->
        AllocateSlicePointCount(
            rMatrixFormat->x_nMatrixLength);
    repeat(i, rMatrixFormat->x_nMatrixLength)
    {
        rNewMatrixSlice->
            SetSlicePointAmplitude(
                i,
                i * 2,
                rMatrixFormat);
    }

    // send the media in
    {
        assertoff aoNoPalette("nMappedColors > 1");
        assertoff aoNoBitmap("getHBitmap()");
        assertoff aoNoNotReady("IsReadyToDraw()");
        assertoff aoNoStretchBlt("StretchBlt");
        assertoff aoNoClusterInfo1("x_nCurrentScannerSamplingFreqInHz");
        assertoff aoNoClusterInfo2("int(nDeviceHz / num(nWindowHz)) == 128");

        rDoc->
            WelcomeNextOnlineMatrixSlice(
                rNewMatrixSlice,
                ref<CAudioSliceGp>());
    }

    //
    // UI Render
    //

    TESTLOG(
        "",
        "\n"
            "--- Render UI ---\n");

    // create ui
    ref<UiContainer> rUiImage = rImage->NewUiImageMain();

    assertoff aoNoSlotIndexInit1(
        "CRawSlotGp::x_iScannerChannel");
    assertoff aoNoSlotIndexInit2(
        "CRawSlotGp::x_iScannerCluster");
    //assertoff aoNoAccelMap(
        //"m_aDealCommands.GetCount() > 0");

    rUiImage->CreateFloater();

    // force ui update and repaint
    rImage->_x_sizeTestFixedCacheUi = size(200, 300);
    rUiImage->thispanel()->SetInstantSize(800, 400);
    rUiImage->thispanel()->UpdateContentAndLayoutNow();

    //
    // default palette
    //

    // first update without palette init (B/W)
    {
        assertoff aoNoPalette("nMappedColors > 1");

        rUiImage->thispanel()->UpdatePaint();
        PauseAfterPaletteUiUpdate();
    }

    //
    // colored palette
    // 

    rLocalPaletteConfig->x_bUseBlackAndWhiteMapping = false;

    ref<CAmplitudeToColorIndexGp> rAmplitudeToColorIndex = 
        rGlobalPaletteConfig->x_rAmplitudeToColorIndex;
    ref<CColorListGp> rGammaColorList = 
        rGlobalPaletteConfig->x_rGammaColorList;

    // Gamma graph emulation
    rASSERT(rMatrixFormat->x_nMatrixSliceAmplitudeRange == 11);
    rAmplitudeToColorIndex->Add(6); // [0]
    rAmplitudeToColorIndex->Add(5); // [1]
    rAmplitudeToColorIndex->Add(4); // [2]
    rAmplitudeToColorIndex->Add(4); // [3]
    rAmplitudeToColorIndex->Add(3); // [4]
    rAmplitudeToColorIndex->Add(3); // [5]
    rAmplitudeToColorIndex->Add(3); // [6]
    rAmplitudeToColorIndex->Add(3); // [7]
    rAmplitudeToColorIndex->Add(1); // [8]
    rAmplitudeToColorIndex->Add(1); // [9]
    rAmplitudeToColorIndex->Add(0); // [10]

    // render -------------------------------------------------------------
    rColoring->NotifyPaletteDependedAfterChange();
    rUiImage->thispanel()->UpdatePaint();
    PauseAfterPaletteUiUpdate();
    // ---------------------------------------------------------------------

    //
    // Play with limits
    //

    // small threshold
    rColoring->
        IncreasePaletteLimit(
            type<CPaletteLimitTypeAsThreshold>());

    // saturation increase should have no effect
    rColoring->
        IncreasePaletteLimit(
            type<CPaletteLimitTypeAsSaturation>());

    // render -------------------------------------------------------------
    rColoring->NotifyPaletteDependedAfterChange();
    rUiImage->thispanel()->UpdatePaint();
    PauseAfterPaletteUiUpdate();
    // ---------------------------------------------------------------------

    // small saturation
    rColoring->
        DecreasePaletteLimit(
            type<CPaletteLimitTypeAsSaturation>());

    // render -------------------------------------------------------------
    rColoring->NotifyPaletteDependedAfterChange();
    rUiImage->thispanel()->UpdatePaint();
    PauseAfterPaletteUiUpdate();
    // ---------------------------------------------------------------------

    // larger margins
    rColoring->
        IncreasePaletteLimit(
            type<CPaletteLimitTypeAsThreshold>());
    rColoring->
        DecreasePaletteLimit(
            type<CPaletteLimitTypeAsSaturation>());

    // render -------------------------------------------------------------
    rColoring->NotifyPaletteDependedAfterChange();
    rUiImage->thispanel()->UpdatePaint();
    PauseAfterPaletteUiUpdate();
    // ---------------------------------------------------------------------

    // still larger margins
    rColoring->
        IncreasePaletteLimit(
            type<CPaletteLimitTypeAsThreshold>());
    rColoring->
        DecreasePaletteLimit(
            type<CPaletteLimitTypeAsSaturation>());

    // render -------------------------------------------------------------
    rColoring->NotifyPaletteDependedAfterChange();
    rUiImage->thispanel()->UpdatePaint();
    PauseAfterPaletteUiUpdate();
    // ---------------------------------------------------------------------

    repeat(iSaturate, 50)
    {
        // larger saturation (1) asymmetrical to threshold
        rColoring->
            DecreasePaletteLimit(
                type<CPaletteLimitTypeAsSaturation>());

        // render -------------------------------------------------------------
        rColoring->NotifyPaletteDependedAfterChange();
        rUiImage->thispanel()->UpdatePaint();
        PauseAfterPaletteUiUpdate();
        // --------------------------------------------------------------------
[...]