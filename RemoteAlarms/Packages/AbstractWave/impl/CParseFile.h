// CParseEcgFile.h
#pragma once

class CWaveSegment;
class CWaveDocImpl;

//
// CParseEcgFile - 
//
//   Feeds AnotherCompany SMDEV5 format to the db
//
//ASSUME_IMPL_FOR_IFACE(CParseEcgFileImpl, CParseEcgFile)

class CParseEcgFile : public object
{
public:
    CParseEcgFile();
    //~CParseEcgFile();
    //NEW_LOCAL(CParseEcgFileImpl, CParseEcgFile)
    //NEW_GP(Ralarm, CParseEcgFileImpl, CParseEcgFile)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    //
    // "SMDEV5"
    //

    static const str C_sEcgBlockSignature = "SMDEV";

    // default and duplicate data reference
    static const int C_nEcgMarkerChannel = 0;

    // strict matrix assertion
    static const int C_nChannelSegmentPad = 0;

    enum EEcgHeader
    {
        E_EcgHeader_Signature,  
        E_EcgHeader_DataBytes,  
        E_EcgHeader_DataCrc,    
        E_EcgHeader_BlockType,  
        E_EcgHeader_ExamId,     
        E_EcgHeader_BlockBytes, 
        E_EcgHeader_length,
    };

    static const int C_nEcgBinaryHeaderBytes = 
            E_EcgHeader_length * sizeof(int);
    CASSERT(C_nEcgBinaryHeaderBytes == 24);

    static const int C_nEcgHeaderBlockTypeByte = 
[...]

// Attributes

    // parent
    ptr<CWaveDocImpl> _x_pWaveDoc
            xauto(Get, Set);
    ref<CWaveDocImpl> GetWaveDoc()
            return
                _x_pWaveDoc.Get().Valid();

    // debug
    int x_nMaxParserBlocks = -1
            xauto(Get, Set)
            xassert(value >= -1);

// Operations

    // One-time opener
    //void InitParseEcgFile();

    // Cleanup
    //void CloseParseEcgFile();

    // Init or raw file appended
    bool UpdateFromRawWave();

    // Preload segment data
    void LoadEcgBlock(
            ref<CWaveSegment> rWaveSegment);

// UI

protected:
private:
    //bool _m_bOneTimeInitParseEcgFileOk = false;
    //bool _m_bParseEcgFileOpened = false;

    big m_nnParsedSoFar = 0;
    big m_nnBlocksAllocated = 0;

    void ParseEcgBlock(
            ref<CWaveSegment> rWaveSegment,
            sbuf sbufBlock);

    sbuf ParseEventBlock(
            sbuf sbufBlock);

    void _init_AttachToParseEcgFile();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
