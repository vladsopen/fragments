// CNetTestParapack.cpp

#include "CNetProject.h"
#include "CNetTestParapack.h"
#include "VirtOs_file.h"
#include "CNetParapackImpl.h"
#include "VirtOs_dir.h"
#include "VirtOs_CPathInfoList.h"
#include "VirtOs_CPathInfo.h"
#include "CFileBankTreeIfaceGp.h"
#include "CFileMultipartSplitIfaceGp.h"
#include "CFileMultipartMergeIfaceGp.h"
#include "VirtOs_text.h"

CNetTestParapack::CNetTestParapack()
{
    _init_AttachToNetTestParapack();
}

void CNetTestParapack::_init_AttachToNetTestParapack()
{
    //x_r->_x_p = this;
}

void CNetTestParapack::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToNetTestParapack();

    rFAIL("make sure it is OK to expose CNetTestParapack!");
}

ref<CPathInfoList> CNetTestParapack::DumpAllFiles(
        path pathTestRoot)
{
    ref<CPathInfoList> rPathInfoList = 
        dir(pathTestRoot).GetPathInfoListIncludingSubfolders(oo);
    rPathInfoList->SortPathsAscending();

    CPathInfoList::DDumpFormat dataDumpFormat = 
        CPathInfoList::DDumpFormat();

    dataDumpFormat.m_nDumpContentFirstBytes = 100;

    str sDump =
        rPathInfoList->
            DumpFilesInDirs(dataDumpFormat);

    sDump =
        sDump.
            GetReplaced(
                pathTestRoot,
                "");

    TESTLOG(
        "",
        "\n" +
            //"[] [] [] [] [] [] [] [] [] [] [] [] [] [] [] [] [] [] [] []\n" + 
            sDump + 
            //"[] [] [] [] [] [] [] [] [] [] [] [] [] [] [] [] [] [] [] []\n"
            "\n");

    return rPathInfoList;
}

void CNetTestParapack::TestBanks(
        bool bSameBank,
        bool bFileMultipart)
{
    path pathTestRoot =
        sys::GGetTempFolder().
            GetAppendedPath(
                (bSameBank? "1bank" : "2banks") +
                "_" +
                (bFileMultipart? "Multi" : "Single"));

    path pathOutbox =
        pathTestRoot.GetAppendedPath("Outbox");
    path pathInbox = 
        pathTestRoot.GetAppendedPath("Inbox");
    path pathOutboxPacked =
        pathTestRoot.GetAppendedPath("OutboxPacked");
    path pathInboxUnpacked = 
        pathTestRoot.GetAppendedPath("InboxUnpacked");

    path pathPlacingPacked = 
        bSameBank? pathOutbox : pathOutboxPacked;

    array<str> asPacketContent =
        array<str>()
            << "Pack this and unpack in parallel."
            << "Binary here: [" + StrChar(0) + "] with more text.";

    ref<CFileBankAbstractIfaceGp> rFakeNetOuboxFileBank = 
        (ref<CFileBankAbstractIfaceGp>) 
        ref<CFileBankTreeIfaceGp>();

    rFakeNetOuboxFileBank->
        InitFileBank(
            pathOutbox);

    if (bFileMultipart)
    {
        ref<CFileMultipartSplitIfaceGp> rSplit;

        path pathToSplit = 
            pathTestRoot.GetAppendedPath("TestSplittable.txt");

        str sError;
        file(pathToSplit).
            Save(asPacketContent.GetAt(1), out sError);
        rASSTR(sError);

        unidef unidefExtraHeader;
        unidefExtraHeader.DefineInt("TestMeta", 12345);

        sError = "";
        rSplit->
            SplitFileIntoParts(
                pathToSplit,
                "", // sRestoreIntoSubdirs
                1000, // nMaxPartBytes
                rFakeNetOuboxFileBank,
                unidefExtraHeader,
                out sError);
        rASSTR(sError);
    }
    else
    {
        repeat(i, asPacketContent.GetCount())
        {
            path pathName = "Test" + Str00(i, 3) + ".txt";
            if (bSameBank)
            {
                path pathFullRecordNameWithExt;
                str sError;
                rFakeNetOuboxFileBank->
                    SaveNextRecordToFileBank(
                        pathName,
                        path(), // pathForceLocation
                        asPacketContent.GetAt(i),
                        out pathFullRecordNameWithExt,
                        out sError);
                rASSTR(sError);

                TESTLOG(
                    "",
                    "\n"
                        "In Outbox bank: " + 
                        pathFullRecordNameWithExt + 
                        "\n");
            }
            else
            {
                str sError;
                file(pathOutbox.GetAppendedPath(pathName)).
                    Save(asPacketContent.GetAt(i), out sError);
                rASSTR(sError);
            }
        }
    }

    TESTLOG(
        "",
        "\n" +
            "---------------------------------------\n"
            "----- Ready to pack to send -----------\n"
            "---------------------------------------\n");

    DumpAllFiles(pathTestRoot);

    ref<CNetParapackImpl> rParapackOut;

    rParapackOut->
        StartNetParapack(
            false, // bUnpack,
            pathOutbox,
            pathPlacingPacked);

    repeat(i, asPacketContent.GetCount())
    {
        rParapackOut->
            HandleParapackPolling();

        TESTLOG(
            "",
            "\n" +
                "---------------------------------------\n"
                "----- Sent " + Str(i) + " -----------\n"
                "---------------------------------------\n");

        DumpAllFiles(pathTestRoot);
    }

    ref<CNetParapackImpl> rParapackIn;

    rParapackIn->
        StartNetParapack(
            true, // bUnpack,
            pathPlacingPacked,
            bSameBank? pathPlacingPacked : pathInboxUnpacked);

    repeat(i, asPacketContent.GetCount())
    {
        rParapackIn->
            HandleParapackPolling();

        TESTLOG(
            "",
            "\n" +
                "---------------------------------------\n"
                "----- Received " + Str(i) + " -----------\n"
                "---------------------------------------\n");

        DumpAllFiles(pathTestRoot);

        if (bFileMultipart)
        {
            ref<CPathInfoList> rPathInfoList = 
                dir(pathOutbox).GetPathInfoListIncludingSubfolders(oo);
            rPathInfoList->SortPathsAscending();

            ref<CPathInfo> rIterPathInfo;
            for (
                iter iScan;
                rPathInfoList->
                    Iterate(out iScan, out rIterPathInfo);)
            {
                if (rIterPathInfo->IsDir())
                {
                    continue;
                }

                assertoff aoPartial("Merge is missing");
                assertoff aoJunk("Merge cannot recognive part file name");

                ref<CFileMultipartMergeIfaceGp> rMerge;

                rMerge->x_bDeletePartFilesAfterSuccessfulMerge = true;

                rMerge->x_pathIgnoreExt = 
                    CNetParapackImpl::C_sParapackExt;

                unidef unidefExtraHeader;

                str sError;
                path pathMerged = 
                    rMerge->
                        TryMergeFileParts(
                            CFileMultipartMergeIfaceGp::E_MergeMode_RecreateSourceFile,
                            rIterPathInfo->GetFullPath(),
                            pathTestRoot.GetAppendedPath("RestoredMultipart"),
                            out unidefExtraHeader,
                            out_IGNORE(unidef), // LateMeta
                            out sError);

                if (sError == "")
                {
                    rASSERT(unidefExtraHeader.LookupInt("TestMeta", 0) == 12345);

                    TESTLOG(
                        "",
                        "\n" +
                            "+++++++++++++++++++++++++++\n"
                            "+++++ MERGE SUCCESS! ++++++\n"
                            "+++++++++++++++++++++++++++\n");

                    DumpAllFiles(pathTestRoot);
                }
                else
                {
                    TESTLOG(
                        "",
                        "\n" +
                            "Merge rejected " +
                            rIterPathInfo->GetFullPath() +
                            "\n   " + 
                            sError + "\n");
                }
            }
        }
    }
}

void CNetTestParapack::OnTestClass()
{
#ifdef ONLYPORT
    return;
#endif

    TESTLOG(
        "",
        "\n\n" +
            "======================================================\n"
            "((((((((((((((((( TWO )))--->>>((((( BANKS )))))))))))\n"
            "======================================================\n"
            "\n");

    TestBanks(
        false, // bSameBank
        false); // bFileMultipart

    TESTLOG(
        "",
        "\n\n" +
            "===============================================\n"
            "((((((((((((((((( -> SAME BANK <- )))))))))))))\n"
            "===============================================\n"
            "\n");

    TestBanks(
        true, // bSameBank
        false); // bFileMultipart

    TESTLOG(
        "",
        "\n\n" +
            "===============================================\n"
            "(( (( (( (( MULTIPART (-> SAME BANK <-) )) )) ))\n"
            "===============================================\n"
            "\n");

    TestBanks(
        true, // bSameBank
        true); // bFileMultipart
}