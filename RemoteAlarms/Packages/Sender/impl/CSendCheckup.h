// CSendCheckup.h
#pragma once

#include "CDividoBasicDialogIfaceGp.h"
#include "CSenderSessionImpl.h"

class CArchiveFusionCreatorIfaceGp;
class CPiPatientIfaceGp;
//#include "CSendParcelImpl.h"

class CTaskIface;
class CFileLockIfaceGp;
class CCheckupHeaderIface;
class CPiCheckupIfaceGp;

//
// CSendCheckup - 
//
//   ExpertMail sender setup and start
//
//ASSUME_IMPL_FOR_IFACE(CSendCheckupImpl, CSendCheckup)

class CSendCheckup : public CDividoBasicDialogIfaceGp //CSendParcelImpl
{
public:
    CSendCheckup();
    //~CSendCheckup();
    //NEW_LOCAL(CSendCheckupImpl, CSendCheckup)
    //NEW_GP(Ralarm, CSendCheckupImpl, CSendCheckup)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    static const int C_nCheckupMultipartSplitBytes = 
            1 * 1024 * 1024;
            //sync:_Ralarm_split_bytes_

// Attributes

    //// parent
    ptr<CSenderSessionImpl> _x_pSenderSession
            xauto(Get);
    ref<CSenderSessionImpl> GetSenderSession()
            return
                _x_pSenderSession.Get().Valid();

    // Data to send
    rp<CPiPatientIfaceGp> _x_rpSendPiPatient
            xauto(Get);
    rp<CPiCheckupIfaceGp> _x_rpSendPiCheckup
            xauto(Get);
    ref<CCheckupHeaderIface> x_rCheckupHeader
            xauto(Get);

    // Current sending state
    ref<CTaskIface> x_rCheckupSendingTask
            xauto(Get);

    // ReplyTo support
    key x_keyReplyToAvailable = key()
            xauto(Get);
    bool x_bReplyToOriginalSender = false
            xauto(Get, Set);

    // Huge file filter
    bool x_bSendRawFiles = false
            xauto(Get, Set);

    // Note: temporary in memory unidefs!
        SEE_ALSO(x_unidefMapAttachmentNameToContent)
    big x_nnMaxSendInitialBytes =
            CIoCommonIface::C_nnMaxSendInitialBytes
            xauto(Get, Set)
            xassert(value >= -1);
    big x_nnMaxSendAttachmentBytes =
            CIoCommonIface::C_nnMaxSendAttachmentBytes
            xauto(Get, Set)
            xassert(value >= -1);

    // Strict from/to user check
    bool x_bSendingForExpertMail = true
            xauto(Get, Set);

// Operations

    // Common init
    void InitSendCheckupOrBackup(
            ref<CSenderSessionImpl> rSenderSession);

    // Init from [Send] menu
    void PrepareToSendCheckup(
            ref<CSenderSessionImpl> rSenderSession,
            ref<CPiCheckupIfaceGp> rPiCheckup,
            ref<CCheckupHeaderIface> rCheckupHeader);

    // Actual sending from dialog button
    bool HandleGoSend();

    // Backup mode, no checkup filter
    bool GoSendPatientToBackup(
            ref<CPiPatientIfaceGp> rPiPatient);

// UI

    // SendCheckupDialogBottom
    ref<UiContainer> NewUiSendCheckupDialogBottom();
    ptrmap<CUiPanelGp> _m_ptrmapUiSendCheckupDialogBottom;

    // SendCheckupDialogContent
    ref<UiContainer> NewUiSendCheckupDialogContent();
    ptrmap<CUiPanelGp> _m_ptrmapUiSendCheckupDialogContent;

    // SendCheckupDialog
    ref<UiContainer> NewUiSendCheckupDialog();
    ptrmap<CUiPanelGp> _m_ptrmapUiSendCheckupDialog;

protected:

    SEE_ALSO(CDividoBasicDialogIfaceGp)

    virtual res OnGetDialogIcon()
            return
                RES__EXPORT_L();

    virtual sloc OnGetDialogCaption()
            return
                TRANSLATE(
                    "Send Data"[... UI-string-removed]");

    virtual ref<UiContainer> OnNewUiDividoDialogContent()
            ;

    virtual ref<UiContainer> OnNewUiDividoDialogButtons()
            ;

private:
    bool _m_bOneTimeInitSendCheckupOk = false;
    bool _m_bSendCheckupOpened = false;

    int m_nCompressionPercent = -111;

    void SendCheckupFiles(
            ref<CCheckupHeaderIface> rCheckupHeader,
            path pathAttachFusion,
            str sJobNavigationSampleKey,
            str sSentCheckupKey);

    void PutCheckupIntoFusion(
            ref<CArchiveFusionCreatorIfaceGp> rArchiveFusionCreator,
            out unidef& out_unidefMapAttachmentNameToContent,
            ref<CPiCheckupIfaceGp> rSendPiCheckup,
            ref<CPiPatientIfaceGp> rFromPiPatient,
            big nnTotalInitialBytes,
            out big& out_nnTotalAttachmentBytes,
            out str& out_sJobNavigationSampleKey,
            out str& out_sLoadWarnings,
            out str& out_sLoadError);

    void _init_AttachToSendCheckup();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiSendCheckupDialogContent.cpp)
class UiSendCheckupDialogContent : public UiContainer
{
public:
    virtual void Ui();

    ref<CSendCheckup> m_rSendCheckup;

private:
    alikex _m_alikexEditLabel;
    bool m_bExtra = false;
    key m_keyLastSelectedFrom;
    key m_keyLastSelectedTo;
};

// (UiSendCheckupDialogBottom.cpp)
class UiSendCheckupDialogBottom : public UiContainer
{
public:
    virtual void Ui();

    ref<CSendCheckup> m_rSendCheckup;

private:
};
