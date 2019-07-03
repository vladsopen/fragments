// CArchiveLzmaOptionsIfaceGp.h
#pragma once

#include "CArchiveBasicOptionsIfaceGp.h"

class CArchiveLzmaSessionIfaceGp;

//
// CArchiveLzmaOptionsIfaceGp - 
//
//   Compression options for LZMA implementation in 7zip.
//     See LZMA.txt and sources in Sevenz.lib for internal details.
//
//ASSUME_IMPL_FOR_IFACE(CArchiveLzmaOptionsImpl, CArchiveLzmaOptionsIfaceGp)

class CArchiveLzmaOptionsIfaceGp : public CArchiveBasicOptionsIfaceGp
{
public:
    CArchiveLzmaOptionsIfaceGp();
    //NEW_LOCAL(CArchiveLzmaOptionsImpl, CArchiveLzmaOptionsIfaceGp)
    //NEW_GP(SysCommon, CArchiveLzmaOptionsImpl, CArchiveLzmaOptionsIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Allows LZMA to use multiple thread internally if possible (on Windows only)
    //   See also EArchiveMainThread which moved ALL operations into a separate thread.
    static const int C_nMinLzmaCpuThreads = 1;
    static const int C_nMaxLzmaCpuThreads = 4;
    int x_nLzmaCpuThreads = 2
            xauto(Get, Set)
            xassert(
                value >= C_nMinLzmaCpuThreads 
                &&
                value <= C_nMaxLzmaCpuThreads);

    // Do you want anyone to be able to decompress your data or
    //   you want to restrict decompression to only our apps?
    //   (see LzmaEnc_WriteProperties)
    bool x_bShowPublic7zipHeader = false
            xauto(Get, Set);

    // Encry[...]LzmaStream = false
            xauto(Get, Set);

    // This will provide the correct unpacked size.
    //   (only supported in limited compression modes)
    SEE_ALSO(JustGetDefaultCompressedHeaderBytes())
    bool x_bCreatePrivateHeader = false
            xauto(Get, Set);

    // Improves speed degrades the ratio
    bool x_bLzmaFast = false
            xauto(Get, Set);

    // Controls compression ratio vs memory consumption
    //   This is the compression block size visible in the callback.
    static const int C_nMinLzmaDictionaryFactor = 12; // 4K
    static const int C_nMaxLzmaDictionaryFactor = 27; // 128-200MB 
        // - up 10% ratio on unordered distro
        // - up  5% ratio on ordered distro
    int x_nLzmaDictionaryFactor = 23 // 8MB
            xauto(Get, Set)
            xassert(
                value >= C_nMinLzmaDictionaryFactor 
                &&
                value <= C_nMaxLzmaDictionaryFactor);

    // Usually big number gives a little bit better compression ratio 
    //   and slower compression process.
    static const int C_nMinLzmaFastBytes = 5;
    static const int C_nMaxLzmaFastBytes = 273;
        // - no help with unordered distro
    int x_nLzmaFastBytes = 32
            xauto(Get, Set)
            xassert(
                value >= C_nMinLzmaFastBytes
                &&
                value <= C_nMaxLzmaFastBytes);

    // Match finder cycles
    static const int C_nMinLzmaMatchCycles = 1;
    static const int C_nMaxLzmaMatchCyclesExtremelySlow = 10000;
        // - 40x slow, 1% gain (unordered distro to 14.4M in 40 min)
    static const int C_nMaxLzmaMatchCyclesInsaneAndUseless = 1000 * 1000;
        // - just never ending
    int x_nLzmaMatchCycles = 32
            xauto(Get, Set)
            xassert(
                value >= C_nMinLzmaMatchCycles
                &&
                value <= C_nMaxLzmaMatchCyclesInsaneAndUseless);

    // Sometimes -lc=4 gives gain for big files.
    //  Often it's better to set lc0, if you change -lp switch.
    static const int C_nMinLzmaLiterals = 0;
    static const int C_nMaxLzmaLiterals = 8;
        // - no help with unordered distro
    int x_nLzmaLiterals = 3
            xauto(Get, Set)
            xassert(
                value >= C_nMinLzmaLiterals
                &&
                value <= C_nMaxLzmaLiterals);

    // -lp switch is intended for periodical data when period is 
    //    equal 2^N. For example, for 32-bit (4 bytes) 
    //    periodical data you can use lp=2. Often it's better to set lc0, 
    //    if you change lp switch.
    static const int C_nMinLzmaLoops = 0;
    static const int C_nMaxLzmaLoops = 4;
        // - no help with unordered distro
    int x_nLzmaLoops = 0
            xauto(Get, Set)
            xassert(
                value >= C_nMinLzmaLoops
                &&
                value <= C_nMaxLzmaLoops);

    // -pb switch is intended for periodical data 
    //    when period is equal 2^N.
    // see LenPriceEnc_UpdateTables(&p->lenEnc, 1 << p->pb, p->ProbPrices);
    static const int C_nMinLzmaPrices = 0;
    static const int C_nMaxLzmaPrices = 4;
    int x_nLzmaPrices = 2
            xauto(Get, Set)
            xassert(
                value >= C_nMinLzmaPrices
                &&
                value <= C_nMaxLzmaPrices);

    // Algorithms from hc* (HashChain) group doesn't provide good compression 
    //   ratio, but they often works pretty fast in combination with 
    //   fast mode (-a0 = x_bLzmaFast).
    //
    //   Memory requirements depend from dictionary size 
    //     (parameter "d" in table below). 
    //
    //    MF_ID     Memory                   Description
    //
    //     bt2    d *  9.5 + 4MB  Binary Tree with 2 bytes hashing.
    //     bt3    d * 11.5 + 4MB  Binary Tree with 3 bytes hashing.
    //     bt4    d * 11.5 + 4MB  Binary Tree with 4 bytes hashing.
    //     hc4    d *  7.5 + 4MB  Hash Chain with 4 bytes hashing.
    // 
    // Select HashChain or BinaryTree:
    bool x_bLzmaBinaryTree = true
            xauto(Get, Set);
    // Select hash bytes:
    static const int C_nMinLzmaHashBytes = 2;
    static const int C_nMaxLzmaHashBytes = 4;
    int x_nLzmaHashBytes = 4
            xauto(Get, Set)
            xassert(
                value >= C_nMinLzmaHashBytes
                &&
                value <= C_nMaxLzmaHashBytes);

// Operations

// UI

protected:
    virtual ref<CArchiveBasicSessionIfaceGp> OnNewArchiveBasicSession()
            return
                ref<CArchiveLzmaSessionIfaceGp>();
private:
    //bool _m_bOneTimeInitArchiveLzmaOptionsIfaceGpOk = false;

    void _init_AttachToArchiveLzmaOptionsIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
