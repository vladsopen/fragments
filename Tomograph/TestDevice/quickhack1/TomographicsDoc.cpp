//***********************************************
// WARNING: UNSAFE PROTOTYPE CODE TO THROW AWAY!
// Intentionally Cta incompatible! You can not 
// copypaste it into production! Tied to MFC.
// Run as debug so you have buffer checks.
//***********************************************
// TomographicsDoc.cpp : implementation of the CTomographicsDoc class
//

#include "Precompiled.h"
#include "Tomographics.h"

#include "TomographicsDoc.h"
#include "CTomoSample.h"
#include "CWaveFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTomographicsDoc

IMPLEMENT_DYNCREATE(CTomographicsDoc, CDocument)

BEGIN_MESSAGE_MAP(CTomographicsDoc, CDocument)
END_MESSAGE_MAP()

// CTomographicsDoc construction/destruction

CTomographicsDoc::CTomographicsDoc()
{
    // ***** WARNING: REMEMBER SDI OnNewDocument()!!! *****

    xx_bSomeDataReceived = false;
    xx_bLoadedFromFile = false;
}

CTomographicsDoc::~CTomographicsDoc()
{
}

BOOL CTomographicsDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    // TODO: add reinitialization code here
    // (SDI documents will reuse this document)

    xx_apTomoSample.SetCount(0);
    //CTomoSample::s_nMaxAmplitude = 0;
    xx_bSomeDataReceived = false;
    xx_bLoadedFromFile = false;

    xx_sStatusText = 
        "Tomographics Test Version " C_nTomographicsVersionFull "\n\n" 
            C_nTomographicsVersionNews;

    AddNewChannelPair();

    return TRUE;
}

// CTomographicsDoc serialization

void CTomographicsDoc::Serialize(CArchive& ar)
{
    // MFC info
    CFile* pFile = ar.GetFile();
    path pathDoc = pFile->GetFilePath();
    path pathWave = pathDoc + ".wav";

    printf("\n" "Wave file <" + pathWave + "> ");

    // new wave iface
    CWaveFile* pWaveFile = new CWaveFile();

    if (ar.IsStoring())
    {
        // Save first bytes
        if (xx_apTomoSample.GetCount() > 0)
        {
            CString sDump;
            repeat(i, 100)
            {
                int nAmplitude0 = xx_apTomoSample.GetAt(0)->xx_anCurve.GetAt(i);
                int nAmplitude1 = xx_apTomoSample.GetAt(1)->xx_anCurve.GetAt(i);
                CString s;
                s.Format(
                    "\n" "% 10i % 10i  |  % 10x % 10x", 
                    nAmplitude0, nAmplitude1, nAmplitude0, nAmplitude1);
                sDump += s;
            }

            ar << sDump;
        }

        // Wave
        printf("storing... ");

        // Open MMIO wave
        WAVEFORMATEX dataWaveFormat = { 0 };

        dataWaveFormat.wFormatTag = WAVE_FORMAT_PCM;
        dataWaveFormat.nChannels = 2;
        dataWaveFormat.nSamplesPerSec = 44100;
        dataWaveFormat.wBitsPerSample = 16;
        dataWaveFormat.nBlockAlign = 
            dataWaveFormat.wBitsPerSample / 8; // block size of data 

        // for buffer estimation 
        dataWaveFormat.nAvgBytesPerSec = 
            dataWaveFormat.nSamplesPerSec * dataWaveFormat.nBlockAlign; 

        dataWaveFormat.cbSize = 0; // bytes of extra information 

        if (FAILED(pWaveFile->OpenWaveFile(
                (LPTSTR) (const char*) pathWave, 
                &dataWaveFormat, 
                CWaveFile::E_WaveTransfer_Write)))
        {
            printf(
                "\n"
                    " *** Failed to open Wave file *** ");
        }
        else
        {
            // calc length
            int nWaveBytes = 0;
            repeat(iSample, xx_apTomoSample.GetCount())
            {
                nWaveBytes += 
                    xx_apTomoSample.GetAt(iSample)->
                        ExportToStereoInterleaveWaveCountBytes(
                            (short*) NULL); // just count bytes
            }

            // alloc buffer
            char* pcWaveBuffer = new char[nWaveBytes]; 
            ZeroMemory(pcWaveBuffer, nWaveBytes);

            // export doc data
            int iExportOffsetBytes = 0;
            repeat(iSample, xx_apTomoSample.GetCount())
            {
                short* pshortStart = 
                    (short*) (pcWaveBuffer + iExportOffsetBytes);

                // First channel
                iExportOffsetBytes += 
                    xx_apTomoSample.GetAt(iSample)->
                        ExportToStereoInterleaveWaveCountBytes(
                            pshortStart);

                // Second channel
                iExportOffsetBytes += 
                    xx_apTomoSample.GetAt(iSample + 1)->
                        ExportToStereoInterleaveWaveCountBytes(
                            pshortStart);

                iSample++;
            }

            ASSERT(iExportOffsetBytes == nWaveBytes);

            // write
            UINT nBytesWritten = (UINT) -1;
            pWaveFile->WriteWave(nWaveBytes, (BYTE*) pcWaveBuffer, &nBytesWritten);

            if ((int) nBytesWritten == nWaveBytes)
            {
                printf("OK ");
            }
            else
            {
                printf(
                    "\n"
                        "    **** FAILED to write, only written " + 
                        Str((int) nBytesWritten) + " bytes ***** ");
            }

            // done
            delete[] pcWaveBuffer;
            pcWaveBuffer = NULL;
        }
    }
    else
    {
        printf("loading ");

        xx_bLoadedFromFile = true;

        // Open MMIO wave
        if (FAILED(
            pWaveFile->OpenWaveFile(
                (LPTSTR) (const char*) pathWave, 
                (WAVEFORMATEX*) NULL, 
                CWaveFile::E_WaveTransfer_Read)))
        {
            printf(
                "\n"
                    " *** Failed to open Wave file *** ");
        }
        else
        {
            // get size
            int nWaveBytes = pWaveFile->GetWaveSizeInBytes();
            printf(Str(nWaveBytes) + " bytes ");
            if (nWaveBytes == 0)
            {
                printf("    *** Wave file is blank! *** ");
            }
            else
            {
                // read
                char* pcWaveBuffer = new char[nWaveBytes]; 
                ZeroMemory(pcWaveBuffer, nWaveBytes);
                DWORD nBytesRead = (DWORD) -1;
                pWaveFile->ReadWave(
                    (BYTE*) pcWaveBuffer, 
                    nWaveBytes, 
                    &nBytesRead);

                if ((int) nBytesRead == nWaveBytes)
                {
                    printf("OK ");
                }
                else
                {
                    printf(
                        "\n"
                            "    **** FAILED, loaded " + 
                            Str((int) nBytesRead) + " bytes ***** ");
                }

                // Import
                ReceiveRawDataIsEndOfSample(
                    pcWaveBuffer,
                    max(0, min(nWaveBytes, (int) nBytesRead)));

                // done
                delete[] pcWaveBuffer;
                pcWaveBuffer = NULL;
            }
        }
    }

    printf("\n");

    // free waver
    pWaveFile->CloseWaveFile();
    delete pWaveFile;
    pWaveFile = NULL;
}

// CTomographicsDoc diagnostics

#ifdef _DEBUG
void CTomographicsDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CTomographicsDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

void CTomographicsDoc::SetStatusText(
        str sInfo)
{
    xx_sStatusText = "";
    UpdateAllViews(NULL);

    Sleep(100);

    xx_sStatusText = sInfo;
    UpdateAllViews(NULL);
}

str CTomographicsDoc::GetStatusText()
{
    return xx_sStatusText;
}

void CTomographicsDoc::AddNewChannelPair()
{
    repeat(iExistingChannel, xx_apTomoSample.GetCount())
    {
        VERIFY(xx_apTomoSample.GetAt(iExistingChannel)->
            TryAppendAmplitudeIsOverflow(0));
    }

    repeat(iChannel, C_nTomoChannels)
    {
        xx_apTomoSample.Add(new CTomoSample(iChannel, this));
    }
}

CTomoSample* CTomographicsDoc::GetCurrentChannelSample(
        int iChannel)
{
    // Make sure enough channels allocated
    ASSERT(iChannel < C_nTomoChannels);

    int iPos = xx_apTomoSample.GetCount() - C_nTomoChannels + iChannel;

    CTomoSample* pSample = xx_apTomoSample.GetAt(iPos);
    ASSERT(pSample->xx_iChannel == iChannel);

    return pSample;
}

bool CTomographicsDoc::ReceiveRawDataIsEndOfSample(
        const char* pcBuffer,
        int nBytes)
{
    xx_bSomeDataReceived = true;
    ASSERT(nBytes % 4 == 0);
    const short* pshortBuffer = (const short*) pcBuffer;
    int nTotalAmplitudes = nBytes / 2;

    bool bSkipToEnd = false;
    int nCountFf = 0;
    int nCountNonFf = 0;
    repeat(iAmplitude, nTotalAmplitudes)
    {
        if (!bSkipToEnd)
        {
            bool bOverflow1 = 
                AppendAmplitudeToChannelIsOverflow(0, pshortBuffer[iAmplitude + 0]);

            bool bOverflow2 = 
                bOverflow1 ||
                AppendAmplitudeToChannelIsOverflow(1, pshortBuffer[iAmplitude + 1]);

            ASSERT(bOverflow1 == bOverflow2);

            if (bOverflow1 || bOverflow2)
            {
                bSkipToEnd = true;
                printf(
                    "\n"
                        "    End of sample at amplitude [" + 
                        Str(iAmplitude) + "], "
                        " byte +" + Str(iAmplitude * 4) + "\n");
            }
            else
            {
                // we have 2 channels
                iAmplitude++;
            }
        }

        if (bSkipToEnd)
        {
            int nAmplitude = pshortBuffer[iAmplitude];
            nAmplitude &= 0xffff;
            if (nAmplitude == 0xffff)
            {
                nCountFf++;
            }
            else
            {
                nCountNonFf++;

                if (nCountNonFf == 1)
                {
                    printf(
                        "   ***** Unexpected non-FF amplitudes: ");
                }

                printf(
                    "%04x ",
                    nAmplitude);
            }
        }
    }

    if (bSkipToEnd)
    {
        printf(
            "\n    " +
                Str(nCountNonFf) + " non-FF amplitudes found, " +
                Str(nCountFf) + " amplitudes are FF at the end of sample\n");
    }

    UpdateAllViews(NULL);

    return bSkipToEnd;
}

bool CTomographicsDoc::AppendAmplitudeToChannelIsOverflow(
        int iChannel,
        short shortAmplitude)
{
    CTomoSample* pSample = GetCurrentChannelSample(iChannel);

    int nAmplitude = (unsigned short) shortAmplitude;
    nAmplitude &= 0xffff;

    if (pSample->TryAppendAmplitudeIsOverflow(nAmplitude))
    {
        AddNewChannelPair();
        CTomoSample* pAddToSample = GetCurrentChannelSample(iChannel);
        ASSERT(pAddToSample != pSample);

        if (nAmplitude != 0xffff)
        {
            printf(
                "\n    ***** Non-0xffff amplitude after the end of sample: %04x\n",
                nAmplitude);
        }

        //VERIFY(!pAddToSample->TryAppendAmplitudeIsOverflow(nAmplitude));

        return true;
    }
    else
    {
        return false;
    }
}
