// CNetProtocolAbstractIfaceGp.h
#pragma once

//
// CNetProtocolAbstractIfaceGp -
//
//   Stored in Locator to control server packet format
//     (created for public packet I/O)
//

class CNetProtocolAbstractIfaceGp : public object
{
public:
    CNetProtocolAbstractIfaceGp();
    //~CNetProtocolAbstractIfaceGp();
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

    // Non-virtual header solution
    unidef x_unidefSetHttpHeaders
            xauto(Get, Set);

// Operations

    // One-time opener
    //void InitNetProtocolAbstract();

    // Cleanup
    //void CloseNetProtocolAbstract();

    // HTTP-style <Key: Simple Value>
    unidef GetExtraHttpHeaders()
            vhook;

    // Handle HTTP POST and header values attached to a file
    void ReceiveDownloadedMeta(
            unidef unidefMeta,
            path pathDownloadedFile,
            out str& out_sRaiseNetError)
            vhook;

    // CloudFeedControl with Quest response
    void ReceiveDownloadedFeed(
            unidef unidefFeed,
            out str& out_sRaiseNetError)
            vhook;

// UI

protected:

    virtual unidef OnGetExtraHttpHeaders()
            v1null;
    virtual void OnReceiveDownloadedMeta(
            unidef unidefMeta,
            path pathDownloadedFile,
            out str& out_sRaiseNetError)
            v1null;
    virtual void OnReceiveDownloadedFeed(
            unidef unidefFeed,
            out str& out_sRaiseNetError)
            v1null;

private:
    //bool _m_bOneTimeInitNetProtocolAbstractIfaceGpOk = false;
    //bool _m_bNetProtocolAbstractIfaceGpOpened = false;

    void _init_AttachToNetProtocolAbstractIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
