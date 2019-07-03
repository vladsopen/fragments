// CArchiveTestLzma.cpp

#include "CSysCommonProject.h"
#include "CArchiveTestLzma.h"
#include "VirtOs_dir.h"
#include "VirtOs_file.h"
#include "CArchiveLzmaSessionIfaceGp.h"
#include "VirtOs_CFileList.h"
#include "CArchiveLzmaOptionsIfaceGp.h"
#include "CArchiveTestCallback.h"
#include "VirtOs_CInterLock.h"
#include "CArchiveBrotliSessionIfaceGp.h"
#include "CArchiveBrotliOptionsIfaceGp.h"

CArchiveTestLzma::CArchiveTestLzma()
{
    _init_AttachToTestLzma();
}

void CArchiveTestLzma::_init_AttachToTestLzma()
{
    //x_r->_x_p = this;
}

void CArchiveTestLzma::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToTestLzma();

    rFAIL("make sure it is OK to expose CArchiveTestLzma!");
}

void CArchiveTestLzma::TestFileToFile()
{
    //
    // Test file compression
    //

    path pathTestRoot = 
        sys::GGetTempFolder().
            GetAppendedPath("TestLzma");
    str sDeleteError;
    dir(pathTestRoot).EraseDirCompletely(out sDeleteError);
    rASSERT(dir(pathTestRoot).GetFiles()->GetCount() == 0);

    path pathSource = pathTestRoot.GetAppendedPath("Source.txt");
    path pathArchive = pathTestRoot.GetAppendedPath("Archive.lzma");
    path pathUnpack = pathTestRoot.GetAppendedPath("Unpack.txt");

    str sContent = "Test compress this with LZMA wrapper!";
    if (m_sForceSource != "")
    {
        sContent = m_sForceSource;
    }
    str sError;
    file(pathSource).Save(sContent, out sError);
    rASSERTM(sError == "", sError);

    // Compress
    {
        ref<CArchiveLzmaSessionIfaceGp> rLzma;
        ref<CArchiveLzmaOptionsIfaceGp> rOptions;

        rLzma->
            RunArchiveFileToFile(
                CArchiveLzmaSessionIfaceGp::E_ArchiveCommand_Compress,
                rOptions,
                pathSource,
                pathArchive,
                out sError);
        GCheckRunError(
            sError, 
            m_sExpectInCompressError, 
            file(pathArchive).IsExist()? "x" : "");
    }

    //
    // Test file decompression
    //

    // DeCompress
    {
        ref<CArchiveLzmaSessionIfaceGp> rLzma;

        rLzma->
            RunArchiveFileToFile(
                CArchiveLzmaSessionIfaceGp::E_ArchiveCommand_Decompress,
                (ptr<CArchiveLzmaOptionsIfaceGp>) null(),
                pathArchive,
                pathUnpack,
                out sError);
        GCheckRunError(
            sError, 
            m_sExpectInDecompressError, 
            file(pathUnpack).IsExist()? "eist" : "");
    }

    // Check
    str sRestored;
    file(pathUnpack).Load(out sRestored, out sError);
    rASSERTM(sError == "", sError);
    rASSERT(sRestored == sContent);
}

void CArchiveTestLzma::TestInMemoryLzma()
{
    //
    // Test in-memory compression
    //

    str sContent = 
        "In-memory compression is only intended for very small blocks. "
            "I want this string to be long enough so that unpacked size would "
            "exceed the archive length.";
    if (m_sForceSource != "")
    {
        sContent = m_sForceSource;
    }

    sbuf sbufArchive;
    sbuf sbufRestored = StrChar(0, m_nReserveOutputBytes);
    str sError;

    // Compress
    {
        ref<CArchiveLzmaSessionIfaceGp> rLzma;
        ref<CArchiveLzmaOptionsIfaceGp> rOptions;

        sbufArchive = 
            StrChar(
                0, 
                m_nReserveOutputBytes + 
                    (rOptions->x_bCreatePrivateHeader?
                        rLzma->JustGetDefaultCompressedHeaderBytes()
                        :
                        0));

        rLzma->
            RunArchiveInMemory(
                CArchiveLzmaSessionIfaceGp::E_ArchiveCommand_Compress,
                rOptions,
                (sbuf) sContent,
                out sbufArchive,
                out sError);
        GCheckRunError(
            sError, 
            m_sExpectInCompressError, 
            sbufArchive);
    }

    //
    // Test in-memory decompression
    //

    // DeCompress
    {
        ref<CArchiveLzmaSessionIfaceGp> rLzma;

        rLzma->
            RunArchiveInMemory(
                CArchiveLzmaSessionIfaceGp::E_ArchiveCommand_Decompress,
                (ptr<CArchiveLzmaOptionsIfaceGp>) null(),
                sbufArchive,
                out sbufRestored,
                out sError);
        GCheckRunError(
            sError, 
            m_sExpectInDecompressError,
            sbufRestored);
    }

    // Check
    rASSERT(sbufRestored == sContent || m_bExpectDecodeMatchFailure);
}

void CArchiveTestLzma::TestInMemoryBrotli()
{
    //
    // Test in-memory compression
    //

    str sContent = 
        "In-memory compression is only intended for very small blocks. "
            "I want this string to be long enough so that unpacked size would "
            "exceed the archive length.";
    if (m_sForceSource != "")
    {
        sContent = m_sForceSource;
    }

    sbuf sbufArchive;
    sbuf sbufRestored = StrChar(0, m_nReserveOutputBytes);
    str sError;

    // Compress
    {
        ref<CArchiveBrotliSessionIfaceGp> rBrotli;
        ref<CArchiveBrotliOptionsIfaceGp> rOptions;

        rOptions->x_nBrotliQuality = 0;
        //rOptions->x_nBrotliDictionaryFactor = ;

        /*
        sbufArchive = 
            StrChar(
                0, 
                m_nReserveOutputBytes + 
                    (rOptions->x_bCreatePrivateHeader?
                        rBrotli->JustGetDefaultCompressedHeaderBytes()
                        :
                        0));
                        */

        rBrotli->
            RunArchiveInMemory(
                CArchiveBrotliSessionIfaceGp::E_ArchiveCommand_Compress,
                rOptions,
                (sbuf) sContent,
                out sbufArchive,
                out sError);
        GCheckRunError(
            sError, 
            m_sExpectInCompressError, 
            sbufArchive);
    }

    //
    // Test in-memory decompression
    //

    // DeCompress
    {
        ref<CArchiveBrotliSessionIfaceGp> rBrotli;

        rBrotli->
            RunArchiveInMemory(
                CArchiveBrotliSessionIfaceGp::E_ArchiveCommand_Decompress,
                (ptr<CArchiveBrotliOptionsIfaceGp>) null(),
                sbufArchive,
                out sbufRestored,
                out sError);
        GCheckRunError(
            sError, 
            m_sExpectInDecompressError,
            sbufRestored);
    }

    // Check
    rASSERT(sbufRestored == sContent || m_bExpectDecodeMatchFailure);
}

void CArchiveTestLzma::TestCallback()
{
    //
    // Test in-memory compression
    //

    str sContent = 
        "Callback archiving is only useful for large data blocks. "
            "When compression takes a long time and we cannot fit all data "
            "into RAM our callback allows us to stream files block by block "
            "tracking procedure progress.";
    if (m_sForceSource != "")
    {
        sContent = m_sForceSource;
    }

    sbuf sbufArchive;
    sbuf sbufRestored;
    str sError;

    // Compress
    {
        ref<CArchiveLzmaSessionIfaceGp> rLzma;
        ref<CArchiveLzmaOptionsIfaceGp> rOptions;
        ref<CArchiveTestCallback> rArchiveTestCallback = 
            m_rForceInputArchiveTestCallback; // adds error emulation, 
                // real app does not need to store a member callback ref

        rArchiveTestCallback->x_sbufInput = (sbuf) sContent;

        rLzma->
            RunArchiveWithCallback(
                CArchiveLzmaSessionIfaceGp::E_ArchiveCommand_Compress,
                rOptions,
                rArchiveTestCallback->x_sbufInput.Get().Len(),
                rArchiveTestCallback,
                out sError);
        sbufArchive = rArchiveTestCallback->x_sbufOutput;

        GCheckRunError(
            sError, 
            m_sExpectInCompressError, 
            // oops! we (app) have to clear dead data manually!
            sError != ""? sbuf() : sbufArchive);
    }

    //
    // Test in-memory decompression
    //

    // DeCompress
    {
        ref<CArchiveLzmaSessionIfaceGp> rLzma;
        ref<CArchiveTestCallback> rArchiveTestCallback =
            m_rForceOutputArchiveTestCallback; // adds error emulation, 
                // real app does not need to store a member callback ref

        rArchiveTestCallback->x_sbufInput = sbufArchive;

        rLzma->
            RunArchiveWithCallback(
                CArchiveLzmaSessionIfaceGp::E_ArchiveCommand_Decompress,
                (ptr<CArchiveLzmaOptionsIfaceGp>) null(),
                rArchiveTestCallback->x_sbufInput.Get().Len(),
                rArchiveTestCallback,
                out sError);
        sbufRestored = rArchiveTestCallback->x_sbufOutput;

        GCheckRunError(
            sError, 
            m_sExpectInDecompressError,
            // oops! we (app) have to clear dead data manually!
            sError != ""? sbuf() : sbufRestored);
    }

    // Check
    rASSERT(sbufRestored == sContent || m_bExpectDecodeMatchFailure);
}

void CArchiveTestLzma::TestPiecemeal()
{
    //
    // Test in-memory compression
    //

    str sContent = 
        "Callback archiving is only useful for large data blocks. "
            "When compression takes a long time and we cannot fit all data "
            "into RAM our callback allows us to stream files block by block "
            "tracking procedure progress.";
    if (m_sForceSource != "")
    {
        sContent = m_sForceSource;
    }

    sbuf sbufArchive;
    sbuf sbufRestored;
    str sError;

    // Compress
    {
        ref<CArchiveLzmaSessionIfaceGp> rLzma;
        ref<CArchiveLzmaOptionsIfaceGp> rOptions;
        ref<CArchiveTestCallback> rArchiveTestCallback = 
            m_rForceInputArchiveTestCallback; // adds error emulation, 
                // real app does not need to store a member callback ref

        rArchiveTestCallback->x_sbufInput = (sbuf) sContent;

        rLzma->
            RunArchiveWithCallback(
                CArchiveLzmaSessionIfaceGp::E_ArchiveCommand_Compress,
                rOptions,
                rArchiveTestCallback->x_sbufInput.Get().Len(),
                rArchiveTestCallback,
                out sError);
        sbufArchive = rArchiveTestCallback->x_sbufOutput;

        GCheckRunError(
            sError, 
            m_sExpectInCompressError, 
            // oops! we (app) have to clear dead data manually!
            sError != ""? sbuf() : sbufArchive);
    }

    //
    // Test in-memory decompression
    //

    // DeCompress
    {
        ref<CArchiveLzmaSessionIfaceGp> rLzma;
        ref<CArchiveTestCallback> rArchiveTestCallback =
            m_rForceOutputArchiveTestCallback; // adds error emulation, 
                // real app does not need to store a member callback ref

        rArchiveTestCallback->x_sbufInput = sbufArchive;

        rLzma->
            RunArchiveWithCallback(
                CArchiveLzmaSessionIfaceGp::E_ArchiveCommand_Decompress,
                (ptr<CArchiveLzmaOptionsIfaceGp>) null(),
                rArchiveTestCallback->x_sbufInput.Get().Len(),
                rArchiveTestCallback,
                out sError);
        sbufRestored = rArchiveTestCallback->x_sbufOutput;

        GCheckRunError(
            sError, 
            m_sExpectInDecompressError,
            // oops! we (app) have to clear dead data manually!
            sError != ""? sbuf() : sbufRestored);
    }

    // Check
    rASSERT(sbufRestored == sContent || m_bExpectDecodeMatchFailure);
}

void CArchiveTestLzma::GCheckRunError(
        str sError, 
        str sExpectInError,
        str sOutput)
{
    if (sExpectInError == "")
    {
        rASSERTM(sError == "", sError);
        rASSERT(sOutput.Len() > 0);
    }
    else
    {
        rASSERTM(
            sError.FindPos(sExpectInError) >= 0,
            "Result: \"" + sError + 
                "\" does not match expected \"" + 
                sExpectInError + 
                "\""); 
        rASSERT(sOutput.Len() == 0);
    }
}

void CArchiveTestLzma::GTestLzmas()
{
    CInterLock lock(
        CInterLock::SYNC,
        "TestLzma",
        30000);

    str sLargeSource;
    int nLarge = 301 * 1000;
    sLargeSource.ReserveAlloc(nLarge);
    repeat(i, nLarge - 1)
    {
        sLargeSource.AppendChar(char(i ^ (i >> 7) ^ (i >> 12)));
    }

    //
    // File-->File
    //

    {
        ref<CArchiveTestLzma> rTestLzma;

        rTestLzma->m_bEncrypt = false;

        rTestLzma->TestFileToFile();
    }

    {
        ref<CArchiveTestLzma> rTestLzma;

        rTestLzma->m_bEncrypt = true;

        rTestLzma->TestFileToFile();
    }

    // Large file
    {
        ref<CArchiveTestLzma> rTestLzma;

        rTestLzma->m_bEncrypt = false;
        rTestLzma->m_sForceSource = sLargeSource;
        rTestLzma->m_nReserveOutputBytes = sLargeSource.Len();

        rTestLzma->TestFileToFile();
    }

    //
    // Callback
    //

    {
        ref<CArchiveTestLzma> rTestLzma;

        rTestLzma->m_bEncrypt = true;

        rTestLzma->TestCallback();
    }

    {
        ref<CArchiveTestLzma> rTestLzma;

        rTestLzma->m_bEncrypt = false;
        rTestLzma->m_sForceSource = sLargeSource;
        rTestLzma->m_nReserveOutputBytes = sLargeSource.Len();

        rTestLzma->TestCallback();
    }

    {
        ref<CArchiveTestLzma> rTestLzma;

        rTestLzma->m_bEncrypt = true;
        rTestLzma->m_rForceInputArchiveTestCallback->x_sForceErrorOnRead = 
            "My demo read error.";
        rTestLzma->m_sExpectInCompressError = 
            "My demo read error.";
        rTestLzma->m_sExpectInDecompressError = "Data format error";
        rTestLzma->m_bExpectDecodeMatchFailure = true;

        rTestLzma->TestCallback();
    }

    {
        ref<CArchiveTestLzma> rTestLzma;

        rTestLzma->m_bEncrypt = false;
        rTestLzma->m_rForceOutputArchiveTestCallback->x_sForceErrorOnWrite = 
            "My demo write error.";
        rTestLzma->m_sExpectInDecompressError = 
            "My demo write error.";
        rTestLzma->m_bExpectDecodeMatchFailure = true;

        rTestLzma->TestCallback();
    }

    //
    // RAM-->RAM
    //

    {
        ref<CArchiveTestLzma> rTestLzma;

        rTestLzma->m_bEncrypt = false;

        rTestLzma->TestInMemoryLzma();
    }

    {
        ref<CArchiveTestLzma> rTestLzma;

        rTestLzma->m_bEncrypt = true;

        rTestLzma->TestInMemoryLzma();
    }

    {
        ref<CArchiveTestLzma> rTestLzma;

        rTestLzma->TestInMemoryBrotli();
    }

    {
        ref<CArchiveTestLzma> rTestLzma;

        rTestLzma->m_bEncrypt = true;
        rTestLzma->m_bEncryptWithHeader = true;

        rTestLzma->TestInMemoryLzma();
    }

    // Large buffer
    {
        ref<CArchiveTestLzma> rTestLzma;

        rTestLzma->m_bEncrypt = true;
        rTestLzma->m_sForceSource = sLargeSource;
        rTestLzma->m_nReserveOutputBytes = sLargeSource.Len();
        
        rTestLzma->TestInMemoryLzma();
    }

    // Overflow only in unpack
    {
        ref<CArchiveTestLzma> rTestLzma;

        rTestLzma->m_bEncrypt = false;
        rTestLzma->m_nReserveOutputBytes = 148;

        //rTestLzma->m_sExpectInCompressError = "Cannot write output";
        rTestLzma->m_sExpectInDecompressError = "Cannot write output";
        rTestLzma->m_bExpectDecodeMatchFailure = true;

        rTestLzma->TestInMemoryLzma();
    }
    {
        ref<CArchiveTestLzma> rTestLzma;

        rTestLzma->m_bEncrypt = false;
        rTestLzma->m_nReserveOutputBytes = 148;

        //rTestLzma->m_sExpectInCompressError = "Cannot write output";
        rTestLzma->m_sExpectInDecompressError = "Insufficient unzip alloc";
        rTestLzma->m_bExpectDecodeMatchFailure = true;

        rTestLzma->TestInMemoryBrotli();
    }

    // Overflow both ways
    {
        ref<CArchiveTestLzma> rTestLzma;

        rTestLzma->m_bEncrypt = true;
        rTestLzma->m_nReserveOutputBytes = 20;

        rTestLzma->m_sExpectInCompressError = "Cannot write output";
        rTestLzma->m_sExpectInDecompressError = "Input EOF";
        rTestLzma->m_bExpectDecodeMatchFailure = true;

        rTestLzma->TestInMemoryLzma();
    }
    {
        ref<CArchiveTestLzma> rTestLzma;

        rTestLzma->m_bEncrypt = true;
        rTestLzma->m_nReserveOutputBytes = 20;

        rTestLzma->m_sExpectInCompressError = ""; //"Cannot write output";
        rTestLzma->m_sExpectInDecompressError = "Insufficient unzip alloc";
        rTestLzma->m_bExpectDecodeMatchFailure = true;

        rTestLzma->TestInMemoryBrotli();
    }

    //
    // Piecemeal
    //

    /*
    // Overflow both ways
    {
        ref<CArchiveTestLzma> rTestLzma;

        rTestLzma->m_bEncrypt = true;
        rTestLzma->m_nReserveOutputBytes = 20;

        rTestLzma->m_sExpectInCompressError = "Cannot write output";
        rTestLzma->m_sExpectInDecompressError = "Input EOF";
        rTestLzma->m_bExpectDecodeMatchFailure = true;

        rTestLzma->TestInMemory();
    }
    */
}

void CArchiveTestLzma::OnTestClass()
{
#ifndef ONLYPORT
    GTestLzmas();
#endif
}
