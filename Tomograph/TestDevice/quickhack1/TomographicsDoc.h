//***********************************************
// WARNING: UNSAFE PROTOTYPE CODE TO THROW AWAY!
// Intentionally Cta incompatible! You can not 
// copypaste it into production! Tied to MFC.
// Run as debug so you have buffer checks.
//***********************************************// TomographicsDoc.h : interface of the CTomographicsDoc class
#pragma once

class CTomoSample;

class CTomographicsDoc : public CDocument
{
protected: // create from serialization only
    CTomographicsDoc();
    DECLARE_DYNCREATE(CTomographicsDoc)

// Attributes
public:

    #define C_nTomoChannels 2

    array<CTomoSample*> xx_apTomoSample;
    bool xx_bSomeDataReceived;
    bool xx_bLoadedFromFile;

    bool ReceiveRawDataIsEndOfSample(
            const char* pcBuffer,
            int nBytes);

    CTomoSample* GetCurrentChannelSample(
            int iChannel);

    void AddNewChannelPair();

    void SetStatusText(
            str sInfo);
    str GetStatusText();

private:
    bool AppendAmplitudeToChannelIsOverflow(
            int iChannel,
            short shortAmplitude);

    str xx_sStatusText;

// Overrides
public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);

// Implementation
public:
    virtual ~CTomographicsDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    DECLARE_MESSAGE_MAP()
};

