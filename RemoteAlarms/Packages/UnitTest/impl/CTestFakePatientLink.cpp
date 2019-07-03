// CTestFakePatientLink.cpp

#include "CProject.h"
#include "CTestFakePatientLink.h"
#include "CPiOrganizerIfaceGp.h"
#include "CTestSendReceive.h"
#include "CPatInfoStoreIfaceGp.h"
#include "CPiPatientIfaceGp.h"
#include "CPatientSampleIfaceGp.h"
#include "CPatientBaseConnectionIfaceGp.h"

CTestFakePatientLink::CTestFakePatientLink()
{
    _init_AttachToTestFakePatientLink();
}

void CTestFakePatientLink::_init_AttachToTestFakePatientLink()
{
    //x_r->_x_p = this;
}

void CTestFakePatientLink::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToTestFakePatientLink();

    rFAIL("make sure it is OK to expose CTestFakePatientLink!");
}

path CTestFakePatientLink::OnGetNewFullAttachmentFilePath()
{
    if (CTestSendReceive::DOneEnd* pdataReceivingEnd = 
            _m_pdataReceivingEnd)
    {
        ref<CTestSendReceive> rTestSendReceive = 
            GetTestSendReceive();

        TESTLOG(
            "",
            "FakePatientLink: Attach!\n");

        return
            pdataReceivingEnd->
                _m_rPatientBaseConnection->
                    GetNewAttachmentPrefix();
    }
    else
    {
        rFAIL("init CTestFakePatientLink before calling it!");
        return path();
    }

    m_nAddedAttachment++;

    TESTLOG(
        "",
        "FakePatientLink: Attach=" + Str(m_nAddedAttachment) + "\n");

    return 
        sys::GGetTempFolder().
            GetAppendedPath("FakePatientLinkAttach").
            GetAppendedPath(Str00(m_nAddedAttachment, 8));
}

void CTestFakePatientLink::OnHandleStartNewCheckup()
{
    TESTLOG(
        "",
        "FakePatientLink: HandleStartNewCheckup() called.\n");
}

void CTestFakePatientLink::OnSendSampleToPatient(
        ref<CPatientSampleIfaceGp> rPatientSample)
{
    TESTLOG(
        "",
        "FakePatientLink: SendSample\n");

    if (CTestSendReceive::DOneEnd* pdataReceivingEnd = 
            _m_pdataReceivingEnd)
    {
        ref<CTestSendReceive> rTestSendReceive = 
            GetTestSendReceive();

        ref<CPiOrganizerIfaceGp> rPiOrganizer =
            pdataReceivingEnd->
                _m_rPatInfoStore->
                    GetAllPatientsPiOrganizer();

        ref<CPiPatientIfaceGp> rIterPatient;
        int nPatients = 0;
        for (
            iter iPatient;
            rPiOrganizer->
                IteratePiOrganizerPatients(
                    out iPatient,
                    out rIterPatient);)
                    //key(),
                    //false);) // bOnlySelected
        {
            nPatients++;
            rASSERT(nPatients == 1); // test receiver only has 1

            rTestSendReceive->
                SendSampleInfoTo(
                    rPatientSample,
                    out *pdataReceivingEnd,
                    rIterPatient);
        }

        rASSERT(nPatients > 0);
    }
    else
    {
        rFAIL("init CTestFakePatientLink before calling it!");
    }
}

