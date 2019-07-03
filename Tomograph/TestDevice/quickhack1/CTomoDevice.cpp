//***********************************************
// WARNING: UNSAFE PROTOTYPE CODE TO THROW AWAY!
// Intentionally Cta incompatible! You can not 
// copypaste it into production! Tied to MFC.
// Run as debug so you have buffer checks.
//***********************************************
// CTomoDevice.cpp

#include "Precompiled.h"
#include "Tomographics.h"
#include "CTomoDevice.h"
#include "TomographicsDoc.h"
#include "TomographicsView.h"
#include "CTomoSample.h"

CTomoDevice::CTomoDevice()
{
    m_bInitialized = false;
    m_hDevice = INVALID_HANDLE_VALUE;
    m_nBlockCount = 0;
    m_bTerse = false;
    xx_bEnablePolling = false;
    m_iReadingSample = 0;
    m_nReadSamplePoints = 0;
    xx_bEmulateSignal = false;
    xx_nMaxRows = 1;
    m_bWasEndOfSample = true;
}

#define SIOK(call) GSiOk((call), #call)

bool GSiOk(
        SI_STATUS eSiStatus,
        str sFunction)
{
    if (eSiStatus != SI_SUCCESS)
    {
        printf(
            "\n" 
                "***** SI error in " + sFunction + " *****\n   ----> %s\n", 
                (const char*) CTomoDevice::GStringizeSiUsbErrorCode(eSiStatus));

        return false;
    }
    else
    {
        return true;
    }
}

void GFail()
{
    printf("\n" "Tomographics test terminated.\n");
    //getch();
    throw 0; // (dont do like this EVER!)
}

void CTomoDevice::Init()
{
    SetForegroundWindow(GetConsoleWindow());
    if (m_bInitialized)
        return;

    m_bInitialized = true;

    try
    {
        theApp.EnsureConsoleReady();

        printf("Tomographics test " __DATE__ " started...\n\n");

        DWORD dwNumDevices = (DWORD) off;
        if (!SIOK(SI_GetNumDevices(&dwNumDevices)))
            GFail();

        printf("Number of Devices: %i\n", dwNumDevices);

        repeat(iDevice, dwNumDevices)
        {
            printf("\n" "Device #%i of %i:\n", iDevice, dwNumDevices);

            // Device strings
            SI_DEVICE_STRING szString = { 0 };
            SIOK(SI_GetProductString(iDevice, &szString, SI_RETURN_SERIAL_NUMBER));
            printf("    Serial #%s;\n", szString);
            SIOK(SI_GetProductString(iDevice, &szString, SI_RETURN_DESCRIPTION));
            printf("    Description: %s\n", szString);
            SIOK(SI_GetProductString(iDevice, &szString, SI_RETURN_LINK_NAME));
            printf("    Link Name: %s\n", szString);
            SIOK(SI_GetProductString(iDevice, &szString, SI_RETURN_VID));
            printf("    VID: %s\n", szString);
            SIOK(SI_GetProductString(iDevice, &szString, SI_RETURN_PID));
            printf("    PID: %s\n", szString);
        }

        // Open
        ReInitLibrary();

        //RunGetVersions();

        FlushBuffers();

        printf("\n" "Tomographics init complete.\n");

        /*
        while (kbhit()) 
        {
            Sleep(100);
            getch();
            Sleep(100);
        }
        getch();
        */

        //printf("\n" "Closing device...\n");
        //SIOK(SI_Close(hDevice));
        //hDevice = 0;
    }
    catch (int)
    {
        // see GFail()
    }
}

void CTomoDevice::FlushBuffers()
{
    try
    {
        // Reset
        printf("Flushing buffers...\n");
        SIOK(SI_FlushBuffers(m_hDevice, true, true));
    }
    catch (int)
    {
        // see GFail()
    }
}

void CTomoDevice::ReInitLibrary()
{
    try
    {
        if (m_hDevice != INVALID_HANDLE_VALUE)
        {
            printf("\n" "Closing device for reinit... ");
            SIOK(SI_Close(m_hDevice));
            m_hDevice = INVALID_HANDLE_VALUE;
        }

        // Open
        printf("\n" "Opening device - ");
        m_hDevice = INVALID_HANDLE_VALUE;
        if (SIOK(SI_Open(0, &m_hDevice)))
        {
            printf("Handle: 0x%x ", (int) m_hDevice);

            // Check timeouts
            {
                DWORD dwReadTimeout = (DWORD) off;
                DWORD dwWriteTimeout = (DWORD) off;
                SIOK(SI_GetTimeouts(&dwReadTimeout, &dwWriteTimeout));
                printf(
                    "(R/W timeouts: %i/%i ms ",
                    dwReadTimeout, 
                    dwWriteTimeout);
            }

            // Set timeouts
            DWORD dwReadTimeout = 1000;
            DWORD dwWriteTimeout = 1000;
            printf("--> ");
            SIOK(SI_SetTimeouts(dwReadTimeout, dwWriteTimeout));

            // Re-Check timeouts
            {
                DWORD dwReadTimeout = (DWORD) off;
                DWORD dwWriteTimeout = (DWORD) off;
                SIOK(SI_GetTimeouts(&dwReadTimeout, &dwWriteTimeout));
                printf(
                    "%i/%i ms)\n",
                    dwReadTimeout, 
                    dwWriteTimeout);
            }
        }
    }
    catch (int)
    {
        // see GFail()
    }
}

void CTomoDevice::ClearAll()
{
    Init();

    try
    {
        printf("\n");

        SendCommand(0x01, "Erase all");

        // Reset
        //printf("Flushing buffers...\n");
        //SIOK(SI_FlushBuffers(m_hDevice, true, true));

        //printf("next block...\n");
        //printf("\n" "Block count: %i\n", m_nBlockCount);
    }
    catch (int)
    {
        // see GFail()
    }
}

bool CTomoDevice::SendCommand(
        int nCode,
        str sId)
{
    printf(
        "Writing '%s' command (0x%02x)... ", 
        (const char *) sId,
        nCode);

    DWORD dwBytesWritten = (DWORD) off;
    if (SIOK(SI_Write(m_hDevice, &nCode, 1, &dwBytesWritten)))
    {
        printf("OK, %i bytes written.\n", dwBytesWritten);
        return true;
    }
    else
    {
        return false;
    }
}

str CTomoDevice::RunGetSampleCount(
        int nBytesToRead)
{
    str sResult = "GetSampleCount failed, see log";

    Init();

    try
    {
        // Start 
        printf("\n");
        if (SendCommand(0x03, "Get Sample Count"))
        {
            char* pcRead = new char[nBytesToRead + 100];
            printf("Reading %i bytes response... ", nBytesToRead);
            DWORD dwInBytes = (DWORD) off;
            if (SIOK(SI_Read(m_hDevice, pcRead, nBytesToRead, &dwInBytes)) ||
                GetAsyncKeyState(VK_SHIFT) < 0)
            {
                if (GetAsyncKeyState(VK_SHIFT) < 0 && dwInBytes == 0)
                    dwInBytes = 2;

                printf("OK, %i bytes received: \n", dwInBytes);

                repeat(iByte, dwInBytes)
                {
                    printf("%02x ", (int) (unsigned char) pcRead[iByte]);
                }

                printf("\n");

                // Reset
                //printf("Flushing buffers...\n");
                //SIOK(SI_FlushBuffers(m_hDevice, true, true));

                int nSampleCount = 
                    (unsigned char) pcRead[0];
                sResult.Format(
                    "Sample count in the device memory: \n\n     %i\n\n"
                        "(Null byte: %i)\n"
                        //"\n\n"
                        //"WARNING: You may need to Flush Buffers or "
                        //"Reset Connection before reading data!"
                        , 
                    nSampleCount,
                    pcRead[1]);

                //AfxMessageBox(sResult);
            }

            delete[] pcRead;
            pcRead = NULL;
        }
    }
    catch (int)
    {
        // see GFail()
    }

    return sResult;
}

str CTomoDevice::RunGetVersions()
{
    str sResult = "GetVersions failed, see log";

    Init();

    try
    {
        // Start 
        printf("\n");
        if (SendCommand(0x02, "Get Versions"))
        {
            int nBytesToRead = 4;
            char* pcRead = new char[nBytesToRead + 100];
            printf("Reading %i bytes response... ", nBytesToRead);
            DWORD dwInBytes = (DWORD) off;
            if (SIOK(SI_Read(m_hDevice, pcRead, nBytesToRead, &dwInBytes)) ||
                GetAsyncKeyState(VK_SHIFT) < 0)
            {
                if (GetAsyncKeyState(VK_SHIFT) < 0 && dwInBytes == 0)
                {
                    printf("*** Forcing 2 bytes ***\n");
                    nBytesToRead = 2;
                }

                if (GetAsyncKeyState(VK_CONTROL) < 0)
                {
                    printf("*** Forcing valid data for testing ***\n");
                    pcRead[0] = C_nTomographicsExpectedNewFeaturesVersion;
                    pcRead[1] = '\xab';
                    pcRead[2] = C_nTomographicsExpectedOldCutOffVersion;
                    pcRead[3] = '\xcd';
                }

                printf("OK, %i bytes received: \n", dwInBytes);

                repeat(iByte, dwInBytes)
                {
                    printf("%02x ", (int) (unsigned char) pcRead[iByte]);
                }

                printf("\n");

                // Reset
                //printf("Flushing buffers...\n");
                //SIOK(SI_FlushBuffers(m_hDevice, true, true));

                int nNewFeaturesVersion = 
                    (unsigned char) pcRead[0];
                int nOldCutOffVersion = 
                    (unsigned char) pcRead[2];
                int nZeroPad1 = 
                    (unsigned char) pcRead[1];
                int nZeroPad2 = 
                    (unsigned char) pcRead[3];

                str sReport = 
                    "Version Check OK.\n"
                        "(Device firmware is compatible\n"
                        "with the test software)";

                if (nNewFeaturesVersion < C_nTomographicsExpectedNewFeaturesVersion)
                {
                    sReport = 
                        "WARNING: Device firmware does not support\n"
                            "some features the program can test.\n"
                            "\n"
                            "You should update the firmware!"
                            ;
                }
                else
                if (nNewFeaturesVersion > C_nTomographicsExpectedNewFeaturesVersion)
                {
                    sReport = 
                        "WARNING: This program cannot test some new features\n"
                            "available in the device firmware.\n"
                            "\n"
                            "You should update the test application!"
                            ;
                }

                if (nOldCutOffVersion < C_nTomographicsExpectedOldCutOffVersion)
                {
                    sReport = 
                        "ERROR: The device firmware is OBSOLETE and\n"
                            "it is NOT COMPATIBLE with this software!\n"
                            "\n"
                            "PLEASE UPDATE THE FIRMWARE!"
                            ;
                }
                else
                if (nOldCutOffVersion > C_nTomographicsExpectedOldCutOffVersion)
                {
                    sReport = 
                        "ERROR: The test software is OBSOLETE and\n"
                            "it is NOT COMPATIBLE with the device firmware!\n"
                            "\n"
                            "PLEASE USE AN UPDATED TEST PROGRAM!"
                            ;
                }

                if (dwInBytes != 4 || 
                    nNewFeaturesVersion == 0 ||
                    nOldCutOffVersion == 0 ||
                    nNewFeaturesVersion == 0xff ||
                    nOldCutOffVersion == 0xff ||
                    nZeroPad1 != 0xab ||
                    nZeroPad2 != 0xcd)
                {
                    sReport = 
                        "ERROR: Failed to check firmware versions!\n"
                            "Unexpected data format."
                            ;
                }

                sResult.Format(
                    ""
                        "%s"
                        "\n"
                        "\n"
                        "NewFeaturesVersion: %02x\n"
                        "\n"
                        "OldCutOffVersion: %02x\n"
                        "\n"
                        "Expected: %02x / %02x\n"
                        , 
                    (const char*) sReport,
                    nNewFeaturesVersion, 
                    nOldCutOffVersion, 
                    C_nTomographicsExpectedNewFeaturesVersion,
                    C_nTomographicsExpectedOldCutOffVersion);

            }

            delete[] pcRead;
            pcRead = NULL;
        }
    }
    catch (int)
    {
        // see GFail()
    }

    return sResult;
}

static int g_nSkipSomeTimers = 0;

void CTomoDevice::SkipSomeTimers()
{
    g_nSkipSomeTimers = 5;
}

void CTomoDevice::PollNextBlock(
        CTomographicsView* pView)
{
    if (g_nSkipSomeTimers > 0)
    {
        g_nSkipSomeTimers--;
        return;
    }
    else
    if (GetAsyncKeyState(VK_LBUTTON) < 0)
    {
        g_nSkipSomeTimers = 5;
    }

    if (m_hDevice == INVALID_HANDLE_VALUE)
        return;

    if (!xx_bEnablePolling)
        return;

    try
    {
        // Start 
        m_nBlockCount++;
        printf("\n");
        if (SendCommand(0x00, "Read"))
        {
            printf("Reading... ");
            char pcIn[2052] = { 0 };
            DWORD dwInBytes = (DWORD) off;
            if (SIOK(SI_Read(m_hDevice, &pcIn, sizeof(pcIn), &dwInBytes)))
            {
                printf("OK, %i bytes received: ", dwInBytes);

                if (dwInBytes > 4)
                {
                    // Get length
                    int nLength = *(unsigned short*) pcIn;
                    printf("Index: %i, ", nLength);
                    /*
                    nLength = (unsigned char) pcIn[0];
                    nLength <<= 8;
                    nLength |= (unsigned char) pcIn[1];
                    printf("or %i (HI,LO)\n", nLength);
                    */

                    // Get Signature
                    printf("Signature EE/00: %02x %02x\n", 
                        (int) (unsigned char) pcIn[2],
                        (int) (unsigned char) pcIn[3]);

                    if (GetAsyncKeyState(VK_SHIFT) < 0)
                        m_bTerse = true;

                    char* pcData = pcIn + 4;
                    int nDataBytes = dwInBytes - 4;

                    //
                    // Signal Emulation support
                    //
                    if (xx_bEmulateSignal)
                    {
                        short* pshortSignal = (short*) pcData;
                        int nSignalPoints = nDataBytes / 2;
                        ASSERT(nDataBytes % 4 == 0);
                        bool bSampleEnded = false;
                        repeat(iPoint, nSignalPoints)
                        {
                            short shortEmulatedAmplitude = 0;
                            if (!bSampleEnded)
                            {
                                shortEmulatedAmplitude = 
                                    short((m_iReadingSample + 1) * 100 +
                                        rand() % 50);

                                if (m_nReadSamplePoints >= 
                                        CTomoSample::C_nTomoRealAmplitudesPerSample * 2)
                                {
                                    m_iReadingSample++;
                                    m_nReadSamplePoints = 0;
                                    bSampleEnded = true;

                                    printf(
                                        "\n    (emulator ends the sample "
                                            "padding with FF)\n");
                                }

                                m_nReadSamplePoints++;
                            }

                            if (bSampleEnded)
                            {
                                int C_nFfff = 0xffff;
                                shortEmulatedAmplitude = (short) C_nFfff;
                            }

                            pshortSignal[iPoint] = shortEmulatedAmplitude;
                        }
                    }

                    // check for FF
                    bool nValidBytes = 0;
                    repeat(iCheck, nDataBytes)
                    {
                        if (pcData[iCheck] != '\xff')
                            nValidBytes++;
                    }

                    // Save to doc
                    bool bWasEndOfSample = m_bWasEndOfSample;
                    bool bEndOfSample = 
                        pView->GetDocument()->
                            ReceiveRawDataIsEndOfSample(
                                pcData,
                                nDataBytes);

                    m_bWasEndOfSample = bEndOfSample;

                    if (nValidBytes == 0)
                    {
                        printf("    All data is 0xFF.\n");
                    }
                    else
                    if (!m_bTerse)
                    {
                        const int C_nBytesPerColumn = 16;
                        int iColumn = C_nBytesPerColumn - 4;
                        int nMaxRows = xx_nMaxRows;
                        if (bEndOfSample || bWasEndOfSample)
                            nMaxRows = oo;

                        int iRow = 0;
                        repeat(iByte, dwInBytes)
                        {
                            printf("%02x ", (int) (unsigned char) pcIn[iByte]);
                            iColumn++;

                            if (iColumn % C_nBytesPerColumn == 0)
                            {
                                printf("\n");
                                iRow++;

                                if (iRow > nMaxRows)
                                {
                                    printf("...\n");
                                    break;
                                }
                            }
                        }
                    }
                }
                else
                {
                    printf("Short block, seems like the end\n");
                    //break;
                }

                printf("\n");
            }
        }

        //printf("next block...\n");
        //printf("\n" "Block count: %i\n", m_nBlockCount);
    }
    catch (int)
    {
        // see GFail()
    }
}

sloc CTomoDevice::GStringizeSiUsbErrorCode(
        int nSiError)
{
    // (I don't use TRY_STRINGIZE_CODE because for a few error codes I 
    //  managed to provide some friendly description)

    sloc slocError;

    if (nSiError == SI_SUCCESS)
    {
        slocError = 
            TRANSLATE(
                "USB operat[... UI-string-removed]");
    }
    else
    if (nSiError == SI_DEVICE_NOT_FOUND)
    {
        slocError = 
            TRANSLATE(
                "USB device[... UI-string-removed]");
    }
    else
    if (nSiError == SI_INVALID_HANDLE)
    {
        slocError = 
            TRANSLATE(
                "USB device[... UI-string-removed]");
    }
    else
    if (nSiError == SI_READ_ERROR)
    {
        slocError = 
            TRANSLATE(
                "USB read f[... UI-string-removed]");
    }
    else
    if (nSiError == SI_RX_QUEUE_NOT_READY)
    {
        slocError = 
            TRANSLATE(
                "USB data i[... UI-string-removed]");
    }
    else
    if (nSiError == SI_WRITE_ERROR)
    {
        slocError = 
            TRANSLATE(
                "USB write [... UI-string-removed]");
    }
    else
    if (nSiError == SI_RESET_ERROR)
    {
        slocError = 
            TRANSLATE(
                "USB reset [... UI-string-removed]");
    }
    else
    if (nSiError == SI_INVALID_PARAMETER)
    {
        slocError = 
            TRANSLATE(
                "USB operat[... UI-string-removed]");
    }
    else
    if (nSiError == SI_INVALID_REQUEST_LENGTH)
    {
        slocError = 
            TRANSLATE(
                "USB reques[... UI-string-removed]");
    }
    else
    if (nSiError == SI_DEVICE_IO_FAILED)
    {
        slocError = 
            TRANSLATE(
                "USB input/[... UI-string-removed]");
    }
    else
    if (nSiError == SI_INVALID_BAUDRATE)
    {
        slocError = 
            TRANSLATE(
                "USB baudra[... UI-string-removed]");
    }
    else
    if (nSiError == SI_FUNCTION_NOT_SUPPORTED)
    {
        slocError = 
            TRANSLATE(
                "USB functi[... UI-string-removed]");
    }
    else
    if (nSiError == SI_GLOBAL_DATA_ERROR)
    {
        slocError = 
            TRANSLATE(
                "USB global[... UI-string-removed]");
    }
    else
    if (nSiError == SI_SYSTEM_ERROR_CODE)
    {
        slocError = 
            TRANSLATE(
                "USB system[... UI-string-removed]");
    }
    else
    if (nSiError == SI_READ_TIMED_OUT)
    {
        slocError = 
            TRANSLATE(
                "USB read t[... UI-string-removed]");
    }
    else
    if (nSiError == SI_WRITE_TIMED_OUT)
    {
        slocError = 
            TRANSLATE(
                "USB write [... UI-string-removed]");
    }
    else
    {
        ASSERT(!"unknown SiError code");

        slocError = 
            TRANSLATE(
                "Unknown US[... UI-string-removed]");
    }

    CString s;
    s.Format(" [SI_%02x]", nSiError);
    slocError += s; 

    return slocError;
}

