// CParseWavetrain.h
#pragma once

//
// CParseWavetrain - 
//
//   Parses incoming XVENDOR Wavetrain headers
//
//ASSUME_IMPL_FOR_IFACE(CParseWavetrainImpl, CParseWavetrain)

class CParseWavetrain : public object
{
public:
    CParseWavetrain();
    //NEW_LOCAL(CParseWavetrainImpl, CParseWavetrain)
    //NEW_GP(MyProject, CParseWavetrainImpl, CParseWavetrain)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // How many bytes to read next time
    int GetNextReadBytes(
            int nActiveClusters,
            int nUsefulPerCluster);

    // alarm check
    int x_fLastAlarmsStatus = 0
            xauto(Get, Set)
            xassert(true);

    // debug
    bool IsDisplayingErrors();

// Operations

    // On success returns true and changes preallocated out_sbufClustersJoined
    //   The caller must preallocate out_sbufClustersJoined accordingly.
    bool ParseIncomingStreamIntoJoinedClusterBuffers(
            int nActiveClusters,
            int nUsefulPerCluster,
            sbuf sbufParse,
            out sbuf& out_sbufClustersJoined,
            out int& out_iBufferId); 

    // Full size for normal read.
    //   For actual variable read size call GetNextReadBytes()
    static int GCalcFullUnparsedLengthForAllClusters(
            int nActiveClusters,
            int nUsefulPerCluster);

    // Full size for normal read.
    //   For actual variable read size call GetNextReadBytes()
    static sbuf GComposeEmulatedHeader(
            int iCluster,
            int fPeadalBits,
            int nAddress);
    static int GGetEmulatedHeaderBytes();

// UI

protected:
private:

    // Blocks we need to parse
    static const int C_nAskBlocksAtOnce = 1;

    // Header format
    //   - data block 256 bytes
    //   - header 4 bytes
    //   - signature AA55h,
    //   - address: XXXX XXXX 
[...]
    enum EStreamHeader 
    {
        E_StreamHeader_0x55,
        E_StreamHeader_0xaa,
        E_StreamHeader_AlarmAndClusterIndex,
        E_StreamHeader_AddressHighByte,
        E_StreamHeader_count,

        F_StreamHeader_ValidClusterIndexBits = 0x07, // 0WWW 
        //F_StreamHeader_AlarmBit = 0x08, // P: XXXX XXXX RRRP 0WWW
        F_StreamHeader_AlarmBits = 
            0x40 | 0x20 | 0x10, // P: XXXX XXXX RPPP 0WWW
[...]
        C_nStreamHeaderAlarmBitLeftShift = 4,
        F_StreamHeader_ReservedTestNonZero = 0x80, // for tests
    };

    // normally empty, 
    //   contains previous stream portion when header detection failed
    sbuf m_sbufLeftCorrupted;

    // normally empty, 
    //   after header error recovery contains data to parse before next portion
    //   (decreases the size of the next portion requested)
    sbuf m_sbufUnreadNextTime;

    // extra status check
    bool m_bRecoveringFromCorruption = false;

    bool m_bForceDisplayErrors = true;

    // Raw data size
    int CalcStreamPortionLengthFor1Cluster();

    // catenate with defered data
    sbuf CompilePortionToParse(
            int nActiveClusters,
            int nUsefulPerCluster,
            sbuf sbufParse);

    // signature scanner
    bool CheckAllClusterHeadersAt(
            int nActiveClusters,
            int nUsefulPerCluster,
            sbuf sbufParse,
            int iScan);

    // signature scanner
    int FindValidFormatPos(
            int nActiveClusters,
            int nUsefulPerCluster,
            sbuf sbufParse);

    // signature check
    bool IsValidAlarmAndClusterHeader(
            int iCluster,
            char cAlarmAndClusterIndex);

    // Data extractor works after validation is complete
    void ReadRawDataFrom(
            int nActiveClusters,
            int nUsefulPerCluster,
            sbuf sbufParse,
            out sbuf& out_sbufClustersJoined,
            out int& out_iBufferId,
            int iStartPos); 

    // error trace
    void HandleDiscardCorruptBytes(
            int nBytes,
            sbuf sbufCutFrom,
            str sDebugInfo);

    // tests
    static void _test_GParse(
            const array<str>& asInserts);

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    virtual void OnTestClass();
};
