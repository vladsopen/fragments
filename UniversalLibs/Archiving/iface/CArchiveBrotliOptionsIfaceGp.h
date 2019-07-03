// CArchiveBrotliOptionsIfaceGp.h
#pragma once

#include "CArchiveBasicOptionsIfaceGp.h"

class CArchiveBrotliSessionIfaceGp;

//
// CArchiveBrotliOptionsIfaceGp - 
//
//   Brotli compression settings
//
//ASSUME_IMPL_FOR_IFACE(CArchiveBrotliOptionsImpl, CArchiveBrotliOptionsIfaceGp)

class CArchiveBrotliOptionsIfaceGp : public CArchiveBasicOptionsIfaceGp
{
public:
    CArchiveBrotliOptionsIfaceGp();
    //~CArchiveBrotliOptionsIfaceGp();
    //NEW_LOCAL(CArchiveBrotliOptionsImpl, CArchiveBrotliOptionsIfaceGp)
    //NEW_GP(SysCommon, CArchiveBrotliOptionsImpl, CArchiveBrotliOptionsIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Controls compression ratio vs memory consumption
    static const int C_nMinBrotliQuality = 0;
    static const int C_nMaxBrotliQuality = 11;
    int x_nBrotliQuality = 5
            xauto(Get, Set)
            xassert(
                value >= C_nMinBrotliQuality 
                &&
                value <= C_nMaxBrotliQuality);

    // Controls compression ratio vs memory consumption
    static const int C_nMinBrotliDictionaryFactor = 10; // 1K
    static const int C_nMaxBrotliDictionaryFactor = 24; // 16MB 
    int x_nBrotliDictionaryFactor = C_nMinBrotliDictionaryFactor
            xauto(Get, Set)
            xassert(
                value >= C_nMinBrotliDictionaryFactor 
                &&
                value <= C_nMaxBrotliDictionaryFactor);

    // Default compression mode: 
    //      The compressor does not know anything in
    //      advance about the properties of the input.
    // Or:
    //      Compression mode for UTF-8 format text input.
    bool x_bBrotliSuggestText = false
            xauto(Get, Set);

// Operations

// UI

protected:
    virtual ref<CArchiveBasicSessionIfaceGp> OnNewArchiveBasicSession()
            return
                ref<CArchiveBrotliSessionIfaceGp>();
private:
    //bool _m_bOneTimeInitArchiveBrotliOptionsIfaceGpOk = false;
    //bool _m_bArchiveBrotliOptionsIfaceGpOpened = false;

    void _init_AttachToArchiveBrotliOptionsIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
