// CArchiveExtractorFactoryIfaceGp.h
#pragma once

class CArchiveBasicSessionIfaceGp;

//
// CArchiveExtractorFactoryIfaceGp -
//
//   Detects proper unpacker session type by archive signature
//
//ASSUME_IMPL_FOR_IFACE(CArchiveExtractorFactoryIface, CArchiveExtractorFactoryIfaceGp)
//ASSUME_IMPL_FOR_IFACE(CArchiveExtractorFactoryImpl, CArchiveExtractorFactoryIface)

class CArchiveExtractorFactoryIfaceGp : public object
{
public:
    CArchiveExtractorFactoryIfaceGp();
    //~CArchiveExtractorFactoryIfaceGp();
    //NEW_LOCAL(CArchiveExtractorFactoryImpl, CArchiveExtractorFactoryIfaceGp)
    NEW_GP(SysCommon, CArchiveExtractorFactoryImpl, CArchiveExtractorFactoryIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

// Operations

    // Detect the class
        static const int C_nMaxSignatureBytes = 4;
    ref<CArchiveBasicSessionIfaceGp> NewArchiveBasicSession(
            str sStartingBytes)
            vhook;

// UI

protected:
    SEE_ALSO(CArchiveExtractorFactoryImpl)

    virtual ref<CArchiveBasicSessionIfaceGp> OnNewArchiveBasicSession(
            str sStartingBytes)
            v1pure;

private:
    //bool _m_bOneTimeInitArchiveExtractorFactoryIfaceGpOk = false;
    //bool _m_bArchiveExtractorFactoryIfaceGpOpened = false;

    void _init_AttachToArchiveExtractorFactoryIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
