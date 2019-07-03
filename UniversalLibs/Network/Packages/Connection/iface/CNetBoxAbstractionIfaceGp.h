// CNetBoxAbstractionIfaceGp.h
#pragma once

class CFileBankAbstractIfaceGp;
class CNetStatusIfaceGp;

//
// CNetBoxAbstractionIfaceGp -
//
//   Virtual package definition.
//     App must use a derived class representing box data:
//       - as an in-memory atomic sbuf: CNetBoxAsSbufIface
//       - as an atomic file: CNetBoxAsAtomicFileIfaceGp
//       - as a CFileHeap buffer source
//       - as an app defined exotic data source
//

class CNetBoxAbstractionIfaceGp : public object
{
public:
    CNetBoxAbstractionIfaceGp();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Errors and progress
    ref<CNetStatusIfaceGp> x_rNetStatus
            xauto(Get, Set);

    // Inrernal error handler (optionally overridable)
    void SetStatusToNetError(
            str sDetails)
            vhooked;

    // useless: even SharedFolder driver has to create a copy in the LAN share
    /*
    // A nice hack to tell the driver that we have already prepared
    //   a content file ready to be send and placed it into the folder
    //   pointed to by the Locator path.
    //   If the file is missing, or in a different folder or inaccessible to rename,
    //   or the box object is not in HasReadyContentAsPath() mode
    //   then sending will fail.
    bool x_bSendExistingFileInOutbox = false
            xauto(Get, Set);
            */

    //
    // Any impl must support either sbuf or file content extractor
    //

    //// Check optimal conent representation
    //bool HasReadyContentAsSbuf()
    //        vhook;
    //bool HasReadyContentAsPath()
    //        vhook;

    // Get or convert to the require implementation
    //   Common auto-converters are supplied by IfaceGp
    sbuf GetBoxContentAsSbuf(
            out str& out_sError)
            vhooked;
    path GetBoxContentAsPath(
            out ptr<CFileBankAbstractIfaceGp>& out_pDeleteFromFileBank,
            out str& out_sError)
            vhooked;

    // Size must be small! This is very low level after all splitters and packers
    int GetBoxBytes()
            vhook;

// Operations

// UI

protected:

    virtual void OnSetStatusToNetError(
            str sDetails)
            v1null;

    virtual sbuf OnGetBoxContentAsSbuf(
            out str& out_sError)
            v1pure;

    virtual path OnGetBoxContentAsPath(
            out ptr<CFileBankAbstractIfaceGp>& out_pDeleteFromFileBank,
            out str& out_sError)
            v1pure;

    virtual int OnGetBoxBytes()
            v1pure;

private:
    //bool _m_bOneTimeInitNetBoxAbstractionIfaceGpOk = false;

    void _init_AttachToNetBoxAbstractionIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
