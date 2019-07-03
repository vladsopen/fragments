// CFakeTomographMemory.h
#pragma once

class CTomographEmulator;
class CFakeTomograph;
class CTomogramIface;

//
// CFakeTomographMemory - 
//
//   Tomogram list stored in emulator "flash-memory"
//
//ASSUME_IMPL_FOR_IFACE(CFakeTomographMemoryImpl, CFakeTomographMemory)

class CFakeTomographMemory : public object
{
public:
    CFakeTomographMemory();
    //NEW_LOCAL(CFakeTomographMemoryImpl, CFakeTomographMemory)
    //NEW_GP(MyProject, CFakeTomographMemoryImpl, CFakeTomographMemory)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    static const path C_pathMemoryFolderName = "EmulatorMemory";
    static const path C_pathMemoryFolderForClassTestName = "ClassTestMemory";
    static const path C_pathMemoryFileExt = ".fakeflash";

// Attributes

    // Current selection
    int x_iEditTomogram = 0
            xauto(Get)
            xassert(value >= 0 && (value < GetTomogramCount() || GetTomogramCount() == 0));
    void x_iEditTomogram_Set(
            int value);

    // We use a diffrenent storage for UnitTests
    bool IsClassTestModeStorage();

// array<> Publishing

    int GetTomogramCount()
        { return m_aTomogram.GetCount(); }
    void SetTomogramCount(
            int nNewSize);
    ref<CTomogramIface> GetTomogramAt(
            int iAt);
    ref<CTomogramIface> GetTomogramAtOr(
            int iAt,
            ref<CTomogramIface> rDefaultTomogram);
    void SetTomogramAt(
            int iAt, 
            ref<CTomogramIface> value);
    void AddTomogram(
            ref<CTomogramIface> rTomogram);
    // void InsertAt(int iAt, ref<CTomogramIface> value)
    //     { m_aTomogram.InsertAt(iAt, value); }
    // void RemoveAt(int iAt, int nCount = 1)
    //     { m_aTomogram.RemoveAt(iAt, nCount); }
    // void RemoveAll()
    //     { m_aTomogram.RemoveAll(); }
    bool IterateTomograms(
            out iter& out_i, 
            out ref<CTomogramIface>& out_value);
    // see also IterateBackwards()

    int FindExistingTomogramPos(
            ref<CTomogramIface> rTomogram);

// Operations

    // File ops
    void SaveFakeTomographMemory(
            ref<CTomographEmulator> rTomographEmulator);
    void LoadFakeTomographMemory(
            ref<CTomographEmulator> rTomographEmulator);
    void EnsureFakeTomographMemoryLoaded(
            ref<CTomographEmulator> rTomographEmulator);

    // Explorer button handler
    void OpenExplorerForFlashFolder(
            ref<CTomographEmulator> rTomographEmulator);

// UI

    // FakeMemoryList
    ref<UiContainer> NewUiFakeMemoryList(
            ref<CFakeTomograph> rFakeTomograph,
            ref<CTomographEmulator> rTomographEmulator);
    ptr<CUiPanelGp> _m_pUiFakeMemoryList;

    // FakeMemorySelectionAttributes
    ref<UiContainer> NewUiFakeMemorySelectionAttributes();
    ptr<CUiPanelGp> _m_pUiFakeMemorySelectionAttributes;

    // Try to set a different position from input box
    void ReceiveNewPositionForTomogram(
            ref<CTomogramIface> rTomogram,
            int iNewPos);

    // Valid flag on/off
    void TriggerClearSample();

    // FakeMemorySampleSettings
    ref<UiContainer> NewUiFakeMemorySampleSettings();
    ptr<CUiPanelGp> _m_pUiFakeMemorySampleSettings;

    // FakeMemoryGraph
    ref<UiContainer> NewUiFakeMemoryGraph(
            ref<CFakeTomograph> rFakeTomograph);
    ptr<CUiPanelGp> _m_pUiFakeMemoryGraph;

    // Invalidate paint and marker positions
    void UpdateTomogramGraphAfterCurveEdit();
    void UpdateTomogramGraphAfterMarkerEdit();

    // FakeMemoryRow
    ref<UiContainer> NewUiFakeMemoryRow(
            ref<CTomogramIface> rTomogram);
    ptr<CUiPanelGp> _m_pUiFakeMemoryRow;

protected:
private:
    array< ref<CTomogramIface> > m_aTomogram;

    // only after first load we have some useful data here
    bool _m_bFakeFilesLoaded = false;

    // Save/load folder
    path GetFakeTomographMemoryFolder();

    // Make a descriptive name
    path ComposeBaseFileNameForTomogram(
            ref<CTomogramIface> rTomogram);

    // Save a memory slot
    bool SaveTomogramToEmulatorFile(
            ref<CTomogramIface> rTomogram);

    // Exchange cell data positions
    void MoveTomogramSwappingPosition(
            ref<CTomogramIface> rTomogram,
            int iNewPos);

    // Preallocate storage
    void _init_AllocateEmptyFlashArray();

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiFakeMemoryList.cpp)
class UiFakeMemoryList : public UiContainer
{
public:
    virtual void Ui();

    ref<CFakeTomographMemory> m_rFakeTomographMemory;
    ref<CFakeTomograph> m_rFakeTomograph;
    ref<CTomographEmulator> m_rTomographEmulator;
};

// (UiFakeMemorySelectionAttributes.cpp)
class UiFakeMemorySelectionAttributes : public UiContainer
{
public:
    virtual void Ui();

    ref<CFakeTomographMemory> m_rFakeTomographMemory;
};

// (UiFakeMemoryGraph.cpp)
class UiFakeMemoryGraph : public UiContainer
{
public:
    virtual void Ui();

    ref<CFakeTomographMemory> m_rFakeTomographMemory;
    ref<CFakeTomograph> m_rFakeTomograph;
};

// (UiFakeMemoryRow.cpp)
class UiFakeMemoryRow : public UiContainer
{
public:
    virtual void Ui();

    ref<CFakeTomographMemory> m_rFakeTomographMemory;
    ref<CTomogramIface> m_rTomogram;
};

// (UiFakeMemorySampleSettings.cpp)
class UiFakeMemorySampleSettings : public UiContainer
{
public:
    virtual void Ui();

    ref<CFakeTomographMemory> m_rFakeTomographMemory;
};
