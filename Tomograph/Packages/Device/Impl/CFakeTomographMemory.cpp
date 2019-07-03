// CFakeTomographMemory.cpp

#include "CProject.h"
#include "CFakeTomographMemory.h"
#include "VirtOs_dir.h"
#include "VirtOs_CFileList.h"
#include "CTomographUsb.h"
#include "CTomogramIface.h"
#include "CAppEventFloaterIfaceGp.h"
#include "CCurveIface.h"
#include "CFakeTomograph.h"
#include "CTomographEmulator.h"

CFakeTomographMemory::CFakeTomographMemory()
{
}

void CFakeTomographMemory::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CFakeTomographMemory!");
}

void CFakeTomographMemory::x_iEditTomogram_Set(
        int value)
{
    ASSERT((value >= 0 && value < GetTomogramCount()) || GetTomogramCount() == 0);

    if (value == this->x_iEditTomogram)
        return;

    // change selection
    this->x_iEditTomogram = value;

    // prepare Tomogram view
    ref<CTomogramIface> rTomogram = GetTomogramAt(x_iEditTomogram);
    //rTomogram->AllocateAllCurvePoints();

    // update ui
    _m_pUiFakeMemoryList.DefaultIfNull()->InvalidateContent();
    _m_pUiFakeMemorySelectionAttributes.DefaultIfNull()->InvalidateContent();
    _m_pUiFakeMemorySampleSettings.DefaultIfNull()->InvalidateContent();
    _m_pUiFakeMemoryGraph.DefaultIfNull()->InvalidateContent();
}

bool CFakeTomographMemory::IsClassTestModeStorage()
{
    if (debug::GIsTest())
        return true;

    if (debug("Device.EmulateFlashForClassTests?").InputBool())
        return true;

    return false;
}

path CFakeTomographMemory::GetFakeTomographMemoryFolder()
{
    return 
        CProject::GGetIt().
            GetSharedStorageForRewritableDemoEmulators().
                GetAppendedPath(
                    !IsClassTestModeStorage()?
                        C_pathMemoryFolderName
                        :
                        C_pathMemoryFolderForClassTestName);
}

void CFakeTomographMemory::SaveFakeTomographMemory(
        ref<CTomographEmulator> rTomographEmulator)
{
    rTomographEmulator;

    // Kill old files (exisitng Tomograms may save at a different file name)
    str sErrorCleaningExistingFakeFlash;
    dir(GetFakeTomographMemoryFolder()).EraseDirCompletely(out sErrorCleaningExistingFakeFlash);
    rASSERT(dir(GetFakeTomographMemoryFolder()).GetFiles()->GetCount() == 0);

    // Save current Tomograms
    ref<CTomogramIface> rIterTomogram;
    for (iter i; this->IterateTomograms(out i, out rIterTomogram);)
    {
        SaveTomogramToEmulatorFile(rIterTomogram);
    }
}

bool CFakeTomographMemory::SaveTomogramToEmulatorFile(
        ref<CTomogramIface> rTomogram)
{
    path pathCanonical = 
        GetFakeTomographMemoryFolder().
            GetAppendedPath(ComposeBaseFileNameForTomogram(rTomogram)).
            GetAppendedPath(C_pathMemoryFileExt);

    str sTomogramUnidef = rTomogram->ExposeOut().ExportAsBinary();

    str sError;
    if (debug::GIsTest())
    {
        // do not save temporary changes
    }
    else
    if (!file(pathCanonical).Save(sTomogramUnidef, out sError))
    {
        CProject::GGetProjectWarningFloater()->
            LogNewEvent(
                "Failed to save an emulator data file",
                pathCanonical + " " + sError + " /ERR:FKSF/");

        return false;
    }

    return true;
}

void CFakeTomographMemory::LoadFakeTomographMemory(
        ref<CTomographEmulator> rTomographEmulator)
{
    // save exisiting data first
    //   cannot do this - user may have deleted some files
    //SaveFakeTomographMemory();

    // Clear exisitng data
    m_aTomogram.SetCount(0);

    // working dir
    dir dirMemory(GetFakeTomographMemoryFolder());

    // make sure the folder exists
    str sError;
    dirMemory.CreateDir(out sError);

    // read files
    ref<CFileList> rFileList = 
        dirMemory.
            GetFilesByMask(
                path("*").GetAppendedPath(C_pathMemoryFileExt));

    // scan files
    file fileIter;
    for (iter i; rFileList->Iterate(out i, out fileIter);)
    {
        path pathFull = fileIter.GetFullPath();
        path pathName = pathFull.GetFileNameWithoutExt();

        // Load
        ref<CTomogramIface> rTomogram;
        if (rTomogram->
                LoadTomogramFromExposedFileOrUseUnidef(
                    pathFull, 
                    unidef()))
        {
            // Update tomogram name from actual file name
            rTomogram->x_sTomogramUniqueName = pathFull.GetFileNameWithoutExt();

            // Ensure the length
            rTomogram->
                AllocateAllCurvePoints(
                    rTomographEmulator->
                        x_nTomographSamplePointCount);

            // Add to the 'memory'
            AddTomogram(rTomogram);
        }
    }

    _m_pUiFakeMemoryList.DefaultIfNull()->InvalidateContent();
    _m_pUiFakeMemorySelectionAttributes.DefaultIfNull()->InvalidateContent();
    _m_pUiFakeMemorySampleSettings.DefaultIfNull()->InvalidateContent();
    _m_pUiFakeMemoryGraph.DefaultIfNull()->InvalidateContent();

    _m_bFakeFilesLoaded = true;
}

void CFakeTomographMemory::EnsureFakeTomographMemoryLoaded(
        ref<CTomographEmulator> rTomographEmulator)
{
    if (!_m_bFakeFilesLoaded)
    {
        LoadFakeTomographMemory(
            rTomographEmulator);

        ASSERT(_m_bFakeFilesLoaded);
    }
}

void CFakeTomographMemory::OpenExplorerForFlashFolder(
        ref<CTomographEmulator> rTomographEmulator)
{
    SaveFakeTomographMemory(rTomographEmulator);

    sys::GOpenExplorer(
        GetFakeTomographMemoryFolder(),
        sys::E_ExplorerView_Simple,
        sys::E_ExplorerOpen_Folder);
}

void CFakeTomographMemory::SetTomogramAt(
        int iAt, 
        ref<CTomogramIface> value)
{
    m_aTomogram.SetAt(iAt, value);

    _m_pUiFakeMemoryList.DefaultIfNull()->InvalidateContent();
}

ref<CTomogramIface> CFakeTomographMemory::GetTomogramAt(int iAt)
{ 
    ref<CTomogramIface> rTomogram = m_aTomogram.GetAt(iAt);

    return rTomogram;
}

ref<CTomogramIface> CFakeTomographMemory::GetTomogramAtOr(
        int iAt,
        ref<CTomogramIface> rDefaultTomogram)
{ 
    if (iAt < 0 || iAt >= m_aTomogram.GetCount())
    {
        return rDefaultTomogram;
    }

    ref<CTomogramIface> rTomogram = m_aTomogram.GetAt(iAt);

    return rTomogram;
}

void CFakeTomographMemory::AddTomogram(
        ref<CTomogramIface> rTomogram)
{
    ASSERT(m_aTomogram.Find(rTomogram) == -1);

    m_aTomogram.Add(rTomogram);

    _m_pUiFakeMemoryList.DefaultIfNull()->InvalidateContent();
}

int CFakeTomographMemory::FindExistingTomogramPos(
        ref<CTomogramIface> rTomogram)
{
    int iPos = m_aTomogram.Find(rTomogram);
    rASSERT(iPos != -1);
    return iPos;
}

bool CFakeTomographMemory::IterateTomograms(
        out iter& out_i, 
        out ref<CTomogramIface>& out_value)
{ 
    if (m_aTomogram.Iterate(out out_i, out out_value))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CFakeTomographMemory::SetTomogramCount(
        int nNewSize)
{ 
    m_aTomogram.SetCount(nNewSize); 

    if (x_iEditTomogram >= nNewSize)
    {
        x_iEditTomogram = 0;
    }

    // update ui
    _m_pUiFakeMemoryList.DefaultIfNull()->InvalidateContent();
    _m_pUiFakeMemorySelectionAttributes.DefaultIfNull()->InvalidateTreeState();
    _m_pUiFakeMemoryGraph.DefaultIfNull()->InvalidateContent();
}

ref<UiContainer> CFakeTomographMemory::NewUiFakeMemoryList(
        ref<CFakeTomograph> rFakeTomograph,
        ref<CTomographEmulator> rTomographEmulator)
{
    ref<UiFakeMemoryList> rUi;

    rUi->m_rFakeTomographMemory = this;
    rUi->m_rFakeTomograph = rFakeTomograph;
    rUi->m_rTomographEmulator = rTomographEmulator;

    return rUi;
}

ref<UiContainer> CFakeTomographMemory::NewUiFakeMemorySelectionAttributes()
{
    ref<UiFakeMemorySelectionAttributes> rUi;

    rUi->m_rFakeTomographMemory = this;

    return rUi;
}

ref<UiContainer> CFakeTomographMemory::NewUiFakeMemoryGraph(
        ref<CFakeTomograph> rFakeTomograph)
{
    ref<UiFakeMemoryGraph> rUi;

    rUi->m_rFakeTomographMemory = this;
    rUi->m_rFakeTomograph = rFakeTomograph;

    return rUi;
}

void CFakeTomographMemory::UpdateTomogramGraphAfterCurveEdit()
{
    _m_pUiFakeMemoryGraph.DefaultIfNull()->InvalidatePaint();
}

void CFakeTomographMemory::UpdateTomogramGraphAfterMarkerEdit()
{
    _m_pUiFakeMemoryGraph.DefaultIfNull()->InvalidateContent();
}

ref<UiContainer> CFakeTomographMemory::NewUiFakeMemoryRow(
        ref<CTomogramIface> rTomogram)
{
    ref<UiFakeMemoryRow> rUi;

    rUi->m_rFakeTomographMemory = this;
    rUi->m_rTomogram = rTomogram;

    return rUi;
}

void CFakeTomographMemory::ReceiveNewPositionForTomogram(
        ref<CTomogramIface> rTomogram,
        int iNewPos)
{
    if failed(iNewPos >= 0 && iNewPos < GetTomogramCount())
        return;

    // Do we move up or down the list?
    int iOldPos = m_aTomogram.Find(rTomogram);
    if failed(iOldPos != -1)
        return;

    if (iNewPos == iOldPos)
        return;

    if failed(m_aTomogram.GetAt(iOldPos) == rTomogram)
        return;

    MoveTomogramSwappingPosition(
        rTomogram,
        iNewPos);
}

void CFakeTomographMemory::MoveTomogramSwappingPosition(
        ref<CTomogramIface> rTomogram,
        int iNewPos)
{
    int iOldPos = m_aTomogram.Find(rTomogram);
    if failed(iOldPos != -1)
        return;

    if (iOldPos == iNewPos)
        return;

    // Try exact match first
    ref<CTomogramIface> rExistingTomogram = 
        GetTomogramAt(iNewPos);

    // Move to the new position
    SetTomogramAt(iNewPos, rTomogram);
    SetTomogramAt(iOldPos, rExistingTomogram);

    // Update selection info
    if (x_iEditTomogram == iOldPos)
        x_iEditTomogram = iNewPos;
}

void CFakeTomographMemory::TriggerClearSample()
{
    ref<CTomogramIface> rTomogram = 
        GetTomogramAt(
            x_iEditTomogram);

    // leave picture data alone for now

    _m_pUiFakeMemorySelectionAttributes.DefaultIfNull()->InvalidateContent();
    _m_pUiFakeMemoryGraph.DefaultIfNull()->InvalidateContent();
}

ref<UiContainer> CFakeTomographMemory::NewUiFakeMemorySampleSettings()
{
    ref<UiFakeMemorySampleSettings> rUi;

    rUi->m_rFakeTomographMemory = this;

    return rUi;
}
