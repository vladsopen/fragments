// CSendParcelIfaceGp.h
#pragma once

//#include "CDividoBasicDialogIfaceGp.h"

//
// CSendParcelIfaceGp -
//
//   Abstract sender not necessary for the PatInfo chackup
//
//      Instantiate: NewSendParcel()
//
//ASSUME_IMPL_FOR_IFACE(CSendParcelIface, CSendParcelIfaceGp)
//ASSUME_IMPL_FOR_IFACE(CSendParcelImpl, CSendParcelIface)

class CSendParcelIfaceGp : public object
{
public:
    CSendParcelIfaceGp();
    //~CSendParcelIfaceGp();
    //NEW_LOCAL(CSendParcelImpl, CSendParcelIfaceGp)
    NEW_GP(Ralarm, CSendParcelImpl, CSendParcelIfaceGp)
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
    //void InitSendParcel();

    // Cleanup
    //void CloseSendParcel();

    // Send external data:
    //   small header first, large attachment later
    //   * pathHeaderExt - see - CIoCommonIface::C_pathExpertMailCheckupExt
    //   * nMultipartSplitBytes - see - C_nCheckupMultipartSplitBytes
    void SendParcelFiles(
            str sSubject,
            sbuf sbufSendHeader,
            path pathAttachLarge,
            int nMultipartSplitBytes)
            vhook;

// UI

protected:
    SEE_ALSO(CSendParcelImpl)

    virtual void OnSendParcelFiles(
            str sSubject,
            sbuf sbufSendHeader,
            path pathAttachLarge,
            int nMultipartSplitBytes)
            v1pure;

private:
    //bool _m_bInited = false;
    //bool _m_bOpened = false;

    void _init_AttachToSendParcelIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
