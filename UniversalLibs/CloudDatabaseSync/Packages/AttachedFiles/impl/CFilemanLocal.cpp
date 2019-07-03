// CFilemanLocal.cpp

#include "CProject.h"
//#include "DeclareKeys.h"
#include "CFilemanLocal.h"
#include "CFileClassImpl.h"
#include "CStoreIfaceGp.h"
#include "VirtOs_CPathList.h"
#include "VirtOs_dir.h"
#include "CFileNodeImpl.h"
#include "VirtOs_file.h"
#include "CDbFile.h"
#include "CFilemanNetOut.h"
#include "CFilemanNetCommon.h"
#include "CNetSyncOutIfaceGp.h"

void CFilemanLocal::InitFilemanLocal()
{
    rASSERT(!_m_bOneTimeInitFilemanLocalOk);
    _m_bOneTimeInitFilemanLocalOk = true;
    _m_bFilemanLocalOpened = true;

    // start scan timer
    _m_ontimerLocalScanTimer.
        SetTimeoutInTicks(
            5000,
            this);
}

void CFilemanLocal::CloseFilemanLocal()
{
    if (_m_bFilemanLocalOpened)
    {
        _m_bFilemanLocalOpened = false;

        // stop scan timer
        _m_ontimerLocalScanTimer.
            StopTimer();
    }
}

void CFilemanLocal::ForcePollFilemanLocal(
        out bool& out_bHaveJobs)
{
    TESTLOG(
        "LocalFiles",
        "\n\n"
            "========== Next Poll ===========\n\n");

    // read local directory tree
    RescanLocalFiles(
        out out_bHaveJobs);

    // FSM incomplete files in db
    RescanPendingFiles(
        out out_bHaveJobs);
}

void CFilemanLocal::RescanLocalFiles(
        out bool& out_bHaveJobs)
{
    if (x_sLocalFileScanDisableInfo != "")
    {
        return;
    }

    this->x_nLocalScanCount++;

    path pathScan = GetFilemanLocalRoot();

    str sCreateError;
    dir(pathScan).CreateDir(out sCreateError);

    ref<CPathList> rPathList =
        dir(pathScan).GetFilesAndDirectoriesIncludingSubfolders(oo);

    TESTLOG(
        "LocalFiles",
        "\n\n"
            "----- Local path list " + Str(rPathList->GetCount()) + "....\n\n");

    path pathIter;
    for (
        iter i;
        rPathList->
            Iterate(
                out i,
                out pathIter);)
    {
        // Update db if necessary
        HandleLocalFileScan(
            pathIter,
            pathScan,
            out out_bHaveJobs);
    }
}

str CFilemanLocal::GCleanupFilemanSubDir(
        path pathDir)
{
    rASSERT(pathDir.GetDrive() == "");

    pathDir = pathDir.GetStripBoth(CHARMAP(" \\/"));
    pathDir = pathDir.GetReplaced('\\', '/');

    return pathDir;
}

void CFilemanLocal::PollFileUploadStatus(
        ref<CFileNodeImpl> rFileNode,
        out bool& out_bHaveJobs)
{
    ref<CFileClassImpl> rFileClass = 
        _x_pFileClass.Get().Valid();
    ref<CStoreIfaceGp> rStore = 
        rFileClass->GetCoreStore();

    ref<CDbFile> rDbFile = 
        rFileNode->x_rDbFile;

    path pathLocalFile = 
        rFileNode->GetFileLocalPath();

    // not started?
    if (rDbFile->x_nnFileUploadedBytes < 0)
    {
        // upload is not started yet, check file availability

        // do we have size
        if (rDbFile->x_nnFileBytes == 0)
        {
            // try to update size if file is complete

            big nnTotalBytes = 
                GGetCompleteLocalFileBytes(
                    pathLocalFile);

            if (nnTotalBytes > 0)
            {
                // save file state
                rStore->
                    EnsureLastTraitForNode(
                        rFileNode->GetFilePrimaryKey(),
                        GET_PROPERTY_KEY(CDbFile, x_nnFileBytes),
                        C_keyTieProp,
                        Str(nnTotalBytes),
                        key(),
                        ptr<CActionIfaceGp>());

                // queue for upload
                ref<CFilemanNetCommon> rFilemanNetCommon = 
                    rFileClass->x_rFilemanNetCommon;
                ref<CFilemanNetOut> rFilemanNetOut = 
                    rFilemanNetCommon->x_rFilemanNetOut;

                rFilemanNetOut->
                    StartUploadingLocalFile(
                        pathLocalFile,
                        rFileNode->GetFileDir());

                out_bHaveJobs = true;
            }

            TESTLOG(
                "LocalFiles",
                ""
                    "Detected size " + 
                    Bytes(nnTotalBytes) + 
                    " for \"" + 
                    rFileNode->GetFileName() + 
                    "\".\n");
        }
    }
}

[...]