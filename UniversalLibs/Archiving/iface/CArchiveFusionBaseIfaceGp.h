// CArchiveFusionBaseIfaceGp.h
#pragma once

//
// CArchiveFusionBaseIfaceGp - 
//
//   Multiple ArchiveFile amalgamated into a single ball 
//     with a header
SEE_ALSO(CArchiveFileIfaceGp)
SEE_ALSO(CArchiveFusionCommon)
//

class CArchiveFusionBaseIfaceGp : public object
{
public:
    CArchiveFusionBaseIfaceGp();
    //~CArchiveFusionBaseIfaceGp();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);
    //ref<> Get ()
    //        return
    //            _x_p .Get().Valid();

// Operations

    // One-time opener
    //void InitArchiveFusionBase();

    // Cleanup
    //void CloseArchiveFusionBase();

// UI

protected:
    SEE_ALSO(CArchiveFusionCreatorImpl)
    SEE_ALSO(CArchiveFusionExtractorImpl)

private:
    //bool _m_bOneTimeInitArchiveFusionBaseIfaceGpOk = false;
    //bool _m_bArchiveFusionBaseIfaceGpOpened = false;

    void _init_AttachToArchiveFusionBaseIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
