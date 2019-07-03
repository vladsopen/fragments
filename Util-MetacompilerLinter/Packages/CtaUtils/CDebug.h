// CDebug.h
// (see "CDebug.cpp")

// CDebug - internal Cta testing modes

class CDebug : public CCommon
{
public:
    GENERIC(CDebug, CCommon);
    CDebug();

// Flags

    // Command line, progress indication, etc
    bool IsTracingEnabled() const;

    // Syntax map checking
    bool IsInternalVerificationEnabled() const;

    // Pure parser verification mode
    bool IsOnlyCompareRegeneratedDiff() const;

    // CallBackEndCompiler
    bool IsCallBackEndCompiler() const;

    // CallBackEndCompiler
    bool IsCallPreprocessor() const;

    // Emit comments
    bool IsEmitExtraComments() const;

// Statistics

    struct DStats
    {
        int m_nClassesTotal;
        int m_nClassesTransformImplemented;
    };

    void ResetStats();
    DStats* GetStats();

private:
    mutable bool m_bTrackingFlagInitialized;
    mutable bool m_bForceTracking;
    DStats m_dataStats;
};